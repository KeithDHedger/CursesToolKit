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
#ifdef _IMAGEMAGICK_
	Magick::TerminateMagick();//is this needed?
#endif
	munmap((void*)frameBufferData.frameBufferMapPtr,frameBufferData.screensize);
	delete this->utils;
	delete this->gc;
	delete this->readKey;
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
	this->utils=new CTK_cursesUtilsClass(this);
	this->gc=new CTK_cursesGraphicsClass(this);

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

	this->readKey=new CTK_cursesReadKeyClass(this);
}

/**
* Clear screen. 
*/
void CTK_mainAppClass::CTK_clearScreen(void)
{
	setBothColours(this->colours.windowForeCol,this->colours.windowBackCol,this->colours.use256Colours);
	MOVETO(1,1)
	printf("%s",this->clearScreenCode.c_str());
	fflush(NULL);

	if(THISPAGE.fancyWindow==true)
		this->gc->CTK_drawDialogWindow();

	if(this->menuBar!=NULL)
		{
			this->menuBar->CTK_setMenuBarVisible(THISPAGE.menuBarVisible);
			this->menuBar->CTK_drawDefaultMenuBar();
		}
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
void CTK_mainAppClass::CTK_updateScreen(void *object,void* userdata)//TODO//
{
	CTK_mainAppClass		*app=static_cast<CTK_mainAppClass*>(object);
	CTK_cursesGraphicsClass	cu(app);
//TODO//
	MOVETO(1,1)
//app->CTK_clearScreen();
	app->drawAllGadgets();
return;
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

	if((app->useAppWindow==true) || (userdata==SCREENUPDATEWINDOW))
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
			if(userdata!=SCREENUPDATEBASIC)
				app->pages[app->pageNumber].gadgets[j]->gadgetDirty=true;
			if(userdata!=SCREENUPDATEUNHILITE)
				app->pages[app->pageNumber].gadgets[j]->CTK_drawGadget(app->pages[app->pageNumber].gadgets[j]->hiLited);
			else
				app->pages[app->pageNumber].gadgets[j]->CTK_drawGadget(false);
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
* Draw all gdagets unhilted.
*/
void CTK_mainAppClass::resetAllGadgets(void)
{

	if((this->menuBar!=NULL) && (this->menuBar->CTK_getMenuBarVisible()==true))
		this->menuBar->CTK_drawGadget();

	for(int j=0;j<this->pages[this->pageNumber].gadgets.size();j++)
		{
			this->pages[this->pageNumber].gadgets[j]->gadgetDirty=true;
			this->pages[this->pageNumber].gadgets[j]->CTK_drawGadget(false);
		}
	fflush(NULL);
}

/**
* Draw all gdagets keeping highlight.
*/
void CTK_mainAppClass::drawAllGadgets(void)
{

	if((this->menuBar!=NULL) && (this->menuBar->CTK_getMenuBarVisible()==true))
		this->menuBar->CTK_drawGadget();

	for(int j=0;j<this->pages[this->pageNumber].gadgets.size();j++)
		{
			this->pages[this->pageNumber].gadgets[j]->gadgetDirty=true;
			this->pages[this->pageNumber].gadgets[j]->CTK_drawGadget(this->pages[this->pageNumber].gadgets[j]->hiLited);
		}
	fflush(NULL);
}

/**
* Scroll gadget
* \param bool pageup
* \param bool line up
* \note private
*/
void CTK_mainAppClass::scrollGadget(bool pagescroll,bool lineup)
{
	gadgetType	gt;

	if(THISPAGE.currentGadget==-1)
		return;

	gt=CURRENTGADGET->CTK_getGadgetType();
	if(CURRENTGADGET->hiLited==false)
		return;

	switch(gt)
		{
			case LISTGADGET:
				static_cast<CTK_cursesListBoxClass*>(CURRENTGADGET)->CTK_keyUpDown(lineup,pagescroll);
				break;
			case TEXTGADGET:
				if(pagescroll==true)
					static_cast<CTK_cursesTextBoxClass*>(CURRENTGADGET)->CTK_scrollPage(lineup);
				else
					static_cast<CTK_cursesTextBoxClass*>(CURRENTGADGET)->CTK_scrollLine(lineup);
				break;
			case DROPGADGET:
				if(lineup==true)
					return;
				static_cast<CTK_cursesDropClass*>(CURRENTGADGET)->CTK_doDropDownEvent();
				if(CURRENTGADGET->selectCB!=NULL)
					CURRENTGADGET->selectCB((void*)CURRENTGADGET,(void*)CURRENTGADGET->CTK_getCBUserData());
				this->resetAllGadgets();
				break;
			case EDITGADGET:
			case SRCGADGET:
				if(pagescroll==true)
					{
						if(lineup==false)
							static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getCursLine()+static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getHeight());
						else
							static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getCursLine()-static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getHeight());
					}
				else
					if(lineup==false)
						static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getCursLine()+1);
					else
						static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_gotoLine(static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getCursLine()-1);
				break;
		}
	CURRENTGADGET->CTK_drawGadget(true);
}

