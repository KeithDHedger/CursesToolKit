#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey lfstk) -lcursestoolkit "$0" -o fileselect ||exit 1
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

#include <lfstk/LFSTKFindClass.h>

#include <cursesApplication.h>

CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
LFSTK_findClass			*fc=new LFSTK_findClass();
CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
char					*infolder=NULL;

void buttonselctCB(void *inst)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);

	fprintf(stderr,"Button '%s' clicked.\n",bc->label);
//	mainApp->textBoxes[1]->CTK_updateText(buffer);
	mainApp->runEventLoop=false;
	if(strcmp(bc->label,"  OK  ")==0)
		fprintf(stderr,"File '%s' selected.",lb->listItems[lb->listItemNumber]->label.c_str());
}

void listselctCB(void *inst)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

//	sprintf(buffer,"List item '%s' clicked, user data=%p.",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
	fprintf(stderr,"List item '%s' clicked, user data=%p.\n",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
//	mainApp->textBoxes[1]->CTK_updateText(buffer);
//	mainApp->runEventLoop=false;
//	if(strcmp(ls->listItems[ls->listItemNumber]->label.c_str(),"..")==0)
	if(fc->data[ls->listItemNumber].fileType==FOLDERTYPE)
		{
			//sprintf(buffer,"%s/../",infolder);
			sprintf(buffer,"%s/%s",infolder,fc->data[ls->listItemNumber].name.c_str());
			chdir(buffer);
			infolder=get_current_dir_name();
			fprintf(stderr,">>%s<<\n",infolder);
			fc->LFSTK_findFiles(infolder);
			fc->LFSTK_setSort(false);
			fc->LFSTK_sortByTypeAndName();

			lb->CTK_clearList();
			for(int j=0;j<fc->data.size();j++)
				{
					if(fc->data[j].fileType==FOLDERTYPE)
						sprintf(buffer,"%s/",fc->data[j].name.c_str());
					else
						sprintf(buffer,"%s",fc->data[j].name.c_str());
					lb->CTK_addListItem(buffer,NULL);
				}
		}
	else
		{
			sprintf(buffer,"File: %s",fc->data[ls->listItemNumber].path.c_str());
			//fprintf(stderr,"File: >>%s<<",fc->data[ls->listItemNumber].path.c_str());

			mainApp->textBoxes[0]->CTK_updateText(buffer);
		}
}

void mainloopCB(void *mainc,void *data)
{
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

int main(int argc, char **argv)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	infolder=get_current_dir_name();
	lb->CTK_newListBox(2,2,128,16);

	fc->LFSTK_setFindType(ANYTYPE);
	fc->LFSTK_setFullPath(true);
	fc->LFSTK_findFiles(infolder);
	fc->LFSTK_setSort(false);
	fc->LFSTK_sortByTypeAndName();
//listselctCB(lb);
	for(int j=0;j<fc->data.size();j++)
		{
			if(fc->data[lb->listItemNumber].fileType==FOLDERTYPE)
		{
			//sprintf(buffer,"%s/../",infolder);
		//	fc->LFSTK_findFiles(buffer);
		//	fc->LFSTK_setSort(false);
		//	fc->LFSTK_sortByTypeAndName();
//
		//	lb->CTK_clearList();
		//	for(int j=0;j<fc->data.size();j++)
			//	{
					if(fc->data[j].fileType==FOLDERTYPE)
						sprintf(buffer,"%s/",fc->data[j].name.c_str());
					else
						sprintf(buffer,"%s",fc->data[j].name.c_str());
					
					lb->CTK_addListItem(buffer,NULL);
			//	}
		}

//			fprintf(stderr,"%s\n",fc->data[j].name.c_str());
//			lb->CTK_addListItem(fc->data[j].name.c_str(),NULL);
		}
	lb->CTK_setSelectCB(listselctCB);
	lb->CTK_setEnterDeselects(false);

	mainApp->CTK_addListBox(lb);
	mainApp->CTK_addNewTextBox(2,18,128,1,"File:",false);

	mainApp->CTK_addNewButton(2,19,1,1,"  OK  ");
	mainApp->buttons[0]->CTK_setSelectCB(buttonselctCB);
	mainApp->CTK_addNewButton(124,19,11,1,"CANCEL");
	mainApp->buttons[1]->CTK_setSelectCB(buttonselctCB);

	mainApp->eventLoopCB=mainloopCB;
	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;
	free(infolder);
	delete fc;
	delete mainApp;
	
	return 0;
}
