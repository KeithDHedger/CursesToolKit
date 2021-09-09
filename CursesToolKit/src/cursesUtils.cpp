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
* Utils class.
*/
CTK_cursesUtilsClass::CTK_cursesUtilsClass(CTK_mainAppClass *mc)
{
	this->mc=mc;
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
static bool fileSelectCB(void *inst,void *data)
{
	CTK_cursesChooserClass	*ch=static_cast<CTK_cursesChooserClass*>(inst);
	utilDialogStruct		*ud=static_cast<utilDialogStruct*>(data);
	long					chtype=(long)ch->userData;
	
	switch(chtype)
		{
			case CUOPENFILE:
				if(ch->files->data[ch->lb->listItemNumber].fileType!=FOLDERTYPE)
					{
						char *rp=realpath(ch->filePath.c_str(),NULL);
						if(rp!=NULL)
							{
								ud->results->CTK_updateText(rp);
								free(rp);
							}
						else
							{
								ud->results->CTK_updateText(ch->filePath.c_str());
							}
						ud->defaultGadget->CTK_setEnabled(true);
						ud->defaultGadget->CTK_drawGadget(false);
					}
				break;
			case CUOPENFOLDER:
				{
					char *rp=realpath(ch->filePath.c_str(),NULL);
					if(rp!=NULL)
						{
							ud->results->CTK_updateText(rp);
							free(rp);
						}
					else
						{
							ud->results->CTK_updateText(ch->filePath.c_str());
						}
					ud->defaultGadget->CTK_setEnabled(true);
					ud->defaultGadget->CTK_drawGadget(false);
					return(true);
				}
				break;
			case CUSAVEFILE:
				if(ch->files->data[ch->lb->listItemNumber].fileType!=FOLDERTYPE)
					{
						ud->input->CTK_setText(ch->fileName.c_str());
						ud->input->CTK_drawGadget(false);
					}
				break;
		}
	return true;
}

/**
* Private internal callback for entry.
*/
static bool buttonSelectEntryCB(void *inst,void *data)
{
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	utilDialogStruct		*ud=static_cast<utilDialogStruct*>(data);
	long					btnud=(long)bc->userData;

	switch(btnud)
		{
//entry/query
			case CUQUERYOK:
				ud->isValidData=true;
				ud->intValue=CUQUERYOK;
				ud->stringValue="";
				ud->varType=INTVAR;
				break;
			case CUENTRYOK:
				ud->isValidData=true;
				ud->intValue=CUENTRYOK;
				ud->stringValue=ud->input->CTK_getText();
				ud->varType=INTVAR;
				break;
			case CUENTRYINPUTENTER:
				ud->mc->CTK_setDefaultGadget(CURRENTPAGE(ud->mc).gadgets[2]);
				return(true);
				break;

			case CUQUERYNO:
				ud->isValidData=true;
				ud->intValue=CUQUERYNO;
				ud->varType=INTVAR;
				ud->stringValue="";
				break;
			case CUQUERYCANCEL:
			case CUENTRYCANCEL:
				ud->isValidData=false;
				ud->intValue=CUENTRYCANCEL;
				ud->varType=INTVAR;
				ud->stringValue="";
				break;
//about box
			case CUABOUTCLOSE:
				break;
			case CUABOUTCREDITS:
				ud->mc->CTK_setPage(1);
				ud->mc->CTK_setDefaultGadget(CURRENTPAGE(ud->mc).gadgets[1]);
				return(true);
				break;
			case CUABOUTLICENCE:
				ud->mc->CTK_setPage(2);
				ud->mc->CTK_setDefaultGadget(CURRENTPAGE(ud->mc).gadgets[1]);
				return(true);
				break;
			case CUABOUTDONE:
				ud->mc->CTK_setPage(0);
				ud->mc->CTK_setDefaultGadget(ud->defaultGadget);
				return(true);
				break;
//chooser
			case CUCHOOSEROK:
				ud->isValidData=true;
				ud->intValue=CUCHOOSEROK;
				if(ud->results!=NULL)
					ud->stringValue=ud->results->CTK_getText();
				else
					{
						ud->stringValue=ud->chooser->folderPath;
						ud->stringValue+="/";
						ud->stringValue+=ud->input->CTK_getText();
					}
				if(ud->stringValue.length()==0)
					ud->isValidData=false;
				ud->varType=CHARVAR;
				break;
			case CUCHOOSERSHOW:
				{
					CTK_cursesCheckBoxClass	*cb=static_cast<CTK_cursesCheckBoxClass*>(inst);
					cb->CTK_setValue(!cb->CTK_getValue());
					ud->chooser->CTK_setShowHidden(cb->CTK_getValue());
					ud->chooser->CTK_updateList();
					ud->mc->CTK_updateScreen(ud->mc,NULL);
					return(true);
				}
				break;
			case CUCHOOSERCANCEL:
				ud->isValidData=false;
				break;
		}

	ud->mc->runEventLoop=false;
	return true;
}

/**
* Open/save file/folder convenience dialog.
* \param const char *startdir, default start in current dir.
* \param int choosertype, CUOPENFILE, CUOPENFOLDER or CUSAVEFILE.
* \param bool ignorebroken ignore broken links ( default true ).
* \param const char *filetypes file types to show.
* \param const char *savename default save file name, ignored for open.
* \note Default is to show all file types in open dialog,
* \note filetypes is ';' delimited string eg ".png;.tiff;"
*/
bool CTK_cursesUtilsClass::CTK_fileChooserDialog(const char *startdir,int choosertype,bool ignorebroken,const char *filetypes,const char *savename)
{
	int						genx,geny,genw,genh;
	CTK_cursesButtonClass	*button;
	CTK_mainAppClass		*app=new CTK_mainAppClass;
	CTK_cursesLabelClass	*label;
	char					*folder;
	std::string				labelstr;
	CTK_cursesCheckBoxClass	*checkGadget;

	switch(choosertype)
		{
			case CUOPENFILE:
				app->CTK_setDialogWindow("Open file ...","",-1,-1);
				break;
			case CUOPENFOLDER:
				app->CTK_setDialogWindow("Open folder ...","",-1,-1);
				break;
			case CUSAVEFILE:
				app->CTK_setDialogWindow("Save file ...","",-1,-1);
				break;
		}
		
	CURRENTPAGE(app).fancyWindow=true;
	this->dialogReturnData.isValidData=false;
	this->dialogReturnData.mc=app;

	if(startdir==NULL)
		{
			folder=get_current_dir_name();
			this->inFolder=folder;
		}
	else
		this->inFolder=startdir;	

	genx=CURRENTPAGE(app).boxX+2;
	geny=CURRENTPAGE(app).boxY+2;
	genw=CURRENTPAGE(app).boxW-3;
	genh=CURRENTPAGE(app).boxH-7;

	this->dialogReturnData.chooser=new CTK_cursesChooserClass(app,genx,geny,genw,genh);
	this->dialogReturnData.chooser->lb->gadgetColours.foreCol=FORE_BLACK;
	this->dialogReturnData.chooser->lb->gadgetColours.backCol=BACK_WHITE;

	switch(choosertype)
		{
			case CUSAVEFILE:
				this->dialogReturnData.chooser->userData=(void*)CUSAVEFILE;
				this->dialogReturnData.chooser->CTK_setShowTypes(ANYTYPE);
				break;
			case CUOPENFILE:
				this->dialogReturnData.chooser->userData=(void*)CUOPENFILE;
				this->dialogReturnData.chooser->CTK_setShowTypes(ANYTYPE);
				break;
			case CUOPENFOLDER:
				this->dialogReturnData.chooser->CTK_setShowTypes(FOLDERTYPE);
				this->dialogReturnData.chooser->userData=(void*)CUOPENFOLDER;
				break;
		}

	this->dialogReturnData.chooser->CTK_setShowFileTypes(filetypes);
	this->dialogReturnData.chooser->files->LFSTK_setIgnoreBroken(ignorebroken);
	this->dialogReturnData.chooser->CTK_setShowHidden(false);
	this->dialogReturnData.chooser->CTK_selectFolder(app,this->inFolder.c_str());
	app->CTK_addChooserBox(this->dialogReturnData.chooser);
	this->dialogReturnData.chooser->CTK_setSelectCB(fileSelectCB,(void*)&this->dialogReturnData);

	if(choosertype==CUSAVEFILE)
		{
			this->dialogReturnData.input=app->CTK_addNewInput(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-3,genw,1,savename);
			this->dialogReturnData.input->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
			this->dialogReturnData.input->userData=(void*)CUCHOOSERINPUT;
			this->dialogReturnData.input->redrawAppWindow=false;
			this->dialogReturnData.input->gadgetColours.boxType=INBOX;
			this->dialogReturnData.input->gadgetColours.foreCol=FORE_BLACK;
			this->dialogReturnData.input->gadgetColours.backCol=BACK_WHITE;
		}
	else
		{
			if(choosertype==CUOPENFOLDER)
				this->dialogReturnData.results=app->CTK_addNewTextBox(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-3,genw,1,startdir,false);
			else
				this->dialogReturnData.results=app->CTK_addNewTextBox(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-3,genw,1,"",false);
			this->dialogReturnData.results->gadgetColours.boxType=INBOX;
			this->dialogReturnData.results->gadgetColours.foreCol=FORE_BLACK;
			this->dialogReturnData.results->gadgetColours.backCol=BACK_WHITE;
		}

	labelstr=this->CTK_padString("OK",CHOOSERBTNWIDTH);
	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,3,CHOOSERBTNWIDTH,0);
	this->dialogReturnData.defaultGadget=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,CHOOSERBTNWIDTH,1,labelstr.c_str());
	this->dialogReturnData.defaultGadget->userData=(void*)CUCHOOSEROK;
	this->dialogReturnData.defaultGadget->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

	if(choosertype==CUOPENFILE)
		this->dialogReturnData.defaultGadget->CTK_setEnabled(false);

	labelstr=this->CTK_padString("Show Hidden",CHOOSERBTNWIDTH);
	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,3,CHOOSERBTNWIDTH,1);
	checkGadget=app->CTK_addNewCheckBox(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,CHOOSERBTNWIDTH,labelstr.c_str());
	checkGadget->userData=(void*)CUCHOOSERSHOW;
	checkGadget->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

	labelstr=this->CTK_padString("Cancel",CHOOSERBTNWIDTH);
	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,3,CHOOSERBTNWIDTH,2);
	button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,CHOOSERBTNWIDTH,1,labelstr.c_str());
	button->userData=(void*)CUCHOOSERCANCEL;
	button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

	SETHIDECURS;
	app->CTK_setDefaultGadget(this->dialogReturnData.chooser->lb);
	app->CTK_clearScreen();
	app->CTK_updateScreen(app,NULL);
	fflush(NULL);
	app->CTK_mainEventLoop(0,false,true);
	delete app;
	if(this->mc!=NULL)
		{
			this->mc->CTK_clearScreen();
			this->mc->CTK_updateScreen(this->mc,NULL);
		}
	fflush(NULL);
	return(this->dialogReturnData.isValidData);
}

