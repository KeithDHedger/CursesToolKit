/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:49:48 GMT 2019 keithdhedger@gmail.com

 * This file (cursesApplication.cpp) is part of CursesToolKit.

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

#include <poll.h>
#include "cursesGlobals.h"

/**
*  \brief Main app class destroy.
*/
CTK_mainAppClass::~CTK_mainAppClass()
{
	delete this->menuBar;
	for(int k=0;k<this->pages.size();k++)
		{
			for(int j=0;j<this->pages[k].labels.size();j++)
				delete this->pages[k].labels[j];
			for(int j=0;j<this->pages[k].textBoxes.size();j++)
				delete this->pages[k].textBoxes[j];
			for(int j=0;j<this->pages[k].buttons.size();j++)
				delete this->pages[k].buttons[j];
			for(int j=0;j<this->pages[k].inputs.size();j++)
				delete this->pages[k].inputs[j];
			for(int j=0;j<this->pages[k].lists.size();j++)
				delete this->pages[k].lists[j];
			for(int j=0;j<this->pages[k].checkBoxes.size();j++)
				delete this->pages[k].checkBoxes[j];
			for(int j=0;j<this->pages[k].editBoxes.size();j++)
				delete this->pages[k].editBoxes[j];
			for(int j=0;j<this->pages[k].srcEditBoxes.size();j++)
				delete this->pages[k].srcEditBoxes[j];
			for(int j=0;j<this->pages[k].choosers.size();j++)
				delete this->pages[k].choosers[j];
			for(int j=0;j<this->pages[k].dropDowns.size();j++)
				delete this->pages[k].dropDowns[j];
			for(int j=0;j<this->pages[k].fbImages.size();j++)
				delete this->pages[k].fbImages[j];
		}

	this->pages.clear();
	termkey_destroy(this->tk);
	fflush(NULL);
	Magick::TerminateMagick();//is this needed?
	munmap((void*)frameBufferData.frameBufferMapPtr,frameBufferData.screensize);
}

/**
*  \brief Main app class.
*/
CTK_mainAppClass::CTK_mainAppClass()
{
	int		fbfd=0;
	winsize	w;

    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
	this->maxRows=w.ws_row;
	this->maxCols=w.ws_col;
	this->pages.clear();
	this->CTK_addPage();
	this->pageNumber=0;

	this->tk=termkey_new(0,TERMKEY_FLAG_CTRLC|TERMKEY_FLAG_SPACESYMBOL);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}

	if(frameBufferData.fbIsMapped==false)
		{
// Open the file for reading and writing
			fbfd=open("/dev/fb0",O_RDWR);
			if(!fbfd)
				{
					fprintf(stderr,"Error: cannot open framebuffer device.\n");
					return;
				}

// Get fixed screen information
			if(ioctl(fbfd,FBIOGET_FSCREENINFO,&frameBufferData.frameBufferInfo))
				{
					fprintf(stderr,"Error reading fixed information.\n");
					return;
				}

// map fb to user mem
			frameBufferData.screensize=frameBufferData.frameBufferInfo.smem_len;
			frameBufferData.frameBufferMapPtr=(char*)mmap(NULL,frameBufferData.screensize,(PROT_READ | PROT_WRITE),MAP_SHARED,fbfd,0);
			close(fbfd);
			frameBufferData.fbIsMapped=true;

#ifdef _IMAGEMAGICK_
//init imagemagick
			Magick::InitializeMagick(NULL);
#endif					
			this->frameBufferData.charWidth=frameBufferData.frameBufferInfo.line_length/this->maxCols/4;
			this->frameBufferData.charHeight=frameBufferData.screensize/frameBufferData.frameBufferInfo.line_length/this->maxRows;
			this->frameBufferData.screenWidth=frameBufferData.frameBufferInfo.line_length/4;
			this->frameBufferData.screenHeight=frameBufferData.screensize/frameBufferData.frameBufferInfo.line_length;
			//fprintf(stderr,"cw=%i ch=%i\n",this->frameBufferData.charWidth,this->frameBufferData.charHeight);
		}
}

/**
* Clear screen. 
*/
void CTK_mainAppClass::CTK_clearScreen(void)
{
	setBothColours(this->colours.windowForeCol,this->colours.windowBackCol,this->colours.use256Colours);
	printf("\e[2J\e[H");
	fflush(NULL);
}

/**
* Add main menu bar. 
*/
void CTK_mainAppClass::CTK_addNewMenuBar(void)
{
	this->menuBar=new CTK_cursesMenuClass();
	this->menuBar->CTK_setUpdateCB(this->CTK_updateScreen,this);
	this->menuBar->CTK_setColours(this->colours);
}

/**
* Create and add new text box gadget.
* \note text only.
*/
CTK_cursesTextBoxClass* CTK_mainAppClass::CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass();
	txtbox->tabWidth=this->tabWidth;
	txtbox->CTK_newBox(x,y,width,hite,txt,selectable);
	txtbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].textBoxes.push_back(txtbox);
	return(txtbox);
}

