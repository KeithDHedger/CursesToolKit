/*
 *
 * ©K. D. Hedger. Thu 27 Jun 20:34:15 BST 2019 keithdhedger@gmail.com

 * This file (cursesDropDown.cpp) is part of CursesToolKit.

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
* Menus class destroy.
*/
CTK_cursesDropClass::~CTK_cursesDropClass()
{
	this->items.clear();
}

/**
* Menus class constuctor.
*/
CTK_cursesDropClass::CTK_cursesDropClass(CTK_mainAppClass *mc)
{
	varsStruct	vsitem;

	this->CTK_setCommon(mc);

	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("dropforecol",this->gadgetColours.foreCol);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("dropbackcol",this->gadgetColours.backCol);
	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("drophiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("drophilitebackcol",this->gadgetColours.hiliteBackCol);

	this->gadgetColours.gadgetCustom1ForeCol=this->mc->gc->CTK_getColourFromNamedVar("dropmenuforecol",this->gadgetColours.gadgetCustom1ForeCol);
	this->gadgetColours.gadgetCustom1BackCol=this->mc->gc->CTK_getColourFromNamedVar("dropmenubackcol",this->gadgetColours.gadgetCustom1BackCol);
	this->gadgetColours.gadgetCustom2ForeCol=this->mc->gc->CTK_getColourFromNamedVar("dropmenuhiliteforecol",this->gadgetColours.gadgetCustom2ForeCol);
	this->gadgetColours.gadgetCustom2BackCol=this->mc->gc->CTK_getColourFromNamedVar("dropmenuhilitebackcol",this->gadgetColours.gadgetCustom2BackCol);


	this->gadgetColours.disabledForeCol=this->mc->gc->CTK_getColourFromNamedVar("dropdisabledforecol",this->gadgetColours.disabledForeCol);
	this->gadgetColours.disabledBackCol=this->mc->gc->CTK_getColourFromNamedVar("dropdisabledbackcol",this->gadgetColours.disabledBackCol);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"dropfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;

	this->type=DROPGADGET;
}

/**
* New drop button.
*/
void CTK_cursesDropClass::CTK_newDropButton(int x,int y,int width,int hite,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->label=label;
	this->holdLabel=label;
	this->gadgetDirty=true;
}

/**
* Add item to drop down.
*/
void CTK_cursesDropClass::CTK_addDropItem(const char *name)
{
	dropDownStruct dss;

	dss.label=name;
	if(strlen(name)>this->maxWidth)
		this->maxWidth=strlen(name);
	this->items.push_back(dss);
}

/**
* Handle drop down events.
*/
void CTK_cursesDropClass::CTK_doDropDownEvent(void)
{
	int		iy=this->sy+this->hite;
	bool	loop=true;
	int		selection=this->selectedItem;
	int		maxitems=this->items.size();

	if(maxitems==0)
		return;

	if(selection<0 || selection>maxitems)
		selection=0;

	this->drawList(selection);

	while(loop==true)
		{
			this->mc->readKey->CTK_getInput();
			if(this->mc->readKey->isHexString==true)
				{
					switch(this->mc->readKey->specialKeyName)
						{
							case CTK_KEY_ESC:
								loop=false;
								continue;
								break;

							case CTK_KEY_UP:
								selection--;
//skip disabled menu items
//TODO//
								while((selection>-1) && (this->items[selection].enabled==false))
									selection--;
								if(selection<0)
									selection=0;
								break;

							case CTK_KEY_DOWN:
								{
									int oldsel=selection;
									for(int j=selection;j<maxitems;j++)
										{
											if(this->items[j].enabled==false)
											selection++;
										}
									if(selection>maxitems || this->items[selection].enabled==false)
										{
											selection=oldsel;
											this->selectedItem=oldsel;
											this->drawList(selection);
										}
								}
								break;

							case CTK_KEY_HOME:
								selection=0;
								break;

							case CTK_KEY_END:
								selection=this->items.size()-1;
								break;

							case CTK_KEY_ENTER:
							case CTK_KEY_RETURN:
								if(selection>-1)
									this->selectedItem=selection;
								loop=false;
								continue;
								break;

							default:
								break;
						}
					}
			this->drawList(selection);
		}

	if(this->selectedItem>-1)
		this->label=this->items[this->selectedItem].label;
	this->mc->CTK_clearScreen();
	this->mc->CTK_updateScreen(this->mc,SCREENUPDATEALL);
	if(this->selectDeselects==true)
		this->hiLited=false;
}

void CTK_cursesDropClass::drawList(int selection)
{
	int	iy=this->sy+this->hite;

	for(int j=0;j<this->items.size();j++)
		{
			if(this->items[j].enabled==false)
				this->gc->CTK_printJustLineColour(this->items[j].label.c_str(),this->sx,iy+j,this->maxWidth,LEFTJUSTIFY,this->gadgetColours.disabledForeCol,this->gadgetColours.disabledBackCol);
			else
				{
					if(selection==j)
						this->gc->CTK_printJustLineColour(this->items[j].label.c_str(),this->sx,iy+j,this->maxWidth,LEFTJUSTIFY,this->gadgetColours.gadgetCustom1ForeCol,this->gadgetColours.gadgetCustom1BackCol);
					else
						this->gc->CTK_printJustLineColour(this->items[j].label.c_str(),this->sx,iy+j,this->maxWidth,LEFTJUSTIFY,this->gadgetColours.gadgetCustom2ForeCol,this->gadgetColours.gadgetCustom2BackCol);
				}
		}
	fflush(NULL);
}

/**
* Clear drop down list.
*/
void CTK_cursesDropClass::CTK_clearList(void)
{
	this->selectedItem=-1;
	this->items.clear();
	this->label=this->holdLabel;
	this->gadgetDirty=true;
}

void CTK_cursesDropClass::CTK_setItemEnabled(int item,bool enable)
{
	this->items[item].enabled=enable;
	this->gadgetDirty=true;
}

bool CTK_cursesDropClass::CTK_getItemEnabled(int item)
{
	return(this->items[item].enabled);
}

//over ridden funcs
/**
* Draw gadget.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesDropClass::CTK_drawGadget(bool hilite)
{
	std::string	str=this->label;

	if(this->gadgetDirty==false)
		return;

	this->hiLited=hilite;

	MOVETO(this->sx,this->sy);

	if(this->gadgetColours.useFancy==true)
		str+=" >>";

	if(hilite==true)
		this->gc->CTK_printJustLineColour(str.c_str(),this->sx,sy,str.length(),LEFTJUSTIFY,this->gadgetColours.hiliteForeCol,this->gadgetColours.hiliteBackCol);
	else
		this->gc->CTK_printJustLineColour(str.c_str(),this->sx,sy,str.length(),LEFTJUSTIFY,this->gadgetColours.foreCol,this->gadgetColours.backCol);
}


