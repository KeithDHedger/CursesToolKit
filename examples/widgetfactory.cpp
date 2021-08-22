#if 0

#
#©keithhedger Mon 9 Mar 12:50:13 GMT 2020 kdhedger68713@gmail.com
#
#CTKApp.cpp is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#at your option) any later version.
#
#DevelopmentFiles is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with DevelopmentFiles.  If not, see <http://www.gnu.org/licenses/>.
#

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s --show-error-list=yes --gen-suppressions="
#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

pushd ..
	make||exit 100
popd

name=$(basename $0 .cpp)

g++ -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs -D_DEBUGCODE_ $(pkg-config --cflags --libs Magick++ ncurses ) -lcursestoolkit -lboost_system -lboost_filesystem -lboost_regex "$0" -o $name ||exit 1
echo "done compiling ..."

LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND "./$name" "$@"
retval=$?
rm $name
#reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesGlobals.h>

CTK_mainAppClass				*mainApp=new CTK_mainAppClass();
CTK_cursesTextBoxClass			*resulttextbox;
CTK_cursesTextBoxClass			*resultbuttonstextbox;
CTK_cursesDropClass				*dropdown;
CTK_cursesSourceEditBoxClass	*srceditbox;
CTK_cursesEditBoxClass			*editbox;
CTK_cursesListBoxClass			*list1;
CTK_cursesListBoxClass			*list2;
CTK_cursesTextBoxClass			*helptextbox;
CTK_cursesProgressBarClass		*progressStatic;
CTK_cursesProgressBarClass		*progressPulse;
CTK_cursesProgressBarClass		*progressPulse1;
CTK_cursesProgressBarClass		*progressIndicator;

bool							mbarVis=true;
int								b1Cnt=0;
int								b2Cnt=0;
bool							doMainLoop=true;

#define FILEMENU 0
#define QUITITEM 5
#define TABMENU 2
#define NEXTTAB 9
#define PREVTAB 10
#define TABWIDTH 4
#define	NEXTPAGE 2000
#define	PREVPAGE 2001
#define BUTTON1	4000
#define BUTTON2	4001
#define BUTTON3	4002
#define CHECK1 5000
#define CHECK2 5001
#define CHECK3 5002
#define HELPMENU 3
#define HELPITEM 0

bool menuselctCB(void *inst,void *userdata)
{
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);

	fprintf(stderr,"Menu==%i Item==%i\n",mc->menuNumber,mc->menuItemNumber);
	if(mc->menuNumber==HELPMENU)
		{
			switch(mc->menuItemNumber)
				{
					case HELPITEM:
						mainApp->CTK_setPage(0);
						mainApp->CTK_setDefaultGadget(helptextbox);
						mainApp->CTK_setDefaultGadget(helptextbox);
						break;
				}
		}

	if(mc->menuNumber==TABMENU)
		{
			switch(mc->menuItemNumber)
				{
					case NEXTTAB:
						mainApp->CTK_nextPage();
						break;
					case PREVTAB:
						mainApp->CTK_previousPage();
						break;
					default:
						mainApp->CTK_setPage(mc->menuItemNumber);
						if(mc->menuItemNumber==3)
							{
								mainApp->CTK_setDefaultGadget(list1);
							}
						return(true);
						break;
				}
		}

	if((mc->menuNumber==FILEMENU) && (mc->menuItemNumber==QUITITEM))//TODO//exit edit box;
		{
			mainApp->runEventLoop=false;
			doMainLoop=false;
		}
	return(true);
}

void mainloopCBIn(void *mainc,void *data)
{
fprintf(stderr,"here\n");
}

void mainloopCBOut(void *mainc,void *data)
{
fprintf(stderr,"here\n");
}

void doPulseBar(void)
{
	progressPulse->CTK_pulseBar();
}

bool buttonselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	long					ud=(long)userdata;
	char					*srcbuffercopy;

	if(userdata!=NULL)
		{
			switch(ud)
				{
					case NEXTPAGE:
						mainApp->CTK_nextPage();
						break;
					case PREVPAGE:
						mainApp->CTK_previousPage();
						break;
					case BUTTON1:
						b1Cnt++;
						sprintf(buffer,"Button '%s' clicked, cnt %i.",bc->label,b1Cnt);
						mainApp->CTK_setPage(3);
						mainApp->CTK_setDefaultGadget(list1);
						return(true);
						break;
					case BUTTON2:
						b2Cnt++;
						sprintf(buffer,"Button '%s' clicked, cnt %i.",bc->label,b2Cnt);
						mainApp->CTK_setPage(0);
						mainApp->CTK_setDefaultGadget(helptextbox);
						return(true);
						break;
					case BUTTON3:
						CTK_cursesShellClass shell(mainApp);
						shell.CTK_startShell();
						break;
				}
		}
}

