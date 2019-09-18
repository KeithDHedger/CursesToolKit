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
* Private internal callback
*/
static void folderSelectCB(void *inst,void *ud)
{
	char					*buffer=(char*)alloca(PATH_MAX);
	fileUDStruct			*fud=static_cast<fileUDStruct*>(ud);
	CTK_cursesChooserClass	*ch=static_cast<CTK_cursesChooserClass*>(inst);

	sprintf(buffer,"Folder: %s",ch->folderPath.c_str());
	fud->textGadget->CTK_updateText(buffer);
	fud->inst->isValidFile=true;
}

/**
* Private internal callback
*/
static void inputCB(void *inst,void *ud)
{
	fileUDStruct	*fud=static_cast<fileUDStruct*>(ud);
	CTK_cursesInputClass	*inp=static_cast<CTK_cursesInputClass*>(inst);

	if(strlen(inp->CTK_getText())>0)
		fud->btnOK->CTK_setEnabled(true);
	else
		fud->btnOK->CTK_setEnabled(false);
}

/**
* Private internal callback
*/
static void fileSelectCB(void *inst,void *ud)
{

	char					*buffer=(char*)alloca(PATH_MAX);
	fileUDStruct			*fud=static_cast<fileUDStruct*>(ud);
	CTK_cursesChooserClass	*ch=static_cast<CTK_cursesChooserClass*>(inst);

	if((fud->isOpenDialog==true) )
		{
			if(ch->files->data[ch->lb->listItemNumber].fileType!=FOLDERTYPE)
				{
					sprintf(buffer,"File: %s",ch->filePath.c_str());
					fud->textGadget->CTK_updateText(buffer);
					fud->btnOK->CTK_setEnabled(true);
					fud->inst->isValidFile=true;
				}
		}
	else
		{
			if(ch->files->data[ch->lb->listItemNumber].fileType!=FOLDERTYPE)
				{
					fud->inputGadget->CTK_setText(ch->fileName.c_str());
					fud->inst->isValidFile=true;
				}
		}
	if(fud->inst->isValidFile==true)
		fud->btnOK->CTK_setEnabled(true);
}

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

			subfud->labelGadget=selectapp->CTK_addNewLabel(selectapp->maxCols/4,5,selectapp->maxCols/2,selectapp->maxRows-9,(char*)fud->credits);
			subfud->labelGadget->CTK_setJustify(CENTRE);

			subfud->btnOK=selectapp->CTK_addNewButton(fud->inst->CTK_getGadgetPosX(selectapp->maxCols/4,selectapp->maxCols/2,1,strlen("<  Done  >"),0),selectapp->maxRows-3,6,1," Done ");//btnok
			subfud->app=selectapp;
			subfud->btnOK->CTK_setSelectCB(buttonSelectCB,(void*)subfud);

			selectapp->CTK_appWindow(selectapp->maxCols/4-2,3,selectapp->maxCols/2+3,selectapp->maxRows-5,"Credits ...",NULL);
			selectapp->CTK_setDefaultGadget(subfud->btnOK);
			selectapp->CTK_mainEventLoop();

			fud->app->CTK_appWindow((fud->app->maxCols/2)-(ABOUTWIDTH/2),(fud->app->maxRows/2)-6,ABOUTWIDTH,10,"About ...",NULL);
		//	fud->app->CTK_setDefaultGadget(fud->btnOK);
			fud->app->CTK_updateScreen(fud->app,NULL);
		//	fud->app->useAppWindow=false;
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

			subfud->textGadget=selectapp->CTK_addNewTextBox(selectapp->maxCols/4,5,selectapp->maxCols/2,selectapp->maxRows-9,true,(char*)fud->licenceFilename);

			subfud->btnOK=selectapp->CTK_addNewButton(fud->inst->CTK_getGadgetPosX(selectapp->maxCols/4,selectapp->maxCols/2,1,strlen("<  Done  >"),0),selectapp->maxRows-3,6,1," Done ");//btnok
			subfud->app=selectapp;
			subfud->btnOK->CTK_setSelectCB(buttonSelectCB,(void*)subfud);

			selectapp->CTK_appWindow(selectapp->maxCols/4-2,3,selectapp->maxCols/2+3,selectapp->maxRows-5,"Licence ...",NULL);
			selectapp->CTK_setDefaultGadget(subfud->btnOK);
			selectapp->CTK_mainEventLoop();

			fud->app->CTK_appWindow((fud->app->maxCols/2)-(ABOUTWIDTH/2),(fud->app->maxRows/2)-6,ABOUTWIDTH,10,"About ...",NULL);
			//fud->app->CTK_setDefaultGadget(fud->btnOK);
			//fud->app->pages[fud->app->pageNumber].currentGadget+=-1;
			//fud->app->noHiliteChange=true;//TODO//
			////fud->app->noHiliteChange=true;
			//fud->app->setHilite(true);
			//fud->btnNo->hiLited=false;
			///fud->app->CTK_mainEventLoop(-10);
			///fud->app->runEventLoop=true;
			fud->app->CTK_updateScreen(fud->app,NULL);
			///fud->app->useAppWindow=false;
			//fud->app->pages[fud->app->pageNumber].currentGadget=0;
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

	if(fud->chooserGadget!=NULL)
		{
			fud->chooserGadget->CTK_setShowHidden(cb->CTK_getValue());
			fud->chooserGadget->CTK_updateList();
			return;
		}
}

