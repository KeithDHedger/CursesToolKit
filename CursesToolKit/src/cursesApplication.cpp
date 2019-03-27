/*
 *
 * ©K. D. Hedger. Tue 19 Mar 13:36:20 GMT 2019 keithdhedger@gmail.com

 * This file (cursesApplication.cpp) is part of CursesMenus.

 * CursesMenus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesMenus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesMenus.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cursesApplication.h"

CTK_mainAppClass::~CTK_mainAppClass()
{
	delete this->menuBar;
	for(int j=0;j<this->textBoxes.size();j++)
		delete this->textBoxes[j];
	for(int j=0;j<this->buttons.size();j++)
		delete this->buttons[j];
	for(int j=0;j<this->inputs.size();j++)
		delete this->inputs[j];
}

CTK_mainAppClass::CTK_mainAppClass()
{
}

void CTK_mainAppClass::CTK_addMenuBar(void)
{
	this->menuBar=new CTK_cursesMenuClass();
	this->menuBar->CTK_setUpdateCB(this->CTK_updateScreen,this);
}

void CTK_mainAppClass::CTK_addTextBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass();
	txtbox->CTK_newBox(x,y,width,hite,txt,selectable);
	this->textBoxes.push_back(txtbox);
}

void CTK_mainAppClass::CTK_addButton(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesButtonClass	*btn=new CTK_cursesButtonClass();
	btn->CTK_newButton(x,y,width,hite,label);
	this->buttons.push_back(btn);
}

void CTK_mainAppClass::CTK_addInput(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesInputClass	*inp=new CTK_cursesInputClass();
	inp->CTK_newInput(x,y,width,hite,label);
	this->inputs.push_back(inp);
}

void CTK_mainAppClass::CTK_updateScreen(void *object,void* userdata)
{
	CTK_mainAppClass	*app=static_cast<CTK_mainAppClass*>(object);

	printf("\e[2J\e[H");

	for(int j=0;j<app->textBoxes.size();j++)
		{
			if(app->hiliteTxtBoxNum==j)
				app->textBoxes[j]->CTK_drawBox(true);
			else
				app->textBoxes[j]->CTK_drawBox(false);
		}

	for(int j=0;j<app->buttons.size();j++)
		{
			if(app->hiliteBtnNum==j)
				app->buttons[j]->CTK_drawButton(true);
			else
				app->buttons[j]->CTK_drawButton(false);
		}

	for(int j=0;j<app->inputs.size();j++)
		{
			if(app->hiliteInputNum==j)
				app->inputs[j]->CTK_drawInput(true);
			else
				app->inputs[j]->CTK_drawInput(false);
		}

	app->menuBar->CTK_drawMenuBar();
}

void CTK_mainAppClass::CTK_mainEventLoop(void)
{
	int				selection=CONT;
	TermKey			*tk;
	TermKeyResult	ret;
	TermKeyKey		key;
	TermKeyFormat	format=TERMKEY_FORMAT_VIM;
	char			tstr[3]={'_',0,0};

	tk = termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}

	this->CTK_updateScreen(this,NULL);
	SETHIDECURS;
	fflush(NULL);
	this->runEventLoop=true;
	while(this->runEventLoop==true)
		{
			ret=termkey_waitkey(tk,&key);
//fprintf(stderr,"key.code.sym=%i<<\n",key.code.sym);
//fprintf(stderr,"key.code.codepoint=%i<<\n",key.code.codepoint);
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
										this->hiliteBtnNum=-1;
										this->hiliteTxtBoxNum=-1;
										this->hiliting=NONE;
										this->CTK_updateScreen(this,NULL);
										selection=this->menuBar->CTK_doMenuEvent(0,1,true);
										break;
									case TERMKEY_SYM_TAB:
										if(this->hiliting==NONE)
											this->hiliting=BUTTONS;
										switch(this->hiliting)
											{
												case BUTTONS:
													this->hiliteBtnNum++;
													if(this->hiliteBtnNum>=this->buttons.size())
														{
															this->hiliteBtnNum=-1;
															this->hiliting=TEXT;
														}
													else
														break;
												case TEXT:
													this->hiliteTxtBoxNum++;
													if(this->hiliteTxtBoxNum>=this->textBoxes.size())
														{
															this->hiliteTxtBoxNum=-1;
															this->hiliting=INPUTS;
														}
													else
														{
															if(this->textBoxes[this->hiliteTxtBoxNum]->CTK_getSelectable()==true)
																break;
															else
																{
																	this->hiliteTxtBoxNum++;
																	if(this->hiliteTxtBoxNum>=this->textBoxes.size())
																		{
																			this->hiliteTxtBoxNum=-1;
																			this->hiliting=INPUTS;
																		}
																	else
																		break;
																}
														}
												case INPUTS:
													this->hiliteInputNum++;
													if(this->hiliteInputNum>=this->inputs.size())
														{
															this->hiliteInputNum=-1;
															this->hiliting=NONE;
														}
													else
														break;
											}
										break;
//scroll txt boxes
									case TERMKEY_SYM_UP:
										if(this->hiliteTxtBoxNum!=-1)
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(true);
										break;
									case TERMKEY_SYM_DOWN:
										if(this->hiliteTxtBoxNum!=-1)
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(false);
										break;
									case TERMKEY_SYM_PAGEUP:
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(true);
										break;
									case TERMKEY_SYM_PAGEDOWN:
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(false);
										break;

									case TERMKEY_SYM_ENTER:
										if(this->hiliteBtnNum!=-1)
											{
												this->buttons[this->hiliteBtnNum]->selectCB((void*)this->buttons[this->hiliteBtnNum]);
												this->hiliteBtnNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteInputNum!=-1)
											{
												this->inputs[this->hiliteInputNum]->CTK_doInput();
												this->hiliteInputNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										break;
								}
						}

					case TERMKEY_TYPE_UNICODE:
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
			this->eventLoopCB(this,NULL);
		}
}