/**
* Run menu events
* \note Private.
*/
void CTK_mainAppClass::runMenus(void)
{
	if(THISPAGE.menusActive==false)
		return;
	this->resetAllGadgets();
	
	int	hg=THISPAGE.currentGadget;
	int hp=this->pageNumber;
	if(hg!=-1)//TODO//
		CURRENTGADGET->hiLited=false;
	THISPAGE.currentGadget=-1;
	if((this->menuBar!=NULL) && (this->menuBar->CTK_getMenuBarEnable()==true) && (this->menuBar->CTK_getMenuBarVisible()==true))
		{
			this->menuBar->CTK_doMenuEvent(0,1,true);
			this->menuBar->CTK_drawDefaultMenuBar();
			this->CTK_clearScreen();
			this->resetAllGadgets();
		}

	if((hg!=-1) && (this->pageNumber==hp))
		{
			this->CTK_setDefaultGadget(THISPAGE.gadgets[hg]);
			CURRENTGADGET->gadgetDirty=true;
			CURRENTGADGET->hiLited=true;
			CURRENTGADGET->CTK_drawGadget(true);
		}
}

/**
* Activate gadget.
* \note Private.
*/
void CTK_mainAppClass::activateGadget(void)
{
//	fprintf(stderr,"gadget num=%i\n",THISPAGE.currentGadget);
	int	thispnum=-1;
	int	newpn=-1;
	
	if(THISPAGE.currentGadget==-1)
		return;

	gadgetType	thisgadgettype=CURRENTGADGET->CTK_getGadgetType();

	THISPAGE.retainHighliting=false;
	THISPAGE.ignoreFirstTab=true;

	switch(thisgadgettype)
		{
//buttons
			case CHECKGADGET:
				THISPAGE.retainHighliting=true;
				THISPAGE.ignoreFirstTab=false;
			case IMAGEGADGET:
			case BUTTONGADGET:
				if((CURRENTGADGET->selectCB!=NULL) && (CURRENTGADGET->hiLited==true))
					CURRENTGADGET->selectCB(CURRENTGADGET,(void*)CURRENTGADGET->CTK_getCBUserData());
				break;
//lists
			case LISTGADGET:
				THISPAGE.ignoreFirstTab=false;
				THISPAGE.retainHighliting=true;
				if(static_cast<CTK_cursesListBoxClass*>(CURRENTGADGET)->CTK_getMultipleSelect()==true)
					{
						static_cast<CTK_cursesListBoxClass*>(CURRENTGADGET)->CTK_toggleItem(static_cast<CTK_cursesListBoxClass*>(CURRENTGADGET)->listItemNumber);
					}
				if(CURRENTGADGET->selectCB!=NULL)
					CURRENTGADGET->selectCB((void*)CURRENTGADGET,(void*)CURRENTGADGET->CTK_getCBUserData());
				CURRENTGADGET->CTK_drawGadget(true);
				break;
//drop gadget
			case DROPGADGET:
				THISPAGE.retainHighliting=true;
				THISPAGE.ignoreFirstTab=false;
				static_cast<CTK_cursesDropClass*>(CURRENTGADGET)->CTK_doDropDownEvent();
				if(CURRENTGADGET->selectCB!=NULL)
					CURRENTGADGET->selectCB((void*)CURRENTGADGET,(void*)CURRENTGADGET->CTK_getCBUserData());
				this->resetAllGadgets();
				CURRENTGADGET->gadgetDirty=true;
				CURRENTGADGET->CTK_drawGadget(true);
				break;
//input
			case INPUTGADGET:
				static_cast<CTK_cursesInputClass*>(CURRENTGADGET)->CTK_doInput();
				break;
//edit boxes
			case EDITGADGET:
			case SRCGADGET:
				thispnum=this->pageNumber;
				newpn=-1;
				THISPAGE.ignoreFirstTab=false;
				THISPAGE.retainHighliting=true;
				if(CURRENTGADGET->CTK_getGadgetType()==EDITGADGET)
					static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_doEvent(false,static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getStrings(),static_cast<CTK_cursesEditBoxClass*>(CURRENTGADGET)->CTK_getStrings());
				else
					static_cast<CTK_cursesSourceEditBoxClass*>(CURRENTGADGET)->CTK_doEvent(true,static_cast<CTK_cursesSourceEditBoxClass*>(CURRENTGADGET)->CTK_getStrings(),static_cast<CTK_cursesSourceEditBoxClass*>(CURRENTGADGET)->CTK_getSrcStrings());

				newpn=this->pageNumber;
				if(newpn==thispnum)
					{
						this->CTK_emptyIPBuffer();
						CURRENTGADGET->gadgetDirty=true;
						CURRENTGADGET->CTK_drawGadget(true);
					}
				if(this->eventLoopCBOut!=NULL)
					this->eventLoopCBOut(this,this->userData);
				break;
		}

	if((THISPAGE.retainHighliting==false) && (THISPAGE.currentGadget!=-1))
		{
			CURRENTGADGET->CTK_drawGadget(false);
			CURRENTGADGET->hiLited=false;
		}
}