/**
* Private
*/
bool CTK_cursesUtilsClass::runOpenFile(CTK_mainAppClass *app,const char *wname,bool open,const char *filename,const char *filetypes)
{
	CTK_cursesChooserClass	*chooser;
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	char					*buffer=(char*)alloca(256);
	fileUDStruct			*fud=new fileUDStruct;
	bool					retval=false;
	coloursStruct			cs;
	int						x=6;
	int						y=4;
	int						w=selectapp->maxCols-10;
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

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=false;
	fud->isOpenDialog=open;

	if(open==false)
		title="Save File ...";
	selectapp->CTK_appWindow(x,y,w,h,wname,title);

	chooser=new CTK_cursesChooserClass(selectapp,lx,ly,lw,lh);
	fud->chooserGadget=chooser;
	chooser->CTK_setShowTypes(ANYTYPE);
	chooser->CTK_setShowFileTypes(filetypes);
	chooser->CTK_setShowHidden(false);
	chooser->CTK_selectFolder(selectapp,this->inFolder.c_str());
	selectapp->CTK_addChooserBox(chooser);
	chooser->CTK_setSelectCB(fileSelectCB,fud);

	if(open==true)
		{
			sprintf(buffer,"File: %s",filename);
			fud->textGadget=selectapp->CTK_addNewTextBox(lx,ty,lw,1,buffer,false);
		}
	else
		{
			fud->inputGadget=selectapp->CTK_addNewInput(lx,ty,lw,1,filename);
			fud->inputGadget->CTK_setSelectCB(inputCB,fud);
		}

	fud->btnOK=selectapp->CTK_addNewButton(lx,by,1,1,"  OK  ");
	fud->btnOK->CTK_setSelectCB(buttonSelectCB,fud);
	if((filename==NULL) || (strcmp(filename,"")==0))
		fud->btnOK->CTK_setEnabled(false);
	fud->btnCancel=selectapp->CTK_addNewButton(lx+lw-10,by,11,1,"CANCEL");
	fud->btnCancel->CTK_setSelectCB(buttonSelectCB,fud);

	fud->checkGadget=selectapp->CTK_addNewCheckBox(lx+(lw/2)-8,by,14,"Show Hidden");
	fud->checkGadget->CTK_setSelectCB(checkSelectCB,fud);
	fud->checkGadget->CTK_setSelectDeselects(false);

	selectapp->CTK_setDefaultGadget(chooser->lb);
	selectapp->CTK_mainEventLoop();

	if(fud->buttonPressed==CANCELBUTTON)
		retval=false;

	if((fud->buttonPressed==YESBUTTON) && (this->isValidFile==true) && (open==true))
		{
			this->stringResult=fud->textGadget->CTK_getText().substr(6);
			retval=true;
		}

	if((open==false) && (fud->buttonPressed==YESBUTTON) && (strlen(fud->inputGadget->CTK_getText())>0))
		{
			this->inFolder=chooser->folderPath;
			this->stringResult=fud->inputGadget->CTK_getText();
			retval=true;
		}

	delete fud;
	delete selectapp;
	return(retval);
}

