#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

#USEVALGRIND="valgrind --leak-check=full"

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey ncurses Magick++ ) -lcursestoolkit "$0" -o texteditor ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./texteditor "$@"
retval=$?
rm texteditor
reset
exit $retval

#endif

#include <libgen.h>

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

const char	*menuNames[]={"File","Edit","Navigation","Tabs","BookMarks","Help",NULL};
const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
const char	*editMenuNames[]={" _Copy"," C_ut"," _Paste",NULL};
const char	*navMenuNames[]={" _Goto Line"," _Find"," Find _Next",NULL};
const char	*tabMenuNames[]={" _Next Tab"," _Prev Tab",NULL};
const char	*bmMenuNames[]={" _Remove All Marks"," _Toggle Mark",NULL};
const char	*helpMenuNames[]={" _Help"," About ",NULL};

CTK_mainAppClass			*mainApp=new CTK_mainAppClass();
int							windowRows=mainApp->maxRows-3;
int							windowCols=mainApp->maxCols;
//int						windowCols=40;
int							showLineNumbers=4;
std::vector<bookmarkStruct>	bms;
int							newCnt=0;
std::string					clip="";
//int							startSelection=-1;
//int							endSelection=-1;

CTK_cursesEditBoxClass* getSrcBox(int page)
{
	for(int k=0;k<mainApp->pages[page].gadgets.size();k++)
		{
			if(mainApp->pages[page].gadgets[k]->CTK_getGadgetType()==EDITGADGET)
				return(static_cast<CTK_cursesEditBoxClass*>(mainApp->pages[page].gadgets[k]));
		}
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
	CTK_cursesEditBoxClass	*srcbox;

	mainApp->menuBar->CTK_clearMenu(BMMENU);
	while(bmMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(BMMENU,bmMenuNames[cnt++]);

	for(int j=0;j<bms.size();j++)
		CTK_freeAndNull(&bms[j].label);
	bms.clear();

	for(unsigned j=0;j<mainApp->pages.size();j++)
		{
			srcbox=getSrcBox(j);
			//for(int k=0;k<mainApp->pages[j].srcEditBoxes[0]->CTK_getLineCnt();k++)
			//for(int k=0;k<mainApp->pages[j].gadgets.size();k++)
				{
				//	if(mainApp->pages[j].gadgets[k]->CTK_getGadgetType()==SRCGADGET)
						{
						//	srcbox=static_cast<CTK_cursesSourceEditBoxClass*>(mainApp->pages[j].gadgets[k]);
							for(int k=0;k<srcbox->CTK_getLineCnt();k++)
//fprintf(stderr,"j=%i k=%i\n",j,k);
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
				
		}
}
//void rebuildBMMenu(void)
//{
//	int		cnt=0;
//	char	buffer[4096]={0,};
//	CTK_cursesEditBoxClass	*edbox;
//
//	mainApp->menuBar->CTK_clearMenu(BMMENU);
//	while(bmMenuNames[cnt]!=NULL)
//		mainApp->menuBar->CTK_addMenuItem(BMMENU,bmMenuNames[cnt++]);
//
//	for(int j=0;j<bms.size();j++)
//		CTK_freeAndNull(&bms[j].label);
//	bms.clear();
//	
//	for(unsigned j=0;j<mainApp->pages.size();j++)
//		{
//			for(int k=0;k<mainApp->pages[j].editBoxes[0]->CTK_getLineCnt();k++)
//				{
//					if(mainApp->pages[j].editBoxes[0]->CTK_getBookMark(k)==true)
//						{
//							bookmarkStruct bm;
//							bm.pageNum=j;
//							bm.lineNum=mainApp->pages[j].editBoxes[0]->CTK_getLineAtY(k);
//							sprintf(buffer,"Tab %i, Line %i",bm.pageNum,bm.lineNum);
//							bm.label=strdup(buffer);
//							bms.push_back(bm);
//							mainApp->menuBar->CTK_addMenuItem(BMMENU,(const char*)buffer);
//						}
//				}
//		}
//}

void menuSelectCB(void *inst,void *userdata)
{
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);
	
//	fprintf(stderr,"Menu item (%i) '%s' of menu (%i) '%s' selected.\n",mc->menuItemNumber,mc->menuNames[mc->menuNumber]->menuItem[mc->menuItemNumber]->menuName,mc->menuNumber,mainApp->menuBar->menuNames[mc->menuNumber]->menuName);
	CTK_cursesEditBoxClass	*srcbox=getSrcBox(mainApp->pageNumber);;

	switch(mc->menuNumber)
		{
			case FILEMENU:
				switch(mc->menuItemNumber)
					{
						case NEWITEM:
							{
								char	*uddata;
								asprintf(&uddata,"/tmp/Untitled-%i",++newCnt);
								srcbox->CTK_setRunLoop(false);
								mainApp->CTK_addPage();
								srcbox=mainApp->CTK_addNewEditBox(mainApp,1,3,windowCols,windowRows,false,"\n");
								srcbox->CTK_setShowLineNumbers(showLineNumbers);
								mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)uddata);
								rebuildTabMenu();
								rebuildBMMenu();
							}
							break;

						case OPENITEM:
							{
								std::string				str;
								CTK_cursesUtilsClass	cu;
								char					*buffer=get_current_dir_name();
								cu.CTK_openFile(mainApp,"Open File",buffer);
								if(cu.isValidFile==true)
									{
										srcbox->CTK_setRunLoop(false);
										mainApp->CTK_addPage();
										srcbox=mainApp->CTK_addNewEditBox(mainApp,1,3,windowCols,windowRows,true,cu.stringResult.c_str());
										srcbox->CTK_setShowLineNumbers(showLineNumbers);
										mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(cu.stringResult.c_str()));
										rebuildTabMenu();
										rebuildBMMenu();
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
							}
							break;
						case SAVEASITEM:
							{
								char					*buffer=(char*)alloca(PATH_MAX);
								CTK_cursesUtilsClass	cu;
								char					*holdstr=strdup((char*)mainApp->pages[mainApp->pageNumber].userData);

								cu.CTK_openFile(mainApp,"Save As ...",dirname(holdstr),false,basename(holdstr));
								if(cu.isValidFile==true)
									{
										sprintf(buffer,"%s/%s",cu.inFolder.c_str(),cu.stringResult.c_str());
										FILE *f=fopen(cu.stringResult.c_str(),"w+");
										if(f!=NULL)
											{
												fprintf(f,"%s",srcbox->CTK_getBuffer());
												CTK_freeAndNull((char**)&mainApp->pages[mainApp->pageNumber].userData);
												mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(buffer));
												fclose(f);
												srcbox->isDirty=false;
												rebuildTabMenu();
												rebuildBMMenu();
											}
									}
								free(holdstr);
							}
							break;
						case CLOSEITEM:
							if(srcbox->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									cu.CTK_queryDialog(mainApp,"File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS);
									fprintf(stderr,"Button pressed=%i\n",cu.intResult);
								}
							srcbox->CTK_setRunLoop(false);
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
									cu.CTK_queryDialog(mainApp,"File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS);
									fprintf(stderr,"Button pressed=%i\n",cu.intResult);
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
								if(cu.CTK_entryDialog(mainApp,"Goto Line Number?","","Jump To Line ...",NULL,true))
									{
										srcbox->CTK_gotoLine(atoi(cu.stringResult.c_str()));
										mainApp->CTK_updateScreen(mainApp,NULL);
										srcbox->CTK_doEvent(false,srcbox->CTK_getStrings(),srcbox->CTK_getStrings());
										break;
									}
								mainApp->CTK_updateScreen(mainApp,NULL);
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
							mainApp->CTK_updateScreen(mainApp,NULL);
							break;
						case PREVTAB:
							mainApp->CTK_previousPage();
							mainApp->CTK_updateScreen(mainApp,NULL);
							break;
						default:
							mainApp->CTK_setPage(mc->menuItemNumber-2);
							mainApp->CTK_updateScreen(mainApp,NULL);
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
							cu->CTK_aboutDialog(mainApp,"Text Editor","Text Editor Example","Copyright 2019 K.D.Hedger","keithdhedger@gmail.com","http://keithhedger.freeddns.org","K.D.Hedger","../LICENSE");
							delete cu;
						}
					 	break;
					}
				break;
		}
}

