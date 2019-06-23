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

/**
* Utils class destroy.
*/
CTK_cursesUtilsClass::~CTK_cursesUtilsClass()
{
}

/**
* Utils class.
*/
CTK_cursesUtilsClass::CTK_cursesUtilsClass()
{
}

/**
* Explode a <String> splitting on a character.
*/
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

/**
* Explode a <String> splitting on a character OR width.
* \note takes into account tabwidth.
* \note offset = start counting columns from this.
* \note incdelim = include split char in the string.
* \note tabwidth should match terminal/console tab width.
*/
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

/**
* Explode a <String> splitting on a character OR width.
* \note takes into account tabwidth.
* \note incdelim = include split char in the string.
* \note tabwidth should match terminal/console tab width.
*/
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

/**
* Private
*/
static void setPathAndType(char *retbuffer,const char *path,int type)
{
	switch(type)
		{
			case FOLDERTYPE:
				sprintf(retbuffer,"%s/",path);
				break;
			case FOLDERLINKTYPE:
				sprintf(retbuffer,"%s@",(char*)path);
				break;
			case FILETYPE:
				sprintf(retbuffer,"%s",path);
				break;
			case BROKENLINKTYPE:
			case FILELINKTYPE:
				sprintf(retbuffer,"%s@",path);
				break;
		}
}

#if 1
/**
* Private internal callback
*/
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
					setPathAndType(buffer,fud->find->data[j].name.c_str(),fud->find->data[j].fileType);
					ls->CTK_addListItem(buffer,NULL);
				}
		}
				
	if(fud->isSelectFolder==true)
		{
			sprintf(buffer,"Folder: %s",fud->inst->inFolder.c_str());
			fud->app->pages[0].textBoxes[0]->CTK_updateText(buffer);
			fud->inst->isValidFile=true;
		}
	else
		{
			if((fud->isOpenDialog==true) )
				{
					if(fud->find->data[ls->listItemNumber].fileType!=FOLDERTYPE)
						{
							sprintf(buffer,"File: %s",fud->find->data[ls->listItemNumber].path.c_str());
							fud->app->pages[0].textBoxes[0]->CTK_updateText(buffer);
							fud->inst->isValidFile=true;
						}
				}
			else
				{
					if(fud->find->data[ls->listItemNumber].fileType!=FOLDERTYPE)
						{
							fud->app->pages[0].inputs[0]->CTK_setText(fud->find->data[ls->listItemNumber].name.c_str());
							fud->inst->isValidFile=true;
						}
				}
		}
}

#else
/**
* Private internal callback
*/
static void listSelectCB(void *inst,void *ud)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);
	fileUDStruct				*fud=static_cast<fileUDStruct*>(ud);

	fud->isValid=false;

//			if((fud->find->data[ls->listItemNumber].fileType==FOLDERTYPE) || (fud->find->data[ls->listItemNumber].fileType==FOLDERLINKTYPE))
//				{
//					sprintf(buffer,"%s/%s",fud->inst->inFolder.c_str(),fud->find->data[ls->listItemNumber].name.c_str());
//					chdir(buffer);
//					fud->inst->inFolder=get_current_dir_name();
//					fud->find->LFSTK_findFiles(fud->inst->inFolder.c_str());
//					fud->find->LFSTK_setSort(false);
//					fud->find->LFSTK_sortByTypeAndName();
//				}
				
	if(fud->isSelectFolder==true)
		{
			sprintf(buffer,"%s/%s",fud->inst->inFolder.c_str(),fud->find->data[ls->listItemNumber].name.c_str());
			chdir(buffer);
			fud->inst->inFolder=get_current_dir_name();
			fud->find->LFSTK_findFiles(fud->inst->inFolder.c_str());
			fud->find->LFSTK_setSort(false);
			fud->find->LFSTK_sortByTypeAndName();
			sprintf(buffer,"Folder:%s",fud->inst->inFolder.c_str());
			fud->app->pages[0].textBoxes[0]->CTK_updateText(buffer);
			fud->inst->isValidFile=true;

			ls->CTK_clearList();
			for(int j=0;j<fud->find->data.size();j++)
				{
					setPathAndType(buffer,fud->find->data[j].name.c_str(),fud->find->data[j].fileType);
					ls->CTK_addListItem(buffer,NULL);
				}
		}
	else
		{
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
							setPathAndType(buffer,fud->find->data[j].name.c_str(),fud->find->data[j].fileType);
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
}
#endif