/**
* Get user entry convenience dialog.
*/
bool CTK_cursesUtilsClass::CTK_entryDialog(const char *bodytxt,const char *defaulttxt,const char *windowname,const char *dialogtitle,bool hascancel,int dialogwidth)
{
	int						genx,geny,genw;
	CTK_cursesButtonClass	*button;
	CTK_mainAppClass		*app=new CTK_mainAppClass;
	CTK_cursesLabelClass	*label;

	app->CTK_setDialogWindow(windowname,dialogtitle,dialogwidth,11);
	CURRENTPAGE(app).fancyWindow=true;
	this->dialogReturnData.isValidData=false;
	this->dialogReturnData.mc=app;

	genx=CURRENTPAGE(app).boxX+2;
	geny=CURRENTPAGE(app).boxY+2;
	genw=CURRENTPAGE(app).boxW-3;

	label=app->CTK_addNewLabel(genx,geny,genw,4,bodytxt);
	label->CTK_setJustify(CENTREJUSTIFY);

	geny=CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-3;
	this->dialogReturnData.input=app->CTK_addNewInput(CURRENTPAGE(app).boxX+2,geny,CURRENTPAGE(app).boxW-3,1,defaulttxt);
	this->dialogReturnData.input->userData=(void*)CUENTRYINPUTENTER;
	this->dialogReturnData.input->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
	this->dialogReturnData.input->redrawAppWindow=false;

	if(hascancel==false)
		{
			genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,1,11,0);
			button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,11,1,"   OK  ");
			button->userData=(void*)CUENTRYOK;
			button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
		}
	else
		{
			genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,2,11,0);
			button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,11,1,"  OK  ");
			button->userData=(void*)CUENTRYOK;
			button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
			genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,2,11,1);
			button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,11,1,"Cancel");
			button->userData=(void*)CUENTRYCANCEL;
			button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
		}

	app->CTK_setDefaultGadget(this->dialogReturnData.input);

	SETHIDECURS;
	app->CTK_clearScreen();
	app->CTK_updateScreen(app,NULL);
	this->dialogReturnData.input->CTK_doInput();
	app->CTK_mainEventLoop(0,false,true);
	delete app;
	if(this->mc!=NULL)
		{
			this->mc->CTK_clearScreen();
			this->mc->CTK_updateScreen(this->mc,NULL);
		}
	return(this->dialogReturnData.isValidData);
}

