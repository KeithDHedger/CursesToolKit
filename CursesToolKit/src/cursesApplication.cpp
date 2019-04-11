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

#include "cursesApplication.h"

CTK_mainAppClass::~CTK_mainAppClass()
{
	delete this->menuBar;
	for(int k=0;k<this->pages.size();k++)
		{
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
		}
}

CTK_mainAppClass::CTK_mainAppClass()
{
	winsize w;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
	this->maxRows=w.ws_row;
	this->maxCols=w.ws_col;
	this->pages.clear();
	this->CTK_addPage();
	this->pageNumber=0;
}

void CTK_mainAppClass::CTK_clearScreen(void)
{
	setBackColour(this->colours.windowBackCol,this->colours.use256Colours);
	setForeColour(this->colours.windowForeCol,this->colours.use256Colours);
	printf("\e[2J\e[H");
}

void CTK_mainAppClass::CTK_addNewMenuBar(void)
{
	this->menuBar=new CTK_cursesMenuClass();
	this->menuBar->CTK_setUpdateCB(this->CTK_updateScreen,this);
	this->menuBar->CTK_setColours(this->colours);
}

void CTK_mainAppClass::CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass();
	txtbox->CTK_newBox(x,y,width,hite,txt,selectable);
	txtbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].textBoxes.push_back(txtbox);
}

void CTK_mainAppClass::CTK_addNewButton(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesButtonClass	*btn=new CTK_cursesButtonClass();
	btn->CTK_newButton(x,y,width,hite,label);
	btn->CTK_setColours(this->colours);
	this->pages[this->pageNumber].buttons.push_back(btn);
}

void CTK_mainAppClass::CTK_addNewInput(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesInputClass	*inp=new CTK_cursesInputClass();
	inp->CTK_newInput(x,y,width,hite,label);
	inp->CTK_setColours(this->colours);
	this->pages[this->pageNumber].inputs.push_back(inp);
}

void CTK_mainAppClass::CTK_addNewListBox(int x,int y,int width,int hite)
{
}

void CTK_mainAppClass::CTK_addNewCheckBox(int x,int y,int width,const char *label)
{
	CTK_cursesCheckBoxClass	*cb=new CTK_cursesCheckBoxClass();
	cb->CTK_newCheckBox(x,y,width,label);
	cb->CTK_setColours(this->colours);
	this->pages[this->pageNumber].checkBoxes.push_back(cb);
}

void CTK_mainAppClass::CTK_addNewEditBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	CTK_cursesEditBoxClass	*edbox=new CTK_cursesEditBoxClass();
	edbox->CTK_newBox(x,y,width,hite,isfilename,txt,selectable);
	edbox->CTK_setColours(this->colours);
	this->pages[this->pageNumber].editBoxes.push_back(edbox);
}

void CTK_mainAppClass::CTK_addMenuBar(CTK_cursesMenuClass *mb)
{
	this->menuBar=mb;
}

void CTK_mainAppClass::CTK_addTextBox(CTK_cursesTextBoxClass *txtbox)
{
	this->pages[this->pageNumber].textBoxes.push_back(txtbox);
}

void CTK_mainAppClass::CTK_addButton(CTK_cursesButtonClass *btn)
{
	this->pages[this->pageNumber].buttons.push_back(btn);
}

void CTK_mainAppClass::CTK_addInput(CTK_cursesInputClass *inp)
{
	this->pages[this->pageNumber].inputs.push_back(inp);
}

void CTK_mainAppClass::CTK_addListBox(CTK_cursesListBoxClass *lb)
{
	this->pages[this->pageNumber].lists.push_back(lb);
}

void CTK_mainAppClass::CTK_addCheckBox(CTK_cursesCheckBoxClass *cb)
{
	this->pages[this->pageNumber].checkBoxes.push_back(cb);
}

void CTK_mainAppClass::CTK_addEditBox(CTK_cursesEditBoxClass *edbox)
{
	this->pages[this->pageNumber].editBoxes.push_back(edbox);
}

void CTK_mainAppClass::CTK_updateScreen(void *object,void* userdata)
{
	CTK_mainAppClass	*app=static_cast<CTK_mainAppClass*>(object);

	setBackColour(app->colours.windowBackCol,app->colours.use256Colours);
	setForeColour(app->colours.windowForeCol,app->colours.use256Colours);

	if(app->menuBar!=NULL)
		app->menuBar->CTK_drawMenuBar();

	if(app->pageNumber<0)
		{
			app->CTK_clearScreen();
			if(app->menuBar!=NULL)
				app->menuBar->CTK_drawMenuBar();
			fflush(NULL);
			SETNORMAL;
			return;
		}

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
				app->pages[app->pageNumber].editBoxes[j]->CTK_drawBox(true);
			else
				app->pages[app->pageNumber].editBoxes[j]->CTK_drawBox(false);
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
			case HLTEXT:
				this->hiliteTxtBoxNum+=addit;
					
				if((this->hiliteTxtBoxNum<0) || (this->hiliteTxtBoxNum>=this->pages[this->pageNumber].textBoxes.size()))
					{
						this->hiliteTxtBoxNum=-1;
						if(addit==-1)
							this->hiliteBtnNum=this->pages[this->pageNumber].buttons.size();
						this->hiliting+=addit;
						this->setHilite(forward);
					}
				else
					if(this->pages[this->pageNumber].textBoxes[this->hiliteTxtBoxNum]->CTK_getSelectable()==false)
						this->setHilite(forward);
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
			case HLNOMORE:
				this->hiliting=HLNONE;
				break;
		}
}