/**
* Create and add new text box gadget.
* \note can be a file.
*/
CTK_cursesTextBoxClass* CTK_mainAppClass::CTK_addNewTextBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass();
	txtbox->tabWidth=this->tabWidth;
	txtbox->CTK_newBox(x,y,width,hite,"",selectable);
	txtbox->CTK_updateText(txt,isfilename);
	txtbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].textBoxes.push_back(txtbox);
	return(txtbox);
}

/**
* Create and add new button gadget. 
*/
CTK_cursesButtonClass* CTK_mainAppClass::CTK_addNewButton(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesButtonClass	*btn=new CTK_cursesButtonClass();
	btn->CTK_newButton(x,y,width,hite,label);
	btn->CTK_setColours(this->colours);
	this->pages[this->pageNumber].buttons.push_back(btn);
	return(btn);
}

/**
* Create and add new input gadget. 
*/
CTK_cursesInputClass* CTK_mainAppClass::CTK_addNewInput(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesInputClass	*inp=new CTK_cursesInputClass();
	inp->CTK_newInput(x,y,width,hite,label);
	inp->CTK_setColours(this->colours);
	this->pages[this->pageNumber].inputs.push_back(inp);
	return(inp);
}

CTK_cursesListBoxClass* CTK_mainAppClass::CTK_addNewListBox(int x,int y,int width,int hite)
{
	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass();
	lb->CTK_newListBox(x,y,width,hite);
	lb->CTK_setColours(this->colours);
	this->pages[this->pageNumber].lists.push_back(lb);
	return(lb);
}

/**
* Create and add new check box gadget. 
*/
CTK_cursesCheckBoxClass* CTK_mainAppClass::CTK_addNewCheckBox(int x,int y,int width,const char *label)
{
	CTK_cursesCheckBoxClass	*cb=new CTK_cursesCheckBoxClass();
	cb->CTK_newCheckBox(x,y,width,label);
	cb->CTK_setColours(this->colours);
	this->pages[this->pageNumber].checkBoxes.push_back(cb);
	return(cb);
}

/**
* Create and add new edit box gadget. 
*/
CTK_cursesEditBoxClass* CTK_mainAppClass::CTK_addNewEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesEditBoxClass	*edbox=new CTK_cursesEditBoxClass();
	edbox->tabWidth=this->tabWidth;
	edbox->CTK_newBox(x,y,width,hite,isfilename,txt,selectable);
	edbox->mc=mc;
	edbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].editBoxes.push_back(edbox);
	return(edbox);
}

/**
* Create and add new source edit box gadget. 
*/
CTK_cursesSourceEditBoxClass* CTK_mainAppClass::CTK_addNewSourceEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesSourceEditBoxClass	*edbox=new CTK_cursesSourceEditBoxClass();
	edbox->tabWidth=this->tabWidth;
	edbox->CTK_newBox(x,y,width,hite,isfilename,txt,selectable);
	edbox->mc=mc;
	edbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].srcEditBoxes.push_back(edbox);
	return(edbox);
}

/**
* Create and add new label gadget. 
*/
CTK_cursesLabelClass* CTK_mainAppClass::CTK_addNewLabel(int x,int y,int width,int hite,const char *txt)
{
	CTK_cursesLabelClass	*label=new CTK_cursesLabelClass();
	label->tabWidth=this->tabWidth;
	label->CTK_newLabel(x,y,width,hite,txt);
	label->CTK_setColours(this->colours);
	this->pages[this->pageNumber].labels.push_back(label);
	return(label);
}

/**
* Create and add new drop down gadget. 
*/
CTK_cursesDropClass* CTK_mainAppClass::CTK_addNewDropDownBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,const char *label)
{
	CTK_cursesDropClass	*dropbox=new CTK_cursesDropClass();
	dropbox->CTK_newDropButton(x,y,width,hite,label);
	dropbox->mc=mc;
	dropbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].dropDowns.push_back(dropbox);
	return(dropbox);
}

/**
* Create and add new frame buffer image. 
*/
CTK_cursesFBImageClass* CTK_mainAppClass::CTK_addNewFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect)
{
	CTK_cursesFBImageClass	*fbi=new CTK_cursesFBImageClass();
	fbi->mc=this;
	fbi->CTK_newFBImage(x,y,width,hite,filepath,keepaspect);
	this->pages[this->pageNumber].fbImages.push_back(fbi);
	return(fbi);
}

/**
* Add menu bar. 
*/
void CTK_mainAppClass::CTK_addMenuBar(CTK_cursesMenuClass *mb)
{
	this->menuBar=mb;
}

/**
* Add text box gadget. 
*/
void CTK_mainAppClass::CTK_addTextBox(CTK_cursesTextBoxClass *txtbox)
{
	this->pages[this->pageNumber].textBoxes.push_back(txtbox);
}