/**
* User notification convenience dialog.
*/
bool CTK_cursesUtilsClass::CTK_queryDialog(const char *bodytxt,const char *windowname,const char *dialogtitle,int pbuttons,int dialogwidth)
{
	int						genx,geny,genw;
	CTK_cursesButtonClass	*button;
	CTK_mainAppClass		*app=new CTK_mainAppClass;
	CTK_cursesLabelClass	*label;
	int						btnnum=0;
	int						maxbtns=0;
	CTK_cursesButtonClass	*defhold=NULL;
	int						buttons=pbuttons;

	app->CTK_setDialogWindow(windowname,dialogtitle,dialogwidth,8);
	CURRENTPAGE(app).fancyWindow=true;
	this->dialogReturnData.isValidData=false;
	this->dialogReturnData.mc=app;

	genx=CURRENTPAGE(app).boxX+2;
	geny=CURRENTPAGE(app).boxY+2;
	genw=CURRENTPAGE(app).boxW-3;

	label=app->CTK_addNewLabel(genx,geny,genw,4,bodytxt);
	label->CTK_setJustify(CENTREJUSTIFY);

	geny=CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-3;

	if(buttons>0)
		{
			if((buttons&YESBUTTON)==YESBUTTON)
				maxbtns++;
			if((buttons&NOBUTTON)==NOBUTTON)
				maxbtns++;
			if((buttons&CANCELBUTTON)==CANCELBUTTON)
				maxbtns++;
		}
	else
		{
				buttons=CANCELBUTTON;
				maxbtns++;
		}

	geny=CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1;

	if((buttons&YESBUTTON)==YESBUTTON)
		{
			genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,maxbtns,10,btnnum);
			button=app->CTK_addNewButton(genx,geny,10,1," Yes  ");
			button->userData=(void*)CUQUERYOK;
			button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
			defhold=button;
			btnnum++;
		}

	if((buttons&NOBUTTON)==NOBUTTON)
		{
			genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,maxbtns,10,btnnum);
			button=app->CTK_addNewButton(genx,geny,10,1,"  NO  ");
			button->userData=(void*)CUQUERYNO;
			button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
			defhold=button;
			btnnum++;
		}

	if((buttons&CANCELBUTTON)==CANCELBUTTON)
		{
			genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,maxbtns,10,btnnum);
			button=app->CTK_addNewButton(genx,geny,10,1,"CANCEL");
			button->userData=(void*)CUQUERYCANCEL;
			button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
			defhold=button;
			btnnum++;
		}

	app->CTK_setDefaultGadget(defhold);

	SETHIDECURS;
	app->CTK_mainEventLoop(0,true,true);
	if(this->mc!=NULL)
		{
			this->mc->CTK_clearScreen();
			this->mc->CTK_updateScreen(this->mc,NULL);
		}
	delete app;
	return(this->dialogReturnData.isValidData);
}

