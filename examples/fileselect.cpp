#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey) -lcursestoolkit "$0"  -o fileselect ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./fileselect "$@"
retval=$?
rm fileselect
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#include "LFSTKFindClass.h"

#include "cursesApplication.h"

CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
LFSTK_findClass			*files=new LFSTK_findClass();
CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
CTK_cursesCheckBoxClass	*hidden=new CTK_cursesCheckBoxClass();
char					*infolder=NULL;
bool					isValid=false;
char					*lastSelectedItem=NULL;
char					*lastSelectedPath=NULL;

void buttonselctCB(void *inst)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);

	if(strcmp(bc->label,"  OK  ")==0)
		isValid=true;

	fprintf(stderr,"Button '%s' clicked.\n",bc->label);
	mainApp->runEventLoop=false;
}

void listselctCB(void *inst)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

	fprintf(stderr,"List item '%s' clicked, user data=%p.\n",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
	if(files->data[ls->listItemNumber].fileType==FOLDERTYPE)
		{
			sprintf(buffer,"%s/%s",infolder,files->data[ls->listItemNumber].name.c_str());
			chdir(buffer);
			infolder=get_current_dir_name();
			files->LFSTK_findFiles(infolder);
			files->LFSTK_setSort(false);
			files->LFSTK_sortByTypeAndName();

			lb->CTK_clearList();
			for(int j=0;j<files->data.size();j++)
				{
					if(files->data[j].fileType==FOLDERTYPE)
						sprintf(buffer,"%s/",files->data[j].name.c_str());
					else
						sprintf(buffer,"%s",files->data[j].name.c_str());
					lb->CTK_addListItem(buffer,NULL);
				}
		}
	else
		{
			freeAndNull(&lastSelectedItem);
			freeAndNull(&lastSelectedPath);
			asprintf(&lastSelectedItem,"%s",ls->listItems[ls->listItemNumber]->label.c_str());
			asprintf(&lastSelectedPath,"%s",files->data[ls->listItemNumber].path.c_str());
			sprintf(buffer,"File: %s",lastSelectedPath);
			mainApp->pages[0].textBoxes[0]->CTK_updateText(buffer);
		}
}

void checkselctCB(void *inst)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);

	cb->CTK_setValue(!cb->CTK_getValue());

	files->LFSTK_setIncludeHidden(cb->CTK_getValue());
	files->LFSTK_findFiles(infolder);
	files->LFSTK_setSort(false);
	files->LFSTK_sortByTypeAndName();

	lb->CTK_clearList();
	for(int j=0;j<files->data.size();j++)
		{
			if(files->data[j].fileType==FOLDERTYPE)
				sprintf(buffer,"%s/",files->data[j].name.c_str());
			else
				sprintf(buffer,"%s",files->data[j].name.c_str());
			lb->CTK_addListItem(buffer,NULL);
		}
}

void mainloopCB(void *mainc,void *data)
{
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

int main(int argc, char **argv)
{
	char	*buffer=(char*)alloca(PATH_MAX);

	infolder=get_current_dir_name();

	lb->CTK_newListBox(2,2,mainApp->maxCols-2,mainApp->maxRows-6);

	files->LFSTK_setFindType(ANYTYPE);
	files->LFSTK_setFullPath(true);
	files->LFSTK_findFiles(infolder);
	files->LFSTK_setSort(false);
	files->LFSTK_sortByTypeAndName();

	for(int j=0;j<files->data.size();j++)
		{
			if(files->data[lb->listItemNumber].fileType==FOLDERTYPE)
				{
					if(files->data[j].fileType==FOLDERTYPE)
						sprintf(buffer,"%s/",files->data[j].name.c_str());
					else
						sprintf(buffer,"%s",files->data[j].name.c_str());
					
					lb->CTK_addListItem(buffer,NULL);
				}
		}

	lb->CTK_setSelectCB(listselctCB);
	lb->CTK_setEnterDeselects(false);

	mainApp->CTK_addListBox(lb);
	mainApp->CTK_addNewTextBox(2,mainApp->maxRows-3,mainApp->maxCols-2,1,"File:",false);

	mainApp->CTK_addNewButton(2,mainApp->maxRows-1,1,1,"  OK  ");
	mainApp->pages[0].buttons[0]->CTK_setSelectCB(buttonselctCB);
	mainApp->CTK_addNewButton(mainApp->maxCols-2-4,mainApp->maxRows-1,11,1,"CANCEL");
	mainApp->pages[0].buttons[1]->CTK_setSelectCB(buttonselctCB);

	mainApp->CTK_addNewCheckBox((mainApp->maxCols/2)-8,mainApp->maxRows-1,14,"Show Hidden");
	mainApp->pages[0].checkBoxes[0]->CTK_setSelectCB(checkselctCB);
	mainApp->pages[0].checkBoxes[0]->CTK_setEnterDeselects(false);

	mainApp->eventLoopCB=mainloopCB;
	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;
	SETNORMAL;

	free(infolder);
	MOVETO(1,mainApp->maxRows-3);

	if(isValid==true)
		{
			//printf("\nFile '%s' selected.\n",lb->listItems[lb->listItemNumber]->label.c_str());
			printf("\nFile '%s' selected.\n",lastSelectedItem);
			//printf("\nFile '%s' selected.\n",mainApp->pages[0].textBoxes[0]->CTK_getText().c_str());
//			printf("Fullpath: %s\n",files->data[lb->listItemNumber].path.c_str());
			printf("Fullpath: %s\n",lastSelectedPath);
			freeAndNull(&lastSelectedItem);
			freeAndNull(&lastSelectedPath);
		}

	delete files;
	delete mainApp;
	
	return 0;
}