/**
* Add button gadget. 
*/
void CTK_mainAppClass::CTK_addButton(CTK_cursesButtonClass *btn)
{
	this->pages[this->pageNumber].buttons.push_back(btn);
}

/**
* Add input box gadget. 
*/
void CTK_mainAppClass::CTK_addInput(CTK_cursesInputClass *inp)
{
	this->pages[this->pageNumber].inputs.push_back(inp);
}

/**
* Add new list box gadget. 
*/
void CTK_mainAppClass::CTK_addListBox(CTK_cursesListBoxClass *lb)
{
	this->pages[this->pageNumber].lists.push_back(lb);
}

/**
* Add check box gadget. 
*/
void CTK_mainAppClass::CTK_addCheckBox(CTK_cursesCheckBoxClass *cb)
{
	this->pages[this->pageNumber].checkBoxes.push_back(cb);
}

/**
* Add edit box gadget. 
*/
void CTK_mainAppClass::CTK_addEditBox(CTK_cursesEditBoxClass *edbox)
{
	this->pages[this->pageNumber].editBoxes.push_back(edbox);
}

/**
* Add source edit box gadget. 
*/
void CTK_mainAppClass::CTK_addSourceEditBox(CTK_cursesSourceEditBoxClass *edbox)
{
	this->pages[this->pageNumber].srcEditBoxes.push_back(edbox);
}

/**
* Add label gadget. 
*/
void CTK_mainAppClass::CTK_addLabel(CTK_cursesLabelClass *label)
{
	this->pages[this->pageNumber].labels.push_back(label);
}

/**
* Add chooser box gadget. 
*/
void CTK_mainAppClass::CTK_addChooserBox(CTK_cursesChooserClass *cb)
{
	this->pages[this->pageNumber].choosers.push_back(cb);
}

/**
* Add drop down gadget. 
*/
void CTK_mainAppClass::CTK_addDropBox(CTK_cursesDropClass *dd)
{
	this->pages[this->pageNumber].dropDowns.push_back(dd);
}

/**
* Redraw the screen.
* \note object=ptr to main class object
* \note sets highlight if needed.
*/
void CTK_mainAppClass::CTK_updateScreen(void *object,void* userdata)
{
	CTK_mainAppClass		*app=static_cast<CTK_mainAppClass*>(object);
	CTK_cursesGraphicsClass	cu;

	setBothColours(app->colours.windowForeCol,app->colours.windowBackCol,app->colours.use256Colours);

	if((app->menuBar!=NULL) && (app->menuBar->CTK_getMenuBarVisible()==true))
		app->menuBar->CTK_drawMenuBar();

	if(app->pageNumber<0)
		{
			app->CTK_clearScreen();
			if((app->menuBar!=NULL) && (app->menuBar->CTK_getMenuBarVisible()==true))
			//if(app->menuBar!=NULL)
				app->menuBar->CTK_drawMenuBar();
			fflush(NULL);
			SETNORMAL;
			return;
		}

	if(app->useAppWindow==true)
		{
			int	yadj=0;
			if((app->menuBar!=NULL) && (app->menuBar->CTK_getMenuBarVisible()==true))
		//	if(app->menuBar!=NULL)
				yadj=1;
			setBackColour(BACK_BLUE,false);
//TODO//FLICKER//
			printf("\e[2J\e[H");
			if(app->windowName!=NULL)
				cu.CTK_drawBox(app->x,app->y,app->wid,app->hite,OUTBOX,true,true);
			else
				cu.CTK_drawBox(app->x,app->y-1,app->wid,app->hite+1,OUTBOX,true,true);
			MOVETO(1,1+yadj);
			setBothColours(FORE_CYAN,BACK_BLUE,false);//TODO//
			if(app->windowName!=NULL)
				{
					printf("%s\n",app->windowName);
					SETALTCHARSET;
					for(int j=0;j<app->maxCols;j++)
						printf(HBAR);
					SETNORMCHARSET;
				}
			if(app->title!=NULL)
				{
					MOVETO(app->x+(app->wid/2)-(strlen(app->title)/2),app->y)
					setBothColours(FORE_BLUE,app->colours.backCol,false);//TODO//
					printf("%s",app->title);
				}
		}

	for(int j=0;j<app->pages[app->pageNumber].fbImages.size();j++)
		app->pages[app->pageNumber].fbImages[j]->CTK_drawFBImage();

	for(int j=0;j<app->pages[app->pageNumber].dropDowns.size();j++)
		{
			if(app->hiliteDropBoxNum==j)
				app->pages[app->pageNumber].dropDowns[j]->CTK_drawDropButton(true);
			else
				app->pages[app->pageNumber].dropDowns[j]->CTK_drawDropButton(false);
		}

	for(int j=0;j<app->pages[app->pageNumber].labels.size();j++)
		app->pages[app->pageNumber].labels[j]->CTK_drawLabel();

	for(int j=0;j<app->pages[app->pageNumber].textBoxes.size();j++)
		{
			if(app->hiliteTxtBoxNum==j)
				app->pages[app->pageNumber].textBoxes[j]->CTK_drawBox(true);
			else
				app->pages[app->pageNumber].textBoxes[j]->CTK_drawBox(false);
		}

	for(int j=0;j<app->pages[app->pageNumber].buttons.size();j++)
		{
			if(app->hiliteBtnNum==j)
				app->pages[app->pageNumber].buttons[j]->CTK_drawButton(true);
			else
				app->pages[app->pageNumber].buttons[j]->CTK_drawButton(false);
		}

	for(int j=0;j<app->pages[app->pageNumber].inputs.size();j++)
		{
			if(app->hiliteInputNum==j)
				app->pages[app->pageNumber].inputs[j]->CTK_drawInput(true);
			else
				app->pages[app->pageNumber].inputs[j]->CTK_drawInput(false);
		}

	for(int j=0;j<app->pages[app->pageNumber].checkBoxes.size();j++)
		{
			if(app->hiliteCheckBoxNum==j)
				app->pages[app->pageNumber].checkBoxes[j]->CTK_drawCheckBox(true);
			else
				app->pages[app->pageNumber].checkBoxes[j]->CTK_drawCheckBox(false);
		}

	for(int j=0;j<app->pages[app->pageNumber].editBoxes.size();j++)
		{
			if(app->hiliteEditBoxNum==j)
				app->pages[app->pageNumber].editBoxes[j]->CTK_drawBox(true,false);
			else
				app->pages[app->pageNumber].editBoxes[j]->CTK_drawBox(false,false);
		}

	for(int j=0;j<app->pages[app->pageNumber].srcEditBoxes.size();j++)
		{
			if(app->hiliteSourceEditBoxNum==j)
				app->pages[app->pageNumber].srcEditBoxes[j]->CTK_drawBox(true);
			else
				app->pages[app->pageNumber].srcEditBoxes[j]->CTK_drawBox(false);
		}

	for(int j=0;j<app->pages[app->pageNumber].lists.size();j++)
		{
			if(app->hiliteListNum==j)
				app->pages[app->pageNumber].lists[j]->CTK_drawListWindow(true);
			else
				app->pages[app->pageNumber].lists[j]->CTK_drawListWindow(false);
		}

	SETNORMAL;
}

