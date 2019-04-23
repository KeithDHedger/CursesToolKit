/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:51:59 GMT 2019 keithdhedger@gmail.com

 * This file (cursesMenus.cpp) is part of CursesToolKit.

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

#include "cursesMenus.h"

CTK_cursesMenuClass::~CTK_cursesMenuClass()
{
	for(int j=0;j<menuNames.size();j++)
		{
			this->CTK_clearMenu(j);
			delete this->menuNames[j];
		}
	this->menuNames.clear();
	//delete this->menuNames;
	termkey_destroy(this->tk);
}

CTK_cursesMenuClass::CTK_cursesMenuClass()
{
	winsize	w;
	this->tk=termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}

    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
	rows=w.ws_row;
	maxRows=rows-mBarHite+1;
	menuHite=rows-mBarHite;
	this->menuNames.clear();
	CTK_setColours(this->colours);
}

void CTK_cursesMenuClass::CTK_drawMenuBar(void)
{
	int	x=1;
	int y=1;

	for(int j=0;j<this->menuCnt;j++)
		{
			setBackColour(this->colours.backCol);
			setForeColour(this->colours.foreCol);
			MOVETO(x,y);
			printf("%s",this->menuNames[j]->menuName);
			setBackColour(this->colours.windowBackCol);
			printf(" ");
			x+=strlen(this->menuNames[j]->menuName)+1;
		}
	SETNORMAL;
}

char CTK_cursesMenuClass::setShortCut(const char *name)
{
	for(int j=0;j<strlen(name);j++)
		{
			if(name[j]=='_')
				return(toupper(name[j+1]));
		}
	return(0);
}

void CTK_cursesMenuClass::CTK_addMenuItem(int menunum,const char *name)
{
	menuStruct	*menu=new menuStruct;

	menu->menuName=strdup(name);
	menu->key=setShortCut(name);
	menu->maxWidth=strlen(name);
	if(menu->key!=0)
		{
			this->menuNames[menunum]->itemsHaveKey=true;
			if(this->menuNames[menunum]->maxWidth<strlen(name)+strlen("Ctrl+X"))
				this->menuNames[menunum]->maxWidth=strlen(name)+strlen("Ctrl+X");
		}
	else
		{
			if(this->menuNames[menunum]->maxWidth<menu->maxWidth)
				this->menuNames[menunum]->maxWidth=menu->maxWidth;
		}
	menu->menuItem.clear();
	this->menuNames[menunum]->menuItem[this->menuNames[menunum]->menuItemCnt++]=menu;
}

void CTK_cursesMenuClass::CTK_addMenuToBar(const char *name)
{
	menuStruct	*menu=new menuStruct;

	menu->menuName=strdup(name);
	menu->key=0;
	menu->startCol=this->menuNamesStartX;
	this->menuNamesStartX+=strlen(name)+1;
	menu->menuItem.clear();

	this->menuNames[this->menuCnt++]=menu;
}

void CTK_cursesMenuClass::drawMenuStyle(int menunum,int menuitem,int x,int y,int style,bool doshortcut,bool dopad)
{
	bool		gotus=false;
	menuStruct	*themenu;

	MOVETO(x,y);
	switch(style)
		{
			case FLATINVERT:
				setBackColour(this->colours.hiliteBackCol);
				setForeColour(this->colours.hiliteForeCol);
				break;
			case FLATNORM:
				setBackColour(this->colours.backCol);
				setForeColour(this->colours.foreCol);
				break;
			case BLANK:
				setBackColour(this->colours.windowBackCol);
				setForeColour(this->colours.windowBackCol);
				break;
		}

	if(menuitem<0)
		themenu=this->menuNames[menunum];
	else
		themenu=this->menuNames[menunum]->menuItem[menuitem];

	if(style==BLANK)
		{
			MOVETO(x+this->menuWidth+1,y);
			printf(CLEARTOSOL);
			return;
		}

	for(unsigned j=0;j<this->menuWidth;j++)
		{
			if(j<strlen(themenu->menuName))
				{
					if((themenu->menuName[j]=='_') && (doshortcut==true) && (style!=BLANK))
						{
							j++;
							printf("%s%c%s",UNDERSCOREON,themenu->menuName[j],UNDERSCOREOFF);
							gotus=true;
						}
					else
						printf("%c",themenu->menuName[j]);
				}
			else
				printf(" ");
		}

	if((gotus==true) && (dopad==true))
		{
			MOVETO(x+this->menuWidth-6,y);
			printf(" Ctrl+%c ",themenu->key);
		}

	if(gotus==false)
		if(doshortcut==true)
			printf("  ");
		else
			printf(" ");
	fflush(NULL);
}

int CTK_cursesMenuClass::drawMenuWindow(int menunum,int sx,int sy,int prelight,bool doshortcut)
{
	int	y=sy+1;
	int	maxitems=0;
	int msx=this->menuNames[menunum]->startCol;

	this->menuWidth=0;

	maxitems=this->menuNames[menunum]->menuItemCnt;
	this->menuWidth=this->menuNames[menunum]->maxWidth;
	if(doshortcut==true)
		this->menuWidth--;

	for(int cnt=0;cnt<this->menuNames[menunum]->menuItem.size();cnt++)
		{
			if(prelight==-10000)
				{
					this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,BLANK,doshortcut,true);
				}
			else
				{
					if(prelight==cnt)
						this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,FLATINVERT,doshortcut,true);
					else
						this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,FLATNORM,doshortcut,true);
				}
		}
	SETNORMAL;
	return(maxitems);
}

