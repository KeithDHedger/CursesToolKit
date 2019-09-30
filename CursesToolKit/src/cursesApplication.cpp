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

#ifdef _IMAGEMAGICK_
#include <Magick++.h>
#endif

/**
*  \brief Main app class destroy.
*/
CTK_mainAppClass::~CTK_mainAppClass()
{
	delete this->menuBar;
	for(int k=0;k<this->pages.size();k++)
		{
			for(int j=0;j<this->pages[k].gadgets.size();j++)
				delete this->pages[k].gadgets[j];
		}

	this->pages.clear();
	fflush(NULL);
	termkey_destroy(this->tk);
#ifdef _IMAGEMAGICK_
	Magick::TerminateMagick();//is this needed?
#endif
	munmap((void*)frameBufferData.frameBufferMapPtr,frameBufferData.screensize);
	delete this->utils;
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
	this->utils=new CTK_cursesUtilsClass;

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
	this->menuBar=new CTK_cursesMenuClass(this);
	this->menuBar->CTK_setUpdateCB(this->CTK_updateScreen,this);
}

/**
* Create and add new text box gadget.
* \note text only.
*/
CTK_cursesTextBoxClass* CTK_mainAppClass::CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass(this);
	txtbox->tabWidth=this->tabWidth;
	txtbox->CTK_newBox(x,y,width,hite,txt,selectable);
	this->pages[this->pageNumber].gadgets.push_back(txtbox);
	return(txtbox);
}

/**
* Create and add new text box gadget.
* \note can be a file.
*/
CTK_cursesTextBoxClass* CTK_mainAppClass::CTK_addNewTextBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass(this);
	txtbox->tabWidth=this->tabWidth;
	txtbox->CTK_newBox(x,y,width,hite,"",selectable);
	txtbox->CTK_updateText(txt,isfilename);
	txtbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].gadgets.push_back(txtbox);
	return(txtbox);
}

/**
* Create and add new button gadget. 
*/
CTK_cursesButtonClass* CTK_mainAppClass::CTK_addNewButton(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesButtonClass	*btn=new CTK_cursesButtonClass(this);
	btn->CTK_newButton(x,y,width,hite,label);
	this->pages[this->pageNumber].gadgets.push_back(btn);
	return(btn);
}

/**
* Create and add new input gadget. 
*/
CTK_cursesInputClass* CTK_mainAppClass::CTK_addNewInput(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesInputClass	*inp=new CTK_cursesInputClass(this);
	inp->CTK_newInput(x,y,width,hite,label);
	this->pages[this->pageNumber].gadgets.push_back(inp);
	return(inp);
}

CTK_cursesListBoxClass* CTK_mainAppClass::CTK_addNewListBox(int x,int y,int width,int hite)
{
	CTK_cursesListBoxClass	*lb=new CTK_cursesListBoxClass(this);
	lb->CTK_newListBox(x,y,width,hite);
	this->pages[this->pageNumber].gadgets.push_back(lb);
	return(lb);
}

/**
* Create and add new check box gadget. 
*/
CTK_cursesCheckBoxClass* CTK_mainAppClass::CTK_addNewCheckBox(int x,int y,int width,const char *label)
{
	CTK_cursesCheckBoxClass	*cb=new CTK_cursesCheckBoxClass(this);
	cb->CTK_newCheckBox(x,y,width,label);
	this->pages[this->pageNumber].gadgets.push_back(cb);
	return(cb);
}

/**
* Create and add new edit box gadget. 
*/
CTK_cursesEditBoxClass* CTK_mainAppClass::CTK_addNewEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesEditBoxClass	*edbox=new CTK_cursesEditBoxClass(this);
	edbox->tabWidth=this->tabWidth;
	edbox->CTK_newBox(x,y,width,hite,isfilename,txt,selectable);
	this->pages[this->pageNumber].gadgets.push_back(edbox);
	return(edbox);
}

