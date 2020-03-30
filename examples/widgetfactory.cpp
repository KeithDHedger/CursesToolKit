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

name=$(basename $0 .cpp)

g++ -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++ ncurses) -lcursestoolkit "$0" -o $name ||exit 1
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

bool							mbarVis=true;
int								b1Cnt=0;
int								b2Cnt=0;

#define FILEMENU 0
#define QUITITEM 5
#define TABMENU 2
#define NEXTTAB 8
#define PREVTAB 9
#define TABWIDTH 4
#define	NEXTPAGE 2000
#define	PREVPAGE 2001
#define BUTTON1	4000
#define BUTTON2	4001
#define CHECK1 5000
#define CHECK2 5001

bool menuselctCB(void *inst,void *userdata)
{
	CTK_cursesMenuClass	*mc=static_cast<CTK_cursesMenuClass*>(inst);

	fprintf(stderr,"Menu=%i Item=%i\n",mc->menuNumber,mc->menuItemNumber);

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
							mainApp->CTK_setDefaultGadget(list1);
						break;
				}
		}

	if((mc->menuNumber==FILEMENU) && (mc->menuItemNumber==QUITITEM))//TODO//exit edit box;
		mainApp->runEventLoop=false;
}

void mainloopCBIn(void *mainc,void *data)
{
}

void mainloopCBOut(void *mainc,void *data)
{
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
						resultbuttonstextbox->CTK_updateText(buffer);
						srcbuffercopy=srceditbox->CTK_getBuffer();
						fprintf(stderr,">>%s<<",srcbuffercopy);
						free(srcbuffercopy);
						break;
					case BUTTON2:
						b2Cnt++;
						sprintf(buffer,"Button '%s' clicked, cnt %i.",bc->label,b2Cnt);
						resultbuttonstextbox->CTK_updateText(buffer);
						srcbuffercopy=editbox->CTK_getBuffer();
						fprintf(stderr,">>%s<<",srcbuffercopy);
						free(srcbuffercopy);
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
	const char	*tabMenuNames[]={" _Instructions "," _Edit Box"," _Code Box"," _Lists"," L_abels"," _Input Box"," _Buttons"," _Dialogs"," _Next Tab"," _Prev Tab",NULL};
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

	mainApp->CTK_setTabWidth(TABWIDTH);
	coloursStruct cs;
	cs.windowBackCol=BACK_WHITE;
	cs.fancyGadgets=true;

	CTK_cursesUtilsClass	cu;
	cu.CTK_splashScreen(mainApp,"Basic example of CTK gadgets.\nThis is the simple non-blocking splash screen.\nShould be used if your app takes a while to start up.\nIt will disappear in 2 seconds");
//	sleep(2);
//	mainApp->CTK_mainEventLoop(-2000);

//custom menu colours
	cs.menuHiliteBackCol=BACK_BLACK;
	cs.menuHiliteForeCol=FORE_GREEN;

	cs.menuForeCol=FORE_WHITE;
	cs.menuBackCol=BACK_RED;
	cs.disabledForeCol=FORE_BOLD_RED;
	mainApp->CTK_setColours(&cs,true);

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
//reset colours to defaults
	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	mainApp->CTK_setColours(&cs,false);

//page 0
//instructions
	genx=3;
	geny=3;
	genw=mainApp->maxCols-4;
	genh=mainApp->maxRows-9;

	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Instructions");
	label->CTK_setJustify(CENTREJUSTIFY);
	geny+=3;
	textbox=mainApp->CTK_addNewTextBox(genx,geny,genw,genh,sampletxt);
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
	geny+=3;
	editbox=mainApp->CTK_addNewEditBox(mainApp,genx,geny,genw,genh,true,"../ChangeLog");
	editbox->CTK_setShowLineNumbers(false);
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
	geny+=3;
	srceditbox=mainApp->CTK_addNewSourceEditBox(mainApp,genx,geny,genw,genh,true,"./widgetfactory.cpp");
	srceditbox->CTK_setShowLineNumbers(4);
	cs.backCol=BACK_BLACK;
	cs.foreCol=FORE_WHITE;
	cs.textBoxType=NOBOX;
	srceditbox->CTK_setColours(&cs,true);
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
	geny+=3;
//normal list box
	list1=mainApp->CTK_addNewListBox(genx,geny,genw/2,genh-3);
	list1->CTK_addListItem("Item 1");
	list1->CTK_addListItem("Item 2");
	list1->CTK_addListItem("Item 3",(void*)0xdeadbeef);
	list1->CTK_addListItem("Item 4");
	list1->CTK_addListItem("Item 5");
	list1->CTK_addListItem("Item 6");
	list1->CTK_addListItem("Item 7 is a really really really really really really really really really really really really really long line and gets truncated",(void*)7);
	list1->CTK_setSelectCB(listselctCB,(void*)1);
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

	cs.textBoxType=INBOX;
	cs.backCol=BACK_WHITE;
	cs.foreCol=FORE_BLACK;
	resulttextbox=mainApp->CTK_addNewTextBox(genx,geny+genh-1,genw,1,"Result");
	resulttextbox->CTK_setColours(&cs,true);
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
	geny+=3;
	label=mainApp->CTK_addNewLabel(genx,geny,genw,2,"Default justified non selectable label.\nLine 2 of label.");
	label=mainApp->CTK_addNewLabel(genx,geny+=4,genw,2,"Centred non selectable label.\nLine 2 of label.");
	label->CTK_setJustify(CENTREJUSTIFY);
	label=mainApp->CTK_addNewLabel(genx,geny+=4,genw,2,"Right justified non selectable label.\nLine 2 of label.");
	label->CTK_setJustify(RIGHTJUSTIFY);
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
	label=mainApp->CTK_addNewLabel(genx,geny,genw,1,"Buttons, Space toggles check boxs");
	label->CTK_setJustify(CENTREJUSTIFY);
	geny+=3;

//general buttons
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,12,0);
	button=mainApp->CTK_addNewButton(genx,geny,12,1,"Button 1");
	button->CTK_setSelectCB(buttonselctCB,(void*)BUTTON1);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny,12,1,"Button 2");
	button->CTK_setSelectCB(buttonselctCB,(void*)BUTTON2);

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
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	checkbox=mainApp->CTK_addNewCheckBox(genx,geny,10,"Menus Off");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)CHECK1);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(' ');
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	checkbox=mainApp->CTK_addNewCheckBox(genx,geny,10,"Edit Off ");
	checkbox->CTK_setSelectCB(checkselctCB,(void*)CHECK2);
	checkbox->CTK_setSelectDeselects(false);
	checkbox->CTK_setSelectKey(' ');

