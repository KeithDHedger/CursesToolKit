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

//#include "LFSTKFindClass.h"
//
//#include <cursesApplication.h>
#include <cursesGlobals.h>

CTK_mainAppClass	*mainApp=new CTK_mainAppClass();

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
const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cut L_ine"," _Paste",NULL};
const char	*navMenuNames[]={" _Goto Line"," _Find"," Find _Next",NULL};
const char	*tabMenuNames[]={" Next Tab"," Prev Tab",NULL};
const char	*helpMenuNames[]={" _Help"," About",NULL};

void menuSelectCB(void *inst)
{
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);
	
	fprintf(stderr,"Menu item (%i) '%s' of menu (%i) '%s' selected.\n",mc->menuItemNumber,mc->menuNames[mc->menuNumber]->menuItem[mc->menuItemNumber]->menuName,mc->menuNumber,mainApp->menuBar->menuNames[mc->menuNumber]->menuName);

	switch(mc->menuNumber)
		{
			case FILEMENU:
				switch(mc->menuItemNumber)
					{
						case NEWITEM:
							{
								mainApp->CTK_addPage();
								mainApp->CTK_addNewEditBox(1,3,mainApp->maxCols,16,false,"new page\n");
							}
							break;
						case OPENITEM:
							{
								std::string str;
								CTK_cursesUtilsClass	cu;
								cu.CTK_openFile(mainApp);
								if(cu.isValidFile==true)
									{
								//delete cu;
										fprintf(stderr,"file=%s\n",cu.selectedFile.c_str());
										mainApp->CTK_addPage();
										mainApp->CTK_addNewEditBox(1,3,mainApp->maxCols,16,true,cu.selectedFile.c_str());
									}
							}
							break;
						case SAVEITEM:
							break;
						case SAVEASITEM:
							break;
						case CLOSEITEM:
							mainApp->CTK_removePage(mainApp->pageNumber);
							if(mainApp->pageNumber==-1)
								mainApp->runEventLoop=false;
							break;
						case QUITITEM:
							mainApp->runEventLoop=false;
							break;
					}
				break;
			case EDITMENU:
				break;
			case NAVMENU:
				break;
			case TABMENU:
			{
			int pagenum=-1;
				switch(mc->menuItemNumber)
					{
						case NEXTTAB:
							pagenum=mainApp->CTK_nextPage();
							mainApp->CTK_updateScreen(mainApp,NULL);
							break;
						case PREVTAB:
							pagenum=mainApp->CTK_previousPage();
							mainApp->CTK_updateScreen(mainApp,NULL);
							break;
					}
				fprintf(stderr,"pagenum=%i\n",pagenum);
				}
				break;
			case HELPMENU:
				break;
		}
}

void mainloopCB(void *mainc,void *data)
{
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
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
	mainApp->CTK_addNewEditBox(1,3,mainApp->maxCols,16,true,"/media/LinuxData/Development64/Projects/CursesToolKit/CursesToolKit/IGNORE/fstab-copy2");
	//mainApp->pages[0].editBoxes[0]->CTK_setColours(cs);
#if 0
//	cs.boxType=OUTBOX;
//cs.fancyGadgets=true;
	mainApp->CTK_addNewTextBox(3,3,80,8,sampletxt);
//	mainApp->textBoxes[0]->CTK_setColours(cs);
	mainApp->CTK_addNewTextBox(3,13,80,1,"Results",false);
	cs.textBoxType=INBOX;
	mainApp->textBoxes[1]->CTK_setColours(cs);
//cs.fancyGadgets=false;

	//cs.backCol=BACK_YELLOW;
	//cs.foreCol=FORE_BLACK;
	//mainApp->CTK_addNewEditBox(101,3,64,8,sampletxt);
	mainApp->CTK_addNewEditBox(101,3,64,8,true,"../ChangeLog");
	mainApp->pages[0].editBoxes[0]->CTK_setColours(cs);

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;

	mainApp->CTK_addNewButton(8,16,30,1,"Hello World");
	mainApp->buttons[0]->CTK_setColours(cs);
	mainApp->buttons[0]->CTK_setSelectCB(buttonselctCB);
	mainApp->CTK_addNewButton(32,16,11,1,"A Button");
	mainApp->buttons[1]->CTK_setSelectCB(buttonselctCB);
	mainApp->buttons[1]->CTK_setColours(cs);

	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_addNewInput(8,19,36,1,"Some input");
	mainApp->inputs[0]->CTK_setColours(cs);


	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	lb->CTK_newListBox(86,3,10,4);
	lb->CTK_addListItem("Item 1");
	lb->CTK_addListItem("Item 2");
	lb->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	lb->CTK_addListItem("Item 4");
	lb->CTK_addListItem("Item 5");
	lb->CTK_addListItem("Item 6");
	lb->CTK_addListItem("Item 7",(void*)7);
	lb->CTK_setSelectCB(listselctCB);
	//cs.backCol=BACK_GREEN;
	lb->CTK_setColours(cs);

	mainApp->CTK_addListBox(lb);

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;
	mainApp->CTK_addNewCheckBox(85,9,10,"A Checkbox");
	mainApp->checkBoxes[0]->CTK_setSelectCB(checkselctCB);
	mainApp->checkBoxes[0]->CTK_setEnterDeselects(false);
	mainApp->CTK_addNewCheckBox(85,11,10,"Checkbox 2");
	mainApp->checkBoxes[1]->CTK_setSelectCB(checkselctCB);
	mainApp->checkBoxes[1]->CTK_setEnterDeselects(false);
	mainApp->checkBoxes[0]->CTK_setColours(cs);
	mainApp->checkBoxes[1]->CTK_setColours(cs);
#endif
	//mainApp->pages[0].menusActive=false;
	mainApp->eventLoopCB=mainloopCB;
	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;


//	for(int j=0;j<mainApp->pages[0].editBoxes[0]->txtstrings.size();j++)
//		fprintf(stderr,"%s",mainApp->pages[0].editBoxes[0]->txtstrings[j].c_str());

	return 0;
}