/**
* Create and add new source edit box gadget. 
*/
CTK_cursesSourceEditBoxClass* CTK_mainAppClass::CTK_addNewSourceEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesSourceEditBoxClass	*srcbox=new CTK_cursesSourceEditBoxClass(this);
	srcbox->tabWidth=this->tabWidth;
	srcbox->CTK_newBox(x,y,width,hite,isfilename,txt,selectable);
	this->pages[this->pageNumber].gadgets.push_back(srcbox);
	return(srcbox);
}

/**
* Create and add new label gadget. 
*/
CTK_cursesLabelClass* CTK_mainAppClass::CTK_addNewLabel(int x,int y,int width,int hite,const char *txt)
{
	CTK_cursesLabelClass	*label=new CTK_cursesLabelClass(this);
	label->tabWidth=this->tabWidth;
	label->CTK_newLabel(x,y,width,hite,txt);
	this->pages[this->pageNumber].gadgets.push_back(label);
	return(label);
}

/**
* Create and add new drop down gadget. 
*/
CTK_cursesDropClass* CTK_mainAppClass::CTK_addNewDropDownBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,const char *label)
{
	CTK_cursesDropClass	*dropbox=new CTK_cursesDropClass(this);
	dropbox->CTK_newDropButton(x,y,width,hite,label);
	this->pages[this->pageNumber].gadgets.push_back(dropbox);
	return(dropbox);
}

/**
* Create and add new frame buffer image. 
*/
CTK_cursesFBImageClass* CTK_mainAppClass::CTK_addNewFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect)
{
	CTK_cursesFBImageClass	*fbi=new CTK_cursesFBImageClass(this);
	fbi->CTK_newFBImage(x,y,width,hite,filepath,keepaspect);
	this->pages[this->pageNumber].gadgets.push_back(fbi);
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
	this->pages[this->pageNumber].gadgets.push_back(txtbox);
}

/**
* Add button gadget. 
*/
void CTK_mainAppClass::CTK_addButton(CTK_cursesButtonClass *btn)
{
	this->pages[this->pageNumber].gadgets.push_back(btn);
}

/**
* Add input box gadget. 
*/
void CTK_mainAppClass::CTK_addInput(CTK_cursesInputClass *inp)
{
	this->pages[this->pageNumber].gadgets.push_back(inp);
}

/**
* Add new list box gadget. 
*/
void CTK_mainAppClass::CTK_addListBox(CTK_cursesListBoxClass *lb)
{
	this->pages[this->pageNumber].gadgets.push_back(lb);
}

/**
* Add check box gadget. 
*/
void CTK_mainAppClass::CTK_addCheckBox(CTK_cursesCheckBoxClass *cb)
{
	this->pages[this->pageNumber].gadgets.push_back(cb);
}

/**
* Add edit box gadget. 
*/
void CTK_mainAppClass::CTK_addEditBox(CTK_cursesEditBoxClass *edbox)
{
	this->pages[this->pageNumber].gadgets.push_back(edbox);
}

/**
* Add source edit box gadget. 
*/
void CTK_mainAppClass::CTK_addSourceEditBox(CTK_cursesSourceEditBoxClass *edbox)
{
	this->pages[this->pageNumber].gadgets.push_back(edbox);
}

/**
* Add label gadget. 
*/
void CTK_mainAppClass::CTK_addLabel(CTK_cursesLabelClass *label)
{
	this->pages[this->pageNumber].gadgets.push_back(label);
}

/**
* Add chooser box gadget. 
*/
void CTK_mainAppClass::CTK_addChooserBox(CTK_cursesChooserClass *cb)
{
	this->pages[this->pageNumber].gadgets.push_back(cb);
}

/**
* Add drop down gadget. 
*/
void CTK_mainAppClass::CTK_addDropBox(CTK_cursesDropClass *dd)
{
	this->pages[this->pageNumber].gadgets.push_back(dd);
}

