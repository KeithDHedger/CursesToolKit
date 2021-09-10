#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ "$0" -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs  ncurses Magick++) -lcursestoolkit -o codeeditor ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./codeeditor
retval=$?
rm codeeditor
exit $retval

#endif

#include <libgen.h>
#include <fstream>

#include <cursesGlobals.h>

#define TABWIDTH 4

#define FILEMENU 0
#define EDITMENU 1
#define NAVMENU 2
#define TABMENU 3
#define BMMENU 4
#define HELPMENU 5

struct bookmarkStruct
{
	int		pageNum=-1;
	int		lineNum=-1;
	char	*label=NULL;
};

enum {NEWITEM=0,OPENITEM,SAVEITEM,SAVEASITEM,CLOSEITEM,QUITITEM};
enum {COPYWORD=0,CUTWORD,PASTE};
enum {GOTOLINE=0,FIND,FINDNEXT};
enum {NEXTTAB=0,PREVTAB};
enum {REMOVEMARKS=0,TOGGLEMARK};
enum {HELP=0,ABOUT};

CTK_mainAppClass			*mainApp=new CTK_mainAppClass();
int							windowRows=mainApp->maxRows-3;
int							windowCols=mainApp->maxCols;
int							showLineNumbers=4;
std::vector<bookmarkStruct>	bms;

const char					*menuNames[]={"File","Edit","Navigation","Tabs","BookMarks","Help",NULL};
const char					*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
const char					*editMenuNames[]={" _Copy"," C_ut"," _Paste",NULL};
const char					*navMenuNames[]={" _Goto Line"," _Find"," Find _Next",NULL};
const char					*tabMenuNames[]={" _Next Tab"," _Prev Tab",NULL};
const char					*bmMenuNames[]={" _Remove All Marks"," _Toggle Mark",NULL};
const char					*helpMenuNames[]={" _Help"," About ",NULL};

int							newCnt=0;
std::string					clip="";
int							genw;
int							geny;
int							genx;
int							genh;
bool						usenativehiliter=true;

CTK_cursesSourceEditBoxClass* getSrcBox(int page)
{
	for(long unsigned int k=0;k<mainApp->pages[page].gadgets.size();k++)
		{
			if(mainApp->pages[page].gadgets[k]->CTK_getGadgetType()==SRCGADGET)
				return(static_cast<CTK_cursesSourceEditBoxClass*>(mainApp->pages[page].gadgets[k]));
		}
	return(NULL);
}