/**
* About convenience dialog.
*/
void CTK_cursesUtilsClass::CTK_aboutDialog(const char *appname,const char *appinfo,const char *copyright,const char *email,const char *website,const char *credits,const char *licence,int dialogwidth)
{
	int						genx,geny,genw;
	CTK_cursesButtonClass	*button;
	CTK_mainAppClass		*app=new CTK_mainAppClass;
	CTK_cursesLabelClass	*label;
	CTK_cursesTextBoxClass	*textbox;

	app->CTK_setDialogWindow("About ...","",ABOUTWIDTH,ABOUTHITE);
	CURRENTPAGE(app).fancyWindow=true;
	this->dialogReturnData.isValidData=false;
	this->dialogReturnData.mc=app;

	genx=CURRENTPAGE(app).boxX+2;
	geny=CURRENTPAGE(app).boxY+1;
	genw=CURRENTPAGE(app).boxW-3;

//page 0 main
	label=app->CTK_addNewLabel(genx,geny,genw,4,appname);
	label->CTK_setJustify(CENTREJUSTIFY);
	geny+=2;
	label=app->CTK_addNewLabel(genx,geny,genw,4,appinfo);
	label->CTK_setJustify(CENTREJUSTIFY);
	geny++;
	label=app->CTK_addNewLabel(genx,geny,genw,4,copyright);
	label->CTK_setJustify(CENTREJUSTIFY);
	geny++;
	label=app->CTK_addNewLabel(genx,geny,genw,4,email);
	label->CTK_setJustify(CENTREJUSTIFY);
	geny++;
	label=app->CTK_addNewLabel(genx,geny,genw,4,website);
	label->CTK_setJustify(CENTREJUSTIFY);

	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,3,11,0);
	this->dialogReturnData.defaultGadget=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,11,1," Close ");
	this->dialogReturnData.defaultGadget->userData=(void*)CUABOUTCLOSE;
	this->dialogReturnData.defaultGadget->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,3,11,1);
	button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,11,1,"Credits");
	button->userData=(void*)CUABOUTCREDITS;
	button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,3,11,2);
	button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,11,1,"Licence");
	button->userData=(void*)CUABOUTLICENCE;
	button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);