/**
* \note private.
*/
void CTK_mainAppClass::setHilite(bool forward)
{
	int addit=1;
	if(forward==false)
		addit=-1;

	switch(this->hiliting)
		{
			case HLNONE:
				this->hiliting+=addit;
				if(this->hiliting==-2)
					{
						this->hiliting=HLNOMORE-1;
						this->hiliteEditBoxNum=this->pages[this->pageNumber].editBoxes.size();
					}
				this->setHilite(forward);
				break;
			case HLBUTTONS:
				this->hiliteBtnNum+=addit;
				if((this->hiliteBtnNum>=0) && (this->hiliteBtnNum<this->pages[this->pageNumber].buttons.size()))
					{
						if(this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->CTK_getEnabled()==false)
							this->hiliteBtnNum+=addit;
					}

				if((this->hiliteBtnNum<0) || (this->hiliteBtnNum>=this->pages[this->pageNumber].buttons.size()))
					{
						this->hiliteBtnNum=-1;
						this->hiliting+=addit;
						if(addit==-1)
							this->hiliting=HLNONE;
						else
							this->setHilite(forward);
					}
				break;

			case HLDROPDOWNS:
				this->hiliteDropBoxNum+=addit;
				if((this->hiliteDropBoxNum<0) || (this->hiliteDropBoxNum>=this->pages[this->pageNumber].dropDowns.size()))
					{
						this->hiliteDropBoxNum=-1;
						if(addit==-1)
							this->hiliteBtnNum=this->pages[this->pageNumber].buttons.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				break;

			case HLTEXT:
				this->hiliteTxtBoxNum+=addit;
				if((this->hiliteTxtBoxNum<0) || (this->hiliteTxtBoxNum>=this->pages[this->pageNumber].textBoxes.size()))
					{
						this->hiliteTxtBoxNum=-1;
						if(addit==-1)
							this->hiliteDropBoxNum=this->pages[this->pageNumber].dropDowns.size();
						this->hiliting+=addit;
						this->setHilite(forward);
						break;
					}
				else
					{
						if(this->pages[this->pageNumber].textBoxes[this->hiliteTxtBoxNum]->CTK_getSelectable()==false)
							this->setHilite(forward);
						break;
					}
				break;
			case HLINPUTS:
				this->hiliteInputNum+=addit;
				if((this->hiliteInputNum<0) || (this->hiliteInputNum>=this->pages[this->pageNumber].inputs.size()))
					{
						this->hiliteInputNum=-1;
						if(addit==-1)
							this->hiliteTxtBoxNum=this->pages[this->pageNumber].textBoxes.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				break;
			case HLLISTS:
				this->hiliteListNum+=addit;
				if((this->hiliteListNum<0) || (this->hiliteListNum>=this->pages[this->pageNumber].lists.size()))
					{
						this->hiliteListNum=-1;
						if(addit==-1)
							this->hiliteInputNum=this->pages[this->pageNumber].inputs.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				break;
			case HLCHKBOXS:
				this->hiliteCheckBoxNum+=addit;
				if((this->hiliteCheckBoxNum<0) || (this->hiliteCheckBoxNum>=this->pages[this->pageNumber].checkBoxes.size()))
					{
						this->hiliteCheckBoxNum=-1;
						if(addit==-1)
							this->hiliteListNum=this->pages[this->pageNumber].lists.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				break;
			case HLEDITBOXES:
				this->hiliteEditBoxNum+=addit;
				if((this->hiliteEditBoxNum<0) || (this->hiliteEditBoxNum>=this->pages[this->pageNumber].editBoxes.size()))
					{
						this->hiliteEditBoxNum=-1;
						if(addit==-1)
							this->hiliteCheckBoxNum=this->pages[this->pageNumber].checkBoxes.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				break;
			case HLSRCEDITBOXES:
				this->hiliteSourceEditBoxNum+=addit;
				if((this->hiliteSourceEditBoxNum<0) || (this->hiliteSourceEditBoxNum>=this->pages[this->pageNumber].srcEditBoxes.size()))
					{
						this->hiliteSourceEditBoxNum=-1;
						if(addit==-1)
							this->hiliteEditBoxNum=this->pages[this->pageNumber].editBoxes.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				break;
			case HLNOMORE:
				this->hiliting=HLNONE;
				break;
		}
}

/**
* Main event loop.
* \note Handles highlighting selecting etc etc.
* \note runcnt<0 Wait upto abs(runcnt) ms for a keypress, run 1 main loop if timeout.
* \note runcnt=0 Default, run main loop continously.
* \note runcnt>0 run main loop runcnt times.
*/
void CTK_mainAppClass::CTK_mainEventLoop(int runcnt)
{
	int				selection=CONT;
	TermKeyResult	ret;
	TermKeyKey		key;
	char			tstr[3]={'_',0,0};
	int				tab=1;
	int				countdown=runcnt;
	struct pollfd	fd;

	this->CTK_clearScreen();
	this->CTK_updateScreen(this,NULL);
	this->useAppWindow=false;
	SETHIDECURS;
	fflush(NULL);
	this->runEventLoop=true;

	fd.fd=0;/* the file descriptor we passed to termkey_new() */
	fd.events=POLLIN;

	while(this->runEventLoop==true)
		{
			if(runcnt<0)
				{
					poll(&fd,1,runcnt*-1);
					this->runEventLoop=false;// Timed out
					if(fd.revents&(POLLIN|POLLHUP|POLLERR))
						termkey_advisereadable(tk);

					while((ret=termkey_getkey(tk,&key))==TERMKEY_RES_KEY);
				}
			else
				{
					ret=termkey_waitkey(this->tk,&key);
				}
			if(this->eventLoopCBIn!=NULL)
				this->eventLoopCBIn(this,this->userData);

//fprintf(stderr,"key.code.sym=%i<<\n",key.code.sym);
//fprintf(stderr,"key.code.codepoint=%i<<\n",key.code.codepoint);
//fprintf(stderr,"key.modifiers=%i<<\n",key.modifiers);
//char	*buffer=(char*)alloca(256);;
//termkey_strfkey(tk, buffer, 255, &key, format);
//fprintf(stderr,"buffer=%s<<\n",buffer);
//fprintf(stderr,"keymod=%i key=%i =%c\n",key.modifiers,key.code.codepoint,key.code.codepoint);
			switch(key.type)
				{
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_ESCAPE:
										if(this->pages[this->pageNumber].menusActive==false)
											break;
										this->hiliteBtnNum=-1;
										this->hiliteTxtBoxNum=-1;
										this->hiliteInputNum=-1;
										this->hiliteListNum=-1;
										this->hiliteCheckBoxNum=-1;
										this->hiliteTxtBoxNum=-1;
										this->hiliteSourceEditBoxNum=-1;
										this->hiliteDropBoxNum=-1;
										this->hiliting=HLNONE;
										this->CTK_updateScreen(this,NULL);
										if((this->menuBar!=NULL) && (this->menuBar->CTK_getMenuBarEnable()==true) && (this->menuBar->CTK_getMenuBarVisible()==true))
											{
												selection=this->menuBar->CTK_doMenuEvent(0,1,true);
												this->menuBar->CTK_drawDefaultMenuBar();
											}
										break;

//next group
									case TERMKEY_SYM_RIGHT:
										this->hiliting++;
										this->setHiliteNone();
										setHilite(true);
										break;
//prev group
									case TERMKEY_SYM_LEFT:
										this->setHiliteNone();
										setHilite(false);
										this->hiliting++;
										setHilite(false);
										break;
//tab select
									case TERMKEY_SYM_TAB:
										if((key.modifiers==1) || (key.modifiers==2))
											setHilite(false);
										else
											setHilite(true);
										break;

//scroll txt boxes and lists
									case TERMKEY_SYM_UP:
//prev button/checkbox
										if((this->hiliteBtnNum!=-1) || (this->hiliteCheckBoxNum!=-1) || (this->hiliteDropBoxNum!=-1))
											{
												setHilite(false);
												break;
											}
										if(this->hiliteTxtBoxNum!=-1)
											this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(true);
										if(this->hiliteListNum!=-1)
											this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(true);
										if(this->hiliteEditBoxNum!=-1)
											{
												if(this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getCurrentY()==0)
													break;
												this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getCursLine()-1);
											}
										if(this->hiliteSourceEditBoxNum!=-1)
											{
												if(this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getCurrentY()==0)
													break;
												this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getCursLine()-1);
											}
										break;

									case TERMKEY_SYM_DOWN:
//next button/checkbox
										if((this->hiliteBtnNum!=-1) || (this->hiliteCheckBoxNum!=-1) || (this->hiliteDropBoxNum!=-1))
											{
												setHilite(true);
												break;
											}
										if(this->hiliteTxtBoxNum!=-1)
											this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(false);
										if(this->hiliteListNum!=-1)
											this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(false);
										if(this->hiliteEditBoxNum!=-1)
											this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getCursLine()+1);
										if(this->hiliteSourceEditBoxNum!=-1)
											this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getCursLine()+1);
										break;

									case TERMKEY_SYM_PAGEUP:
										if(this->hiliteEditBoxNum!=-1)
											{
												if(this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getCurrentY()-this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getHeight()<=0)
													{
														this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_gotoLine(1);
														break;
													}
												this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getCursLine()-this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getHeight());
											}
										if(this->hiliteSourceEditBoxNum!=-1)
											{
												if(this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getCurrentY()-this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getHeight()<=0)
													{
														this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_gotoLine(1);
														break;
													}
												this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getCursLine()-this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getHeight());
											}

											if(this->hiliteTxtBoxNum!=-1)
												this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(true);
											if(this->hiliteListNum!=-1)
												this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(true,true);
											break;

									case TERMKEY_SYM_PAGEDOWN:
											if(this->hiliteEditBoxNum!=-1)
												this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getCursLine()+this->pages[this->pageNumber].editBoxes[hiliteEditBoxNum]->CTK_getHeight());
											
											if(this->hiliteSourceEditBoxNum!=-1)
												this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_gotoLine(this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getCursLine()+this->pages[this->pageNumber].srcEditBoxes[hiliteSourceEditBoxNum]->CTK_getHeight());

											if(this->hiliteTxtBoxNum!=-1)
												this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(false);
											if(this->hiliteListNum!=-1)
												this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(false,true);
											break;

									//case TERMKEY_SYM_ENTER:
									default:
										if((key.code.sym!=this->selectKey) && (this->hiliteCheckBoxNum==-1) && (this->hiliteListNum==-1) && (this->hiliteDropBoxNum==-1))
											break;
//do drop down events
										if(this->hiliteDropBoxNum!=-1)
											{
												this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum]->CTK_doDropDownEvent();
												if((this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum]->selectCB!=NULL) && (this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum]->selectedItem>-1))
													this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum]->selectCB((void*)this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum],(void*)this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum]->selectCBUserData);
												if(this->pages[this->pageNumber].dropDowns[this->hiliteDropBoxNum]->CTK_getEnterDeselects()==true)
													this->hiliteDropBoxNum=-1;
												this->CTK_updateScreen(this,NULL);
											}

