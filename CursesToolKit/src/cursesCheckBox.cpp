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

#include "cursesGlobals.h"

/**
*  \brief Checkbox class destroy.
*/
CTK_cursesCheckBoxClass::~CTK_cursesCheckBoxClass()
{
	CTK_freeAndNull(&this->label);
}

/**
*  \brief Checkbox class.
*/
CTK_cursesCheckBoxClass::CTK_cursesCheckBoxClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->type=CHECKGADGET;
}

/**
*  Draw checkbox.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesCheckBoxClass::CTK_drawGadget(bool hilite)
{

	char	val=' ';

	if(this->gadgetDirty==false)
		return;

	MOVETO(this->sx,this->sy);
	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
	else
		setBothColours(FORE_WHITE,BACK_BLUE,false);

	if(this->value==true)
		val='X';
	if(this->colours.fancyGadgets==false)
		printf("%c %s",val,this->label);
	else
		printf("[%c] %s",val,this->label);

	fflush(NULL);
}

/**
*  New checkbox.
*/
void CTK_cursesCheckBoxClass::CTK_newCheckBox(int x,int y,int width,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=1;
	this->gadgetDirty=true;
	asprintf(&(this->label),"%s",label);
}

/**
*  Set checkbox value.
*/
void CTK_cursesCheckBoxClass::CTK_setValue(bool val)
{
	this->value=val;
	this->gadgetDirty=true;
}

/**
*  Get checkbox value.
*/
bool CTK_cursesCheckBoxClass::CTK_getValue(void)
{
	return(this->value);
}
