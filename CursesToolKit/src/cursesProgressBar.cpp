/*
 *
 * ©K. D. Hedger. Fri 19 Apr 16:09:52 BST 2019 keithdhedger@gmail.com

 * This file (cursesLabel.cpp) is part of CursesToolKit.

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
* Progress Bar class destroy.
*/
CTK_cursesProgressBarClass::~CTK_cursesProgressBarClass()
{
}

/**
* Progress Bar class.
*/
CTK_cursesProgressBarClass::CTK_cursesProgressBarClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->isSelectable=false;
	this->type=BARGADGET;
}

/**
* New Progress Bar.
*/
void CTK_cursesProgressBarClass::CTK_newBar(int x,int y,int width,double min,double max, double val)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=1;
	this->gadgetDirty=true;
	this->maxvalue=max;
	this->minvalue=min;
	this->value=val;
}

/**
* Draw Progress Bar.
*/
void CTK_cursesProgressBarClass::CTK_drawGadget(bool hilite)
{
	double	absscale=(double)this->wid/this->maxvalue;
	double	abswid=absscale*this->value;

	setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);

	MOVETO(this->sx,this->sy);
	printf("%*s", this->wid,"");

	MOVETO(this->sx,this->sy);
	for(int j=0;j<abswid;j++)
		printf("#");
}

void CTK_cursesProgressBarClass::CTK_setValue(double val)
{
	this->value=val;
	if(this->value>this->maxvalue)
		this->value=this->maxvalue;
	if(this->value<this->minvalue)
		this->value=this->minvalue;
}

double CTK_cursesProgressBarClass::CTK_getValue(void)
{
	return(this->value);
}

void CTK_cursesProgressBarClass::CTK_setMinValue(double val)
{
	this->minvalue=val;
}

double CTK_cursesProgressBarClass::CTK_getMinValue(void)
{
	return(this->minvalue);
}

void CTK_cursesProgressBarClass::CTK_setMaxValue(double val)
{
	this->minvalue=val;
}

double CTK_cursesProgressBarClass::CTK_getMaxValue(void)
{
	return(this->minvalue);
}
