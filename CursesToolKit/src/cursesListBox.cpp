/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:52:24 GMT 2019 keithdhedger@gmail.com

 * This file (cursesListBox.cpp) is part of CursesToolKit.

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
* List class destroy.
*/
CTK_cursesListBoxClass::~CTK_cursesListBoxClass()
{
	this->CTK_clearList();
}

/**
* List class.
*/
CTK_cursesListBoxClass::CTK_cursesListBoxClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->type=LISTGADGET;
}

/**
* Add new item to list.
* \note Disposing of user data is up to the caller.
*/
void CTK_cursesListBoxClass::CTK_addListItem(const char *label,void *ud)
{
	listItemStruct	*ls=new listItemStruct;
	ls->label=label;
	ls->userData=ud;
	this->listItems.push_back(ls);
	this->gadgetDirty=true;
}

/**
* Draw list.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesListBoxClass::CTK_drawGadget(bool hilite)
{
	char	buffer[4096];
	char	selected;

	if(	this->gadgetDirty==false)
		return;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.listBoxType,false);

	for(int j=0;j<this->hite;j++)
		{
			if((hilite==true) && (this->listItemNumber==(j+this->listStart)))
				setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);//TODO//
			else
				setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);

			if(j<this->listItems.size())
				{
					if(this->multi==false)
						this->gc->CTK_printLine(this->listItems[j+this->listStart]->label.c_str(),this->sx,this->sy+j,this->wid);
					else
						{
							if(this->selections[j+this->listStart]==true)
								selected='X';
							else
								selected=' ';
							sprintf(buffer,"[%c] %s",selected,this->listItems[j+this->listStart]->label.c_str());
							this->gc->CTK_printLine(buffer,this->sx,this->sy+j,this->wid);
						}
				}
			else
				{
					this->gc->CTK_printLine(this->blank.c_str(),this->sx,this->sy+j,this->wid);
				}
		}
	MOVETO(this->sx,this->sy+this->hite);
	this->gadgetDirty=false;
}

/**
* Clear list.
* \note any user data set on item should be disposed of by the caller first.
*/
void CTK_cursesListBoxClass::CTK_clearList(void)
{
	for(int j=0;j<this->listItems.size();j++)
		{
			this->listItems[j]->label.clear();//TODO//
			delete this->listItems[j];
		}
	this->listItems.clear();
	this->listStart=0;
	this->listItemNumber=0;
	this->gadgetDirty=true;
}

/**
* New list.
*/
void CTK_cursesListBoxClass::CTK_newListBox(int x,int y,int width,int hite)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->blank.insert(this->blank.begin(),width,' ');
	this->gadgetDirty=true;
}

/**
* Move selection.
*/
void CTK_cursesListBoxClass::CTK_keyUpDown(bool doup,bool page)
{
	int	lines=1;

	if(page==true)
		lines=this->hite-1;

	for(int j=0;j<lines;j++)
		{
			if(doup==true)
				{
					this->listItemNumber--;
					if(this->listItemNumber<this->listStart)
						this->listStart--;
					if(this->listItemNumber<0)
						{
							this->listItemNumber=0;
							this->listStart=0;
						}
				}
			else
				{
					this->listItemNumber++;
					if(this->listItemNumber<this->listItems.size())
						{
							if((this->listItemNumber-this->listStart)>=this->hite)
								this->listStart++;
						}
					else
						this->listItemNumber--;
				}
		}
	this->gadgetDirty=true;
}

/**
* Select all items in list.
*/
void CTK_cursesListBoxClass::CTK_selectAll(void)
{
	for(int j=0;j<this->listItems.size();j++)
		this->selections[j]=true;
	this->gadgetDirty=true;
}

/**
* De-Select all items in list.
*/
void CTK_cursesListBoxClass::CTK_selectNone(void)
{
	for(int j=0;j<this->listItems.size();j++)
		this->selections[j]=false;
	this->gadgetDirty=true;
}

/**
* Set whether to allow multiple selections.
*/
void CTK_cursesListBoxClass::CTK_setMultipleSelect(bool multi)
{
	this->multi=multi;
	this->selections.clear();
	for(int j=0;j<this->listItems.size();j++)
		this->selections.push_back(false);
}

/**
* Get whether to allow multiple selections.
*/
bool CTK_cursesListBoxClass::CTK_getMultipleSelect(void)
{
	return(this->multi);
}

/**
* Toggle select item.
*/
void CTK_cursesListBoxClass::CTK_toggleItem(int item)
{
	this->selections[item]=!this->selections[item];
	this->gadgetDirty=true;
}

/**
* Set select item.
*/
void CTK_cursesListBoxClass::CTK_setItem(int item,bool set)
{
	this->selections[item]=set;
	this->gadgetDirty=true;
}

/**
* Get multiple selection list.
*/
const std::vector<bool> CTK_cursesListBoxClass::CTK_getSelections(void)
{
	return(this->selections);
}