/**
* Private
*/
bool CTK_cursesUtilsClass::runSelectFolder(CTK_mainAppClass *app,const char *wname,const char *folder)
{
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	CTK_cursesChooserClass	*chooser;
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

	fud->app=selectapp;
	fud->inst=this;
	fud->isValid=true;

	selectapp->CTK_appWindow(x,y,w,h,wname,title);

	chooser=new CTK_cursesChooserClass(selectapp,lx,ly,lw,lh);
	fud->chooserGadget=chooser;
	chooser->CTK_setShowTypes(FOLDERTYPE);
	chooser->CTK_setShowHidden(false);
	chooser->CTK_selectFolder(selectapp,this->inFolder.c_str());
	selectapp->CTK_addChooserBox(chooser);
	chooser->CTK_setSelectCB(folderSelectCB,fud);

	sprintf(buffer,"Folder: %s",folder);
	fud->textGadget=selectapp->CTK_addNewTextBox(lx,ty,lw,1,buffer,false);

	fud->btnOK=selectapp->CTK_addNewButton(lx,by,1,1,"  OK  ");
	fud->btnOK->CTK_setSelectCB(buttonSelectCB,fud);
	fud->btnCancel=selectapp->CTK_addNewButton(lx+lw-10,by,11,1,"CANCEL");
	fud->btnCancel->CTK_setSelectCB(buttonSelectCB,fud);

	fud->checkGadget=selectapp->CTK_addNewCheckBox(lx+(lw/2)-8,by,14,"Show Hidden");
	fud->checkGadget->CTK_setSelectCB(checkSelectCB,fud);
	fud->checkGadget->CTK_setSelectDeselects(false);

	this->isValidFile=true;

	selectapp->CTK_setDefaultGadget(chooser->lb);
	selectapp->CTK_mainEventLoop();

	if(fud->buttonPressed==CANCELBUTTON)
		retval=false;

	if((fud->buttonPressed==YESBUTTON) && (this->isValidFile==true) )
		{
			this->stringResult=fud->textGadget->CTK_getText().substr(8);
			retval=true;
		}

	delete fud;
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
void CTK_cursesUtilsClass::CTK_openFile(CTK_mainAppClass *app,const char *wname,const char *startdir,bool open,const char *filename,const char *filetypes)
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
	this->isValidFile=this->runOpenFile(app,wname,open,filename,filetypes);
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
	fud->labelGadget=selectapp->CTK_addNewLabel((selectapp->maxCols/2)-(dialogwidth/2),(selectapp->maxRows/2)-5,dialogwidth,5,bodytxt);
	fud->labelGadget->CTK_setJustify(CENTRE);
	fud->labelGadget->CTK_setColours(cs);

	fud->inputGadget=selectapp->CTK_addNewInput((selectapp->maxCols/2)-(dialogwidth/2)+1,(selectapp->maxRows/2)+1,dialogwidth-2,1,defaulttxt);

	if(hascancel==true)
		maxbtns=2;

	cs.foreCol=FORE_WHITE;
	cs.backCol=BACK_BLUE;
	fud->btnOK=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum++),(selectapp->maxRows/2)+3,6,1,"  OK  ");
	fud->btnOK->CTK_setColours(cs);
	if(hascancel==true)
		{
			fud->btnCancel=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"CANCEL");
			fud->btnCancel->CTK_setColours(cs);
			fud->btnCancel->CTK_setSelectCB(buttonSelectCB,fud);
		}
	fud->btnOK->CTK_setSelectCB(buttonSelectCB,fud);

	//selectapp->CTK_mainEventLoop(-10);
	selectapp->CTK_updateScreen(selectapp,NULL);
	selectapp->CTK_setDefaultGadget(fud->inputGadget,false);

	fud->inputGadget->CTK_doInput();
	selectapp->CTK_mainEventLoop(0,false);
	app->CTK_clearScreen();
	this->intResult=fud->buttonPressed;
	this->stringResult=fud->inputGadget->CTK_getText();

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
	CTK_cursesButtonClass*	defhold=NULL;
	fileUDStruct			*fud=new fileUDStruct;
	coloursStruct			cs;
	CTK_mainAppClass		*selectapp=new CTK_mainAppClass();
	int						btnnum=0;
	int						maxbtns=0;

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
	fud->labelGadget=selectapp->CTK_addNewLabel((selectapp->maxCols/2)-(dialogwidth/2),(selectapp->maxRows/2)-3,dialogwidth,5,bodytxt);
	fud->labelGadget->CTK_setColours(cs);
	fud->labelGadget->CTK_setJustify(CENTRE);

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
			fud->btnOK=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"  OK  ");
			fud->btnOK->CTK_setColours(cs);
			fud->btnOK->CTK_setSelectCB(buttonSelectCB,fud);
			defhold=fud->btnOK;
			btnnum++;
		}

	if((buttons&NOBUTTON)==NOBUTTON)
		{
			fud->btnNo=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"  NO  ");
			fud->btnNo->CTK_setColours(cs);
			fud->btnNo->CTK_setSelectCB(buttonSelectCB,fud);
			defhold=fud->btnNo;
			btnnum++;
		}

	if((buttons&CANCELBUTTON)==CANCELBUTTON)
		{
			fud->btnCancel=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,10,btnnum),(selectapp->maxRows/2)+3,6,1,"CANCEL");
			fud->btnCancel->CTK_setColours(cs);
			fud->btnCancel->CTK_setSelectCB(buttonSelectCB,fud);
			defhold=fud->btnCancel;
			btnnum++;
		}

	selectapp->CTK_setDefaultGadget(defhold);
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

	fud->inst=this;
	fud->credits=credits;
	fud->licenceFilename=licence;

	selectapp=new CTK_mainAppClass();
	fud->app=selectapp;
	cs.windowBackCol=BACK_WHITE;
	cs.textBoxType=OUTBOX;
	cs.labelBoxType=NOBOX;
	cs.fancyGadgets=true;
	selectapp->CTK_setColours(cs);

	selectapp->CTK_appWindow((selectapp->maxCols/2)-(dialogwidth/2)-1,(selectapp->maxRows/2)-6,dialogwidth+1,ABOUTHITE,"About ...",NULL);
	fud->app=selectapp;