//do src edit events
										if(this->hiliteSourceEditBoxNum!=-1)
											{
												this->pages[this->pageNumber].srcEditBoxes[this->hiliteSourceEditBoxNum]->CTK_doEvent(true,this->pages[this->pageNumber].srcEditBoxes[this->hiliteSourceEditBoxNum]->CTK_getStrings(),this->pages[this->pageNumber].srcEditBoxes[this->hiliteSourceEditBoxNum]->CTK_getSrcStrings());
												this->hiliteSourceEditBoxNum=-1;
												this->CTK_emptyIPBuffer();
												this->CTK_updateScreen(this,NULL);
												if(this->eventLoopCBOut!=NULL)
													this->eventLoopCBOut(this,this->userData);
												continue;
											}

//do edit events
										if(this->hiliteEditBoxNum!=-1)
											{
												this->pages[this->pageNumber].editBoxes[this->hiliteEditBoxNum]->CTK_doEvent(false,this->pages[this->pageNumber].editBoxes[this->hiliteEditBoxNum]->CTK_getStrings(),this->pages[this->pageNumber].editBoxes[this->hiliteEditBoxNum]->CTK_getStrings());
												this->hiliteEditBoxNum=-1;
												this->CTK_emptyIPBuffer();
												this->CTK_updateScreen(this,NULL);
												if(this->eventLoopCBOut!=NULL)
													this->eventLoopCBOut(this,this->userData);
												continue;
											}
											
										if(this->hiliteBtnNum!=-1)
											{
												if(this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->CTK_getEnabled()==false)
													{
														continue;
													}
												//else
												//	{
												if(this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->selectCB!=NULL)
													this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->selectCB((void*)this->pages[this->pageNumber].buttons[this->hiliteBtnNum],(void*)this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->selectCBUserData);
													}
												if(this->pages[this->pageNumber].buttons.size()>this->hiliteBtnNum)
													if(this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->CTK_getEnterDeselects()==true)
														this->hiliteBtnNum=-1;
												this->CTK_updateScreen(this,NULL);
											//}

										if(this->hiliteInputNum!=-1)
											{
												this->pages[this->pageNumber].inputs[this->hiliteInputNum]->CTK_doInput();
												this->hiliteInputNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
//lists
										if(this->hiliteListNum!=-1)
											{
												if(key.code.sym!=this->pages[this->pageNumber].lists[this->hiliteListNum]->CTK_getSelectKey())
													break;

												if(this->pages[this->pageNumber].lists[this->hiliteListNum]->CTK_getMultipleSelect()==true)
													{
														this->pages[this->pageNumber].lists[this->hiliteListNum]->CTK_toggleItem(this->pages[this->pageNumber].lists[this->hiliteListNum]->listItemNumber);
													}
												if(this->pages[this->pageNumber].lists[this->hiliteListNum]->selectCB!=NULL)
													this->pages[this->pageNumber].lists[this->hiliteListNum]->selectCB((void*)this->pages[this->pageNumber].lists[this->hiliteListNum],this->pages[this->pageNumber].lists[this->hiliteListNum]->selectCBUserData);
												if(this->pages[this->pageNumber].lists.size()>this->hiliteListNum)
													if(this->pages[this->pageNumber].lists[this->hiliteListNum]->CTK_getEnterDeselects()==true)
														this->hiliteListNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
//check boxes
										if(this->hiliteCheckBoxNum!=-1)
											{
												if(key.code.sym!=this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->CTK_getSelectKey())
													break;
												if(this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->selectCB!=NULL)
													this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->selectCB((void*)this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum],this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->selectCBUserData);
												if(this->pages[this->pageNumber].checkBoxes.size()>this->hiliteCheckBoxNum)
													if(this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->CTK_getEnterDeselects()==true)
														this->hiliteCheckBoxNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										break;
								}
						}

					case TERMKEY_TYPE_UNICODE:
						if((this->menuBar==NULL) || (this->menuBar->enableShortcuts==false))
							break;
						if(key.modifiers==TERMKEY_KEYMOD_CTRL)
							{
								tstr[1]=toupper(key.code.codepoint);
								//tstr[1]=key.code.codepoint;
								for(int j=0;j<this->menuBar->menuNames.size();j++)
									{
										if(this->menuBar->CTK_doShortCutKey(tstr[1],j)==true)
											{
												this->menuBar->menuNumber=j;
												this->menuBar->selectCB(this->menuBar);
												break;
											}
									}
							}
						break;
				}
			this->CTK_emptyIPBuffer();
			this->CTK_updateScreen(this,NULL);
			if(this->eventLoopCBOut!=NULL)
				this->eventLoopCBOut(this,this->userData);
			if(runcnt>0)
				{
					countdown--;
					if(countdown==0)
						this->runEventLoop=false;
				}
		}
}