bool listselctCB(void *inst,void *userdata)
{
	char						*buffer=(char*)alloca(256);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);
	std::string					itemstr="";

	sprintf(buffer,"List %i List item '%s' clicked, user data=%p.",(long)userdata,ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);

	if(ls->CTK_getMultipleSelect()==true)
		{
			std::vector<bool>	sels=ls->CTK_getSelections();
			for(unsigned j=0;j<sels.size();j++)
				{
					if(sels[j]==true)
					{
						itemstr+=ls->listItems[j]->label;
						itemstr+=":";
					}
				}
			sprintf(buffer,"List %i List items '%s' clicked, user data=%p.",(long)userdata,itemstr.c_str(),ls->listItems[ls->listItemNumber]->userData);
		}
	resulttextbox->CTK_updateText(buffer);
}

bool checkselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);

	cb->CTK_setValue(!cb->CTK_getValue());
	if((long)userdata==CHECK1)
		{
			mainApp->menuBar->CTK_setMenuBarEnable(!cb->CTK_getValue());
			mainApp->menuBar->enableShortcuts=!cb->CTK_getValue();
		}

	if((long)userdata==CHECK2)
		mainApp->menuBar->menuNames[1]->menuEnabled=!cb->CTK_getValue();

	if((long)userdata==CHECK3)
		{
			mainApp->menuBar->CTK_setMenuBarVisible(true,!cb->CTK_getValue());
			mainApp->CTK_clearScreen();
			mainApp->CTK_updateScreen(mainApp,NULL);
		}

	sprintf(buffer,"CheckBox '%s' clicked ... Value=%i",cb->label,cb->CTK_getValue());
	mainApp->menuBar->CTK_drawDefaultMenuBar();
	resultbuttonstextbox->CTK_updateText(buffer);
}

bool dropboxCB(void *inst,void *userdata)
{
	char				*buffer=(char*)alloca(256);
	CTK_cursesDropClass	*db=static_cast<CTK_cursesDropClass*>(inst);

	sprintf(buffer,"Drop box item=%i label=%s",db->selectedItem,db->label.c_str());
	resultbuttonstextbox->CTK_updateText(buffer);
}

bool pagekeyCB(CTK_mainAppClass *app,void *userdata)
{
	char				*buffer=(char*)alloca(256);
	//fprintf(stderr,"Key scancode >>%s<<\nspecial key name=%i\nUserData=%p\n",app->readKey->inputBuffer.c_str(),app->readKey->specialKeyName,userdata);
	if(app->readKey->specialKeyName==CTK_KEY_ENTER)
		{
			fprintf(stderr,"I handled this key\n");
			return(true);
		}

	if(app->readKey->specialKeyName==CTK_KEY_RIGHT)
		{
			progressPulse1->CTK_setValue(progressPulse1->CTK_getValue()+1.0);
			progressPulse1->gadgetDirty=true;


			progressStatic->CTK_setValue(progressStatic->CTK_getValue()+1.0);
			progressStatic->gadgetDirty=true;
			progressIndicator->CTK_setValue(progressIndicator->CTK_getValue()+0.25);
			progressIndicator->gadgetDirty=true;
			mainApp->CTK_updateScreen(mainApp,NULL);
	//fprintf(stderr,"value=%f\n",progress->value);
			sprintf(buffer,"Progress bar value=%f",progressStatic->CTK_getValue());
			resultbuttonstextbox->CTK_updateText(buffer);
			return(true);
		}
	if(app->readKey->specialKeyName==CTK_KEY_LEFT)
		{
			progressPulse1->CTK_setValue(progressPulse1->CTK_getValue()-1.0);
			progressPulse1->gadgetDirty=true;

			progressStatic->CTK_setValue(progressStatic->CTK_getValue()-1.0);
			progressStatic->gadgetDirty=true;
			progressIndicator->CTK_setValue(progressIndicator->CTK_getValue()-0.25);
			progressIndicator->gadgetDirty=true;
			mainApp->CTK_updateScreen(mainApp,NULL);
	//fprintf(stderr,"value=%f\n",progress->value);
			sprintf(buffer,"Progress bar value=%f",progressStatic->CTK_getValue());
			resultbuttonstextbox->CTK_updateText(buffer);

			return(true);
		}
	return(false);
}

