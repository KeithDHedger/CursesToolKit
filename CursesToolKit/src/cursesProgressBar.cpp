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
void CTK_cursesProgressBarClass::CTK_newBar(int x,int y,int width,int hite,const char *txt)
{
//	this->sx=x;
//	this->sy=y;
//	this->wid=width;
//	this->hite=hite;
//	this->centre=(this->wid/2)+this->sx;
//	this->gadgetDirty=true;
//	this->blank.insert(this->blank.begin(),width,' ');
//	this->CTK_updateText(txt);
}

/**
* Draw Progress Bar.
*/
void CTK_cursesProgressBarClass::CTK_drawGadget(bool hilite)
{
//	int xcnt=0;
//	int ycnt=0;
//	int j=0;
//
//	if(this->gadgetDirty==false)
//		return;
//
//	if(this->colours.fancyGadgets==true)
//		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.labelBoxType,true);
//
//	if(this->txtStrings.size()==0)
//		return;
//
//	for(int j=0;j<this->hite;j++)
//		{
//			if(j<this->txtStrings.size())
//				this->gc->CTK_printJustLineColour(this->txtStrings[j].c_str(),this->sx,this->sy+j,this->wid,this->justify,this->colours.foreCol,this->colours.backCol);
//		}
}