//results
	genx=3;
	geny=6;
	cs.textBoxType=INBOX;
	resultbuttonstextbox=mainApp->CTK_addNewTextBox(genx,geny+genh-1,genw,1,"Result");
	resultbuttonstextbox->CTK_setColours(&cs,true);

	geny=6;
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,0);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Prev Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)PREVPAGE);
	genx=mainApp->utils->CTK_getGadgetPosX(3,mainApp->maxCols-4,2,13,1);
	button=mainApp->CTK_addNewButton(genx,geny+genh+2,13,1,"Next Page");
	button->CTK_setSelectCB(buttonselctCB,(void*)NEXTPAGE);

//	label=mainApp->CTK_addNewLabel(3,geny-8+genh+2,genw,1,"Test Label");
//	label->CTK_setJustify(CENTREJUSTIFY);

//page 6
//dialogs with back window
	mainApp->CTK_addPage();
	mainApp->CTK_setDialogWindow("Fancy Window","Dialog  Name",-1,20);
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
	mainApp->eventLoopCBIn=mainloopCBIn;
	mainApp->eventLoopCBOut=mainloopCBOut;

	mainApp->CTK_setDefaultGadget(textbox);
	mainApp->CTK_mainEventLoop(0,true,true);

	delete mainApp;
	SETSHOWCURS;
	printf(NORMCHARSET);
	return 0;
}
