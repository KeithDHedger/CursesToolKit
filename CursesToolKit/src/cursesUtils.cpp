/*
 *
 * ©K. D. Hedger. Sat  6 Apr 17:05:25 BST 2019 keithdhedger@gmail.com

 * This file (cursesUtils.cpp) is part of CursesToolKit.

 * CursesToolKit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesToolKit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesToolKit.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "cursesUtils.h"
//#include "cursesGlobals.h"

CTK_cursesUtilsClass::~CTK_cursesUtilsClass()
{
}

CTK_cursesUtilsClass::CTK_cursesUtilsClass()
{
}

std::vector<std::string> CTK_cursesUtilsClass::CTK_explode(const std::string s,const char c)
{
	std::string buff;
	std::vector<std::string> v;

	for(unsigned int j=0;j<s.length();j++)
		{
			if(s.c_str()[j]!=c)
				buff+=s.c_str()[j];
			else
				{
					v.push_back(buff);
					buff="";
				}
		}

	if(buff!="")
		v.push_back(buff);

	return(v);
}

static void listselctCB(void *inst)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);

	fprintf(stderr,"List item '%s' clicked, user data=%p.\n",ls->listItems[ls->listItemNumber]->label.c_str(),ls->listItems[ls->listItemNumber]->userData);

}

void CTK_cursesUtilsClass::runOpenFile(CTK_mainAppClass *app)
{
	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	CTK_cursesCheckBoxClass	*hidden=new CTK_cursesCheckBoxClass();
	LFSTK_findClass			*files=new LFSTK_findClass();
//	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	char					*buffer=(char*)alloca(256);

	this->inFolder=get_current_dir_name();

	lb->CTK_newListBox(2,2,app->maxCols-2,selectapp->maxRows-6);

	files->LFSTK_setFindType(ANYTYPE);
	files->LFSTK_setFullPath(true);
	files->LFSTK_findFiles(this->inFolder.c_str());
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

	selectapp->CTK_addListBox(lb);
	selectapp->CTK_addNewTextBox(2,selectapp->maxRows-3,selectapp->maxCols-2,1,"File:",false);

	selectapp->CTK_addNewButton(2,selectapp->maxRows-1,1,1,"  OK  ");
//	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonselctCB);
	selectapp->CTK_addNewButton(selectapp->maxCols-2-4,selectapp->maxRows-1,11,1,"CANCEL");
//	selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonselctCB);

	selectapp->CTK_addNewCheckBox((selectapp->maxCols/2)-8,selectapp->maxRows-1,14,"Show Hidden");
//	selectapp->pages[0].checkBoxes[0]->CTK_setSelectCB(checkselctCB);
	selectapp->pages[0].checkBoxes[0]->CTK_setEnterDeselects(false);

	//selectapp->eventLoopCB=mainloopCB;
	selectapp->CTK_mainEventLoop();
}

std::string CTK_cursesUtilsClass::CTK_openFile(CTK_mainAppClass *app)
{
	app->CTK_clearScreen();
	this->runOpenFile(app);
	return(this->inFolder);
}