//body
	asprintf(&aboutbuffer,"%s\n\n%s\n%s\n%s\n%s\n",appname,appinfo,copyright,email,website);
	fud->labelGadget=selectapp->CTK_addNewLabel((selectapp->maxCols/2)-(dialogwidth/2),(selectapp->maxRows/2)-5,dialogwidth,6,aboutbuffer);
	fud->labelGadget->CTK_setJustify(CENTRE);

	if(credits!=NULL)
		maxbtns++;
	if(licence!=NULL)
		maxbtns++;

	fud->btnOK=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,11,btnnum++),(selectapp->maxRows/2)+2,6,1," Close ");//btnok
	fud->btnOK->CTK_setSelectCB(buttonSelectCB,(void*)fud);

	if(credits!=NULL)
		{
			fud->btnNo=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,11,btnnum),(selectapp->maxRows/2)+2,6,1,"Credits");//buttonno
			fud->btnNo->CTK_setSelectCB(buttonSelectCB,(void*)fud);
			btnnum++;
		}

	if(licence!=NULL)
		{
			fud->btnCancel=selectapp->CTK_addNewButton(CTK_getGadgetPosX((selectapp->maxCols/2)-(dialogwidth/2),dialogwidth,maxbtns,11,btnnum),(selectapp->maxRows/2)+2,6,1,"Licence");//buttoncancel
			fud->btnCancel->CTK_setSelectCB(buttonSelectCB,(void*)fud);
		}

	selectapp->CTK_setDefaultGadget(fud->btnOK);
	selectapp->CTK_mainEventLoop();
	app->CTK_clearScreen();
	free(aboutbuffer);
}

/**
* Calulate X position for gadget given total width, gadget width, gadget count and gadget number.
* \param int sx start position of bounding box.
* \param int wid of bounding box.
* \param int gadgetcnt total number of gadgets.
* \param int gadgetwid gadget width.
* \param int gadgetnum this gadget number ( base 0 ).
*/
int CTK_cursesUtilsClass::CTK_getGadgetPosX(int sx,int wid,int gadgetcnt,int gadgetwid,int gadgetnum)
{
	int x;
	double secs=(wid/(gadgetcnt+1));

	x=(sx+(secs/2))+(secs*(gadgetnum))+((secs/2)-(gadgetwid/2));
	return(x);
}

/**
* Pad string front and back.
* \note if label length is an odd number, 1 is subtracted from the back padding.
*/
std::string CTK_cursesUtilsClass::CTK_padString(std::string label,int maxlen)
{
	std::string	labelstr=label;
	int			padlen=(maxlen/2)-(labelstr.length()/2);

	labelstr.insert(labelstr.begin(),padlen,' ');
	labelstr.insert(labelstr.end(),padlen-(label.length()%2),' ');
	return(labelstr);
}

/**
* Non blocking splash screen.
*/
void CTK_cursesUtilsClass::CTK_splashScreen(CTK_mainAppClass *app,const char *text)
{
	CTK_cursesGraphicsClass 	gc;
	CTK_cursesUtilsClass		cu;
	std::string					str=text;
	std::vector<std::string>	txtStrings;
	int							centre=(app->maxCols/2);
	int							boxx=4;
	int							boxy=4;
	int							boxw=app->maxCols-7;
	int							boxh=app->maxRows-7;
	int							offset=0;
	txtStrings.clear();

	SETHIDECURS;
	txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',app->maxCols-24,app->tabWidth,24,false);

	app->CTK_clearScreen();
	gc.CTK_drawBox(boxx,boxy,boxw,boxh,PLAINBOX,true,false);
	for(int j=0;j<txtStrings.size();j++)
		{
			MOVETO(centre-((txtStrings[j].length()-offset)/2),11+j);
			printf("%s",txtStrings[j].c_str());
		}
	fflush(NULL);
}