int main(int argc, char **argv)
{
	CTK_cursesButtonClass			*button;
	CTK_cursesTextBoxClass			*textbox;
	CTK_cursesInputClass			*input;
//	CTK_cursesListBoxClass			*list;
	CTK_cursesCheckBoxClass			*checkbox;
	CTK_cursesLabelClass			*label;

	const char	*menuNames[]={"File","Edit","Tabs","Help",NULL};
	const char	*fileMenuNames[]={" _New"," _Open"," _Save"," Save _As"," _Close"," _Quit",NULL};
	const char	*editMenuNames[]={" _Copy Word"," C_ut Word"," Copy _Line"," Cut L_ine"," _Paste",NULL};
	const char	*tabMenuNames[]={" _Instructions "," _Edit Box"," _Code Box"," _Lists"," L_abels"," I_nput Box"," _Buttons"," P_rogress Bars"," _Dialogs"," _Next Tab"," _Prev Tab",NULL};
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
\n\n\
List two is a multiple selection list, use SPACE to toggle.\n\
The check boxes are the same.\n\
Both lists and checkboxes can have the 'select' key set by the caller.\n\
\n\n\
Drop boxes act the same as menus once selcted in the normal way\n\
";

	int	genx;
	int	geny;
	int genw;
	int	genh;

//system("setterm --background white");
//mainApp->windowColours.backCol=BACK_BOLD_WHITE;
	mainApp->CTK_setTabWidth(TABWIDTH);

	CTK_cursesUtilsClass	cu;
	cu.CTK_splashScreen(mainApp,"Basic example of CTK gadgets.\nThis is the simple non-blocking splash screen.\nShould be used if your app takes a while to start up.\nIt will disappear in 2 seconds");
//	sleep(2);
//	mainApp->CTK_mainEventLoop(-2000);

	mainApp->CTK_addNewMenuBar();

	mainApp->menuBar->CTK_addMenuToBar(menuNames[0]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[1]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[2]);
	mainApp->menuBar->CTK_addMenuToBar(menuNames[3]);

	int cnt=0;
	while(fileMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(0,fileMenuNames[cnt++]);

	cnt=0;
	while(editMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(1,editMenuNames[cnt++]);

	cnt=0;
	while(tabMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(2,tabMenuNames[cnt++]);

	cnt=0;
	while(helpMenuNames[cnt]!=NULL)
		mainApp->menuBar->CTK_addMenuItem(3,helpMenuNames[cnt++],false);

	char	buffer[2048];
	for(int j=0;j<100;j++)
		{
			sprintf(buffer," Really long menu, menu item %i",j);
			mainApp->menuBar->CTK_addMenuItem(3,buffer,false);
		}
	mainApp->menuBar->CTK_setSelectCB(menuselctCB,NULL);
	for(int j=2;j<10;j++)
		mainApp->menuBar->menuNames[3]->menuItem[j]->menuEnabled=false;

//page 0
//instructions
	genx=3;
	geny=3;
	genw=mainApp->maxCols-4;
	genh=mainApp->maxRows-9;

	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Instructions");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;
	helptextbox=mainApp->CTK_addNewTextBox(genx,geny,genw,genh,sampletxt);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,1,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 1
//edit box
	mainApp->CTK_addPage();
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Edit Box");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;
	editbox=mainApp->CTK_addNewEditBox(mainApp,genx,geny,genw,genh,true,"../ChangeLog");
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 2 //TODO//
//source code box
	mainApp->CTK_addPage();
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Code Editor Box");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;
	bool usenativehiliter=true;
//	srceditbox=mainApp->CTK_addNewSourceEditBox(mainApp,genx,geny,genw,genh,true,"../CursesToolKit/src/cursesApplication.cpp",usenativehiliter);
	srceditbox=mainApp->CTK_addNewSourceEditBox(mainApp,genx,geny,genw,genh,true,"../CursesToolKit/src/cursesApplication.cpp");
	srceditbox->CTK_setShowLineNumbers(4);
	srceditbox->liveUpdate=true;
	srceditbox->CTK_setUseNative(usenativehiliter);
	//srceditbox->CTK_setStyleFile("./esc.style");
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 3
//list boxes
	mainApp->CTK_addPage();
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"List Box's");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;
//normal list box
	list1=mainApp->CTK_addNewListBox(genx,geny,genw/2,genh-3);
	list1->CTK_addListItem("Item 0");
	list1->CTK_addListItem("Item 1");
	list1->CTK_addListItem("Item 2");
	list1->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	list1->CTK_addListItem("Item 4");
	list1->CTK_addListItem("Item 5");
	list1->CTK_addListItem("Item 6");
	list1->CTK_addListItem("Item 7 is a really really really really really really really really really really really really really long line and gets truncated",(void*)7);
	list1->CTK_addListItem("Item 8");
	list1->CTK_addListItem("Item 9");
	list1->CTK_addListItem("Item 10");
	list1->CTK_addListItem("Item 11");
	list1->CTK_addListItem("Item 12");
	list1->CTK_addListItem("Item 13");
	list1->CTK_addListItem("Item 14");
	list1->CTK_addListItem("Item 15");
	list1->CTK_addListItem("Item 16");
	list1->CTK_addListItem("Item 17");
	list1->CTK_addListItem("Item 18");
	list1->CTK_addListItem("Item 19");
	list1->CTK_addListItem("Item 20");
	list1->CTK_addListItem("Item 21");
	list1->CTK_addListItem("Item 22");
	list1->CTK_addListItem("Item 23");
	list1->CTK_addListItem("Item 24");
	list1->CTK_addListItem("Item 25");
	list1->CTK_addListItem("Item 26");
	list1->CTK_addListItem("Item 27");
	list1->CTK_addListItem("Item 28");
	list1->CTK_addListItem("Item 29");
	list1->CTK_addListItem("Item 30");
	list1->CTK_setSelectCB(listselctCB,(void*)1);
	list1->CTK_selectItem(list1->CTK_findByLabel("Item 10"));
	
//multi select list
	list2=mainApp->CTK_addNewListBox(genx+(genw/2)+3,geny,(genw/2)-2,genh-3);
	list2->CTK_addListItem("Item 1");
	list2->CTK_addListItem("Item 2");
	list2->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	list2->CTK_addListItem("Item 4");
	list2->CTK_addListItem("Item 5");
	list2->CTK_addListItem("Item 6");
	list2->CTK_addListItem("Item 7 is a really really really really really really really really really really really really really long line and gets truncated",(void*)7);
	list2->CTK_addListItem("Item 8");
	list2->CTK_addListItem("Item 9");
	list2->CTK_addListItem("Item 10");
	list2->CTK_addListItem("Item 11");
	list2->CTK_addListItem("Item 12");
	list2->CTK_addListItem("Item 13");
	list2->CTK_addListItem("Item 14");
	list2->CTK_addListItem("Item 15");
	list2->CTK_addListItem("Item 16");
	list2->CTK_setSelectCB(listselctCB,(void*)2);

	list2->CTK_setMultipleSelect(true);
	list2->CTK_setSelectDeselects(false);
	list2->CTK_setItem(3,true);	

	resulttextbox=mainApp->CTK_addNewTextBox(genx,geny+genh-1,genw,1,"Result");
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 4
//labels
	mainApp->CTK_addPage();
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Labels");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,2,"Default justified non selectable label.\nLine 2 of label.");
	label->gadgetColours.boxType=PLAINBOX;
	label=mainApp->CTK_addNewLabel(genx,geny+=4,genw,2,"Centred non selectable label.\nLine 2 of label.");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=PLAINBOX;
	label=mainApp->CTK_addNewLabel(genx,geny+=4,genw,2,"Right justified non selectable label.\nLine 2 of label.");
	label->CTK_setJustify(RIGHTJUSTIFY);
	label->gadgetColours.boxType=PLAINBOX;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	geny=6;
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 4
//input box
	mainApp->CTK_addPage();
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Input Box");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;
	input=mainApp->CTK_addNewInput(genx,geny,genw,1,"Some input");
	input->redrawAppWindow=false;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 5
//buttons
	mainApp->CTK_addPage();
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Buttons, space toggles check boxs, use arrow keys to set progress box value, TAB/SHIFT TAB to select control.");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;

//general buttons
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,4,12,0);
	button=mainApp->CTK_addNewButton(genx,geny,12,1," Lists  ");
	button->CTK_setSelectCB(buttonselctCB,(void*)BUTTON1);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,4,13,1);
	button=mainApp->CTK_addNewButton(genx,geny,12,1,"Shell");
	button->CTK_setSelectCB(buttonselctCB,(void*)BUTTON3);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,4,13,2);
	button=mainApp->CTK_addNewButton(genx,geny,12,1,"  Help  ");
	button->gadgetColours.foreCol=FORE_BLACK;
	button->gadgetColours.backCol=BACK_GREEN;
	button->CTK_setSelectCB(buttonselctCB,(void*)BUTTON2);

	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,4,13,3);
	button=mainApp->CTK_addNewButton(genx,geny,12,1,"Disabled");
	button->CTK_setEnabled(false);