void CTK_mainAppClass::CTK_mainEventLoop(void)
{
	int				selection=CONT;
	TermKey			*tk;
	TermKeyResult	ret;
	TermKeyKey		key;
	TermKeyFormat	format=TERMKEY_FORMAT_VIM;
	char			tstr[3]={'_',0,0};
	int				tab=1;

	tk = termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}

	this->CTK_clearScreen();
	this->CTK_updateScreen(this,NULL);
	SETHIDECURS;
	fflush(NULL);
	this->runEventLoop=true;
	while(this->runEventLoop==true)
		{
			ret=termkey_waitkey(tk,&key);
//fprintf(stderr,"key.code.sym=%i<<\n",key.code.sym);
//fprintf(stderr,"key.code.codepoint=%i<<\n",key.code.codepoint);
//fprintf(stderr,"key.modifiers=%i<<\n",key.modifiers);
//char	*buffer=(char*)alloca(256);;
//termkey_strfkey(tk, buffer, 255, &key, format);
//fprintf(stderr,"buffer=%s<<\n",buffer);
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
										this->hiliting=HLNONE;
										this->CTK_updateScreen(this,NULL);
										if(this->menuBar!=NULL)
											selection=this->menuBar->CTK_doMenuEvent(0,1,true);
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
										if(this->hiliteTxtBoxNum!=-1)
											this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(true);
										if(this->hiliteListNum!=-1)
											this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(true);
										break;
									case TERMKEY_SYM_DOWN:
										if(this->hiliteTxtBoxNum!=-1)
											this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(false);
										if(this->hiliteListNum!=-1)
											this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(false);
										break;
									case TERMKEY_SYM_PAGEUP:
										if(this->hiliteTxtBoxNum!=-1)
											this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(true);
										if(this->hiliteListNum!=-1)
											this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(true,true);
										break;
									case TERMKEY_SYM_PAGEDOWN:
										if(this->hiliteTxtBoxNum!=-1)
											this->pages[this->pageNumber].textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(false);
										if(this->hiliteListNum!=-1)
											this->pages[this->pageNumber].lists[hiliteListNum]->CTK_keyUpDown(false,true);
										break;

									case TERMKEY_SYM_ENTER:
										if(this->hiliteEditBoxNum!=-1)
											{
												this->pages[this->pageNumber].editBoxes[this->hiliteEditBoxNum]->CTK_doEditEvent();
												this->hiliteEditBoxNum=-1;
												this->CTK_updateScreen(this,NULL);
												continue;
											}
											
										if(this->hiliteBtnNum!=-1)
											{
												if(this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->selectCB!=NULL)
													this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->selectCB((void*)this->pages[this->pageNumber].buttons[this->hiliteBtnNum],(void*)this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->selectCBUserData);
												if(this->pages[this->pageNumber].buttons[this->hiliteBtnNum]->CTK_getEnterDeselects()==true)
													this->hiliteBtnNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteInputNum!=-1)
											{
												this->pages[this->pageNumber].inputs[this->hiliteInputNum]->CTK_doInput();
												this->hiliteInputNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteListNum!=-1)
											{
												if(this->pages[this->pageNumber].lists[this->hiliteListNum]->selectCB!=NULL)
													this->pages[this->pageNumber].lists[this->hiliteListNum]->selectCB((void*)this->pages[this->pageNumber].lists[this->hiliteListNum],this->pages[this->pageNumber].lists[this->hiliteListNum]->selectCBUserData);
												if(this->pages[this->pageNumber].lists[this->hiliteListNum]->CTK_getEnterDeselects()==true)
													this->hiliteListNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteCheckBoxNum!=-1)
											{
												if(this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->selectCB!=NULL)
													this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->selectCB((void*)this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum],this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->selectCBUserData);
												if(this->pages[this->pageNumber].checkBoxes[this->hiliteCheckBoxNum]->CTK_getEnterDeselects()==true)
													this->hiliteCheckBoxNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										break;
								}
						}

					case TERMKEY_TYPE_UNICODE:
						if(this->menuBar==NULL)
							break;
						
						if(key.modifiers==TERMKEY_KEYMOD_CTRL)
							{
								tstr[1]=toupper(key.code.codepoint);
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
			this->CTK_updateScreen(this,NULL);
			if(this->eventLoopCB!=NULL)
				this->eventLoopCB(this,NULL);
		}
}

void CTK_mainAppClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}

int CTK_mainAppClass::CTK_addPage(void)
{
	pageStruct	ps;

	this->pages.push_back(ps);
	this->pageNumber++;
	return(this->pageNumber);
}

void CTK_mainAppClass::CTK_setPage(int pagenum)
{
	if((pagenum>0) && (pagenum<this->pages.size()))
		this->pageNumber=pagenum;
}

int CTK_mainAppClass::CTK_previousPage(void)
{
	if(this->pageNumber>0)
		this->pageNumber--;
	return(this->pageNumber);
}

int CTK_mainAppClass::CTK_nextPage(void)
{
	if(this->pageNumber<this->pages.size()-1)
		this->pageNumber++;
	return(this->pageNumber);
}

int CTK_mainAppClass::CTK_removePage(int pagenum)
{
	if((pagenum>=0) && (pagenum<this->pages.size()))
		{
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




