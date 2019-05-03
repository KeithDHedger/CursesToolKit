#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey ) -lcursestoolkit "$0" -o texteditor ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./texteditor "$@"
retval=$?
rm texteditor
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#include <cursesGlobals.h>

CTK_mainAppClass	*mainApp=new CTK_mainAppClass();
int					windowRows=mainApp->maxRows-3;
int					windowCols=mainApp->maxCols;

#define TABWIDTH 4

#define FILEMENU 0
#define EDITMENU 1
#define NAVMENU 2
#define TABMENU 3
#define HELPMENU 4

enum {NEWITEM=0,OPENITEM,SAVEITEM,SAVEASITEM,CLOSEITEM,QUITITEM};
enum {COPYWORD=0,CUTWORD,COPYLINE,CUTLINE,PASTE};
enum {GOTOLINE=0,FIND,FINDNEXT};
enum {NEXTTAB=0,PREVTAB};
enum {HELP=0,ABOUT};

const char	*menuNames[]={"File","Edit","Navigation","Tabs","Help",NULL};
const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," Clos_e"," _Quit",NULL};
const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cut L_ine"," _Paste",NULL};
const char	*navMenuNames[]={" _Goto Line"," _Find"," Find _Next",NULL};
const char	*tabMenuNames[]={" Next Tab"," Prev Tab",NULL};
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
								mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_setRunLoop(false);
								mainApp->CTK_addPage();
								mainApp->CTK_addNewEditBox(mainApp,1,3,windowCols,windowRows,false,"\n");
								mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)uddata);
								rebuildTabMenu();
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
										mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_setRunLoop(false);
										mainApp->CTK_addPage();
										mainApp->CTK_addNewEditBox(mainApp,1,3,windowCols,windowRows,true,cu.stringResult.c_str());
										mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(cu.stringResult.c_str()));
										rebuildTabMenu();
									}
								free(buffer);
							}
							break;
						case SAVEITEM:
							{
								FILE *f=fopen((char*)mainApp->pages[mainApp->pageNumber].userData,"w+");
								if(f!=NULL)
									{
										fprintf(f,"%s",mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_getBuffer());
										fclose(f);
										mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty=false;
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
												fprintf(f,"%s",mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_getBuffer());
												freeAndNull((char**)&mainApp->pages[mainApp->pageNumber].userData);
												mainApp->CTK_setPageUserData(mainApp->pageNumber,(void*)strdup(buffer));
												fclose(f);
												mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty=false;
												rebuildTabMenu();
											}
									}
								free(holdstr);
							}
							break;
						case CLOSEITEM:
							if(mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									cu.CTK_queryDialog(mainApp,"File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS);
									fprintf(stderr,"Button pressed=%i\n",cu.intResult);
								}
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_setRunLoop(false);
							freeAndNull((char**)&(mainApp->pages[mainApp->pageNumber].userData));
							mainApp->CTK_removePage(mainApp->pageNumber);
							if(mainApp->pageNumber==-1)
								mainApp->runEventLoop=false;
							else
								rebuildTabMenu();
							break;
						case QUITITEM:
							if(mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty==true)
								{
									CTK_cursesUtilsClass	cu;
									cu.CTK_queryDialog(mainApp,"File has changed ...\nDo you want to save it?",(const char*)mainApp->pages[mainApp->pageNumber].userData,"Save ...",ALLBUTTONS);
									fprintf(stderr,"Button pressed=%i\n",cu.intResult);
								}
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_setRunLoop(false);
							mainApp->runEventLoop=false;
							break;
					}
				break;
			case EDITMENU:
				switch(mc->menuItemNumber)
					{
						case COPYWORD:
							clip=mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_getCurrentWord();
							break;
						case COPYLINE:
							clip=mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_getCurrentLine();
							break;
						case CUTWORD:
							clip=mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_getCurrentWord();
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_deleteCurrentWord();
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty=true;
							break;
						case CUTLINE:
							clip=mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_getCurrentLine();
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_deleteCurrentLine();
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty=true;
							break;
						case PASTE:
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_insertText(clip.c_str());
							mainApp->pages[mainApp->pageNumber].editBoxes[0]->isDirty=true;
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
										mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_gotoXY(0,atoi(cu.stringResult.c_str())-1);
										mainApp->CTK_updateScreen(mainApp,NULL);
										mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_doEditEvent();
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
			case HELPMENU:
				switch(mc->menuItemNumber)
					{
					 case HELP:
					 	break;
					 case ABOUT:
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
	cs.windowBackCol=BACK_WHITE;
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
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_setColours(cs);
	mainApp->CTK_addNewEditBox(mainApp,1,3,windowCols,windowRows,true,"../ChangeLog");
	mainApp->CTK_setPageUserData(0,(void*)strdup("../ChangeLog"));
	mainApp->menuBar->CTK_addMenuItem(TABMENU,"../ChangeLog");

	mainApp->CTK_mainEventLoop();
	delete mainApp;
	SETSHOWCURS;

	return 0;
}