void rebuildTabMenu(void)
{
	int	cnt=0;

	mainApp->menuBar->CTK_clearMenu(TABMENU);
	while(tabMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(TABMENU,tabMenuNames[cnt++]);

	for(unsigned j=0;j<mainApp->pages.size();j++)
		mainApp->menuBar->CTK_addMenuItem(TABMENU,(const char*)mainApp->pages[j].userData);
}

void rebuildBMMenu(void)
{
	int		cnt=0;
	char	buffer[4096]={0,};
	CTK_cursesSourceEditBoxClass	*srcbox;

	mainApp->menuBar->CTK_clearMenu(BMMENU);
	while(bmMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(BMMENU,bmMenuNames[cnt++]);

	for(long unsigned int  j=0;j<bms.size();j++)
		CTK_freeAndNull(&bms[j].label);
	bms.clear();

	for(unsigned j=0;j<mainApp->pages.size();j++)
		{
			srcbox=getSrcBox(j);
			for(int k=0;k<srcbox->CTK_getLineCnt();k++)
				if(srcbox->CTK_getBookMark(k)==true)
					{
						bookmarkStruct bm;
						bm.pageNum=j;
						bm.lineNum=srcbox->CTK_getLineAtY(k);
						sprintf(buffer,"Tab %i, Line %i",bm.pageNum,bm.lineNum);
						bm.label=strdup(buffer);
						bms.push_back(bm);
						mainApp->menuBar->CTK_addMenuItem(BMMENU,(const char*)buffer);
					}
		}
}

bool menuSelectCB(void *inst,void *userdata)
{
	CTK_cursesMenuClass				*mc=static_cast<CTK_cursesMenuClass*>(inst);
	CTK_cursesSourceEditBoxClass	*srcbox=getSrcBox(mainApp->pageNumber);

	switch(mc->menuNumber)
		{
			case FILEMENU:
				switch(mc->menuItemNumber)
					{
						case NEWITEM:
							{
								char	*uddata;
								asprintf(&uddata,"/tmp/Untitled-%i",++newCnt);
								std::ofstream output(uddata);
								srcbox->CTK_setRunLoop(false);
								mainApp->CTK_addPage();
								srcbox=mainApp->CTK_addNewSourceEditBox(mainApp,genx,geny,genw,genh,true,uddata);
								srcbox->CTK_setShowLineNumbers(4);
								srcbox->liveUpdate=true;
								srcbox->CTK_setUseNative(usenativehiliter);
								srcbox->gadgetColours.boxType=NOBOX;
								mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)uddata);
								rebuildTabMenu();
								rebuildBMMenu();
								mainApp->CTK_clearScreen();
							}
							break;

						case OPENITEM:
							{
								std::string				str;
								CTK_cursesUtilsClass	cu;
								char					*buffer=get_current_dir_name();
								cu.CTK_fileChooserDialog(buffer,CUOPENFILE);
								if(cu.dialogReturnData.isValidData==true)
									{
										srcbox->CTK_setRunLoop(false);
										mainApp->CTK_addPage();
										srcbox=mainApp->CTK_addNewSourceEditBox(mainApp,genx,geny,genw,genh,true,cu.dialogReturnData.stringValue.c_str());
										srcbox->CTK_setShowLineNumbers(4);
										srcbox->liveUpdate=true;
										srcbox->CTK_setUseNative(usenativehiliter);
										srcbox->gadgetColours.boxType=NOBOX;
										srcbox->gadgetColours.foreCol=FORE_WHITE;
										srcbox->gadgetColours.backCol=BACK_BLACK;
										mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(cu.dialogReturnData.stringValue.c_str()));
										rebuildTabMenu();
										rebuildBMMenu();
										mainApp->CTK_clearScreen();
									}
								free(buffer);
							}
							break;
						case SAVEITEM:
							{
								FILE *f=fopen((char*)mainApp->pages[mainApp->pageNumber].userData,"w+");
								if(f!=NULL)
									{
										fprintf(f,"%s",srcbox->CTK_getBuffer());
										fclose(f);
										srcbox->isDirty=false;
									}
								srcbox->CTK_updateText((char*)mainApp->pages[mainApp->pageNumber].userData,true);
							}
							break;
						case SAVEASITEM:
							{
								char					*buffer=(char*)alloca(PATH_MAX);
								CTK_cursesUtilsClass	cu;
								char					*holdstr=strdup((char*)mainApp->pages[mainApp->pageNumber].userData);

								cu.CTK_fileChooserDialog(dirname(holdstr),CUSAVEFILE,NULL,basename(holdstr));
								if(cu.dialogReturnData.isValidData==true)
									{
										sprintf(buffer,"%s",cu.dialogReturnData.stringValue.c_str());
										FILE *f=fopen(buffer,"w+");
										if(f!=NULL)
											{
												fprintf(f,"%s",srcbox->CTK_getBuffer());
												CTK_freeAndNull((char**)&mainApp->pages[mainApp->pageNumber].userData);
												mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(buffer));
												fclose(f);
												srcbox->isDirty=false;
												srcbox->CTK_updateText(buffer,true);
												rebuildTabMenu();
												rebuildBMMenu();
											}
									}
								free(holdstr);
							}
							break;
						case CLOSEITEM:
							srcbox->CTK_setRunLoop(false);
							if(srcbox->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									if(cu.CTK_queryDialog("File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS)==true)
										fprintf(stderr,"Button pressed=%i\n",cu.dialogReturnData.intValue);
								}
							CTK_freeAndNull((char**)&(mainApp->pages[mainApp->pageNumber].userData));
							mainApp->CTK_removePage(mainApp->pageNumber);
							if(mainApp->pageNumber==-1)
								mainApp->runEventLoop=false;
							else
								{
									rebuildTabMenu();
									rebuildBMMenu();
								}
							break;
						case QUITITEM:
							if(srcbox->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									if(cu.CTK_queryDialog("File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS)==true)
										fprintf(stderr,"Button pressed=%i\n",cu.dialogReturnData.intValue);
								}
							srcbox->CTK_setRunLoop(false);
							mainApp->runEventLoop=false;
							break;
					}
				break;
			case EDITMENU:
				switch(mc->menuItemNumber)
					{
						case COPYWORD:
							if(srcbox->isSelecting==true)
								{
									clip=srcbox->CTK_getSelection();
									srcbox->CTK_finishSelecting();
								}
							else
								clip=srcbox->CTK_getCurrentWord();
							break;
						case CUTWORD:
							if(srcbox->isSelecting==true)
								{
									clip=srcbox->CTK_getSelection();
									srcbox->CTK_deleteSelection();
								}
							else
								{
									clip=srcbox->CTK_getCurrentWord();
									srcbox->CTK_deleteCurrentWord();
								}
							
							srcbox->isDirty=true;
							srcbox->CTK_getBuffer();
							break;
						case PASTE:
							srcbox->CTK_insertText(clip.c_str());
							srcbox->isDirty=true;
							srcbox->CTK_getBuffer();
							break;
					}
				break;
			case NAVMENU:
				switch(mc->menuItemNumber)
					{
						case GOTOLINE:
							{
								CTK_cursesUtilsClass	cu;
								if(cu.CTK_entryDialog("Goto line","","Jump To Line ...","",true,40)==true)
									{
										srcbox->CTK_gotoLine(atoi(cu.dialogReturnData.stringValue.c_str()));
										mainApp->CTK_updateScreen(mainApp,SCREENUPDATEBASIC);
										srcbox->CTK_doEvent(true,srcbox->CTK_getStrings(),srcbox->CTK_getSrcStrings());
										break;
									}
							}
							break;
						case FIND:
							break;
						case FINDNEXT:
							break;
					}
				break;
			case TABMENU:
			{
				switch(mc->menuItemNumber)
					{
						case NEXTTAB:
							mainApp->CTK_nextPage();
							mainApp->CTK_clearScreen();
							mainApp->CTK_setDefaultGadget(mainApp->CTK_getGadgetNum(mainApp->pageNumber,SRCGADGET,1));
							mainApp->CTK_updateScreen(mainApp,SCREENUPDATEBASIC);
							break;
						case PREVTAB:
							mainApp->CTK_previousPage();
							mainApp->CTK_clearScreen();
							mainApp->CTK_setDefaultGadget(mainApp->CTK_getGadgetNum(mainApp->pageNumber,SRCGADGET,1));
							mainApp->CTK_updateScreen(mainApp,SCREENUPDATEBASIC);
							break;
						default:
							mainApp->CTK_setPage(mc->menuItemNumber-2);
							mainApp->CTK_clearScreen();
							mainApp->CTK_setDefaultGadget(mainApp->CTK_getGadgetNum(mainApp->pageNumber,SRCGADGET,1));
							mainApp->CTK_updateScreen(mainApp,SCREENUPDATEBASIC);
							break;
					}
				}
				break;

			case BMMENU:
				switch(mc->menuItemNumber)
					{
						case REMOVEMARKS:
							for(unsigned j=0;j<mainApp->pages.size();j++)
								{
									srcbox=getSrcBox(j);
									for(int k=0;k<srcbox->CTK_getLineCnt();k++)
										srcbox->CTK_setBookMark(k,false);
								}
							rebuildBMMenu();
							break;
						case TOGGLEMARK:
							srcbox->CTK_toggleBookMark(srcbox->CTK_getCursLine());
							rebuildBMMenu();
							break;
						default:
							mainApp->CTK_setPage(bms[mc->menuItemNumber-2].pageNum);
							srcbox=getSrcBox(mainApp->pageNumber);
							srcbox->CTK_gotoLine(bms[mc->menuItemNumber-2].lineNum);
							break;
					}
				break;

			case HELPMENU:
				switch(mc->menuItemNumber)
					{
					 case HELP:
					 	break;
					 case ABOUT:
					 	{
						 	CTK_cursesUtilsClass	*cu;
						 	cu=new CTK_cursesUtilsClass;
								cu->CTK_aboutDialog("Text Editor","Text Editor Example","Copyright 2019 K.D.Hedger","keithdhedger@gmail.com","http://keithhedger.freeddns.org","K.D.Hedger","../LICENSE",80);
							delete cu;
						}
						break;
					}
				break;
		}
	return(true);
}

void mainloopCB(void *mainc,void *data)
{
}

int main(int argc, char **argv)
{
	CTK_cursesSourceEditBoxClass	*srcbox;

	genw=windowCols;
	geny=2;
	genx=1;
	genh=windowRows;
	mainApp->windowColours.foreCol=FORE_WHITE;
	mainApp->windowColours.backCol=BACK_BLACK;

	mainApp->CTK_setTabWidth(TABWIDTH);
	mainApp->CTK_addNewMenuBar();

	mainApp->menuBar->CTK_addMenuToBar(menuNames[FILEMENU]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[EDITMENU]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[NAVMENU]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[TABMENU]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[BMMENU]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[HELPMENU]);

	int cnt=0;
	while(fileMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(FILEMENU,fileMenuNames[cnt++]);

	mainApp->menuBar->CTK_setMenuShortCut(FILEMENU,QUITITEM,'q');

	cnt=0;
	while(editMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(EDITMENU,editMenuNames[cnt++]);

	mainApp->menuBar->CTK_setMenuShortCut(EDITMENU,PASTE,'v');
	mainApp->menuBar->CTK_setMenuShortCut(EDITMENU,COPYWORD,'c');
	mainApp->menuBar->CTK_setMenuShortCut(EDITMENU,CUTWORD,'x');

	cnt=0;
	while(navMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(NAVMENU,navMenuNames[cnt++]);

	cnt=0;
	while(helpMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(HELPMENU,helpMenuNames[cnt++]);

	cnt=0;
	while(tabMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(TABMENU,tabMenuNames[cnt++]);

	mainApp->menuBar->CTK_setSelectCB(menuSelectCB,NULL);

	srcbox=mainApp->CTK_addNewSourceEditBox(mainApp,genx,geny,genw,genh,true,"../CursesToolKit/src/cursesApplication.cpp");
	srcbox->CTK_setShowLineNumbers(4);
	srcbox->liveUpdate=true;
	srcbox->CTK_setUseNative(usenativehiliter);
	srcbox->gadgetColours.boxType=NOBOX;

	srcbox->CTK_setShowLineNumbers(showLineNumbers);
	mainApp->CTK_setPageUserData(0,(void*)strdup("../CursesToolKit/src/cursesSourceEditBox.cpp"));
	mainApp->menuBar->CTK_addMenuItem(TABMENU,"../CursesToolKit/src/cursesSourceEditBox.cpp");

//clear screen etc
	mainApp->CTK_mainEventLoop();

	for(long unsigned int  j=0;j<bms.size();j++)
		CTK_freeAndNull(&bms[j].label);

	for(long unsigned int  k=0;k<mainApp->pages.size();k++)
		free(mainApp->pages[k].userData);
	delete mainApp;
	SETSHOWCURS;

	return 0;
}