/**
* Set gadget colours and box styles etc.
*/
void CTK_mainAppClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}

int CTK_mainAppClass::CTK_addPage(void)
{
	pageStruct	ps;

	ps.buttons.clear();
	ps.textBoxes.clear();
	ps.inputs.clear();
	ps.lists.clear();
	ps.checkBoxes.clear();
	ps.editBoxes.clear();
	ps.srcEditBoxes.clear();
	ps.labels.clear();

	this->pages.push_back(ps);
	this->pageNumber=this->pages.size()-1;
	return(this->pageNumber);
}

/**
* Set current page and update screen.
*/
void CTK_mainAppClass::CTK_setPage(int pagenum)
{
	if((pagenum>=0) && (pagenum<this->pages.size()))
		this->pageNumber=pagenum;
	this->CTK_clearScreen();
}

/**
* Goto prev page and update screen.
*/
int CTK_mainAppClass::CTK_previousPage(void)
{
	if(this->pageNumber>0)
		this->pageNumber--;
	this->CTK_clearScreen();
	return(this->pageNumber);
}

/**
* Goto next page and update screen.
*/
int CTK_mainAppClass::CTK_nextPage(void)
{
	if(this->pageNumber<this->pages.size()-1)
		this->pageNumber++;
	this->CTK_clearScreen();
	return(this->pageNumber);
}

