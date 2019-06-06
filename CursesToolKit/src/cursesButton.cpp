/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:50:59 GMT 2019 keithdhedger@gmail.com

 * This file (cursesButton.cpp) is part of CursesToolKit.

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

#include "cursesButton.h"

/**
* Button class destroy.
*/
CTK_cursesButtonClass::~CTK_cursesButtonClass()
{
	CTK_freeAndNull(&this->label);
}

/**
* Button class.
*/
CTK_cursesButtonClass::CTK_cursesButtonClass()
{
}

/**
* Draw button.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesButtonClass::CTK_drawButton(bool hilite)
{
	MOVETO(this->sx,this->sy);
	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
	else
		setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);

	if(this->colours.fancyGadgets==true)
		printf("< %s >",this->label);
	else
		printf("%s",this->label);

	fflush(NULL);
}

/**
* New button.
*/
void CTK_cursesButtonClass::CTK_newButton(int x,int y,int width,int hite,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;

	asprintf(&(this->label),"%s",label);
}

/**
* Set button 'pressed' callback.
*/
void CTK_cursesButtonClass::CTK_setSelectCB(void (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

/**
*  Set whether to deselect gadget after 'pressing'.
*/
void CTK_cursesButtonClass::CTK_setEnterDeselects(bool deselect)
{
	this->enterDeselects=deselect;
}

/**
* Get whether to deselect gadget after 'pressing'.
*/
bool CTK_cursesButtonClass::CTK_getEnterDeselects(void)
{
	return(this->enterDeselects);
}

/**
* Set button colours etc.
*/
void CTK_cursesButtonClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}