/**
* Private internal callback
*/
static void buttonSelectCB(void *inst,void *ud)
{
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	fileUDStruct			*fud=static_cast<fileUDStruct*>(ud);

	if(strcmp(bc->label,"Credits")==0)
		{
			fileUDStruct		*subfud=new fileUDStruct;
			CTK_mainAppClass	*selectapp=new CTK_mainAppClass();

			coloursStruct		cs;
			cs.labelBoxType=INBOX;
			cs.fancyGadgets=true;
			selectapp->CTK_setColours(cs);

			selectapp->CTK_addNewLabel(selectapp->maxCols/4,5,selectapp->maxCols/2,selectapp->maxRows-9,(char*)fud->credits);
			selectapp->pages[0].labels[0]->CTK_setJustify(CENTRE);

			selectapp->CTK_addNewButton((selectapp->maxCols/2)-3,selectapp->maxRows-3,6,1," Done ");
			subfud->app=selectapp;
			selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,(void*)subfud);

			selectapp->CTK_appWindow(selectapp->maxCols/4-2,3,selectapp->maxCols/2+3,selectapp->maxRows-5,"Credits ...",NULL);
			selectapp->CTK_setDefaultGadget(BUTTON,0);
			selectapp->CTK_mainEventLoop();

			fud->app->CTK_appWindow((fud->app->maxCols/2)-(ABOUTWIDTH/2),(fud->app->maxRows/2)-6,ABOUTWIDTH,10,"About ...",NULL);
			fud->app->CTK_updateScreen(fud->app,NULL);
			fud->app->useAppWindow=false;
			return;
		}

	if(strcmp(bc->label,"Licence")==0)
		{
			fileUDStruct		*subfud=new fileUDStruct;
			CTK_mainAppClass	*selectapp=new CTK_mainAppClass();
			coloursStruct		cs;
	
			cs.textBoxType=INBOX;
			cs.fancyGadgets=true;
			selectapp->CTK_setColours(cs);

			selectapp->CTK_addNewEditBox(selectapp,selectapp->maxCols/4,5,selectapp->maxCols/2,selectapp->maxRows-9,true,(char*)fud->licenceFilename);
			selectapp->pages[0].editBoxes[0]->CTK_setEditable(false);

			selectapp->CTK_addNewButton((selectapp->maxCols/2)-3,selectapp->maxRows-3,6,1," Done ");
			subfud->app=selectapp;
			selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,(void*)subfud);

			selectapp->CTK_appWindow(selectapp->maxCols/4-2,3,selectapp->maxCols/2+3,selectapp->maxRows-5,"Licence ...",NULL);
			selectapp->CTK_setDefaultGadget(BUTTON,0);
			selectapp->CTK_mainEventLoop();

			fud->app->CTK_appWindow((fud->app->maxCols/2)-(ABOUTWIDTH/2),(fud->app->maxRows/2)-6,ABOUTWIDTH,10,"About ...",NULL);
			fud->app->CTK_updateScreen(fud->app,NULL);
			fud->app->useAppWindow=false;
			return;
		}

	if(strcmp(bc->label," Done ")==0)
		{
			fud->app->runEventLoop=false;
			return;
		}

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

/**
* Private internal callback
*/
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
			setPathAndType(buffer,fud->find->data[j].name.c_str(),fud->find->data[j].fileType);
			fud->list->CTK_addListItem(buffer,NULL);
		}
}

/**
* Private
*/
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
					setPathAndType(buffer,files->data[j].name.c_str(),files->data[j].fileType);
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
			this->stringResult=selectapp->pages[0].textBoxes[0]->CTK_getText().substr(6);
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

