/*
 *
 * ©K. D. Hedger. Tue 27 Aug 11:22:14 BST 2019 keithdhedger@gmail.com

 * This file (cursesGadget.cpp) is part of CursesToolKit.

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
* Gadget destructor.
* \note Should not be called by user, gadgets are destroyed on window close.
*/
CTK_cursesGadgetClass::~CTK_cursesGadgetClass()
{
}

CTK_cursesGadgetClass::CTK_cursesGadgetClass()
{
}

const gadgetType CTK_cursesGadgetClass::CTK_getGadgetType(void)
{
	return(this->type);
}

//virtual funcs and data
/**
* Draw gadget.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesGadgetClass::CTK_drawGadget(bool hilite)
{
}

/**
* Set gadget 'pressed' callback.
*/
void CTK_cursesGadgetClass::CTK_setSelectCB(void (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

/**
* get gadget CB user data.
*/
const void *CTK_cursesGadgetClass::CTK_getCBUserData(void)
{
	return(this->selectCBUserData);
}

/**
* Set gadget colours etc.
*/
void CTK_cursesGadgetClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}