//page 1 credits
	app->CTK_addPage();
	app->CTK_setDialogWindow("Credits ...","",dialogwidth,-1);
	CURRENTPAGE(app).fancyWindow=true;
	genx=CURRENTPAGE(app).boxX+2;
	geny=CURRENTPAGE(app).boxY+2;
	genw=CURRENTPAGE(app).boxW-3;
	label=app->CTK_addNewLabel(genx,geny,genw,CURRENTPAGE(app).boxH-4,credits);
	label->CTK_setJustify(CENTREJUSTIFY);
	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,1,8,0);
	button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,8,1,"Done");
	button->userData=(void*)CUABOUTDONE;
	button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

//page 2 licence
	app->CTK_addPage();
	app->CTK_setDialogWindow("Licence ...","",dialogwidth,-1);
	CURRENTPAGE(app).fancyWindow=true;
	genx=CURRENTPAGE(app).boxX+2;
	geny=CURRENTPAGE(app).boxY+2;
	genw=CURRENTPAGE(app).boxW-3;
	textbox=app->CTK_addNewTextBox(genx,geny,genw,CURRENTPAGE(app).boxH-4,true,licence,true);
	
	genx=this->CTK_getGadgetPosX(CURRENTPAGE(app).boxX,CURRENTPAGE(app).boxW,1,8,0);
	button=app->CTK_addNewButton(genx,CURRENTPAGE(app).boxY+CURRENTPAGE(app).boxH-1,8,1,"Done");
	button->userData=(void*)CUABOUTDONE;
	button->CTK_setSelectCB(buttonSelectEntryCB,(void*)&this->dialogReturnData);

	app->CTK_setPage(0);
	app->CTK_setDefaultGadget(this->dialogReturnData.defaultGadget);

	SETHIDECURS;
	app->CTK_mainEventLoop(0,true,true);
	if(this->mc!=NULL)
		{
			this->mc->CTK_clearScreen();
			this->mc->CTK_updateScreen(this->mc,NULL);
		}
	delete app;
}