//drop box
	geny+=3;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,1,19,0);
	dropdown=mainApp->CTK_addNewDropDownBox(mainApp,genx,geny,15,1,"Drop Label     ");
	dropdown->CTK_addDropItem("Hide Status Bar");
	dropdown->CTK_addDropItem("Show Status Bar");
	dropdown->CTK_addDropItem("Item 3         ");
	dropdown->CTK_setSelectCB(dropboxCB,(void*)0xdeadbeef);
	dropdown->CTK_setItemEnabled(2,false);
	dropdown->CTK_setSelectDeselects(false);

//check box
	geny+=3;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,3,13,0);
	checkbox=mainApp->CTK_addNewCheckBox(genx,geny,10,"Menus Off");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)CHECK1);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(' ');
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,3,13,1);
	checkbox=mainApp->CTK_addNewCheckBox(genx,geny,10,"Menus Vis");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)CHECK3);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(' ');
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,3,13,2);
	checkbox=mainApp->CTK_addNewCheckBox(genx,geny,10,"Edit Off ");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)CHECK2);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(' ');

//results
	genx=3;
	geny=6;
	resultbuttonstextbox=mainApp->CTK_addNewTextBox(genx,geny+genh-1,genw,1,"Result");

	geny=6;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 6
//progress bars
	mainApp->CTK_addPage();
	mainApp->pages[mainApp->pageNumber].pageKey=pagekeyCB;
	mainApp->pages[mainApp->pageNumber].userData=(void*)0xdeadbeef;
	geny=3;
	genx=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Progress bars, use arrow keys to set progress box value.");
	label->CTK_setJustify(CENTREJUSTIFY);
	label->gadgetColours.boxType=0;
	geny+=3;

