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

#include "cursesGlobals.h"

/**
* Menus class destroy.
*/
CTK_cursesMenuClass::~CTK_cursesMenuClass()
{
	for(int j=0;j<menuNames.size();j++)
		{
			this->CTK_clearMenu(j,true);
			delete this->menuNames[j];
		}
	this->menuNames.clear();
}

/**
* Menus class.
*/
CTK_cursesMenuClass::CTK_cursesMenuClass(CTK_mainAppClass *mc)
{
//	winsize	w;

 //  ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
//	this->rows=w.ws_row;
	fprintf(stderr,"mc=%i\n",mc->maxRows);
	this->rows=mc->maxRows;
	this->maxRows=this->rows-mBarHite+1;
	this->menuHite=this->rows-mBarHite;
	this->menuNames.clear();
	this->CTK_setCommon(mc);
	this->type=MENUGADGET;
}

/**
* Draw menu bar.
*/
void CTK_cursesMenuClass::CTK_drawGadget(bool hilite)//TODO//
{
	int	x=1;
	int y=1;

	if(this->menuBarVisible==false)
		return;

	for(int j=0;j<this->menuCnt;j++)
		{
			if(this->enableMenuBar==true)
				setBothColours(this->colours.menuForeCol,this->colours.menuBackCol,this->colours.use256Colours);
			else
				setBothColours(this->colours.disabledForeCol,this->colours.menuBackCol,this->colours.use256Colours);
			MOVETO(x,y);

			if((this->menuShowing==true) && (this->menuNumber==j))
				{
					if(this->menuNames[j]->menuEnabled==true)
						setBothColours(this->colours.menuHiliteForeCol,this->colours.menuHiliteBackCol,this->colours.use256Colours);
					else
						setBothColours(this->colours.disabledForeCol,this->colours.menuBackCol,this->colours.use256Colours);
					printf( "%s" ,this->menuNames[j]->menuName);
				}
			else
				{
					if(this->menuNames[j]->menuEnabled==false)
						setBothColours(this->colours.disabledForeCol,this->colours.menuBackCol,this->colours.use256Colours);
					printf("%s",this->menuNames[j]->menuName);
				}
			printf(" ");
			x+=strlen(this->menuNames[j]->menuName)+1;
		}
	MOVETO(x,1);
	setBothColours(this->colours.menuForeCol,this->colours.menuBackCol,this->colours.use256Colours);
	printf("%*s",this->mc->maxCols-x+1,"");
	SETNORMAL;
	MOVETO(1,1000);
}

/**
* Private
*/
char CTK_cursesMenuClass::setShortCut(const char *name)
{
	for(int j=0;j<strlen(name);j++)
		{
			if(name[j]=='_')
				return(toupper(name[j+1]));
		}
	return(0);
}

/**
* Set a shortcut key to menu item.
*/
void CTK_cursesMenuClass::CTK_setMenuShortCut(int menunum,int menuitem,char key)
{
	this->menuNames[menunum]->menuItem[menuitem]->shortcutKey=toupper(key);
	this->menuNames[menunum]->itemsHaveKey=true;

	int len=strlen(this->menuNames[menunum]->menuItem[menuitem]->menuName);

	if(this->menuNames[menunum]->maxWidth<len)
		this->menuNames[menunum]->maxWidth=len;
}

/**
* Add menu item to menu.
*/
void CTK_cursesMenuClass::CTK_addMenuItem(int menunum,const char *name,bool shortcut)
{
	menuStruct	*menu=new menuStruct;

	menu->menuName=strdup(name);
	if(shortcut==true)
		menu->menuKey=setShortCut(name);
	menu->maxWidth=strlen(name)+1;

	if(this->menuNames[menunum]->maxWidth<menu->maxWidth)
		this->menuNames[menunum]->maxWidth=menu->maxWidth;

	menu->menuItem.clear();
	this->menuNames[menunum]->menuItem[this->menuNames[menunum]->menuItemCnt++]=menu;
}

/**
* Add menu to menu bar.
*/
void CTK_cursesMenuClass::CTK_addMenuToBar(const char *name)
{
	menuStruct	*menu=new menuStruct;

	menu->menuName=strdup(name);
	menu->menuKey=0;
	menu->shortcutKey=0;
	menu->startCol=this->menuNamesStartX;
	this->menuNamesStartX+=strlen(name)+1;
	menu->menuItem.clear();

	this->menuNames[this->menuCnt++]=menu;
}

