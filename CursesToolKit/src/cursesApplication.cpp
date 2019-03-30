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
	for(int j=0;j<this->textBoxes.size();j++)
		delete this->textBoxes[j];
	for(int j=0;j<this->buttons.size();j++)
		delete this->buttons[j];
	for(int j=0;j<this->inputs.size();j++)
		delete this->inputs[j];
	for(int j=0;j<this->lists.size();j++)
		delete this->lists[j];
	for(int j=0;j<this->checkBoxes.size();j++)
		delete this->checkBoxes[j];
}

CTK_mainAppClass::CTK_mainAppClass()
{
	winsize w;

    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
	this->maxRows=w.ws_row;
	this->maxCols=w.ws_col;
}

void CTK_mainAppClass::CTK_addNewMenuBar(void)
{
	this->menuBar=new CTK_cursesMenuClass();
	this->menuBar->CTK_setUpdateCB(this->CTK_updateScreen,this);
}

void CTK_mainAppClass::CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	CTK_cursesTextBoxClass	*txtbox=new CTK_cursesTextBoxClass();
	txtbox->CTK_newBox(x,y,width,hite,txt,selectable);
	this->textBoxes.push_back(txtbox);
}

void CTK_mainAppClass::CTK_addNewButton(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesButtonClass	*btn=new CTK_cursesButtonClass();
	btn->CTK_newButton(x,y,width,hite,label);
	this->buttons.push_back(btn);
}

void CTK_mainAppClass::CTK_addNewInput(int x,int y,int width,int hite,const char *label)
{
	CTK_cursesInputClass	*inp=new CTK_cursesInputClass();
	inp->CTK_newInput(x,y,width,hite,label);
	this->inputs.push_back(inp);
}

void CTK_mainAppClass::CTK_addNewListBox(int x,int y,int width,int hite)
{
//	CTK_cursesInputClass	*inp=new CTK_cursesInputClass();
//	inp->CTK_newInput(x,y,width,hite,label);
//	this->inputs.push_back(inp);
}

void CTK_mainAppClass::CTK_addNewCheckBox(int x,int y,int width,const char *label)
{
	CTK_cursesCheckBoxClass	*cb=new CTK_cursesCheckBoxClass();
	cb->CTK_newCheckBox(x,y,width,label);
	this->checkBoxes.push_back(cb);
}

void CTK_mainAppClass::CTK_addMenuBar(CTK_cursesMenuClass *mb)
{
	this->menuBar=mb;
}

void CTK_mainAppClass::CTK_addTextBox(CTK_cursesTextBoxClass *txtbox)
{
	this->textBoxes.push_back(txtbox);
}

void CTK_mainAppClass::CTK_addButton(CTK_cursesButtonClass *btn)
{
	this->buttons.push_back(btn);
}

void CTK_mainAppClass::CTK_addInput(CTK_cursesInputClass *inp)
{
	this->inputs.push_back(inp);
}

void CTK_mainAppClass::CTK_addListBox(CTK_cursesListBoxClass *lb)
{
	this->lists.push_back(lb);
}

void CTK_mainAppClass::CTK_addCheckBox(CTK_cursesCheckBoxClass *cb)
{
	this->checkBoxes.push_back(cb);
}

