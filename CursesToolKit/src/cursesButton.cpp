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
CTK_cursesButtonClass::CTK_cursesButtonClass(CTK_mainAppClass *mc)
{
	varsStruct	vsitem;

	this->CTK_setCommon(mc);

	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("btnforecol",this->gadgetColours.foreCol);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("btnbackcol",this->gadgetColours.backCol);
	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("btnhiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("btnhilitebackcol",this->gadgetColours.hiliteBackCol);
	this->gadgetColours.disabledForeCol=this->mc->gc->CTK_getColourFromNamedVar("btndisabledforecol",this->gadgetColours.disabledForeCol);
	this->gadgetColours.disabledBackCol=this->mc->gc->CTK_getColourFromNamedVar("btndisabledbackcol",this->gadgetColours.disabledBackCol);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"btnfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;

	this->type=BUTTONGADGET;
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
	this->gadgetDirty=true;
}

//over ridden funcs
/**
* Draw button.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesButtonClass::CTK_drawGadget(bool hilite)
{
	varsStruct	vsitem;

	if(this->gadgetDirty==false)
		return;

	this->hiLited=hilite;

	MOVETO(this->sx,this->sy);
	if(this->enabled==false)
		{
			setBothColours(this->gadgetColours.disabledForeCol,this->gadgetColours.disabledBackCol);
		}
	else
		{
			if(hilite==true)
				setBothColours(this->gadgetColours.hiliteForeCol,this->gadgetColours.hiliteBackCol);
			else
				setBothColours(this->gadgetColours.foreCol,this->gadgetColours.backCol);
		}

	if(this->gadgetColours.useFancy==true)
		printf("< %s >",this->label);
	else
		printf("%s",this->label);

	fflush(NULL);
}