/**
* Private.
*/
void CTK_cursesMenuClass::drawMenuStyle(int menunum,int menuitem,int x,int y,int style,bool doshortcut,bool dopad)
{
	bool		gotus=false;
	menuStruct	*themenu;

	MOVETO(x,y);
	switch(style)
		{
			case FLATINVERT:
				setBothColours(this->colours.menuHiliteForeCol,this->colours.menuHiliteBackCol,this->colours.use256Colours);
				break;
			case FLATNORM:
				setBothColours(this->colours.menuForeCol,this->colours.menuBackCol,this->colours.use256Colours);
				break;
			case BLANK:
				setBothColours(this->colours.windowBackCol,this->colours.windowBackCol,this->colours.use256Colours);
				break;
			case DISABLED:
				setBothColours(this->colours.disabledForeCol,this->colours.menuBackCol,this->colours.use256Colours);
				break;
		}

	if(menuitem<0)
		themenu=this->menuNames[menunum];
	else
		themenu=this->menuNames[menunum]->menuItem[menuitem];

	if(style==BLANK)
		{
			MOVETO(x+this->menuWidth+8,y);
			printf(CLEARTOSOL);
			return;
		}

	MOVETO(x,y);
	for(unsigned j=0;j<this->menuNames[menunum]->maxWidth-1;j++)
		printf(" ");
	if(this->menuNames[menunum]->itemsHaveKey==true)
		printf("       ");

	MOVETO(x,y);
	for(unsigned j=0;j<strlen(themenu->menuName);j++)
		{
			if((themenu->menuName[j]=='_')  && (style!=BLANK))
				{
					j++;
					printf("%s%c%s",UNDERSCOREON,themenu->menuName[j],UNDERSCOREOFF);
					gotus=true;
				}
			else
				printf("%c",themenu->menuName[j]);
		}

	if(themenu->shortcutKey!=0)
		{
			MOVETO(x+this->menuWidth-1,y);
			printf("Ctrl+%c ",themenu->shortcutKey);
		}
	fflush(stdout);
}

/**
* Private.
*/
int CTK_cursesMenuClass::drawMenuWindow(int menunum,int sx,int sy,int prelight,bool doshortcut)
{
	if(this->menuNames[menunum]->menuEnabled==false)
		return(0);

	int	y=sy+1;
	int	maxitems=0;
	int msx=this->menuNames[menunum]->startCol;
	int sz=this->menuNames[menunum]->menuItem.size();
	int rows=this->mc->maxRows-2;

//rows=this->rows;

	maxitems=this->menuNames[menunum]->menuItemCnt;
	this->menuWidth=this->menuNames[menunum]->maxWidth;
	for(int cnt=0;cnt<sz;cnt++)
		{
			//if(cnt<static_cast<CTK_mainAppClass*>(this->mc)->maxRows-2)
			if(cnt<rows)
				{
					if(prelight==-10000)
						{
							this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,BLANK,doshortcut,true);
						}
					else
						{
							if(this->menuNames[menunum]->menuItem[cnt+this->menuStart]->menuEnabled==true)
								{
									if(prelight==cnt)
										this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,FLATINVERT,doshortcut,true);
									else
										this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,FLATNORM,doshortcut,true);
								}
							else
								{
									this->drawMenuStyle(menunum,cnt+this->menuStart,msx,y++,DISABLED,doshortcut,true);
								}
						}
					fflush(NULL);
				}
			else
				break;
		}
	SETNORMAL;
	return(maxitems);
}

