/*
 *
 * ©K. D. Hedger. Fri 20 Sep 11:25:18 BST 2019 keithdhedger@gmail.com

 * This file (cursesKeyboard.cpp) is part of CursesToolKit.

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
* Keyboard box class destroy.
*/
CTK_cursesKeyboardClass::~CTK_cursesKeyboardClass()
{
}

/**
* Keyboard box class.
*/
CTK_cursesKeyboardClass::CTK_cursesKeyboardClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->type=OSKEYBOARD;
}

/**
* Private.
*/
void CTK_cursesKeyboardClass::updateText(const char *txt)
{
	this->gadgetDirty=true;
	this->text=txt;
	if(this->text.length()>this->wid)
		{
			this->startChar=this->text.length()-this->wid;
			this->curs=this->wid;
		}
	else
		{
			this->startChar=0;
			this->curs=this->text.length();
		}
	this->blank.clear();
	this->blank.insert(this->blank.begin(),this->wid,' ');
}

/**
* New Keyboard gadget.
*/
void CTK_cursesKeyboardClass::CTK_newKeyboard(int x,int y, int w,int h,const char *txt)
{
	this->sx=x;
	this->sy=y;
	this->wid=w;
	this->hite=h;
	this->gadgetDirty=true;
	this->updateText(txt);
}

/**
* Draw Keyboard gadget.
*/
void CTK_cursesKeyboardClass::CTK_drawGadget(bool hilite)
{
	if(this->gadgetDirty==false)
		return;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.inputBoxType,true);

	setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);

	MOVETO(this->sx,this->sy);
	printf("%s",this->text.substr(this->startChar,this->wid).c_str());

	MOVETO(this->sx,this->sy+1);
	setBothColours(FORE_BLACK,BACK_GREEN,this->colours.use256Colours);

	for(int j=0;j<4;j++)
		{
			MOVETO(this->sx,this->sy+j+1);
			printf("%s",osKLine[this->cPage][j]);
		}

	MOVETO(this->sx,this->sy+5);
	printf("%.8s\e(0%c%c\e(B",osKLine[this->cPage][4],0xab,0xac);
	
	MOVETO(this->sx+this->cx,this->sy+this->cy+1);
	setBothColours(FORE_GREEN,BACK_BLACK,this->colours.use256Colours);

	switch (this->cy)
		{
			case 4:
				switch (this->cx)
					{
						case 8:
							printf("\e(0%c\e(B",0xab);
							break;
						case 9:
							printf("\e(0%c\e(B",0xac);
							break;
						default:
							printf("%c",osKLine[this->cPage][this->cy][this->cx]);
							break;
					}
				break;
			default:
				printf("%c",osKLine[this->cPage][this->cy][this->cx]);
				break;
		}
	MOVETO(this->sx+this->curs,this->sy);
	fflush(NULL);
}

/**
* Event loop for Keyboard gadget.
*/
void CTK_cursesKeyboardClass::CTK_doInput(void)
{
	bool			loop=true;

	SETSHOWCURS;
	this->CTK_drawGadget(true);

	while(loop==true)
		{
			this->mc->readKey->CTK_getInput();
			if(this->mc->readKey->isHexString==true)
				{
					switch(this->mc->readKey->specialKeyName)
						{
							case CTK_KEY_ENTER:
							case CTK_KEY_RETURN:
								if(this->cy==4)
									{
										if(this->cx==8)
											{
												this->curs--;
												if(this->curs<0)
													this->curs=0;
												else
													this->text.erase(this->startChar+this->curs,1);
											}
										else if(this->cx==9)
											{
												loop=false;
											}
										else
											{
												switch(osKLine[this->cPage][this->cy][this->cx])
													{
														case '^':
															if(this->cPage==0)
																this->cPage=1;
															else
																this->cPage=0;
															break;
														case '<':
															this->curs--;
															if(this->curs<0)
																{
																	this->curs=0;
																	if(this->startChar>0)
																		this->startChar--;
																}
															break;
														
														case '>':
															if(1+this->curs+this->startChar>this->text.length())
																break;
															this->curs++;
															if(this->curs>this->wid)
																{
																	this->curs=this->wid;
																	if(this->text.length()-this->startChar>this->wid)
																		this->startChar++;
																}
															break;

														case ' ':
															this->text.insert(this->startChar+this->curs,1,' ');
															this->curs++;
															if(this->curs>this->wid)
																{
																	this->curs=this->wid;
																	this->startChar++;
																}
															break;
													}
											}
										break;
									}
								else
									{
										this->text.insert(this->startChar+this->curs,1,osKLine[this->cPage][this->cy][this->cx]);
									}

								this->curs++;
								if(this->curs>this->wid)
									{
										this->curs=this->wid;
										this->startChar++;
									}
								break;

							case CTK_KEY_UP:
								this->cy--;
								if(this->cy<0)
									this->cy=4;
								break;
							case CTK_KEY_DOWN:
								this->cy++;
								if(this->cy>4)
									this->cy=0;
								break;

							case CTK_KEY_LEFT:
								this->cx--;
								if(this->cx<0)
									this->cx=9;
								break;
							case CTK_KEY_RIGHT:
								this->cx++;
								if(this->cx>9)
									this->cx=0;
								break;
						}
					this->CTK_drawGadget(true);
				}
		}
	this->mc->CTK_clearScreen();
	this->mc->CTK_updateScreen(this->mc,NULL);
	fflush(NULL);

	SETHIDECURS;
	fflush(NULL);
}

/**
* Set text of gadget.
*/
void CTK_cursesKeyboardClass::CTK_setText(const char *txt)
{
	this->gadgetDirty=true;
	this->updateText(txt);
}

/**
* Get text of gadget.
*/
const char *CTK_cursesKeyboardClass::CTK_getText(void)
{
	return(this->text.c_str());
}