//sliders
//pulsing
	label=mainApp->CTK_addNewLabel(3,geny,mainApp->maxCols/4,1,"Pulsing chars, No fill, 50%.");
	label->gadgetColours.boxType=NOBOX;
	genx=mainApp->utils->CTK_getGadgetPos(0,mainApp->maxCols,3,mainApp->maxCols/3,2);
	progressPulse1=mainApp->CTK_addNewProgressBar(genx,geny,mainApp->maxCols/3,0.0,20,10.0);
	progressPulse1->CTK_setFillStyle(PULSE);
	progressPulse1->CTK_setPulseStyle(false,true,"#*");
	progressPulse1->CTK_setShowValues(SHOWMINMAX);
geny+=3;
//pulsing
	label=mainApp->CTK_addNewLabel(3,geny,mainApp->maxCols/4,1,"Pulsing colours, no chars, No fill, 50%.");
	label->gadgetColours.boxType=NOBOX;
	genx=mainApp->utils->CTK_getGadgetPos(0,mainApp->maxCols,3,mainApp->maxCols/3,2);
	progressPulse=mainApp->CTK_addNewProgressBar(genx,geny,mainApp->maxCols/3,0.0,20,10.0);
	progressPulse->CTK_setFillStyle(PULSE);
	progressPulse->CTK_setPulseStyle(true,false,"  ");
	progressPulse->CTK_setShowRealValue(true);
	progressPulse->CTK_setShowValues(SHOWVALUE);
	geny+=3;
//pulsing
	label=mainApp->CTK_addNewLabel(3,geny,mainApp->maxCols/4,1,"Pulsing chars, Filled, 50%.");
	label->gadgetColours.boxType=NOBOX;
	genx=mainApp->utils->CTK_getGadgetPos(0,mainApp->maxCols,3,mainApp->maxCols/3,2);
	progressPulse=mainApp->CTK_addNewProgressBar(genx,geny,mainApp->maxCols/3,0.0,20,10.0);
	progressPulse->CTK_setFillStyle(FILLEDPULSE);
	progressPulse->CTK_setPulseStyle(false,true,"#*");
	progressPulse->CTK_setShowValues(SHOWALL);
	geny+=3;
