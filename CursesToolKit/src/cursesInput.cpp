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
	termkey_destroy(this->tk);
}

/**
* Input box class.
*/
CTK_cursesInputClass::CTK_cursesInputClass(CTK_mainAppClass *mc)
{
	this->tk=termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}
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
	if(this->gadgetDirty==false)
		return;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.inputBoxType,true);

	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
	else
		setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);

	MOVETO(this->sx,this->sy);
	printf("%s",this->blank.c_str());
	MOVETO(this->sx,this->sy);
	printf("%s",this->text.substr(this->startChar,this->wid).c_str());
	MOVETO(this->sx+this->curs,this->sy);
}

/**
* Event loop for input gadget.
*/
void CTK_cursesInputClass::CTK_doInput(void)
{
	bool			loop=true;
	TermKeyResult	ret;
	TermKeyKey		key;

	this->CTK_drawGadget(true);
	SETSHOWCURS;
	fflush(NULL);
	while(loop==true)
		{
			ret=termkey_waitkey(this->tk,&key);
			switch(key.type)
				{
					case TERMKEY_TYPE_KEYSYM:
						switch(key.code.sym)
							{
								case TERMKEY_SYM_ENTER:
									if(this->selectCB!=NULL)
										this->selectCB(this,this->selectCBUserData);
								case TERMKEY_SYM_TAB:
								case TERMKEY_SYM_ESCAPE:
									SETHIDECURS;
									fflush(NULL);
									return;
									break;
								case TERMKEY_SYM_DOWN:
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
											SETSHOWCURS;
											fflush(NULL);
											if(this->redrawAppWindow==true)
												this->mc->CTK_updateScreen(this->mc,(void*)2);
											else
												{
													this->mc->CTK_clearScreen();
													this->mc->CTK_updateScreen(this->mc,(void*)1);
												}
											break;
									}
									break;
								case TERMKEY_SYM_LEFT:
									this->curs--;
									if(this->curs<0)
										{
											this->curs=0;
											if(this->startChar>0)
												this->startChar--;
										}
									break;
								case TERMKEY_SYM_RIGHT:
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
								case TERMKEY_SYM_HOME:
									this->curs=0;
									this->startChar=0;
									break;
								case TERMKEY_SYM_END:
									if(this->text.length()>this->wid)
										{
											this->curs=this->wid;
											this->startChar=this->text.length()-this->wid;
										}
									else
										this->curs=this->text.length();
									break;
								case TERMKEY_SYM_BACKSPACE:
								case TERMKEY_SYM_DEL:
									this->curs--;
									if(this->curs<0)
										this->curs=0;
									else
										this->text.erase(this->startChar+this->curs,1);
									break;
								case  TERMKEY_SYM_DELETE:
									this->text.erase(this->startChar+this->curs,1);
									break;
							}
						MOVETO(this->sx+this->curs,this->sy);
						this->CTK_drawGadget(true);
						break;
	
					case TERMKEY_TYPE_UNICODE:
						this->text.insert(this->startChar+this->curs,1,key.code.codepoint);
						this->curs++;
						if(this->curs>this->wid)
							{
								this->curs=this->wid;
								this->startChar++;
							}
						this->CTK_drawGadget(true);
						MOVETO(this->sx+this->curs,this->sy);
						break;
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