/**
* Private
*/
bool CTK_cursesUtilsClass::runSelectFolder(CTK_mainAppClass *app,const char *wname,const char *folder)
{
	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	LFSTK_findClass			*files=new LFSTK_findClass();
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	char					*buffer=(char*)alloca(PATH_MAX);
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
	const char				*title="Select Folder ...";

	cs.fancyGadgets=true;
	cs.textBoxType=INBOX;
	selectapp->CTK_setColours(cs);
	selectapp->CTK_clearScreen();
	lb->CTK_setColours(cs);

	fud->find=files;
	fud->app=selectapp;
	fud->inst=this;
	fud->list=lb;
	fud->isValid=true;
	fud->isSelectFolder=true;

	selectapp->CTK_appWindow(x,y,w,h,wname,title);

	lb->CTK_newListBox(lx,ly,lw,lh);

	files->LFSTK_setFollowLinks(true);
	files->LFSTK_setFindType(FOLDERTYPE);
	files->LFSTK_setFullPath(true);
	files->LFSTK_findFiles(this->inFolder.c_str());
	files->LFSTK_setSort(false);
	files->LFSTK_sortByTypeAndName();

	for(int j=0;j<files->data.size();j++)
		{
					setPathAndType(buffer,files->data[j].name.c_str(),files->data[j].fileType);
					lb->CTK_addListItem(buffer,NULL);
		}

	lb->CTK_setSelectCB(listSelectCB,fud);
	lb->CTK_setEnterDeselects(false);
	selectapp->CTK_addListBox(lb);

	sprintf(buffer,"Folder: %s",folder);
	selectapp->CTK_addNewTextBox(lx,ty,lw,1,buffer,false);

	selectapp->CTK_addNewButton(lx,by,1,1,"  OK  ");
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,fud);
	selectapp->CTK_addNewButton(lx+lw-10,by,11,1,"CANCEL");
	selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonSelectCB,fud);

	selectapp->CTK_addNewCheckBox(lx+(lw/2)-8,by,14,"Show Hidden");
	selectapp->pages[0].checkBoxes[0]->CTK_setSelectCB(checkSelectCB,fud);
	selectapp->pages[0].checkBoxes[0]->CTK_setEnterDeselects(false);

	this->isValidFile=true;

	selectapp->CTK_mainEventLoop();

	if(fud->buttonPressed==CANCELBUTTON)
		retval=false;

	if((fud->buttonPressed==YESBUTTON) && (this->isValidFile==true) )
		{
			this->stringResult=selectapp->pages[0].textBoxes[0]->CTK_getText().substr(8);
			retval=true;
		}

	delete fud;
	delete files;
	delete selectapp;
	
	return(retval);
}

/**
* Select folder convenience dialog.
*/
void CTK_cursesUtilsClass::CTK_selectFolder(CTK_mainAppClass *app,const char *wname,const char *startdir)
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
	this->isValidFile=this->runSelectFolder(app,wname,this->inFolder.c_str());
	app->CTK_clearScreen();
	CTK_freeAndNull(&folder);
}


/**
* Open file convenience dialog.
*/
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
	CTK_freeAndNull(&folder);
}

/**
* Get user entry convenience dialog.
*/
bool CTK_cursesUtilsClass::CTK_entryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *defaulttxt,const char *name,const char *title,bool hascancel,int dialogwidth)
{
	fileUDStruct		*fud=new fileUDStruct;
	coloursStruct		cs;
	CTK_mainAppClass	*selectapp=new CTK_mainAppClass();
	int					maxbtns=1;
	int					btnnum=0;

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=false;
	fud->isOpenDialog=false;

	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_appWindow((selectapp->maxCols/2)-(dialogwidth/2)-1,(selectapp->maxRows/2)-6,dialogwidth+
1,10,name,title);
	cs.labelBoxType=NOBOX;	
	selectapp->CTK_addNewLabel((selectapp->maxCols/2)-(dialogwidth/2),(selectapp->maxRows/2)-5,dialogwidth,5,bodytxt);
	selectapp->pages[0].labels[0]->CTK_setJustify(CENTRE);
	selectapp->pages[0].labels[0]->CTK_setColours(cs);

	selectapp->CTK_addNewInput((selectapp->maxCols/2)-(dialogwidth/2)+1,(selectapp->maxRows/2)+1,dialogwidth-2,1,defaulttxt);

	if(hascancel==true)
		maxbtns=2;

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;
	selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum++),(selectapp->maxRows/2)+3,6,1,"  OK  ");

	selectapp->pages[0].buttons[0]->CTK_setColours(cs);
	if(hascancel==true)
		{
			selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"CANCEL");
			selectapp->pages[0].buttons[1]->CTK_setColours(cs);
			selectapp->pages[0].buttons[1]->CTK_setSelectCB(buttonSelectCB,fud);
		}
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,fud);

	selectapp->CTK_setDefaultGadget(INPUT,0);
	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	this->intResult=fud->buttonPressed;
	this->stringResult=selectapp->pages[0].inputs[0]->CTK_getText();

	if(fud->buttonPressed==CANCELBUTTON)
		return(false);
	else
		return(true);
}

