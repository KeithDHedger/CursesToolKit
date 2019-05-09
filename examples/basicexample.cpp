#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

#USEVALGRIND="valgrind --leak-check=full"

g++ -Wall -ggdb -O0 -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey ) -lcursestoolkit "$0" -o basicexample ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./basicexample "$@"
retval=$?
rm basicexample
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include <cursesApplication.h>
#include <cursesGlobals.h>
CTK_mainAppClass	*mainApp=new CTK_mainAppClass();

#define FILEMENU 0
#define QUITITEM 5
#define TABWIDTH 4

void menuselctCB(void *inst)
{
	char				*buffer=(char*)alloca(256);
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);

	mainApp->pages[mainApp->pageNumber].editBoxes[0]->CTK_setRunLoop(false);
	sprintf(buffer,"Menu item (%i) '%s' of menu (%i) '%s' selected.",mc->menuItemNumber,mc->menuNames[mc->menuNumber]->menuItem[mc->menuItemNumber]->menuName,mc->menuNumber,mainApp->menuBar->menuNames[mc->menuNumber]->menuName);
	mainApp->pages[0].textBoxes[1]->CTK_updateText(buffer);

	if((mc->menuNumber==FILEMENU) && (mc->menuItemNumber==QUITITEM))
		mainApp->runEventLoop=false;
}

void mainloopCB(void *mainc,void *data)
{
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

void buttonselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);

	sprintf(buffer,"Button '%s' clicked.",bc->label);
	mainApp->pages[0].textBoxes[1]->CTK_updateText(buffer);
}

void listselctCB(void *inst,void *userdata)
{
	char						*buffer=(char*)alloca(256);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

	sprintf(buffer,"List item '%s' clicked, user data=%p.",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
	mainApp->pages[0].textBoxes[1]->CTK_updateText(buffer);
}

void checkselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);

	cb->CTK_setValue(!cb->CTK_getValue());
	sprintf(buffer,"CheckBox '%s' clicked ... Value=%i",cb->label,cb->CTK_getValue());
	mainApp->pages[0].textBoxes[1]->CTK_updateText(buffer);
}