//pulsing
	label=mainApp->CTK_addNewLabel(3,geny,mainApp->maxCols/4,1,"Pulsing colours + chars, 100%.");
	label->gadgetColours.boxType=NOBOX;
	genx=mainApp->utils->CTK_getGadgetPos(0,mainApp->maxCols,3,mainApp->maxCols/3,2);
	progressPulse=mainApp->CTK_addNewProgressBar(genx,geny,mainApp->maxCols/3,0.0,20,20.0);
	progressPulse->CTK_setFillStyle(PULSE);
	progressPulse->CTK_setPulseStyle(true,true);
	geny+=3;
//static
	label=mainApp->CTK_addNewLabel(3,geny,mainApp->maxCols/4,1,"Static, No fill, 50%.");
	label->gadgetColours.boxType=NOBOX;
	genx=mainApp->utils->CTK_getGadgetPos(0,mainApp->maxCols,3,mainApp->maxCols/3,2);
	progressStatic=mainApp->CTK_addNewProgressBar(genx,geny,mainApp->maxCols/3,200.0,400.0,300.0);
	progressStatic->CTK_setFillStyle(BAR);
	progressStatic->CTK_setScale(1);
	progressStatic->CTK_setShowRealValue(true);
	progressStatic->CTK_setShowValues(SHOWALL);
	progressStatic->CTK_setShowValuesAsTime(true);
	geny+=3;
//indicator
	label=mainApp->CTK_addNewLabel(3,geny,mainApp->maxCols/4,1,"Static, Slider, Filled, 50%.");
	label->gadgetColours.boxType=NOBOX;
	genx=mainApp->utils->CTK_getGadgetPos(0,mainApp->maxCols,3,mainApp->maxCols/3,2);
	progressIndicator=mainApp->CTK_addNewProgressBar(genx,geny,mainApp->maxCols/3,0.0,130.0,0.0);
	progressIndicator->showHours=true;
	progressIndicator->CTK_setFillStyle(FILLEDINDICATOR);
	progressIndicator->CTK_setShowRealValue(false);
	progressIndicator->CTK_setShowValues(SHOWGAUGE);
	progressIndicator->CTK_setShowValuesAsTime(true);
	progressIndicator->CTK_setScale(0);
	geny+=3;

//results
	genx=3;
	geny=6;
	resultbuttonstextbox=mainApp->CTK_addNewTextBox(genx,geny+genh-1,genw,1,"Result");
	geny=6;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//page 6
//dialogs with back window
	mainApp->CTK_addPage();
	mainApp->CTK_setDialogWindow("Fancy Window","Dialog  Name",80,20);
	CURRENTPAGE(mainApp).fancyWindow=true;
	CURRENTPAGE(mainApp).windowName="Fancy Window";
	CURRENTPAGE(mainApp).dialogName="Dialog  Name";

	genx=CURRENTPAGE(mainApp).boxX+2;
	geny=CURRENTPAGE(mainApp).boxY+2;
	genw=CURRENTPAGE(mainApp).boxW-3;
	genh=CURRENTPAGE(mainApp).boxH;

	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Dialog Information ...");
	label->CTK_setJustify(CENTREJUSTIFY);

	genx=mainApp->utils->CTK_getGadgetPosX(CURRENTPAGE(mainApp).boxX,CURRENTPAGE(mainApp).boxW,1,13,0);
	button=mainApp->CTK_addNewButton(genx,CURRENTPAGE(mainApp).boxY+CURRENTPAGE(mainApp).boxH-1,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
//fin
	mainApp->CTK_setPage(0);
	mainApp->menuBar->CTK_setMenuShortCut(FILEMENU,QUITITEM,'Q');
	mainApp->menuBar->CTK_setMenuShortCut(TABMENU,NEXTTAB,'N');
	mainApp->menuBar->CTK_setMenuShortCut(TABMENU,PREVTAB,'P');

	mainApp->CTK_setDefaultGadget(helptextbox);
	mainApp->CTK_setDefaultGadget(helptextbox);//TODO// horible hack!
	mainApp->CTK_setDefaultGadget(helptextbox);//TODO// horible hack!

	if(false)
		mainApp->CTK_mainEventLoop(0,true,true);
	else
		{
			while(doMainLoop==true)
				{
					mainApp->CTK_mainEventLoop(-250,false);
					doPulseBar();
				}
		}

	delete mainApp;
	SETSHOWCURS;
	printf(NORMCHARSET);
	return 0;
}