/**
* Redraw the screen.
* \note object=ptr to main class object
* \note sets highlight if needed.
*/
void CTK_mainAppClass::CTK_updateScreen(void *object,void* userdata)
{
	CTK_mainAppClass		*app=static_cast<CTK_mainAppClass*>(object);
	CTK_cursesGraphicsClass	cu(app);

	setBothColours(app->colours.windowForeCol,app->colours.windowBackCol,app->colours.use256Colours);

	if((app->menuBar!=NULL) && (app->menuBar->CTK_getMenuBarVisible()==true))
		app->menuBar->CTK_drawGadget();

	if(app->pageNumber<0)
		{
			app->CTK_clearScreen();
			if((app->menuBar!=NULL) && (app->menuBar->CTK_getMenuBarVisible()==true))
				app->menuBar->CTK_drawGadget();
			fflush(NULL);
			SETNORMAL;
			return;
		}

	if((app->useAppWindow==true) || ((long)userdata==2))
		{
			int	yadj=0;
			if((app->menuBar!=NULL) && (app->menuBar->CTK_getMenuBarVisible()==true))
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

	for(int j=0;j<app->pages[app->pageNumber].gadgets.size();j++)
		{
			if(userdata!=NULL)
				app->pages[app->pageNumber].gadgets[j]->gadgetDirty=true;
			app->pages[app->pageNumber].gadgets[j]->CTK_drawGadget(app->pages[app->pageNumber].gadgets[j]->hiLited);
			app->pages[app->pageNumber].gadgets[j]->gadgetDirty=false;
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

	do
		{
			this->pages[this->pageNumber].currentGadget+=addit;
			if(this->pages[this->pageNumber].currentGadget<0)
				this->pages[this->pageNumber].currentGadget=this->pages[this->pageNumber].gadgets.size()-1;
			if(this->pages[this->pageNumber].currentGadget>this->pages[this->pageNumber].gadgets.size()-1)
				this->pages[this->pageNumber].currentGadget=0;
		}
	while(this->pages[this->pageNumber].gadgets[this->pages[this->pageNumber].currentGadget]->CTK_getSelectable()==false);
}

/**
* Switch Termkey instance on/off.
* \note Termkey should be switched off before running external programs.
* \note Switch back on after extenal app exits.
*/
void CTK_mainAppClass::CTK_setTermKeyRun(bool start)
{
	if(start==true)
		termkey_start(this->tk);
	else
		termkey_stop(this->tk);
}

/**
* Main event loop.
* \note Handles highlighting selecting etc etc.
* \note runcnt<0 Wait upto abs(runcnt) ms for a keypress, run 1 main loop if timeout.
* \note runcnt=0 Default, run main loop continously.
* \note runcnt>0 run main loop runcnt times.
*/
int CTK_mainAppClass::CTK_mainEventLoop(int runcnt,bool docls)
{
	int						selection=CONT;
	TermKeyResult			ret;
	TermKeyKey				key;
	char					tstr[3]={'_',0,0};
	int						tab=1;
	int						countdown=runcnt;
	struct pollfd			fd;
	int						thisgadget;
	struct pageStruct		*thispage;
	int						thisgadgettype;
	CTK_cursesGadgetClass	*thisgadgetinst;
	bool					hilite;

	if(docls==true)
		{
			this->CTK_clearScreen();
			this->CTK_updateScreen(this,NULL);
		}

	this->useAppWindow=false;
	SETHIDECURS;
	fflush(NULL);
	this->runEventLoop=true;

	key.type=TERMKEY_TYPE_UNKNOWN_CSI;
	key.code.sym=TERMKEY_SYM_NONE;
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
//										switch(this->pages[this->pageNumber].gadgets[this->pages[this->pageNumber].currentGadget]->CTK_getGadgetType())
			thispage=&this->pages[this->pageNumber];
			thisgadget=thispage->currentGadget;
			if(thisgadget!=-1)
				{
					thisgadgetinst=thispage->gadgets[thisgadget];
					thisgadgettype=thispage->gadgets[thisgadget]->CTK_getGadgetType();
					if((key.code.sym!=TERMKEY_SYM_NONE) || (key.type!=TERMKEY_TYPE_UNKNOWN_CSI))
						thisgadgetinst->gadgetDirty=true;
				}
			else
				{
					thisgadgettype=BASEGADGET;
					thisgadgetinst=NULL;
				}
			hilite=true;

//			if((key.code.sym!=TERMKEY_SYM_NONE) || (key.type!=TERMKEY_TYPE_UNKNOWN_CSI))
//				thisgadgetinst->gadgetDirty=true;

			switch(key.type)
				{
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_ESCAPE:
										if(this->pages[this->pageNumber].menusActive==false)
											break;
										this->CTK_updateScreen(this,NULL);
										if((this->menuBar!=NULL) && (this->menuBar->CTK_getMenuBarEnable()==true) && (this->menuBar->CTK_getMenuBarVisible()==true))
											{
												selection=this->menuBar->CTK_doMenuEvent(0,1,true);
												this->menuBar->CTK_drawDefaultMenuBar();
												this->CTK_updateScreen(this,(void*)1);
											}
										break;
//tab select
									case TERMKEY_SYM_TAB://TODO//no gadgets
									case TERMKEY_SYM_RIGHT:
									case TERMKEY_SYM_LEFT:
									//fprintf(stderr,"000000000000000000000%i\n",this->pages[0].currentGadget);
										switch(key.code.sym)
											{
												case TERMKEY_SYM_TAB:
													if((key.modifiers==1) || (key.modifiers==2))
														hilite=false;
													break;
												case TERMKEY_SYM_LEFT:													
													hilite=false;
													break;
											}
										if(thisgadget!=-1)
											thisgadgetinst->hiLited=false;
										if(this->noHiliteChange==false)
											setHilite(hilite);

										this->pages[this->pageNumber].gadgets[this->pages[this->pageNumber].currentGadget]->gadgetDirty=true;
										this->pages[this->pageNumber].gadgets[this->pages[this->pageNumber].currentGadget]->hiLited=true;
										break;

//scroll txt boxes and lists
									case TERMKEY_SYM_UP:
										if(thisgadget<0)
											break;
										if(thisgadgetinst->hiLited==false)
												break;
										thisgadgetinst->gadgetDirty=true;
										switch(thisgadgettype)
											{
												case TEXTGADGET:
													static_cast<CTK_cursesTextBoxClass*>(thisgadgetinst)->CTK_scrollLine(true);
													break;
												case LISTGADGET:
													static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->CTK_keyUpDown(true);
													break;
												case EDITGADGET:
												case SRCGADGET:
													static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getCursLine()-1);
													break;
												default:
													break;
											}
										break;

									case TERMKEY_SYM_DOWN:
										if(thisgadget<0)
											break;
										if(thisgadgetinst->hiLited==false)
											break;
										thisgadgetinst->gadgetDirty=true;
										switch(thisgadgettype)
											{
												case TEXTGADGET:
													static_cast<CTK_cursesTextBoxClass*>(thisgadgetinst)->CTK_scrollLine(false);
													break;
												case LISTGADGET:
													static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->CTK_keyUpDown(false);
													break;
												case EDITGADGET:
												case SRCGADGET:
													static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getCursLine()+1);
													break;
												case DROPGADGET:
													static_cast<CTK_cursesDropClass*>(thisgadgetinst)->CTK_doDropDownEvent();
													if(thisgadgetinst->selectCB!=NULL)
														thisgadgetinst->selectCB((void*)thisgadgetinst,(void*)thisgadgetinst->CTK_getCBUserData());
													break;
												default:
													break;
											}
										break;

									case TERMKEY_SYM_PAGEUP:
										if(thisgadget<0)
											break;
										thisgadgetinst->gadgetDirty=true;
										switch(thisgadgettype)
											{
												case LISTGADGET:
													static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->CTK_keyUpDown(true,true);
													break;
												case TEXTGADGET:
													static_cast<CTK_cursesTextBoxClass*>(thisgadgetinst)->CTK_scrollPage(true);
													break;
												default:
													break;
												case EDITGADGET:
												case SRCGADGET:
													static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getCursLine()-static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getHeight());
													break;
											}
										break;

									case TERMKEY_SYM_PAGEDOWN:
										if(thisgadget<0)
											break;
										thisgadgetinst->gadgetDirty=true;
										switch(thisgadgettype)
											{
												case LISTGADGET:
													static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->CTK_keyUpDown(false,true);
													break;
												case TEXTGADGET:
													static_cast<CTK_cursesTextBoxClass*>(thisgadgetinst)->CTK_scrollPage(false);
													break;
												default:
													break;
												case EDITGADGET:
												case SRCGADGET:
													static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getCursLine()+static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getHeight());
													break;
											}
										break;

