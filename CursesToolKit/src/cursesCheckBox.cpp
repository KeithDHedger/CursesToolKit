/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:52:51 GMT 2019 keithdhedger@gmail.com

 * This file (cursesCheckBox.cpp) is part of CursesToolKit.

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

#include "cursesCheckBox.h"

CTK_cursesCheckBoxClass::~CTK_cursesCheckBoxClass()
{
}

CTK_cursesCheckBoxClass::CTK_cursesCheckBoxClass()
{
}

void CTK_cursesCheckBoxClass::CTK_drawCheckBox(bool hilite)
{
	char	val=' ';

	MOVETO(this->sx,this->sy);
	if(hilite==true)
		{
			setBackColour(this->colours.hiliteBackCol,this->colours.use256Colours);
			setForeColour(this->colours.hiliteForeCol,this->colours.use256Colours);
		}
	else
		{
//			setBackColour(this->colours.backCol,this->colours.use256Colours);
//			setForeColour(this->colours.foreCol,this->colours.use256Colours);
			setBackColour(BACK_BLUE,this->colours.use256Colours);
			setForeColour(FORE_WHITE,this->colours.use256Colours);
		}
	if(this->value==true)
		val='X';
	if(this->colours.fancyGadgets==false)
		printf("%c %s",val,this->label);
	else
		printf("[%c] %s",val,this->label);
}

void CTK_cursesCheckBoxClass::CTK_newCheckBox(int x,int y,int width,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=1;

	asprintf(&(this->label),"%s",label);
}

void CTK_cursesCheckBoxClass::CTK_setSelectCB(void (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

void CTK_cursesCheckBoxClass::CTK_setValue(bool val)
{
	this->value=val;
}

bool CTK_cursesCheckBoxClass::CTK_getValue(void)
{
	return(this->value);
}

void CTK_cursesCheckBoxClass::CTK_setEnterDeselects(bool deselect)
{
	this->enterDeselects=deselect;
}

bool CTK_cursesCheckBoxClass::CTK_getEnterDeselects(void)
{
	return(this->enterDeselects);
}

void CTK_cursesCheckBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}

