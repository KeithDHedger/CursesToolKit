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

static void listSelectCB(void *inst,void *ud)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);
	fileUDStruct				*fud=static_cast<fileUDStruct*>(ud);

	fud->isValid=false;
	if((fud->find->data[ls->listItemNumber].fileType==FOLDERTYPE) || (fud->find->data[ls->listItemNumber].fileType==FOLDERLINKTYPE))
		{
			sprintf(buffer,"%s/%s",fud->inst->inFolder.c_str(),fud->find->data[ls->listItemNumber].name.c_str());
			chdir(buffer);
			fud->inst->inFolder=get_current_dir_name();
			fud->find->LFSTK_findFiles(fud->inst->inFolder.c_str());
			fud->find->LFSTK_setSort(false);
			fud->find->LFSTK_sortByTypeAndName();

			ls->CTK_clearList();
			for(int j=0;j<fud->find->data.size();j++)
				{
					if((fud->find->data[j].fileType==FOLDERTYPE) || (fud->find->data[j].fileType==FOLDERLINKTYPE))
						sprintf(buffer,"%s/",fud->find->data[j].name.c_str());
					else
						sprintf(buffer,"%s",fud->find->data[j].name.c_str());
					ls->CTK_addListItem(buffer,NULL);
				}
		}
	else
		{
			if(fud->boolVal1==true)
				{
					sprintf(buffer,"File: %s",fud->find->data[ls->listItemNumber].path.c_str());
					fud->app->pages[0].textBoxes[0]->CTK_updateText(buffer);
				}
			else
				{
					fud->app->pages[0].inputs[0]->CTK_setText(fud->find->data[ls->listItemNumber].name.c_str());
				}
			fud->isValid=true;
		}
}

static void buttonSelectCB(void *inst,void *ud)
{
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	fileUDStruct			*fud=static_cast<fileUDStruct*>(ud);

	if(strcmp(bc->label,"CANCEL")==0)
		fud->isValid=false;
	if((strcmp(bc->label,"  OK  ")==0) && (fud->boolVal1==false))
		fud->isValid=true;
	fud->app->runEventLoop=false;
}

void checkSelectCB(void *inst,void *ud)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);
	fileUDStruct			*fud=static_cast<fileUDStruct*>(ud);

	cb->CTK_setValue(!cb->CTK_getValue());

	fud->find->LFSTK_setIncludeHidden(cb->CTK_getValue());
	fud->find->LFSTK_findFiles(fud->inst->inFolder.c_str());
	fud->find->LFSTK_setSort(false);
	fud->find->LFSTK_sortByTypeAndName();

	fud->list->CTK_clearList();
	for(int j=0;j<fud->find->data.size();j++)
		{
			if(fud->find->data[j].fileType==FOLDERTYPE)
				sprintf(buffer,"%s/",fud->find->data[j].name.c_str());
			else
				sprintf(buffer,"%s",fud->find->data[j].name.c_str());
			fud->list->CTK_addListItem(buffer,NULL);
		}
}