/**
* Calulate position for gadget given size of box, gadget size, gadget count and gadget number.
* \param int startx start position of bounding box.
* \param int size size of bounding box.
* \param int numgads total number of gadgets.
* \param int gadsize gadget size.
* \param int gadnum this gadget number ( base 1 ).
*/
int CTK_cursesUtilsClass::CTK_getGadgetPos(int startx,int size,int numgads,int gadsize,int gadnum)
{
	double	secs;
	int		totgadsize=numgads*gadsize;

	secs=((size-totgadsize)/(numgads+1))+0.5;
	return(startx+(secs*(gadnum))+(gadsize*(gadnum-1)));
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
	CTK_cursesGraphicsClass 	gc(app);
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
	app->CTK_clearScreen();
	txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',app->maxCols-24,app->tabWidth,24,false);

	gc.CTK_drawBox(boxx,boxy,boxw,boxh,PLAINBOX,true,false);
	for(int j=0;j<txtStrings.size();j++)
		{
			MOVETO(centre-((txtStrings[j].length()-offset)/2),11+j);
			printf("%s",txtStrings[j].c_str());
		}
	MOVETO(1,1)
	fflush(NULL);
}

/**
* Load variables into vector of varsStructs.
* \param const char *filepath.
* \param bool clear ( optional ).
* \param std::vector<varsStruct> ( optional, set to defined vars when clear=false ).
* \returns std::vector<varsStruct>.
* \note file format=NAMEOFVARIABLE DATATYPE DATA.
* \note if clear is set all vars are cleared before loading, else loaded var overwrites current var.
* \note Escaped space and tab ( \s and \t ) will be unescaped.
*/
std::vector<varsStruct> CTK_cursesUtilsClass::CTK_loadVars(const char *filepath,bool clear,std::vector<varsStruct> invs)
{
	FILE					*fp;
	char					buffer[PATH_MAX];
	varsStruct				vs;
	varsStruct				vshold;
	char					*varname=(char*)alloca(256);
	char					*data=(char*)alloca(256);
	char					*datatype=(char*)alloca(256);

	if (clear==true)
		invs.clear();

	fp=fopen(filepath,"r");
	if(fp!=NULL)
		{
			while(fgets(buffer,PATH_MAX,fp))
				{
					datatype[0]=0;
					data[0]=0;
					varname[0]=0;
					if(buffer[0]=='\n')
						continue;
					sscanf(buffer,"%[^ ] %[^ ] %[^\n]",varname,datatype,data);

					vs.varName=varname;
					if(strcasecmp(datatype,"number")==0)
						vs.vType=INTVAR;
					if(strcasecmp(datatype,"string")==0)
						vs.vType=CHARVAR;

//boolean data
					if(strcasecmp(datatype,"bool")==0)
						{
							vs.vType=BOOLVAR;
							if(strcasecmp(data,"true")==0)
								vs.boolVar=true;

							if(strcasecmp(data,"false")==0)
								vs.boolVar=false;
						}

					switch(vs.vType)
						{
							case INTVAR:
								vs.intVar=strtol(data,NULL,0);
								break;
							case CHARVAR:
								if(data[strlen(data)-1]=='\n')
									data[strlen(data)-1]=0;
								vs.charVar=data;
								boost::replace_all(vs.charVar,"\\s"," ");
								boost::replace_all(vs.charVar,"\\t","\t");
								break;
						}

					if(clear==true)
						invs.push_back(vs);
					else
						{
							int varentry=CTK_getVarEntry(invs,varname);
							if(varentry==-1)
								invs.push_back(vs);
							else
								invs.at(varentry)=vs;
						}
				}
			fclose(fp);
		}
	return(invs);
}