void CTK_mainAppClass::CTK_updateScreen(void *object,void* userdata)
{
	CTK_mainAppClass	*app=static_cast<CTK_mainAppClass*>(object);

	printf("\e[2J\e[H");
//	fflush(NULL);
//	printf("\e[2J\e[H");


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

	for(int j=0;j<app->checkBoxes.size();j++)
		{
			if(app->hiliteCheckBoxNum==j)
				app->checkBoxes[j]->CTK_drawCheckBox(true);
			else
				app->checkBoxes[j]->CTK_drawCheckBox(false);
		}

	for(int j=0;j<app->lists.size();j++)
		{
			if(app->hiliteListNum==j)
				app->lists[j]->CTK_drawListWindow(true);
			else
				app->lists[j]->CTK_drawListWindow(false);
		}

	if(app->menuBar!=NULL)
		app->menuBar->CTK_drawMenuBar();

	SETNORMAL;
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
										this->hiliteInputNum=-1;
										this->hiliteListNum=-1;
										this->hiliteCheckBoxNum=-1;

										this->hiliting=NONE;
										this->CTK_updateScreen(this,NULL);
										selection=this->menuBar->CTK_doMenuEvent(0,1,true);
										break;
									case TERMKEY_SYM_TAB:
										if(key.modifiers==1)
											{
												if(this->hiliting==NONE)
													{
														this->hiliting=CHKBOXS;
														this->hiliteCheckBoxNum=this->checkBoxes.size();
													}
												switch(this->hiliting)
													{
														case CHKBOXS:
															this->hiliteCheckBoxNum--;
															if(this->hiliteCheckBoxNum<0)
																{
																	this->hiliteCheckBoxNum=-1;
																	this->hiliteListNum=this->lists.size();
																	this->hiliting=LISTS;
																}
															else
																break;
														case LISTS:
															this->hiliteListNum--;
															if(this->hiliteListNum<0)
																{
																	this->hiliteListNum=-1;
																	this->hiliteInputNum=this->inputs.size();
																	this->hiliting=INPUTS;
																}
															else
																break;
														case INPUTS:
															this->hiliteInputNum--;
															if(this->hiliteInputNum<0)
																{
																	this->hiliteInputNum=-1;
																	this->hiliteTxtBoxNum=this->textBoxes.size();
																	this->hiliting=TEXT;
																}
															else
																break;
														case TEXT:
															this->hiliteTxtBoxNum--;
															if(this->hiliteTxtBoxNum<0)
																{
																	this->hiliteTxtBoxNum=-1;
																	this->hiliteBtnNum=this->buttons.size();
																	this->hiliting=BUTTONS;
																}
															else
																{
																	if(this->textBoxes[this->hiliteTxtBoxNum]->CTK_getSelectable()==true)
																		break;
																	else
																		{
																			this->hiliteTxtBoxNum--;
																			if(this->hiliteTxtBoxNum<0)
																				{
																					this->hiliteTxtBoxNum=-1;
																					this->hiliteBtnNum=this->buttons.size();
																					this->hiliting=BUTTONS;
																				}
																			else
																				break;
																		}
																}
														case BUTTONS:
															this->hiliteBtnNum--;
															if(this->hiliteBtnNum<0)
																{
																	this->hiliteBtnNum=-1;
																	this->hiliting=NONE;
																}
															else
																break;
													}
											}
										else
											{
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
																	this->hiliting=LISTS;
																}
															else
																break;
														case LISTS:
															this->hiliteListNum++;
															if(this->hiliteListNum>=this->lists.size())
																{
																	this->hiliteListNum=-1;
																	this->hiliting=CHKBOXS;
																}
															else
																break;
														case CHKBOXS:
															this->hiliteCheckBoxNum++;
															if(this->hiliteCheckBoxNum>=this->checkBoxes.size())
																{
																	this->hiliteCheckBoxNum=-1;
																	this->hiliting=NONE;
																}
															else
																break;
													}
											}
										break;
//scroll txt boxes and lists
									case TERMKEY_SYM_UP:
										if(this->hiliteTxtBoxNum!=-1)
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(true);
										if(this->hiliteListNum!=-1)
											this->lists[hiliteListNum]->CTK_keyUpDown(true);
										break;
									case TERMKEY_SYM_DOWN:
										if(this->hiliteTxtBoxNum!=-1)
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollLine(false);
										if(this->hiliteListNum!=-1)
											this->lists[hiliteListNum]->CTK_keyUpDown(false);
										break;
									case TERMKEY_SYM_PAGEUP:
										if(this->hiliteTxtBoxNum!=-1)
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(true);
										if(this->hiliteListNum!=-1)
											this->lists[hiliteListNum]->CTK_keyUpDown(true,true);
										break;
									case TERMKEY_SYM_PAGEDOWN:
										if(this->hiliteTxtBoxNum!=-1)
											this->textBoxes[hiliteTxtBoxNum]->CTK_scrollPage(false);
										if(this->hiliteListNum!=-1)
											this->lists[hiliteListNum]->CTK_keyUpDown(false,true);
										break;

									case TERMKEY_SYM_ENTER:
										if(this->hiliteBtnNum!=-1)
											{
												this->buttons[this->hiliteBtnNum]->selectCB((void*)this->buttons[this->hiliteBtnNum]);
												if(this->buttons[this->hiliteBtnNum]->CTK_getEnterDeselects()==true)
													this->hiliteBtnNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteInputNum!=-1)
											{
												this->inputs[this->hiliteInputNum]->CTK_doInput();
												this->hiliteInputNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteListNum!=-1)
											{
												this->lists[this->hiliteListNum]->selectCB((void*)this->lists[this->hiliteListNum]);
												if(this->lists[this->hiliteListNum]->CTK_getEnterDeselects()==true)
													this->hiliteListNum=-1;
												this->CTK_updateScreen(this,NULL);
											}
										if(this->hiliteCheckBoxNum!=-1)
											{
												this->checkBoxes[this->hiliteCheckBoxNum]->selectCB((void*)this->checkBoxes[this->hiliteCheckBoxNum]);
												if(this->checkBoxes[this->hiliteCheckBoxNum]->CTK_getEnterDeselects()==true)
													this->hiliteCheckBoxNum=-1;
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