bool CTK_cursesUtilsClass::runOpenFile(CTK_mainAppClass *app,bool open)
{
	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	LFSTK_findClass			*files=new LFSTK_findClass();
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	char					*buffer=(char*)alloca(256);
	fileUDStruct			*fud=new fileUDStruct;
	bool					retval=false;
	coloursStruct			cs;

	cs.fancyGadgets=true;
	cs.hiliteBackCol=BACK_CYAN;
	cs.hiliteForeCol=FORE_BLACK;
	cs.foreCol=FORE_BLACK;
	cs.backCol=BACK_WHITE;
	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=INBOX;
	selectapp->CTK_setColours(cs);
	selectapp->CTK_clearScreen();
	lb->CTK_setColours(cs);

	fud->find=files;
	fud->app=selectapp;
	fud->inst=this;
	fud->list=lb;
	fud->isValid=false;
	fud->boolVal1=open;

	lb->CTK_newListBox(3,2,app->maxCols-4,selectapp->maxRows-7);

	files->LFSTK_setFollowLinks(true);
	files->LFSTK_setFindType(ANYTYPE);
	files->LFSTK_setFullPath(true);
	files->LFSTK_findFiles(this->inFolder.c_str());
	files->LFSTK_setSort(false);
	files->LFSTK_sortByTypeAndName();

	for(int j=0;j<files->data.size();j++)
		{
			if((files->data[lb->listItemNumber].fileType==FOLDERTYPE) || (files->data[j].fileType==FOLDERLINKTYPE))
				{
					if((files->data[j].fileType==FOLDERTYPE) || (files->data[j].fileType==FOLDERLINKTYPE))
						sprintf(buffer,"%s/",files->data[j].name.c_str());
					else
						sprintf(buffer,"%s",files->data[j].name.c_str());
					
					lb->CTK_addListItem(buffer,NULL);
				}
		}

	lb->CTK_setSelectCB(listSelectCB,fud);
	lb->CTK_setEnterDeselects(false);

	selectapp->CTK_addListBox(lb);
	if(open==true)
		selectapp->CTK_addNewTextBox(3,selectapp->maxRows-3,selectapp->maxCols-4,1,"File:",false);
	else
		selectapp->CTK_addNewInput(3,selectapp->maxRows-3,selectapp->maxCols-4,1,"");

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_addNewButton(3,selectapp->maxRows-1,1,1,"  OK  ");
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,fud);
	selectapp->CTK_addNewButton(selectapp->maxCols-6-5,selectapp->maxRows-1,11,1,"CANCEL");
	selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonSelectCB,fud);

	selectapp->CTK_addNewCheckBox((selectapp->maxCols/2)-10,selectapp->maxRows-1,14,"Show Hidden");
	selectapp->pages[0].checkBoxes[0]->CTK_setSelectCB(checkSelectCB,fud);
	selectapp->pages[0].checkBoxes[0]->CTK_setEnterDeselects(false);

	selectapp->CTK_mainEventLoop();
	if(fud->isValid==true)
		{
			retval=true;
			if(fud->boolVal1==true)
				this->results=files->data[lb->listItemNumber].path;
			else
				this->results=selectapp->pages[0].inputs[0]->CTK_getText();
		}
	delete fud;
	delete files;
	delete selectapp;
	
	return(retval);
}

void CTK_cursesUtilsClass::CTK_openFile(CTK_mainAppClass *app,const char *startdir,bool open)
{
	char	*folder=NULL;

	app->CTK_clearScreen();
	if(startdir==NULL)
		{
			folder=get_current_dir_name();
			this->inFolder=folder;
		}
	else
		this->inFolder=startdir;	
	this->isValidFile=this->runOpenFile(app,open);
	app->CTK_clearScreen();
	freeAndNull(&folder);
}

bool CTK_cursesUtilsClass::CTK_entryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *defaulttxt,bool hascancel)
{
	fileUDStruct		*fud=new fileUDStruct;
	coloursStruct		cs;
	CTK_mainAppClass	*selectapp=new CTK_mainAppClass();

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=false;
	fud->boolVal1=false;

	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_addNewTextBox((selectapp->maxCols/2)-22,(selectapp->maxRows/2)-6,44,10,"",false);

	selectapp->CTK_addNewTextBox((selectapp->maxCols/2)-20,(selectapp->maxRows/2)-6,40,5,bodytxt,false);
	cs.textBoxType=NOBOX;
	selectapp->pages[0].textBoxes[1]->CTK_setColours(cs);
	selectapp->CTK_addNewInput((selectapp->maxCols/2)-20,(selectapp->maxRows/2),40,1,defaulttxt);

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;
	selectapp->CTK_addNewButton((selectapp->maxCols/2)-19,(selectapp->maxRows/2)+2,6,1,"  OK  ");
	selectapp->pages[0].buttons[0]->CTK_setColours(cs);
	if(hascancel==true)
		{
			selectapp->CTK_addNewButton((selectapp->maxCols/2)+20-11,(selectapp->maxRows/2)+2,6,1,"CANCEL");
			selectapp->pages[0].buttons[1]->CTK_setColours(cs);
			selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonSelectCB,fud);
		}
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,fud);

	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	if(fud->isValid==true)
		this->results=selectapp->pages[0].inputs[0]->CTK_getText();
	return(fud->isValid);
}