/**
* Hilite current gadget.
*/
void CTK_mainAppClass::highLiteGadget(bool forward)
{
	int		thisgadget=this->pages[this->pageNumber].currentGadget;
	int		maxgadgets=this->pages[this->pageNumber].gadgets.size();
	int		newgadget=thisgadget;
	int		holdgadget=thisgadget;
	int		numselectables=0;
	int		selctable=0;

//special cases
	if(maxgadgets>0)
		{
			for(int j=0;j<maxgadgets;j++)
				{
					if(this->pages[this->pageNumber].gadgets[j]->CTK_getSelectable()==true)
						{
							numselectables++;
							selctable=j;
						}
				}
		}

	if(numselectables==0)
		return;
	if(numselectables==1)
		{
			newgadget=selctable;
			goto DRAWGADGET;
		}
//special cases end

//fprintf(stderr,">>>>current gadget=%i current page=%i\n",THISPAGE.currentGadget,this->pageNumber);
	if(THISPAGE.ignoreFirstTab==true)
		{
			if((THISPAGE.currentGadget!=-1))
				{
					THISPAGE.ignoreFirstTab=false;
					CURRENTGADGET->CTK_drawGadget(true);
					CURRENTGADGET->hiLited=true;
					return;
				}
		}

	if((THISPAGE.currentGadget!=-1))
		{
			CURRENTGADGET->gadgetDirty=true;
			CURRENTGADGET->CTK_drawGadget(false);
			CURRENTGADGET->hiLited=false;
		}

	if(forward==true)
		{
			if(thisgadget==-1)
				{
					while(thisgadget<maxgadgets)
						{
							thisgadget++;
							if(THISPAGE.gadgets[thisgadget]->CTK_getSelectable()==true)
								{
									newgadget=thisgadget;
									goto DRAWGADGET;
								}
						}
				}

			if((thisgadget>-1) && (thisgadget<maxgadgets))
				{
					holdgadget=thisgadget;
					while(++thisgadget<maxgadgets)
						{
							if(THISPAGE.gadgets[thisgadget]->CTK_getSelectable()==true)
								{
									newgadget=thisgadget;
									goto DRAWGADGET;
								}
						}
					thisgadget=-1;
					while(++thisgadget<holdgadget)
						{
							if(THISPAGE.gadgets[thisgadget]->CTK_getSelectable()==true)
								{
									newgadget=thisgadget;
									goto DRAWGADGET;
								}
						}
					return;
				}
		}
	else
		{
			if(thisgadget==-1)
				{
					thisgadget=maxgadgets;
					while(thisgadget>0)
						{
							thisgadget--;
							if(THISPAGE.gadgets[thisgadget]->CTK_getSelectable()==true)
								{
									newgadget=thisgadget;
									goto DRAWGADGET;
								}
						}
				}

			if((thisgadget>-1) && (thisgadget<maxgadgets))
				{
					holdgadget=thisgadget;
					while(--thisgadget>-1)
						{
							if(THISPAGE.gadgets[thisgadget]->CTK_getSelectable()==true)
								{
									newgadget=thisgadget;
									goto DRAWGADGET;
								}
						}
					thisgadget=maxgadgets;
					while(--thisgadget>holdgadget)
						{
							if(THISPAGE.gadgets[thisgadget]->CTK_getSelectable()==true)
								{
									newgadget=thisgadget;
									goto DRAWGADGET;
								}
						}
					return;
				}
		}

	DRAWGADGET:
		THISPAGE.currentGadget=newgadget;
		CURRENTGADGET->gadgetDirty=true;
		CURRENTGADGET->CTK_drawGadget(true);
		CURRENTGADGET->hiLited=true;
}

