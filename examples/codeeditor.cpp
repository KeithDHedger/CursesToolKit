#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

#USEVALGRIND="valgrind --leak-check=full"

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey  ncurses ) -lcursestoolkit "$0" -o codeeditor ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./codeeditor "$@"
retval=$?
rm codeeditor
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <fstream>
#include <curses.h>

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
enum {COPYWORD=0,CUTWORD,COPYLINE,CUTLINE,PASTE};
enum {GOTOLINE=0,FIND,FINDNEXT};
enum {NEXTTAB=0,PREVTAB};
enum {REMOVEMARKS=0,TOGGLEMARK};
enum {HELP=0,ABOUT};

CTK_mainAppClass	*mainApp=new CTK_mainAppClass();
int					windowRows=mainApp->maxRows-3;
int					windowCols=mainApp->maxCols;
int					showLineNumbers=4;
std::vector<bookmarkStruct>	bms;

const char	*menuNames[]={"File","Edit","Navigation","Tabs","BookMarks","Help",NULL};
const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," Clos_e"," _Quit",NULL};
const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cu_t Line"," _Paste",NULL};
const char	*navMenuNames[]={" _Goto Line"," _Find"," Find _Next",NULL};
const char	*tabMenuNames[]={" Next Tab"," Prev Tab",NULL};
const char	*bmMenuNames[]={" Remove All Marks"," Toggle Mark",NULL};
const char	*helpMenuNames[]={" _Help"," About",NULL};

int			newCnt=0;
std::string	clip="";

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

	mainApp->menuBar->CTK_clearMenu(BMMENU);
	while(bmMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(BMMENU,bmMenuNames[cnt++]);

	for(int j=0;j<bms.size();j++)
		freeAndNull(&bms[j].label);
	bms.clear();

	for(unsigned j=0;j<mainApp->pages.size();j++)
		{
			for(int k=0;k<mainApp->pages[j].srcEditBoxes[0]->CTK_getLineCnt();k++)
				{
//fprintf(stderr,"j=%i k=%i\n",j,k);
					if(mainApp->pages[j].srcEditBoxes[0]->CTK_getBookMark(k)==true)
						{
							bookmarkStruct bm;
							bm.pageNum=j;
							bm.lineNum=mainApp->pages[j].srcEditBoxes[0]->CTK_getLineAtY(k);
							sprintf(buffer,"Tab %i, Line %i",bm.pageNum,bm.lineNum);
							bm.label=strdup(buffer);
							bms.push_back(bm);
							mainApp->menuBar->CTK_addMenuItem(BMMENU,(const char*)buffer);
						}
				}
		}
}

void menuSelectCB(void *inst)
{
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);
	
