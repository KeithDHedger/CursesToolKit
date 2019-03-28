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

	sprintf(buffer,"Button '%s' clicked.",bc->label);
	mainApp->textBoxes[1]->CTK_updateText(buffer);
}

void listselctCB(void *inst)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

//	sprintf(buffer,"List item '%s' clicked, user data=%p.",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
	fprintf(stderr,"List item '%s' clicked, user data=%p.\n",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);
//	mainApp->textBoxes[1]->CTK_updateText(buffer);
//	mainApp->runEventLoop=false;
	if(strcmp(ls->listItems[ls->listItemNumber]->label.c_str(),"..")==0)
		{
			sprintf(buffer,"%s/../",infolder);
			chdir(buffer);
			infolder=get_current_dir_name();
			fc->LFSTK_findFiles(buffer);
			lb->CTK_clearList();
			for(int j=0;j<fc->data.size();j++)
				{
					fprintf(stderr,"%s\n",fc->data[j].name.c_str());
					lb->CTK_addListItem(fc->data[j].name.c_str(),NULL);
				}
		}
}

void mainloopCB(void *mainc,void *data)
{
	//fprintf(stderr,">>%p %p<<\n",mainc,data);
}

int main(int argc, char **argv)
{
	infolder=get_current_dir_name();
	lb->CTK_newListBox(2,2,80,16);

	fc->LFSTK_setFindType(ANYTYPE);
	fc->LFSTK_findFiles(infolder);
	for(int j=0;j<fc->data.size();j++)
		{
			fprintf(stderr,"%s\n",fc->data[j].name.c_str());
			lb->CTK_addListItem(fc->data[j].name.c_str(),NULL);
		}
	lb->CTK_setSelectCB(listselctCB);
	
	mainApp->CTK_addListBox(lb);

	mainApp->eventLoopCB=mainloopCB;
	mainApp->CTK_mainEventLoop();
	SETSHOWCURS;
	free(infolder);
	delete fc;
	delete mainApp;
	
	return 0;
}
