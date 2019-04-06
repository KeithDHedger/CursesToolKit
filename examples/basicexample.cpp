#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey ) -lcursestoolkit "$0" -o basicexample ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./basicexample "$@"
retval=$?
rm basicexample
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesApplication.h>

CTK_mainAppClass	*mainApp=new CTK_mainAppClass();

#define FILEMENU 0
#define QUITITEM 5
void menuselctCB(void *inst)
{
	char				*buffer=(char*)alloca(256);
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);
	
	sprintf(buffer,"Menu item (%i) '%s' of menu (%i) '%s' selected.",mc->menuItemNumber,mc->menuNames[mc->menuNumber]->menuItem[mc->menuItemNumber]->menuName,mc->menuNumber,mainApp->menuBar->menuNames[mc->menuNumber]->menuName);
	mainApp->textBoxes[1]->CTK_updateText(buffer);

	if((mc->menuNumber==FILEMENU) && (mc->menuItemNumber==QUITITEM))
		mainApp->runEventLoop=false;

}

void mainloopCB(void *mainc,void *data)
{
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

void buttonselctCB(void *inst)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);

	sprintf(buffer,"Button '%s' clicked.",bc->label);
	mainApp->textBoxes[1]->CTK_updateText(buffer);
}

void listselctCB(void *inst)
{
	char						*buffer=(char*)alloca(256);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

	sprintf(buffer,"List item '%s' clicked, user data=%p.",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
	mainApp->textBoxes[1]->CTK_updateText(buffer);
}

void checkselctCB(void *inst)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);

	cb->CTK_setValue(!cb->CTK_getValue());
	sprintf(buffer,"CheckBox '%s' clicked ... Value=%i",cb->label,cb->CTK_getValue());
	mainApp->textBoxes[1]->CTK_updateText(buffer);
}

int main(int argc, char **argv)
{

	const char	*menuNames[]={"_File","_Edit","_Tabs","_Navigation","F_unctions","_Bookmarks","T_ools","_Help",NULL};
	const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
	const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cut L_ine"," _Paste",NULL};
	const char	*navMenuNames[]={" Goto _Define"," _Open Include"," _Goto Line"," Open _Manpage"," _Find"," Find _Next",NULL};
	const char	*helpMenuNames[]={"_Help","A_bout",NULL};
	
	const char	*sampletxt="Press 'ESC' to activate/deactivate menus.\nUse 'LEFT/RIGHT/IP/DOWN' arrow keys to navigate menus.\n\
'HOME/END/PAGEUP/PAGEDOWN' can also be used.\n\
'RETURN' selects a menu item, 'ESC closes menus without selecting.\n\
Shortcut keys can be used ( without the CTRL key when menus are open )\n\
\n\
'TAB' Moves between gadgets.\n\
'UP/DOWN' etc when a text box is highlighted will scroll the text.\n\
\n\
'RETURN' when a button is highlighted will 'click' that button.\n\
\n\
'RETURN' when an input box is highlighted will show the cursor and start getting input.\n\
'RETURN' will stop, 'LEFT/RIGHT/HOME/END/BACKSPACE/DELETE' are used for editing.\n\
Pasting is done via middle click of mouse as usual.\
";

	coloursStruct cs;
	//cs.use256Colours=false;
	cs.windowBackCol=BACK_WHITE;
	//cs.hiliteBackCol=69;
	//cs.hiliteForeCol=53;
	cs.fancyGadgets=true;
	cs.hiliteBackCol=BACK_BLACK;
	cs.hiliteForeCol=FORE_GREEN;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_GREEN;

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
		mainApp->menuBar->CTK_addMenuItem(3,helpMenuNames[cnt++]);

	mainApp->menuBar->CTK_setSelectCB(menuselctCB);

	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_setColours(cs);
//	cs.boxType=OUTBOX;
//cs.fancyGadgets=true;
	mainApp->CTK_addNewTextBox(3,3,80,8,sampletxt);
//	mainApp->textBoxes[0]->CTK_setColours(cs);
	mainApp->CTK_addNewTextBox(3,13,80,1,"Results",false);
	cs.textBoxType=INBOX;
	mainApp->textBoxes[1]->CTK_setColours(cs);
//cs.fancyGadgets=false;

	cs.backCol=BACK_YELLOW;
	cs.foreCol=FORE_BLACK;
	//mainApp->CTK_addNewEditBox(101,3,64,8,sampletxt);
	mainApp->CTK_addNewEditBox(101,3,64,8,true,"/etc/fstab");
	mainApp->editBoxes[0]->CTK_setColours(cs);

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

	mainApp->eventLoopCB=mainloopCB;
	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;


	for(int j=0;j<mainApp->editBoxes[0]->txtstrings.size();j++)
		fprintf(stderr,"%s",mainApp->editBoxes[0]->txtstrings[j].c_str());

	return 0;
}
