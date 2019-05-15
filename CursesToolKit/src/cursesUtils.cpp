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
 
#include "cursesGlobals.h"

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
			if(s[j]!=c)
				buff+=s[j];
			else
				{
					buff+=c;
					v.push_back(buff);
					buff="";
				}
		}

	if(buff!="")
		v.push_back(buff);

	return(v);
}

std::vector<std::string> CTK_cursesUtilsClass::CTK_explodeWidth(const std::string str,const char termchar,int width,int tabwidth,int offset,bool incdelim)
{
	std::string buff;
	std::vector<std::string> v;
	int	column=offset;
	int maxcol=offset+width;

	for(unsigned int j=0;j<str.length();j++)
		{
			if(str[j]=='\t')
				column=((column-1+tabwidth)/tabwidth)*tabwidth;
			column++;			

			if(column>maxcol)
				{
					column=offset+1;
					v.push_back(buff);
					buff="";
				}

			if(str[j]!=termchar)
				{
					buff+=str[j];
				}
			else
				{
					if(incdelim==true)
						buff+=termchar;
					else
						column--;

					v.push_back(buff);
					column=offset;
					buff="";
				}
		}
	if(buff!="")
		v.push_back(buff);

	return(v);
}



std::vector<std::string> CTK_cursesUtilsClass::CTK_explodeWidth(const std::string s,const char c,int width,int tw,bool incdelim)
{
	std::string buff;
	int offset=2;
	std::vector<std::string> v;
	int	column=0+offset;
	int	next_tab_column;

	for(unsigned int j=0;j<s.length();j++)
		{
			if(s[j]=='\t')
				{
					next_tab_column=column + (tw-column % tw);
					while(++column<next_tab_column);
				}
			//else
			column++;

			if((column==width) && (incdelim==false))
				column--;

			if(column==width)
				{
					column=0+offset;
					next_tab_column=0;
					v.push_back(buff);
					buff="";
				}

			if(s[j]!=c)
				{
					buff+=s[j];
				}
			else
				{
					if(incdelim==true)
						buff+=c;
					else
						column--;

					v.push_back(buff);
					column=0+offset;
					next_tab_column=0;
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
			if(fud->isOpenDialog==true)
				{
					sprintf(buffer,"File: %s",fud->find->data[ls->listItemNumber].path.c_str());
					fud->app->pages[0].textBoxes[0]->CTK_updateText(buffer);
					fud->inst->isValidFile=true;
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
		{
			fud->buttonPressed=CANCELBUTTON;
		}
	if(strcmp(bc->label,"  OK  ")==0)
		{
			fud->buttonPressed=YESBUTTON;
		}
	if(strcmp(bc->label,"  NO  ")==0)
		{
			fud->buttonPressed=NOBUTTON;
		}
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

bool CTK_cursesUtilsClass::runOpenFile(CTK_mainAppClass *app,const char *wname,bool open,const char *filename)
{
	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	LFSTK_findClass			*files=new LFSTK_findClass();
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	char					*buffer=(char*)alloca(256);
	fileUDStruct			*fud=new fileUDStruct;
	bool					retval=false;
	coloursStruct			cs;
	int						x=6;
	int						y=4;
	int						w=app->maxCols-10;
	int						h=selectapp->maxRows-6;
	int						lx=x+2;
	int						ly=y+2;
	int						lw=w-3;
	int						lh=h-7;
	int						ty=y+h-3;
	int						by=y+h-1;
	const char				*title="Open File ...";

	cs.fancyGadgets=true;
	cs.textBoxType=INBOX;
	selectapp->CTK_setColours(cs);
	selectapp->CTK_clearScreen();
	lb->CTK_setColours(cs);

	fud->find=files;
	fud->app=selectapp;
	fud->inst=this;
	fud->list=lb;
	fud->isValid=false;
	fud->isOpenDialog=open;

	if(open==false)
		title="Save File ...";
	selectapp->CTK_appWindow(x,y,w,h,wname,title);

	lb->CTK_newListBox(lx,ly,lw,lh);

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
		selectapp->CTK_addNewTextBox(lx,ty,lw,1,"File:",false);
	else
		selectapp->CTK_addNewInput(lx,ty,lw,1,filename);

	selectapp->CTK_addNewButton(lx,by,1,1,"  OK  ");
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,fud);
	selectapp->CTK_addNewButton(lx+lw-10,by,11,1,"CANCEL");
	selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonSelectCB,fud);

	selectapp->CTK_addNewCheckBox(lx+(lw/2)-8,by,14,"Show Hidden");
	selectapp->pages[0].checkBoxes[0]->CTK_setSelectCB(checkSelectCB,fud);
	selectapp->pages[0].checkBoxes[0]->CTK_setEnterDeselects(false);

	selectapp->CTK_mainEventLoop();

	if(fud->buttonPressed==CANCELBUTTON)
		retval=false;

	if((fud->buttonPressed==YESBUTTON) && (this->isValidFile==true) && (open==true))
		{
			this->stringResult=files->data[lb->listItemNumber].path;
			retval=true;
		}

	if((open==false) && (fud->buttonPressed==YESBUTTON) && (strlen(selectapp->pages[0].inputs[0]->CTK_getText())>0))
		{
			this->stringResult=selectapp->pages[0].inputs[0]->CTK_getText();
			retval=true;
		}

	delete fud;
	delete files;
	delete selectapp;
	
	return(retval);
}

void CTK_cursesUtilsClass::CTK_openFile(CTK_mainAppClass *app,const char *wname,const char *startdir,bool open,const char *filename)
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
	this->isValidFile=this->runOpenFile(app,wname,open,filename);
	app->CTK_clearScreen();
	freeAndNull(&folder);
}

bool CTK_cursesUtilsClass::CTK_entryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *defaulttxt,const char *name,const char *title,bool hascancel)
{
	fileUDStruct		*fud=new fileUDStruct;
	coloursStruct		cs;
	CTK_mainAppClass	*selectapp=new CTK_mainAppClass();

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=false;
	fud->isOpenDialog=false;

	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_appWindow((selectapp->maxCols/2)-22,(selectapp->maxRows/2)-5,44,10,name,title);

	cs.labelBoxType=NOBOX;	
	selectapp->CTK_addNewLabel((selectapp->maxCols/2)-20,(selectapp->maxRows/2)-4,41,3,bodytxt);
	selectapp->pages[0].labels[0]->CTK_setColours(cs);

	selectapp->CTK_addNewInput((selectapp->maxCols/2)-20,(selectapp->maxRows/2)+1,41,1,defaulttxt);

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;
	selectapp->CTK_addNewButton((selectapp->maxCols/2)-19,(selectapp->maxRows/2)+3,6,1,"  OK  ");
	selectapp->pages[0].buttons[0]->CTK_setColours(cs);
	if(hascancel==true)
		{
			selectapp->CTK_addNewButton((selectapp->maxCols/2)+20-10,(selectapp->maxRows/2)+3,6,1,"CANCEL");
			selectapp->pages[0].buttons[1]->CTK_setColours(cs);
			selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonSelectCB,fud);
		}
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,fud);

	selectapp->CTK_updateScreen(selectapp,NULL);
	selectapp->pages[0].inputs[0]->CTK_doInput();
	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	this->intResult=fud->buttonPressed;
	this->stringResult=selectapp->pages[0].inputs[0]->CTK_getText();

	if(fud->buttonPressed==CANCELBUTTON)
		return(false);
	else
		return(true);
}