/**
* Handle menu events.
*/
int CTK_cursesMenuClass::CTK_doMenuEvent(int sx,int sy,bool xdoshortcut)
{
	bool			loop=true;
	bool			mainloop=true;
	int				selection=0;
	int				maxitems;
	bool			doshortcut=false;
	bool			sink;

	if(this->menuNumber<0)
		return(CONT);

	this->mc->CTK_updateScreen(this->mc,SCREENUPDATEUNHILITE);

	while(mainloop==true)
		{
			this->menuShowing=true;
			doshortcut=this->menuNames[this->menuNumber]->itemsHaveKey;
			this->menuStart=0;
			if(this->menuNames[this->menuStart]!=NULL)
				maxitems=this->drawMenuWindow(this->menuNumber,sx,1,-1,doshortcut);
			this->CTK_drawGadget();
			SETHIDECURS;
			fflush(NULL);
			loop=true;

			while(loop==true)
				{
					this->mc->readKey->CTK_getInput();
					if(this->mc->readKey->isHexString==true)
						{
							switch(this->mc->readKey->specialKeyName)
								{
									case CTK_KEY_UP:
										selection--;
										if(selection<1)
											{
												selection=1;
												if(menuStart>0)
													menuStart--;
											}
//skip disabled menu items
										while((selection>0) && (this->menuNames[this->menuNumber]->menuItem[selection-1+this->menuStart]->menuEnabled==false))
											selection--;
										while(selection<1)
											{
												selection++;
												this->menuStart--;
											}
										if(this->menuStart<0)
											this->menuStart=0;
										this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
										continue;
										break;

									case CTK_KEY_DOWN:
//skip disabled menu items
										while((selection<maxitems) && (this->menuNames[this->menuNumber]->menuItem[selection]->menuEnabled==false))
											selection++;
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

									case CTK_KEY_LEFT:
										menuStart=0;
										this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
										this->menuNumber--;
										while((this->menuNumber>-1) && (this->menuNames[this->menuNumber]->menuEnabled==false))
											this->menuNumber--;
										if(this->menuNumber<0)
											this->menuNumber=this->menuCnt-1;
										loop=false;
										selection=0;
										this->mc->CTK_updateScreen(this->mc,SCREENUPDATEALL);
										continue;
										break;

									case CTK_KEY_RIGHT:
										menuStart=0;
										this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
										selection=0;
										loop=false;
										this->menuNumber++;
										while((this->menuNumber<this->menuCnt) && (this->menuNames[this->menuNumber]->menuEnabled==false))
											this->menuNumber++;
										if(this->menuNumber>=this->menuCnt)
											this->menuNumber=0;
										this->mc->CTK_updateScreen(this->mc,SCREENUPDATEALL);
										continue;
										break;

									case CTK_KEY_HOME:
										selection=1;
										menuStart=0;
										this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
										continue;
										break;

									case CTK_KEY_END:
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

									case CTK_KEY_PAGEUP:
										menuStart-=menuHite;
										if(menuStart<0)
											menuStart=0;
										this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
										continue;
										break;

									case CTK_KEY_PAGEDOWN:
										if(maxitems<menuHite)
											continue;
										menuStart+=menuHite-1;
										if((menuStart+menuHite)>maxitems)
											menuStart=maxitems-menuHite+1;
										this->drawMenuWindow(this->menuNumber,sx,1,selection-1,doshortcut);
										continue;
										break;

									case CTK_KEY_ENTER:
									case CTK_KEY_RETURN:
										this->mc->CTK_clearScreen();
										this->mc->CTK_updateScreen(this->mc,NULL);
										this->menuItemNumber=selection+this->menuStart-1;
										fflush(NULL);
										if(this->menuItemNumber>-1)
											sink=this->selectCB(this,NULL);
										fflush(NULL);
										return(SELECTED);
										break;

									case CTK_KEY_ESC:
										loop=false;
										selection=BRAKE;
										sink=this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
										return(selection);
										break;

									default:
										break;
								}
						}
					else
						{
							//fprintf(stderr,"Unknown Key ... ");
							//fprintf(stderr,"%s\n",this->mc->readKey->inputBuffer.c_str());
							this->drawMenuWindow(this->menuNumber,sx,1,-10000,doshortcut);
							if(this->CTK_doMenuKey(toupper(this->mc->readKey->inputBuffer.c_str()[0]),this->menuNumber)==true)
								{
									this->selectCB(this,NULL);
									return(SELECTED);
								}
						}
				}
			this->updateCB((void*)this->mc,NULL);
		}
	SETNORMAL;
	return(selection);
}

/**
* Clear all menu items from menu.
* \note User data is the callers responsibility.
*/
void CTK_cursesMenuClass::CTK_clearMenu(int menunum,bool full)
{
	for(int j=0;j<this->menuNames[menunum]->menuItem.size();j++)
		{
			free(this->menuNames[menunum]->menuItem[j]->menuName);
			delete this->menuNames[menunum]->menuItem[j];
		}
	this->menuNames[menunum]->menuItem.clear();
	if(full==true)
		CTK_freeAndNull(&this->menuNames[menunum]->menuName);
	this->menuNames[menunum]->menuItemCnt=0;
	this->menuNames[menunum]->maxWidth=0;
}

/**
* Check for and handle shortcut key.
*/
bool CTK_cursesMenuClass::CTK_doShortCutKey(char key,int menunum)
{
	for(int k=0;k<this->menuNames[menunum]->menuItem.size();k++)
		{
			if((this->menuNames[menunum]->menuItem[k]->menuEnabled==true) && (key==this->menuNames[menunum]->menuItem[k]->shortcutKey))
				{
					this->menuItemNumber=k;
					return(true);
				}
		}
	return(false);
}

/**
* Handle menu key.
*/
bool CTK_cursesMenuClass::CTK_doMenuKey(char key,int menunum)
{
	for(int k=0;k<this->menuNames[menunum]->menuItem.size();k++)
		{
			if(key==this->menuNames[menunum]->menuItem[k]->menuKey)
				{
					this->menuItemNumber=k;
					return(true);
				}
		}
	return(false);
}

/**
* Set menu update callback.
*/
void CTK_cursesMenuClass::CTK_setUpdateCB(void (*update)(void *,void*),void* mainapp)
{
	this->updateCB=update;
}

/**
* Draw main menu bar.
*/
void CTK_cursesMenuClass::CTK_drawDefaultMenuBar(void)
{
	if(this->menuBarVisible==false)
		return;
	this->menuShowing=false;
	this->CTK_drawGadget();
}

/**
* Set whether menu bar is enabled.
*/
void CTK_cursesMenuClass::CTK_setMenuBarEnable(bool enable)
{
	this->enableMenuBar=enable;
}

/**
* Get whether menu bar is enabled.
*/
bool CTK_cursesMenuClass::CTK_getMenuBarEnable(void)
{
	return(this->enableMenuBar);
}

/**
* Set whether menu bar is visible.
* \note To disable shortcuts while menu bar hidden, disable bar as well.
*/
void CTK_cursesMenuClass::CTK_setMenuBarVisible(bool show)
{
	this->menuBarVisible=show;
}

/**
* Get whether menu bar is visible.
*/
bool CTK_cursesMenuClass::CTK_getMenuBarVisible(void)
{
	return(this->menuBarVisible);
}