/**
* Delete page and gadgets.
* \note freeing any user data set is up to the caller.
*/
int CTK_mainAppClass::CTK_removePage(int pagenum)
{
	if((pagenum>=0) && (pagenum<this->pages.size()))
		{
			for(int j=0;j<this->pages[pagenum].labels.size();j++)
				delete this->pages[pagenum].labels[j];
			for(int j=0;j<this->pages[pagenum].textBoxes.size();j++)
				delete this->pages[pagenum].textBoxes[j];
			for(int j=0;j<this->pages[pagenum].buttons.size();j++)
				delete this->pages[pagenum].buttons[j];
			for(int j=0;j<this->pages[j].inputs.size();j++)
				delete this->pages[pagenum].inputs[j];
			for(int j=0;j<this->pages[j].lists.size();j++)
				delete this->pages[pagenum].lists[j];
			for(int j=0;j<this->pages[j].checkBoxes.size();j++)
				delete this->pages[pagenum].checkBoxes[j];
			this->pages.erase(this->pages.begin()+pagenum);
			this->pageNumber=pagenum;
			if(this->pageNumber>=this->pages.size())
				this->pageNumber=this->pages.size()-1;
		}
	return(this->pageNumber);
}

/**
* Set page user data.
*/
void CTK_mainAppClass::CTK_setPageUserData(int pagenum,void *userdata)
{
	this->pages[pagenum].userData=userdata;
}