int main(int argc, char **argv)
{

	const char	*menuNames[]={"File","Edit","Tabs","Navigation","Functions","Bookmarks","Tools","Help",NULL};
	const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
	const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cut L_ine"," _Paste",NULL};
	const char	*navMenuNames[]={" Goto _Define"," _Open Include"," _Goto Line"," Open _Manpage"," _Find"," Find _Next",NULL};
	const char	*helpMenuNames[]={" _Help"," A_bout",NULL};
	
	const char	*sampletxt="Press 'ESC' to activate/deactivate menus.\nUse 'LEFT/RIGHT/IP/DOWN' arrow keys to navigate menus.\n\
'HOME/END/PAGEUP/PAGEDOWN' can also be used.\n\
'RETURN' selects a menu item, 'ESC closes menus without selecting.\n\
Shortcut keys can be used ( without the CTRL key when menus are open )\n\
\n\
'UP/DOWN' etc when a text box is highlighted will scroll the text.\n\
\n\
'TAB' Moves between gadgets.\n\
'SHIFT+TAB' Moves between gadgets backwards.\n\
\n\
'RETURN' when a button is highlighted will 'click' that button.\n\
\n\
'RETURN' when an input box is highlighted will show the cursor and start getting input.\n\
'RETURN' will stop, 'LEFT/RIGHT/HOME/END/BACKSPACE/DELETE' are used for editing.\
\n\
'RETURN' when an edit box is highlighted will begin editing.\n\
'ESC' will exit edit mode.\n\
\n\
Pasting is done via middle click of mouse as usual.\
";

	coloursStruct cs;
	cs.windowBackCol=BACK_WHITE;
	cs.fancyGadgets=true;

//custom menu colours
	cs.hiliteBackCol=BACK_BLACK;
	cs.hiliteForeCol=FORE_GREEN;
	cs.menuForeCol=FORE_WHITE;
	cs.menuBackCol=BACK_RED;

	mainApp->CTK_setTabWidth(TABWIDTH);
	mainApp->CTK_setColours(cs);
	mainApp->CTK_addNewMenuBar();

	mainApp->menuBar->CTK_addMenuToBar(menuNames[0]);//0
	mainApp->menuBar->CTK_addMenuToBar(menuNames[1]);//1
	mainApp->menuBar->CTK_addMenuToBar(menuNames[4]);//2
	mainApp->menuBar->CTK_addMenuToBar(menuNames[7]);//3

	int cnt=0;
	while(fileMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(0,fileMenuNames[cnt++]);

	cnt=0;
	while(editMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(1,editMenuNames[cnt++]);

	cnt=0;
	while(navMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(2,navMenuNames[cnt++]);

	cnt=0;
	while(helpMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(3,helpMenuNames[cnt++],false);

	char	buffer[2048];
	for(int j=0;j<100;j++)
		{
			sprintf(buffer," Really long menu, menu item %i",j);
			mainApp->menuBar->CTK_addMenuItem(3,buffer,false);
		}
	mainApp->menuBar->CTK_setSelectCB(menuselctCB);

//reset colours to defaults
	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_setColours(cs);

	mainApp->CTK_addNewTextBox(3,3,80,8,sampletxt);
	//mainApp->CTK_addNewTextBox(3,3,80,8,true,"/etc/fstab");
	//mainApp->pages[0].textBoxes [0]->CTK_updateText("/etc/fstab",true);
	mainApp->CTK_addNewTextBox(3,13,80,1,"Results",false);

//just set box type to inbox.
	cs.textBoxType=INBOX;
	mainApp->CTK_addNewEditBox(mainApp,101,3,mainApp->maxCols-1-101,8,true,"../ChangeLog");
	mainApp->pages[0].editBoxes[0]->CTK_setColours(cs);

	mainApp->CTK_addNewButton(8,16,30,1,"Hello World");
	mainApp->pages[0].buttons[0]->CTK_setSelectCB(buttonselctCB,NULL);
	mainApp->CTK_addNewButton(32,16,11,1,"A Button");
	mainApp->pages[0].buttons[1]->CTK_setSelectCB(buttonselctCB,NULL);

	mainApp->CTK_addNewInput(8,19,36,1,"Some input");
	mainApp->pages[0].inputs[0]->CTK_setColours(cs);


	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	lb->CTK_newListBox(86,3,10,4);
	lb->CTK_addListItem("Item 1");
	lb->CTK_addListItem("Item 2");
	lb->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	lb->CTK_addListItem("Item 4");
	lb->CTK_addListItem("Item 5");
	lb->CTK_addListItem("Item 6");
	lb->CTK_addListItem("Item 7",(void*)7);
	lb->CTK_setSelectCB(listselctCB,NULL);

//use some 256 colours
	cs.foreCol=0;
	cs.backCol=7;
//set hihlite colour to eye watering 256 colour!
	cs.hiliteForeCol=88;
	cs.hiliteBackCol=148;
	cs.use256Colours=true;
	lb->CTK_setColours(cs);

	mainApp->CTK_addListBox(lb);

	mainApp->CTK_addNewCheckBox(85,9,10,"A Checkbox");
	mainApp->pages[0].checkBoxes[0]->CTK_setSelectCB(checkselctCB,NULL);
	mainApp->pages[0].checkBoxes[0]->CTK_setEnterDeselects(false);
	mainApp->CTK_addNewCheckBox(85,11,10,"Checkbox 2");
	mainApp->pages[0].checkBoxes[1]->CTK_setSelectCB(checkselctCB,NULL);
	mainApp->pages[0].checkBoxes[1]->CTK_setEnterDeselects(false);

	mainApp->CTK_addNewLabel(85,13,40,2,"Default justified non selectable label.\nLine 2 of label.");
	mainApp->CTK_addNewLabel(85,17,40,2,"Centred non selectable label.\nLine 2 of label.");
	mainApp->pages[0].labels[1]->CTK_setJustify(CENTRE);
	mainApp->CTK_addNewLabel(85,21,40,2,"Right justified non selectable label.\nLine 2 of label.");
	mainApp->pages[0].labels[2]->CTK_setJustify(RIGHT);

	mainApp->eventLoopCB=mainloopCB;
	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;

	delete mainApp;
	return 0;
}
