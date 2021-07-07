/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:51:25 GMT 2019 keithdhedger@gmail.com

 * This file (cursesInput.cpp) is part of CursesToolKit.

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
* Input box class destroy.
*/
CTK_cursesInputClass::~CTK_cursesInputClass()
{
}

/**
* Input box class.
*/
CTK_cursesInputClass::CTK_cursesInputClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->type=INPUTGADGET;
}

/**
* Private.
*/
void CTK_cursesInputClass::updateText(const char *txt)
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
* New input gadget.
*/
void CTK_cursesInputClass::CTK_newInput(int x,int y, int w,int h,const char *txt)
{
	this->sx=x;
	this->sy=y;
	this->wid=w;
	this->hite=h;
	this->gadgetDirty=true;
	this->updateText(txt);
}

/**
* Draw input gadget.
*/
void CTK_cursesInputClass::CTK_drawGadget(bool hilite)
{
	int	fc;
	int	bc;

	if(this->gadgetDirty==false)
		return;

	this->hiLited=hilite;
	

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.inputBoxType,false);

	if(hilite==true)
		{
			fc=this->colours.hiliteForeCol;
			bc=this->colours.hiliteBackCol;
		}
	else
		{
			fc=this->colours.foreCol;
			bc=this->colours.backCol;
		}

	if(this->curs>=this->wid)//TODO//
		{
			while(this->curs>=this->wid)
				{
					this->startChar++;
					this->curs--;
				}

			this->gc->CTK_printJustLineColour(this->text.substr(this->startChar,this->wid).c_str(),this->sx,this->sy,this->wid,LEFTJUSTIFY,fc,bc);
			MOVETO(this->sx+this->curs,this->sy);
		}
	else
		{
			if(this->text.length()>0)
				{
					this->gc->CTK_printJustLineColour(this->text.substr(this->startChar,this->wid).c_str(),this->sx,this->sy,this->wid,LEFTJUSTIFY,fc,bc);
					MOVETO(this->sx+this->curs,this->sy);
				}
			else
				{
					this->gc->CTK_printJustLineColour(" ",this->sx,this->sy,this->wid,LEFTJUSTIFY,fc,bc);
					MOVETO(this->sx,this->sy);
				}
		}
//this->gadgetDirty=false;
//	fflush(NULL);
}

/**
* Event loop for input gadget.
*/
void CTK_cursesInputClass::CTK_doInput(void)
{
	bool			loop=true;

	this->CTK_drawGadget(true);
	SETSHOWCURS;
	fflush(NULL);
	while(loop==true)
		{
			this->mc->readKey->CTK_getInput();
			if(this->mc->readKey->isHexString==true)
				{
					switch(this->mc->readKey->specialKeyName)
						{
							case CTK_KEY_ENTER:
							case CTK_KEY_RETURN:
									if(this->selectCB!=NULL)
										this->selectCB(this,this->selectCBUserData);
								case CTK_KEY_TAB:
								case CTK_KEY_ESC:
									SETHIDECURS;
									fflush(NULL);
									return;
									break;
								case CTK_KEY_DOWN:
									{
										int syy=this->sy;
										CTK_cursesKeyboardClass	*oskb=new CTK_cursesKeyboardClass(this->mc);
										while(this->mc->maxRows-(syy+5)<1)
											syy--;
										oskb->CTK_newKeyboard(this->sx,syy,this->wid,6,this->CTK_getText());

										oskb->CTK_drawGadget(false);
										oskb->CTK_doInput();
										this->CTK_setText(oskb->CTK_getText());
										delete oskb;
										if(this->redrawAppWindow==true)
											this->mc->CTK_updateScreen(this->mc,SCREENUPDATEWINDOW);
										else
											{
												this->mc->CTK_clearScreen();
												this->mc->CTK_updateScreen(this->mc,SCREENUPDATEALL);
											}
										if(this->selectCB!=NULL)
											this->selectCB(this,this->selectCBUserData);
										SETHIDECURS;
										fflush(NULL);
										return;
										break;
									}
									break;
								case CTK_KEY_LEFT:
									this->curs--;
									if(this->curs<0)
										{
											this->curs=0;
											if(this->startChar>0)
												this->startChar--;
										}
									break;
								case CTK_KEY_RIGHT:
									if(1+this->curs+this->startChar>this->text.length())
										continue;
									this->curs++;
									if(this->curs>this->wid)
										{
											this->curs=this->wid;
											if(this->text.length()-this->startChar>this->wid)
												this->startChar++;
										}
									break;
								case CTK_KEY_HOME:
									this->curs=0;
									this->startChar=0;
									break;
								case CTK_KEY_END:
									if(this->text.length()>this->wid)
										{
											this->curs=this->wid;
											this->startChar=this->text.length()-this->wid;
										}
									else
										this->curs=this->text.length();
									break;
								case CTK_KEY_BACKSPACE:
									this->curs--;
									if(this->curs<0)
										this->curs=0;
									else
										this->text.erase(this->startChar+this->curs,1);
									break;
								case  CTK_KEY_DELETE:
									this->text.erase(this->startChar+this->curs,1);
									break;
						}
					MOVETO(this->sx+this->curs,this->sy);
					this->CTK_drawGadget(true);
					fflush(NULL);
				}
			else
				{
					this->text.insert(this->startChar+this->curs,this->mc->readKey->inputBuffer.c_str());
					this->curs+=this->mc->readKey->inputBuffer.length();
					if(this->curs>=this->wid)
						{
							this->curs=this->wid;
							this->startChar=this->text.length()-this->wid;
						}
					this->CTK_drawGadget(true);
					fflush(NULL);
				}
		}
	SETHIDECURS;
	fflush(NULL);
}

/**
* Set text of gadget.
*/
void CTK_cursesInputClass::CTK_setText(const char *txt)
{
	this->gadgetDirty=true;
	this->updateText(txt);
}

/**
* Get text of gadget.
*/
const char *CTK_cursesInputClass::CTK_getText(void)
{
	return(this->text.c_str());
}