/**
* Main event loop.
* \note Handles highlighting selecting etc etc.
* \note runcnt<0 Wait upto abs(runcnt) ms for a keypress, run 1 main loop if timeout.
* \note runcnt=0 Default, run main loop continously.
* \note runcnt>0 run main loop runcnt times.
*/
int CTK_mainAppClass::CTK_mainEventLoop(int runcnt,bool docls,bool leavehilited)
{
	SETHIDECURS;

	if(docls==true)
		{
			this->CTK_clearScreen();
			if(leavehilited==false)
				{
					this->resetAllGadgets();
					this->pages[this->pageNumber].currentGadget=-1;
				}
			else
				this->drawAllGadgets();
		}

	this->runEventLoop=true;
	while(this->runEventLoop==true)
		{
			this->readKey->tabIsSpecial=true;
			this->readKey->CTK_getInput();
			//fprintf(stderr,"Key scancode %s\n",this->readKey->inputBuffer.c_str());

			if(this->readKey->isHexString==true)
				{
					switch(this->readKey->specialKeyName)
						{
						case CTK_KEY_UP:
							this->scrollGadget(false,true);
							//fprintf(stderr,"CTK_KEY_UP\n");
							break;
						case CTK_KEY_DOWN:
							this->scrollGadget(false,false);
							//fprintf(stderr,"CTK_KEY_DOWN\n");
							break;
						case CTK_KEY_LEFT:
							this->highLiteGadget(false);
							//fprintf(stderr,"CTK_KEY_LEFT\n");
							break;
						case CTK_KEY_RIGHT:
							this->highLiteGadget(true);
							//fprintf(stderr,"CTK_KEY_RIGHT\n");
							break;
						case CTK_KEY_HOME:
							//fprintf(stderr,"CTK_KEY_HOME\n");
							break;
						case CTK_KEY_END:
							//fprintf(stderr,"CTK_KEY_END\n");
							break;
						case CTK_KEY_PAGEUP://TODO//lists
							this->scrollGadget(true,true);
							//fprintf(stderr,"CTK_KEY_PAGEUP\n");
							break;
						case CTK_KEY_PAGEDOWN:
							this->scrollGadget(true,false);
							//fprintf(stderr,"CTK_KEY_PAGEDOWN\n");
							break;
						case CTK_KEY_INSERT:
							//fprintf(stderr,"CTK_KEY_INSERT\n");
							break;
						case CTK_KEY_DELETE:
							//fprintf(stderr,"CTK_KEY_DELETE\n");
							break;
						case CTK_KEY_ENTER:
						case CTK_KEY_RETURN:
							if(THISPAGE.currentGadget!=-1)
								if(CURRENTGADGET->CTK_getSelectKey()==CTK_KEY_NONE)
									{
										//fprintf(stderr,"CTK_KEY_ENTER/RETURN\n");
										this->activateGadget();
									}
							break;
						case CTK_KEY_BACKSPACE:
							//fprintf(stderr,"CTK_KEY_BACKSPACE\n");
							break;
						case CTK_KEY_TAB:
							this->highLiteGadget(true);
							//fprintf(stderr,"CTK_KEY_TAB\n");
							break;
						case CTK_KEY_BACKTAB:
							this->highLiteGadget(false);
							//fprintf(stderr,"CTK_KEY_BACKTAB\n");
							break;
						case CTK_KEY_ESC:
							this->runMenus();
							//fprintf(stderr,"CTK_KEY_ESC\n");
							break;
						case CTK_KEY_F1:
							//fprintf(stderr,"CTK_KEY_F1\n");
							break;
						case CTK_KEY_F2:
							//fprintf(stderr,"CTK_KEY_F2\n");
							break;
						case CTK_KEY_F3:
							//fprintf(stderr,"CTK_KEY_F3\n");
							break;
						case CTK_KEY_F4:
							//fprintf(stderr,"CTK_KEY_F4\n");
							break;
						case CTK_KEY_F5:
							//fprintf(stderr,"CTK_KEY_F5\n");
							break;
						case CTK_KEY_F6:
							//fprintf(stderr,"CTK_KEY_F6\n");
							break;
						case CTK_KEY_F7:
							//fprintf(stderr,"CTK_KEY_F7\n");
							break;
						case CTK_KEY_F8:
							//fprintf(stderr,"CTK_KEY_F8\n");
							break;
						case CTK_KEY_F9:
							//fprintf(stderr,"CTK_KEY_F9\n");
							break;
						case CTK_KEY_F10:
							//fprintf(stderr,"CTK_KEY_F10\n");
							break;
						case CTK_KEY_F11:
							//fprintf(stderr,"CTK_KEY_F11\n");
							break;
						case CTK_KEY_F12:
							//fprintf(stderr,"CTK_KEY_F12\n");
							break;
						default:
							//fprintf(stderr,"Unknown Key ... ");
							//fprintf(stderr,"%s\n",this->readKey->inputBuffer.c_str());
							break;
						}
				}
			else
				{
//control keys
					if(this->readKey->isControlKey==true)
						switch(this->readKey->controlKeyNumber)
							{
//^M
								case CTK_KEY_RETURN:
									this->activateGadget();
									break;
								default:
//check menu ctrl keys
									//fprintf(stderr,"Control %s Key number=%i\n",this->readKey->inputBuffer.c_str(),this->readKey->controlKeyNumber);
									if((this->menuBar!=NULL) && (this->menuBar->enableShortcuts==true) && (this->menuBar->CTK_getMenuBarEnable()==true))
										{
											for(int j=0;j<this->menuBar->menuNames.size();j++)
												{
													if(this->menuBar->CTK_doShortCutKey(this->readKey->inputBuffer.c_str()[0],j)==true)
														{
															this->menuBar->menuNumber=j;
															this->menuBar->selectCB(this->menuBar,NULL);
															//fprintf(stderr,"Got Menu Control %s Key number=%i\n",this->readKey->inputBuffer.c_str(),this->readKey->controlKeyNumber);
															break;
														}
												}
										}
							}
					else
						{
							if((CURRENTGADGET->CTK_getSelectKey()==this->readKey->inputBuffer.c_str()[0]) && (this->readKey->inputBuffer.length()==1))
								{
									this->activateGadget();
								}
							//fprintf(stderr,"%s\n",this->readKey->inputBuffer.c_str());
						}
				}
		}
	return 0;
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

//	THISPAGE.currentGadget=-1;
	if(this->menuBar!=NULL)
		{
			this->menuBar->CTK_setMenuBarVisible(THISPAGE.menuBarVisible);
			this->menuBar->CTK_drawDefaultMenuBar();
		}
	if((pagenum>=0) && (pagenum<this->pages.size()))
		this->pageNumber=pagenum;
	this->CTK_clearScreen();
//	THISPAGE.currentGadget=-1;
//	THISPAGE.ignoreFirstTab=false;
//	THISPAGE.retainHighliting=false;
//	this->resetAllGadgets();
	this->drawAllGadgets();
	return;



	printf("%s",this->clearScreenCode.c_str());

	if(this->menuBar!=NULL)
		{
			this->menuBar->CTK_setMenuBarVisible(THISPAGE.menuBarVisible);
			this->menuBar->CTK_drawDefaultMenuBar();
		}
	if((pagenum>=0) && (pagenum<this->pages.size()))
		this->pageNumber=pagenum;
}