//select key
									default:
										if(thisgadget<0)
											break;
										if((key.code.sym!=this->selectKey) && (thisgadgetinst->CTK_getSelectKey()==TERMKEY_SYM_NONE))
											continue;
										if(thisgadgetinst->CTK_getSelectKey()!=TERMKEY_SYM_NONE)
											{
												if(key.code.sym!=thisgadgetinst->CTK_getSelectKey())
													continue;
											}

										thisgadgetinst->gadgetDirty=true;
										if(thisgadgetinst->hiLited==true)
											{
												switch(thisgadgettype)
													{
														case LISTGADGET:
															if(static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->CTK_getMultipleSelect()==true)
																static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->CTK_toggleItem(static_cast<CTK_cursesListBoxClass*>(thisgadgetinst)->listItemNumber);
															if(thisgadgetinst->selectCB!=NULL)
																thisgadgetinst->selectCB((void*)thisgadgetinst,(void*)thisgadgetinst->CTK_getCBUserData());
															break;
														case DROPGADGET:
															static_cast<CTK_cursesDropClass*>(thisgadgetinst)->CTK_doDropDownEvent();
															if(thisgadgetinst->selectCB!=NULL)
																thisgadgetinst->selectCB((void*)thisgadgetinst,(void*)thisgadgetinst->CTK_getCBUserData());
															break;
														case BUTTONGADGET://TODO//
														case CHECKGADGET:
														case IMAGEGADGET:
															if(thisgadgetinst->selectCB!=NULL)
																thisgadgetinst->selectCB((void*)thisgadgetinst,(void*)thisgadgetinst->CTK_getCBUserData());
															break;
														case INPUTGADGET:
															static_cast<CTK_cursesInputClass*>(thisgadgetinst)->CTK_doInput();
															break;

														case EDITGADGET:
														case SRCGADGET:
															if(thisgadgetinst->CTK_getGadgetType()==EDITGADGET)
																static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_doEvent(false,static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getStrings(),static_cast<CTK_cursesEditBoxClass*>(thisgadgetinst)->CTK_getStrings());
															else
																static_cast<CTK_cursesSourceEditBoxClass*>(thisgadgetinst)->CTK_doEvent(true,static_cast<CTK_cursesSourceEditBoxClass*>(thisgadgetinst)->CTK_getStrings(),static_cast<CTK_cursesSourceEditBoxClass*>(thisgadgetinst)->CTK_getSrcStrings());
													
															this->CTK_emptyIPBuffer();
															this->CTK_updateScreen(this,NULL);
															if(this->eventLoopCBOut!=NULL)
																this->eventLoopCBOut(this,this->userData);
															continue;
															break;

														default:
															break;
													}

												if(this->pages[this->pageNumber].currentGadget!=-1)
													{
														if(thisgadgetinst->CTK_getSelectDeselects()==true)
															{
																thisgadgetinst->hiLited=!thisgadgetinst->CTK_getSelectDeselects();
																thisgadgetinst->CTK_drawGadget(thisgadgetinst->hiLited);
															}
													}
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
												this->menuBar->selectCB(this->menuBar,NULL);//TODO// add menu number as user data?
												thisgadgetinst->gadgetDirty=true;
												break;
											}
									}
							}
						break;
				}

			//if(key.code.sym!=TERMKEY_SYM_NONE)
			//	{
					this->CTK_emptyIPBuffer();
					this->CTK_updateScreen(this,NULL);
					this->showHilighting=true;
					if(this->eventLoopCBOut!=NULL)
						this->eventLoopCBOut(this,this->userData);
					if(runcnt>0)
						{
							countdown--;
							if(countdown==0)
								this->runEventLoop=false;
						}
			//	}
		}
