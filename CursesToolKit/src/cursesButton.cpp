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

CTK_cursesButtonClass::~CTK_cursesButtonClass()
{
}

CTK_cursesButtonClass::CTK_cursesButtonClass()
{
}

void CTK_cursesButtonClass::CTK_drawButton(bool hilite)
{
	MOVETO(this->sx,this->sy);
	if(hilite==true)
		{
			setBackColour(this->colours.hiliteBackCol,this->colours.use256Colours);
			setForeColour(this->colours.hiliteForeCol,this->colours.use256Colours);
		}
	else
		{
			setBackColour(this->colours.backCol,this->colours.use256Colours);
			setForeColour(this->colours.foreCol,this->colours.use256Colours);
		}
	if(this->colours.fancyGadgets==true)
		printf("< %s >",this->label);
	else
		printf("%s",this->label);
}

void CTK_cursesButtonClass::CTK_newButton(int x,int y,int width,int hite,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;

	asprintf(&(this->label),"%s",label);
}

void CTK_cursesButtonClass::CTK_setSelectCB(void (*select)(void *))
{
	this->selectCB=select;
}

void CTK_cursesButtonClass::CTK_setEnterDeselects(bool deselect)
{
	this->enterDeselects=deselect;
}

bool CTK_cursesButtonClass::CTK_getEnterDeselects(void)
{
	return(this->enterDeselects);
}

void CTK_cursesButtonClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}
