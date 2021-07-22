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
	varsStruct	vsitem;

	this->CTK_setCommon(mc);

	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("checkforecol",this->gadgetColours.foreCol);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("checkbackcol",this->gadgetColours.backCol);
	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("checkhiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("checkhilitebackcol",this->gadgetColours.hiliteBackCol);
	this->gadgetColours.disabledForeCol=this->mc->gc->CTK_getColourFromNamedVar("checkdisabledforecol",this->gadgetColours.disabledForeCol);
	this->gadgetColours.disabledBackCol=this->mc->gc->CTK_getColourFromNamedVar("checkdisabledbackcol",this->gadgetColours.disabledBackCol);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"checkfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;


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

	this->hiLited=hilite;

	MOVETO(this->sx,this->sy);
	if(hilite==true)
		setBothColours(this->gadgetColours.hiliteForeCol,this->gadgetColours.hiliteBackCol);
	else
		setBothColours(this->gadgetColours.foreCol,this->gadgetColours.backCol);

	if(this->value==true)
		val='X';
	if(this->gadgetColours.useFancy==false)
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
	this->CTK_drawGadget(this->hiLited);
}

/**
*  Get checkbox value.
*/
bool CTK_cursesCheckBoxClass::CTK_getValue(void)
{
	return(this->value);
}