/**
* Goto prev page and update screen.
*/
int CTK_mainAppClass::CTK_previousPage(void)
{
	THISPAGE.currentGadget=-1;
	if(this->menuBar!=NULL)
		{
			this->menuBar->CTK_setMenuBarVisible(THISPAGE.menuBarVisible);
			this->menuBar->CTK_drawDefaultMenuBar();
		}
	if(this->pageNumber>0)
		this->pageNumber--;
	this->CTK_clearScreen();
	THISPAGE.currentGadget=-1;
	THISPAGE.ignoreFirstTab=false;
	THISPAGE.retainHighliting=false;
	this->resetAllGadgets();
	return(this->pageNumber);
}

/**
* Goto next page and update screen.
*/
int CTK_mainAppClass::CTK_nextPage(void)
{
	THISPAGE.currentGadget=-1;
	if(this->menuBar!=NULL)
		{
			this->menuBar->CTK_setMenuBarVisible(THISPAGE.menuBarVisible);
			this->menuBar->CTK_drawDefaultMenuBar();
		}

	if(this->pageNumber<this->pages.size()-1)
		this->pageNumber++;

	this->CTK_clearScreen();
	THISPAGE.currentGadget=-1;
	THISPAGE.ignoreFirstTab=false;
	THISPAGE.retainHighliting=false;
	this->resetAllGadgets();
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
				delete this->pages[pagenum].gadgets[j];

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
* Set page to be a dialog window.
* \note see the utility dialogs.
*/
void CTK_mainAppClass::CTK_setDialogWindow(const char *windowname,const char *dialogname,int dialogwidth,int dialoghite)
{
	THISPAGE.windowName=windowname;
	THISPAGE.dialogName=dialogname;
	THISPAGE.fancyWindow=true;

	if((dialogwidth==-1) || (dialogwidth>this->maxCols-7))
		{
			THISPAGE.boxW=this->maxCols-7;
			THISPAGE.boxX=4;
			THISPAGE.boxWM=this->maxCols/2;
		}
	else
		{
			THISPAGE.boxW=dialogwidth;
			THISPAGE.boxX=(this->maxCols/2)-(dialogwidth/2);
			THISPAGE.boxWM=(this->maxCols/2)-1;
			//fprintf(stderr,"x=%i y=%i w=%i h=%i wm=%i hm=%i\n",THISPAGE.boxX,THISPAGE.boxY,THISPAGE.boxW,THISPAGE.boxW,THISPAGE.boxWM,THISPAGE.boxHM);
		}

	if((dialoghite==-1) || (dialoghite>this->maxRows-6))
		{
			THISPAGE.boxH=this->maxRows-6;
			THISPAGE.boxY=4;
			THISPAGE.boxHM=((this->maxRows)/2);
		//	fprintf(stderr,"x=%i y=%i w=%i h=%i wm=%i hm=%i\n",THISPAGE.boxX,THISPAGE.boxY,THISPAGE.boxW,THISPAGE.boxW,THISPAGE.boxWM,THISPAGE.boxHM);
		}
	else
		{
			THISPAGE.boxH=dialoghite;
			THISPAGE.boxY=(this->maxRows/2)-(dialoghite/2)+1;
			THISPAGE.boxHM=this->maxRows/2;
			//fprintf(stderr,"x=%i y=%i w=%i h=%i wm=%i hm=%i\n",THISPAGE.boxX,THISPAGE.boxY,THISPAGE.boxW,THISPAGE.boxW,THISPAGE.boxWM,THISPAGE.boxHM);
		}
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
					THISPAGE.retainHighliting=true;
					THISPAGE.ignoreFirstTab=false;
					this->pages[this->pageNumber].currentGadget=j;
					gadget->gadgetDirty=true;
					gadget->hiLited=true;
					gadget->CTK_drawGadget(true);
					return;
				}
		}
}

/**
* Set select key, default=CTK_KEY_RETURN.
*/
void CTK_mainAppClass::CTK_setSelectKey(int key)
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

