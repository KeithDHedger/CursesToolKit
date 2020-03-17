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
	this->CTK_setCommon(mc);
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

	if(this->items.size()==0)
		return;

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
								while((selection>-1) && (this->items[selection].enabled==false))
									selection--;
								if(selection<0)
									selection=0;
								break;

							case CTK_KEY_DOWN:
								selection++;
//skip disabled menu items
								while((selection<this->items.size()) && (this->items[selection].enabled==false))
									selection++;
								if(selection==this->items.size())
									selection=this->items.size()-1;
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
				setBothColours(this->colours.disabledForeCol,this->colours.menuBackCol,this->colours.use256Colours);
			else
				{
					if(selection==j)
						setBothColours(this->colours.menuHiliteForeCol,this->colours.menuHiliteBackCol,this->colours.use256Colours);
					else
						setBothColours(this->colours.menuForeCol,this->colours.menuBackCol,this->colours.use256Colours);
				}
			this->gc->CTK_printJustLine(this->items[j].label.c_str(),this->sx,iy+j,this->maxWidth);
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
void CTK_cursesDropClass::CTK_drawGadget(bool hilite)//TODO//
{
	std::string	str=this->label;

	if(this->gadgetDirty==false)
		return;

	MOVETO(this->sx,this->sy);
	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
	else
		setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);

	if(this->colours.fancyGadgets==true)
		str+=" >>";

	this->gc->CTK_printJustLine(str.c_str(),this->sx,sy,str.length());

	fflush(NULL);
}


