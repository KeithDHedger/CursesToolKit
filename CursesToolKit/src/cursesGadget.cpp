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
	delete this->gc;
}

CTK_cursesGadgetClass::CTK_cursesGadgetClass()
{
}

void CTK_cursesGadgetClass::CTK_setCommon(CTK_mainAppClass *mc)
{
	this->mc=mc;
	this->gc=new CTK_cursesGraphicsClass(mc);
	this->CTK_setColours(&this->mc->colours);
	this->isSelectable=true;
	this->selectDeselects=true;
	this->enabled=true;
	this->selectKey=CTK_KEY_NONE;
	this->gadgetDirty=true;
	this->hiLited=false;
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
* Set gadget 'selected' callback.
*/
void CTK_cursesGadgetClass::CTK_setSelectCB(bool (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

/**
* Get gadget CB user data.
*/
void *CTK_cursesGadgetClass::CTK_getCBUserData(void)
{
	return(this->selectCBUserData);
}

/**
* Set gadget CB user data.
*/
void CTK_cursesGadgetClass::CTK_setCBUserData(void *userdata)
{
	this->selectCBUserData=userdata;
}

/**
* Set gadget colours etc.
*/
void CTK_cursesGadgetClass::CTK_setColours(coloursStruct *cs,bool force)
{
	this->gc->CTK_setColours(cs,&this->colours,force);
}

/**
*  Set whether to deselect gadget after 'activating'.
*/
void CTK_cursesGadgetClass::CTK_setSelectDeselects(bool deselect)
{
	this->selectDeselects=deselect;
}

/**
* Get whether to deselect gadget after 'activating'.
*/
bool CTK_cursesGadgetClass::CTK_getSelectDeselects(void)
{
	return(this->selectDeselects);
}

/**
*  Set gadget enabled.
*/
void CTK_cursesGadgetClass::CTK_setEnabled(bool enable)
{
	this->enabled=enable;
	this->isSelectable=enable;
	this->gadgetDirty=true;
}

/**
* Get gadget enabled.
*/
bool CTK_cursesGadgetClass::CTK_getEnabled(void)
{
	return(this->enabled);
}

/**
*  Set gadget selectable.
*/
void CTK_cursesGadgetClass::CTK_setSelectable(bool enable)
{
	this->isSelectable=enable;
}

/**
* Get gadget selectable.
*/
bool CTK_cursesGadgetClass::CTK_getSelectable(void)
{
	return(this->isSelectable);
}

/**
* Set select key, default=CTK_KEY_RETURN.
*/
void CTK_cursesGadgetClass::CTK_setSelectKey(int key)
{
	this->selectKey=key;
}

/**
* Get select key, default=CTK_KEY_RETURN.
*/
int CTK_cursesGadgetClass::CTK_getSelectKey(void)
{
	return(this->selectKey);
}