/**
* Set an application window.
* \note see the utility dialogs.
*/
void CTK_mainAppClass::CTK_appWindow(int x,int y,int w,int h,const char *windowname,const char *title)
{
	this->x=x;
	this->y=y;
	this->wid=w;
	this->hite=h;
	this->windowName=windowname;
	this->title=title;
	this->useAppWindow=true;
}

/**
* Set application tab with.
* \note also sets terminal/console tab width via 'tabs' application.
*/
void CTK_mainAppClass::CTK_setTabWidth(int width)
{
	char	buffer[256];

	this->tabWidth=width;
	sprintf(buffer,"tabs -%i",width);
	system(buffer);
	fflush(NULL);
}

/**
* Empty the i/p buffer.
* \note mostly for slow terminals.
*/
void CTK_mainAppClass::CTK_emptyIPBuffer(void)
{
	int stdincopy=dup(STDIN_FILENO);
	tcdrain(stdincopy);
	tcflush(stdincopy,TCIFLUSH);
	close(stdincopy);
}

/**
* Set the gadget to immediately highlight.
*/
void CTK_mainAppClass::CTK_setDefaultGadget(int type,int num)
{
	this->hiliteBtnNum=-1;
	this->hiliteTxtBoxNum=-1;
	this->hiliteInputNum=-1;
	this->hiliteListNum=-1;
	this->hiliteCheckBoxNum=-1;
	this->hiliteEditBoxNum=-1;
	this->hiliteSourceEditBoxNum=-1;
	this->hiliting=HLNONE;

	switch(type)
		{
			case BUTTON:
				this->hiliteBtnNum=num;
				this->hiliting=HLBUTTONS;
				break;
			case TXTBOX:
				this->hiliteTxtBoxNum=num;
				this->hiliting=HLTEXT;
				break;
			case INPUT:
				this->hiliteInputNum=num;
				this->hiliting=HLINPUTS;
				this->CTK_clearScreen();
				this->CTK_updateScreen(this,NULL);
				this->pages[this->pageNumber].inputs[num]->CTK_doInput();
				break;
			case LIST:
				this->hiliteListNum=num;
				this->hiliting=HLLISTS;
				break;
			case CHKBOX:
				this->hiliteCheckBoxNum=num;
				this->hiliting=HLCHKBOXS;
				break;
			case EDBOX:
				this->hiliteEditBoxNum=num;
				this->hiliting=HLEDITBOXES;
				break;
			case SRCBOX:
				this->hiliteSourceEditBoxNum=num;
				this->hiliting=HLSRCEDITBOXES;
				break;
		}
}

/**
* Set select key, default=TERMKEY_SYM_ENTER.
*/
void CTK_mainAppClass::CTK_setSelectKey(TermKeySym key)
{
	this->selectKey=key;
}

/**
* Private.
*/
void CTK_mainAppClass::setHiliteNone(void)
{
	this->hiliteBtnNum=-1;
	this->hiliteTxtBoxNum=-1;
	this->hiliteInputNum=-1;
	this->hiliteListNum=-1;
	this->hiliteCheckBoxNum=-1;
	this->hiliteEditBoxNum=-1;
	this->hiliteSourceEditBoxNum=-1;
	this->hiliteDropBoxNum=-1;
}

struct fbData* CTK_mainAppClass::CTK_getFBData(void)
{
	return(&this->frameBufferData);
}