int CTK_cursesUtilsClass::CTK_queryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *name,const char *title,int buttons)
{
	fileUDStruct		*fud=new fileUDStruct;
	coloursStruct		cs;
	CTK_mainAppClass	*selectapp=new CTK_mainAppClass();
	int					btnnum=0;

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=false;
	fud->isOpenDialog=false;
	fud->buttonsToShow=buttons;

	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_appWindow((selectapp->maxCols/2)-22,(selectapp->maxRows/2)-6,45,10,name,title);
	cs.labelBoxType=NOBOX;	
	selectapp->CTK_addNewLabel((selectapp->maxCols/2)-20,(selectapp->maxRows/2)-4,42,5,bodytxt);
	selectapp->pages[0].labels[0]->CTK_setColours(cs);

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;

	if((buttons&YESBUTTON)==YESBUTTON)
		{
			selectapp->CTK_addNewButton((selectapp->maxCols/2)-19,(selectapp->maxRows/2)+2,6,1,"  OK  ");
			selectapp->pages[0].buttons[btnnum]->CTK_setColours(cs);
			selectapp->pages[0].buttons[btnnum++]->CTK_setSelectCB(buttonSelectCB,fud);
		}

	if((buttons&CANCELBUTTON)==CANCELBUTTON)
		{
			selectapp->CTK_addNewButton((selectapp->maxCols/2)+20-9,(selectapp->maxRows/2)+2,6,1,"CANCEL");
			selectapp->pages[0].buttons[btnnum]->CTK_setColours(cs);
			selectapp->pages[0].buttons[btnnum++]->CTK_setSelectCB(buttonSelectCB,fud);
		}

	if((buttons&NOBUTTON)==NOBUTTON)
		{
			selectapp->CTK_addNewButton((selectapp->maxCols/2)-4,(selectapp->maxRows/2)+2,6,1,"  NO  ");
			selectapp->pages[0].buttons[btnnum]->CTK_setColours(cs);
			selectapp->pages[0].buttons[btnnum]->CTK_setSelectCB(buttonSelectCB,fud);
		}

	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	this->intResult=fud->buttonPressed;
	return(fud->isValid);
}