//	fprintf(stderr,"Menu item (%i) '%s' of menu (%i) '%s' selected.\n",mc->menuItemNumber,mc->menuNames[mc->menuNumber]->menuItem[mc->menuItemNumber]->menuName,mc->menuNumber,mainApp->menuBar->menuNames[mc->menuNumber]->menuName);

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
								mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setRunLoop(false);
								mainApp->CTK_addPage();
								mainApp->CTK_addNewSourceEditBox(mainApp,1,3,windowCols,windowRows,true,uddata);
								mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setShowLineNumbers(showLineNumbers);
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
								cu.CTK_openFile(mainApp,"Open File",buffer);
								if(cu.isValidFile==true)
									{
										mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setRunLoop(false);
										mainApp->CTK_addPage();
										mainApp->CTK_addNewSourceEditBox(mainApp,1,3,windowCols,windowRows,true,cu.stringResult.c_str());
										//mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setInputLang(NULL);
										mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setShowLineNumbers(showLineNumbers);
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
										fprintf(f,"%s",mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getBuffer());
										fclose(f);
										mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty=false;
									}
								mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_updateText((char*)mainApp->pages[mainApp->pageNumber].userData,true);
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
										FILE *f=fopen(buffer,"w+");
										if(f!=NULL)
											{
												fprintf(f,"%s",mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getBuffer());
												freeAndNull((char**)&mainApp->pages[mainApp->pageNumber].userData);
												mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(buffer));
												fclose(f);
												mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty=false;
												mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_updateText(buffer,true);
												rebuildTabMenu();
												rebuildBMMenu();
											}
									}
								free(holdstr);
							}
							break;
						case CLOSEITEM:
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setRunLoop(false);
							if(mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									cu.CTK_queryDialog(mainApp,"File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS);
									fprintf(stderr,"Button pressed=%i\n",cu.intResult);
								}
							freeAndNull((char**)&(mainApp->pages[mainApp->pageNumber].userData));
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
							if(mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									cu.CTK_queryDialog(mainApp,"File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS);
									fprintf(stderr,"Button pressed=%i\n",cu.intResult);
								}
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setRunLoop(false);
							mainApp->runEventLoop=false;
							break;
					}
				break;
			case EDITMENU:
				switch(mc->menuItemNumber)
					{
						case COPYWORD:
							clip=mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getCurrentWord();
							break;
						case COPYLINE:
							clip=mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getCurrentLine();
							break;
						case CUTWORD:
							clip=mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getCurrentWord();
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_deleteCurrentWord();
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty=true;
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getBuffer();
							break;
						case CUTLINE:
							clip=mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getCurrentLine();
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_deleteCurrentLine();
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty=true;
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getBuffer();
							break;
						case PASTE:
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_insertText(clip.c_str());
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->isDirty=true;
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getBuffer();
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
										mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_gotoLine(atoi(cu.stringResult.c_str()));
										mainApp->CTK_updateScreen(mainApp,NULL);
										mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_doEvent(true,mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getStrings(),mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getSrcStrings());
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
								for(int k=0;k<mainApp->pages[j].srcEditBoxes[0]->CTK_getLineCnt();k++)
									mainApp->pages[j].srcEditBoxes[0]->CTK_setBookMark(k,false);
							rebuildBMMenu();
							break;
						case TOGGLEMARK:
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_toggleBookMark(mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_getCursLine());
							rebuildBMMenu();
							break;
						default:
							mainApp->CTK_setPage(bms[mc->menuItemNumber-2].pageNum);
							mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_gotoLine(bms[mc->menuItemNumber-2].lineNum);
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
}

int main(int argc, char **argv)
{
	coloursStruct cs;
	cs.windowBackCol=BACK_BLACK;
	cs.fancyGadgets=false;

	cs.hiliteBackCol=BACK_BLACK;
	cs.hiliteForeCol=FORE_GREEN;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_GREEN;

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

	cnt=0;
	while(editMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(EDITMENU,editMenuNames[cnt++]);

	cnt=0;
	while(navMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(NAVMENU,navMenuNames[cnt++]);

	cnt=0;
	while(helpMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(HELPMENU,helpMenuNames[cnt++]);

	cnt=0;
	while(tabMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(TABMENU,tabMenuNames[cnt++]);

	mainApp->menuBar->CTK_setSelectCB(menuSelectCB);
	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLACK;

	mainApp->CTK_setColours(cs);
	mainApp->CTK_addNewSourceEditBox(mainApp,1,3,windowCols,windowRows,true,"../CursesToolKit/src/cursesSourceEditBox.cpp");
	mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setShowLineNumbers(showLineNumbers);
	mainApp->CTK_setPageUserData(0,(void*)strdup("../CursesToolKit/src/cursesSourceEditBox.cpp"));
	mainApp->menuBar->CTK_addMenuItem(TABMENU,"../CursesToolKit/src/cursesSourceEditBox.cpp");

	//mainApp->pages[mainApp->pageNumber].srcEditBoxes[0]->CTK_setEditable(false);
	rebuildBMMenu();

	mainApp->CTK_mainEventLoop();

	for(int j=0;j<bms.size();j++)
		freeAndNull(&bms[j].label);

	for(int k=0;k<mainApp->pages.size();k++)
		free(mainApp->pages[k].userData);
	delete mainApp;
	SETSHOWCURS;

	return 0;
}