/**
* Save variables into file from vector of varsStructs.
* \param const char *filepath.
* \param std::vector<varsStruct>.
* \note If "filepath" = "1" output is to stdout, "2" goes to stderr.
* \note Spaces and tabs will be escaped.
*/
void CTK_cursesUtilsClass::CTK_saveVars(const char *filepath,std::vector<varsStruct> vs)
{
	FILE		*fp;
	bool		closefp=false;

	switch(filepath[0])
		{
			case '1':
				fp=stdout;
				break;
			case '2':
				fp=stderr;
				break;
			default:
				fp=fopen(filepath,"w+");
				closefp=true;
				break;
		}

	if(fp!=NULL)
		{
			for(int j=0;j<vs.size();j++)
				{
					switch(vs[j].vType)
						{
							case BOOLVAR:
							if(vs[j].boolVar==true)
								fprintf(fp,"%s bool true\n",vs[j].varName.c_str());
							else
								fprintf(fp,"%s bool false\n",vs[j].varName.c_str());
								break;
							case INTVAR:
								if(vs[j].outIntType==DECIMALOUT)
									fprintf(fp,"%s number %i\n",vs[j].varName.c_str(),vs[j].intVar);
								if(vs[j].outIntType==HEXOUT)
									fprintf(fp,"%s number 0x%x\n",vs[j].varName.c_str(),vs[j].intVar);
								if(vs[j].outIntType==OCTALOUT)
									fprintf(fp,"%s number 0%o\n",vs[j].varName.c_str(),vs[j].intVar);
								break;
							case CHARVAR:
								{
									std::string tstr=vs[j].charVar;
									boost::replace_all(tstr," ","\\s");
									boost::replace_all(tstr,"\t","\\t");
									fprintf(fp,"%s string %s\n",vs[j].varName.c_str(),tstr.c_str());
								}
								break;
						}
				}
			if(closefp==true)
				fclose(fp);
		}
}

/**
* find variable from vector of varsStructs.
* \param const char *varname.
* \param std::vector<varsStruct>.
* \note On return varsStruct.vType is set to type of var found.
* \note A type of BADTYPE indicates var not found.
*/
varsStruct CTK_cursesUtilsClass::CTK_findVar(std::vector<varsStruct> vs,const char *varname)
{
	varsStruct retvs;

	for(unsigned j=0;j<vs.size();j++)
		{
			if(varname,vs[j].varName==varname)
				return(vs[j]);
		}
	return(retvs);
}

/**
* find variable from vector of varsStructs.
* \param const char *varname.
* \param std::vector<varsStruct>.
* \note On return varsStruct.vType is set to type of var found.
* \note A type of BADTYPE indicates var not found.
*/
int CTK_cursesUtilsClass::CTK_getVarEntry(std::vector<varsStruct> vs,const char *varname)
{
	for(unsigned j=0;j<vs.size();j++)
		{
			if(vs[j].varName.compare(varname)==0)
				return(j);
		}
	return(-1);
}