/**
* User notification convenience dialog.
*/
int CTK_cursesUtilsClass::CTK_queryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *name,const char *title,int buttons,int dialogwidth)
{
	fileUDStruct		*fud=new fileUDStruct;
	coloursStruct		cs;
	CTK_mainAppClass	*selectapp=new CTK_mainAppClass();
	int					btnnum=0;
	int					maxbtns=0;

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=false;
	fud->isOpenDialog=false;
	fud->buttonsToShow=buttons;

	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_appWindow((selectapp->maxCols/2)-(dialogwidth/2)-1,(selectapp->maxRows/2)-4,dialogwidth+
1,8,name,title);
	cs.labelBoxType=NOBOX;	
	selectapp->CTK_addNewLabel((selectapp->maxCols/2)-(dialogwidth/2),(selectapp->maxRows/2)-3,dialogwidth,5,bodytxt);

	selectapp->pages[0].labels[0]->CTK_setColours(cs);
	selectapp->pages[0].labels[0]->CTK_setJustify(CENTRE);

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;

	if((buttons&YESBUTTON)==YESBUTTON)
		maxbtns++;
	if((buttons&NOBUTTON)==NOBUTTON)
		maxbtns++;
	if((buttons&CANCELBUTTON)==CANCELBUTTON)
		maxbtns++;

	if((buttons&YESBUTTON)==YESBUTTON)
		{
			selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"  OK  ");
			selectapp->pages[0].buttons[btnnum]->CTK_setColours(cs);
			selectapp->CTK_setDefaultGadget(BUTTON,btnnum);
			selectapp->pages[0].buttons[btnnum++]->CTK_setSelectCB(buttonSelectCB,fud);
		}

	if((buttons&NOBUTTON)==NOBUTTON)
		{
			selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"  NO  ");
			selectapp->pages[0].buttons[btnnum]->CTK_setColours(cs);
			selectapp->CTK_setDefaultGadget(BUTTON,btnnum);
			selectapp->pages[0].buttons[btnnum++]->CTK_setSelectCB(buttonSelectCB,fud);
		}

	if((buttons&CANCELBUTTON)==CANCELBUTTON)
		{
			selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"CANCEL");
			selectapp->pages[0].buttons[btnnum]->CTK_setColours(cs);
			selectapp->CTK_setDefaultGadget(BUTTON,btnnum);
			selectapp->pages[0].buttons[btnnum]->CTK_setSelectCB(buttonSelectCB,fud);
		}

	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	this->intResult=fud->buttonPressed;
	return(fud->isValid);
}

/**
* About convenience dialog.
*/
void CTK_cursesUtilsClass::CTK_aboutDialog(CTK_mainAppClass *app,const char *appname,const char *appinfo,const char *copyright,const char *email,const char *website,const char *credits,const char *licence,int dialogwidth)
{
	char				*aboutbuffer;
	fileUDStruct		*fud=new fileUDStruct;
	CTK_mainAppClass	*selectapp;
	coloursStruct		cs;
	int					btnnum=0;
	int					maxbtns=1;

	fud->app=selectapp;
	fud->inst=this;
	fud->credits=credits;
	fud->licenceFilename=licence;

	selectapp=new CTK_mainAppClass();
	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.labelBoxType=NOBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_appWindow((selectapp->maxCols/2)-(dialogwidth/2)-1,(selectapp->maxRows/2)-6,dialogwidth+1,ABOUTHITE,"About ...",NULL);
	fud->app=selectapp;

//body
	asprintf(&aboutbuffer,"%s\n\n%s\n%s\n%s\n%s\n",appname,appinfo,copyright,email,website);
	selectapp->CTK_addNewLabel((selectapp->maxCols/2)-(dialogwidth/2),(selectapp->maxRows/2)-5,dialogwidth,6,aboutbuffer);
	selectapp->pages[0].labels[0]->CTK_setJustify(CENTRE);

	if(credits!=NULL)
		maxbtns++;
	if(licence!=NULL)
		maxbtns++;

	selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,11,btnnum++),(selectapp->maxRows/2)+2,6,1," Close ");
	selectapp->pages[0].buttons[0]->CTK_setSelectCB(buttonSelectCB,(void*)fud);

	if(credits!=NULL)
		{
			selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,11,btnnum),(selectapp->maxRows/2)+2,6,1,"Credits");
			selectapp->pages[0].buttons[btnnum]->CTK_setSelectCB(buttonSelectCB,(void*)fud);
			btnnum++;
		}

	if(licence!=NULL)
		{
			selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,11,btnnum),(selectapp->maxRows/2)+2,6,1,"Licence");
			selectapp->pages[0].buttons[btnnum]->CTK_setSelectCB(buttonSelectCB,(void*)fud);
		}

	selectapp->CTK_setDefaultGadget(BUTTON,0);
	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	free(aboutbuffer);
}

/**
* Calulate X position for gadget given total width, gadget width, gadget count and gadget number.   
*/
int CTK_cursesUtilsClass::CTK_getGadgetPosX(int sx,int wid,int gadgetcnt,int gadgetwid,int gadgetnum)
{
	int x;
	double secs=(wid/(gadgetcnt+1));

	x=(sx+(secs/2))+(secs*(gadgetnum))+((secs/2)-(gadgetwid/2));
	return(x);
}