int CTK_cursesMenuClass::CTK_doMenuEvent(int sx,int sy,bool xdoshortcut)
{
	bool			loop=true;
	bool			mainloop=true;
	int				selection=0;
	int				maxitems;
	int				cnt=0;
	char			tstr[3]={'_',0,0};
	char			buffer[32];

	TermKeyResult	ret;
	TermKeyKey		key;
	TermKeyFormat	format=TERMKEY_FORMAT_VIM;

	if(this->menuNumber<0)
		return(CONT);

	bool doshortcut=false;

	while(mainloop==true)
		{
			doshortcut=this->menuNames[this->menuNumber]->itemsHaveKey;
			if(this->menuNames[this->menuStart]!=NULL)
				maxitems=this->drawMenuWindow(this->menuNumber,sx,1,-1,doshortcut);
			this->CTK_drawMenuBar();
			SETHIDECURS;
			fflush(NULL);
			loop=true;
			while(loop==true)
				{
					ret=termkey_waitkey(this->tk,&key);
					termkey_strfkey(this->tk,buffer,32,&key,format);

					switch(key.type)
						{
							case TERMKEY_TYPE_KEYSYM:
								{
									switch(key.code.sym)
										{
											case TERMKEY_SYM_ESCAPE:
												loop=false;
												selection=BRAKE;
												this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
												return(selection);
												break;
											case TERMKEY_SYM_UP:
												selection--;
												if(selection<1)
													{
														selection=1;
														if(menuStart>0)
															menuStart--;
													}
												this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_DOWN:
												selection++;
												if((selection>maxitems) || (selection>menuHite))
													selection=menuHite-mBarHite;
												if(selection>maxitems)
													selection=maxitems;

												if(selection+menuStart<=maxitems)
													{
														if(selection>menuHite-mBarHite)
															{
																selection=menuHite-mBarHite;
																menuStart++;
															}
													}
												else
													selection--;
												this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_LEFT:
												menuStart=0;
												this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
												this->menuNumber--;
												if(this->menuNumber<0)
													this->menuNumber=0;
												loop=false;
												selection=0;
												continue;
												break;
											case TERMKEY_SYM_RIGHT:
												menuStart=0;
												this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
												selection=0;
												loop=false;
												this->menuNumber++;
												if(this->menuNumber>=this->menuCnt)
													this->menuNumber--;
												continue;
												break;
											case TERMKEY_SYM_HOME:
												selection=1;
												menuStart=0;
												this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_END:
												if(maxitems>rows-2)
													{
														selection=rows-2;
														menuStart=maxitems-rows+2;
														if((menuStart+rows-2)>maxitems)
															menuStart=maxitems-rows+2;
													}
												else
													selection=maxitems;
												this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_PAGEUP:
												menuStart-=menuHite;
												if(menuStart<0)
													menuStart=0;
												this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_PAGEDOWN:
												if(maxitems<menuHite)
													continue;
												menuStart+=menuHite-1;
												if((menuStart+menuHite)>maxitems)
													menuStart=maxitems-menuHite+1;
												this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_ENTER:
												this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
												this->menuItemNumber=selection+this->menuStart-1;
												this->selectCB(this);
												return(SELECTED);
												break;
											default:
												break;
										}
								}

							case TERMKEY_TYPE_UNICODE:
								if(key.modifiers==0)
									{
										this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
										tstr[1]=toupper(key.code.codepoint);
										if(this->CTK_doShortCutKey(tstr[1],this->menuNumber)==true)
											{
												this->selectCB(this);
												return(SELECTED);
											}
									}
								break;

							default:
								break;
						}
				}
			this->updateCB((void*)this->mainApp,NULL);
		}
	SETNORMAL;
	return(selection);
}

void CTK_cursesMenuClass::CTK_clearMenu(int menunum)
{
	for(int j=0;j<this->menuNames[menunum]->menuItem.size();j++)
		{
			free(this->menuNames[menunum]->menuItem[j]->menuName);
			delete this->menuNames[menunum]->menuItem[j];
		}
	this->menuNames[menunum]->menuItem.clear();
	freeAndNull(&this->menuNames[menunum]->menuName);
	this->menuNames[menunum]->menuItemCnt=0;
}

bool CTK_cursesMenuClass::CTK_doShortCutKey(char key,int menunum)
{
	for(int k=0;k<this->menuNames[menunum]->menuItem.size();k++)
		{
			if(key==this->menuNames[menunum]->menuItem[k]->key)
				{
					this->menuItemNumber=k;
					return(true);
				}
		}
	return(false);
}

void CTK_cursesMenuClass::CTK_setUpdateCB(void (*update)(void *,void*),void* mainapp)
{
	this->updateCB=update;
	this->mainApp=mainapp;
}

void CTK_cursesMenuClass::CTK_setSelectCB(void (*select)(void *))
{
	this->selectCB=select;
}

void CTK_cursesMenuClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}