//thispage->currentGadget=-1;;
	return(key.code.codepoint);
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
	ps.currentGadget=-1;
	ps.userData=NULL;
	ps.menusActive=true;
	ps.gadgets.clear();

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
int CTK_mainAppClass::CTK_removePage(int pagenum)//TODO//
{
	if((pagenum>=0) && (pagenum<this->pages.size()))
		{
			for(int j=0;j<this->pages[pagenum].gadgets.size();j++)
			{
				//fprintf(stderr,"j=%i size=%i\n",j,this->pages[pagenum].gadgets.size());
				//fprintf(stderr,"gadget =%p\n",this->pages[pagenum].gadgets[j]);
				//fprintf(stderr,"gadget type=%i\n",this->pages[pagenum].gadgets[j]->CTK_getGadgetType());
				//if(this->pages[pagenum].gadgets[j]->CTK_getGadgetType()==8)
				//	fprintf(stderr,"label=>>>%s<<\n",static_cast<CTK_cursesLabelClass*>(this->pages[pagenum].gadgets[j])->text.c_str());
				delete this->pages[pagenum].gadgets[j];
			}

			this->pages.erase(this->pages.begin()+pagenum);
			this->pageNumber=pagenum;
			if(this->pageNumber>=this->pages.size())
				this->pageNumber=this->pages.size()-1;
			this->pages[this->pageNumber].currentGadget=-1;
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
\note unlite default=true, set to false to NOT unhilite existant gadgets.
*/
void CTK_mainAppClass::CTK_setDefaultGadget(CTK_cursesGadgetClass *gadget,bool unhilite)
{
//un hilite all gadgets
	CTK_cursesGadgetClass *unhigadg;

	if(unhilite==true)
	{
		for(int j=0;j<this->pages[this->pageNumber].gadgets.size();j++)
			{
				unhigadg=this->pages[this->pageNumber].gadgets[j];
				unhigadg->gadgetDirty=true;
				unhigadg->hiLited=false;
				unhigadg->CTK_drawGadget(false);
			}
	}

//hilight default
	for(int j=0;j<this->pages[this->pageNumber].gadgets.size();j++)
		{
			if(this->pages[this->pageNumber].gadgets[j]==gadget)
				{
					this->pages[this->pageNumber].currentGadget=j;
					gadget->gadgetDirty=true;
					gadget->hiLited=true;
					return;
				}
		}
}

/**
* Set select key, default=TERMKEY_SYM_ENTER.
*/
void CTK_mainAppClass::CTK_setSelectKey(TermKeySym key)
{
	this->selectKey=key;
}

struct fbData* CTK_mainAppClass::CTK_getFBData(void)//TODO// put in lib, globals ?
{
	return(&this->frameBufferData);
}

/**
* Get gadget.
* \param int page number.
* \param gadgetType type.
* \param int nth gadget of type.
*/
CTK_cursesGadgetClass* CTK_mainAppClass::CTK_getGadgetNum(int page,gadgetType type,int nth)
{
	int	cntdown=nth;

	for(int j=0;j<this->pages[page].gadgets.size();j++)
		{
			if(this->pages[page].gadgets[j]->CTK_getGadgetType()==type)
				{
					cntdown--;
					if(cntdown==0)
						return(this->pages[page].gadgets[j]);
				}
		}
	return(NULL);
}