void mainloopCB(void *mainc,void *data)
{
//MOVETO(1,2);
//printf("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
//fprintf(stderr,"sx=%i -> se=%i\n",srcbox->startSelection,srcbox->endSelection);
//fflush(NULL);
}

int main(int argc, char **argv)
{
	coloursStruct cs;
	cs.windowBackCol=BACK_WHITE;
	cs.fancyGadgets=false;
	CTK_cursesEditBoxClass	*srcbox;

	cs.hiliteBackCol=BACK_BLACK;
	cs.hiliteForeCol=FORE_GREEN;
	cs.menuForeCol=FORE_BLACK;
	cs.menuBackCol=BACK_GREEN;
	cs.lineNumBackCol=BACK_WHITE;
	cs.lineNumForeCol=FORE_BLACK;

	mainApp->CTK_setTabWidth(TABWIDTH);
	mainApp->CTK_setColours(cs);
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
	mainApp->menuBar->CTK_setMenuShortCut(FILEMENU,OPENITEM,'O');

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
	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_setColours(cs);

	srcbox=mainApp->CTK_addNewEditBox(mainApp,1,3,windowCols,windowRows-1,false,"\n");
	srcbox->CTK_setShowLineNumbers(showLineNumbers);
	mainApp->CTK_setPageUserData(0,(void*)strdup("../ChangeLog"));
	mainApp->menuBar->CTK_addMenuItem(TABMENU,"../ChangeLog");


	srcbox->CTK_updateText("../ChangeLog",true);
	srcbox->CTK_toggleBookMark(10);
	srcbox->CTK_toggleBookMark(56);
	rebuildBMMenu();
	//srcbox->CTK_setEditable(true);
//fprintf(stderr,">%s<\n",typeid(srcbox).name());

	mainApp->eventLoopCBIn=mainloopCB;
	mainApp->CTK_mainEventLoop();

	for(int j=0;j<bms.size();j++)
		CTK_freeAndNull(&bms[j].label);

	for(int k=0;k<mainApp->pages.size();k++)
		free(mainApp->pages[k].userData);

	delete mainApp;
	SETSHOWCURS;

	return 0;
}
