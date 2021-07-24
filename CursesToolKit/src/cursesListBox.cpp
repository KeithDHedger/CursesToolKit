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
	varsStruct	vsitem;

	this->CTK_setCommon(mc);

	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("listforecol",this->mc->windowColours.foreCol);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("listbackcol",this->mc->windowColours.backCol);

	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("listhiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("listhilitebackcol",this->gadgetColours.hiliteBackCol);
	this->gadgetColours.disabledForeCol=this->mc->gc->CTK_getColourFromNamedVar("listdisabledforecol",this->gadgetColours.disabledForeCol);
	this->gadgetColours.disabledBackCol=this->mc->gc->CTK_getColourFromNamedVar("listdisabledbackcol",this->gadgetColours.disabledBackCol);
	this->gadgetColours.selectedForeCol=this->mc->gc->CTK_getColourFromNamedVar("listselectedforecol",this->gadgetColours.selectedForeCol);
	this->gadgetColours.selectedBackCol=this->mc->gc->CTK_getColourFromNamedVar("listselectedbackcol",this->gadgetColours.selectedBackCol);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"listfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"listboxtype");
	if(vsitem.vType==INTVAR)
		this->gadgetColours.boxType=vsitem.intVar;

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
	int		bc;
	int		fc;

	if(	this->gadgetDirty==false)
		return;

	this->hiLited=hilite;

	if(this->gadgetColours.useFancy==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->gadgetColours.boxType,false);

	for(int j=0;j<this->hite;j++)
		{
			if(j<this->listItems.size())
				{

					if(this->multi==false)
						{
							if((hilite==true) && (this->listItemNumber==(j+this->listStart)))
								{
									this->gc->CTK_printJustLineColour(this->listItems[j+this->listStart]->label.c_str(),this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.hiliteForeCol,this->gadgetColours.hiliteBackCol);
								}
							else
								{
									if(this->activeItem==(j+this->listStart))
										{
											this->gc->CTK_printJustLineColour(this->listItems[j+this->listStart]->label.c_str(),this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.selectedForeCol,this->gadgetColours.selectedBackCol);
										}
									else
										this->gc->CTK_printJustLineColour(this->listItems[j+this->listStart]->label.c_str(),this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.foreCol,this->gadgetColours.backCol);
								}
						}
					else
						{
						
							bc=this->gadgetColours.backCol;
							fc=this->gadgetColours.foreCol;
							if(this->selections[j+this->listStart]==true)
								{
									bc=this->gadgetColours.selectedBackCol;
									fc=this->gadgetColours.selectedForeCol;
									selected='X';
								}
							else
								{
									selected=' ';
								}
							sprintf(buffer,"[%c] %s",selected,this->listItems[j+this->listStart]->label.c_str());
							
							if((hilite==true) && (this->listItemNumber==(j+this->listStart)))
								this->gc->CTK_printJustLineColour(buffer,this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.hiliteForeCol,this->gadgetColours.hiliteBackCol);
							else
								this->gc->CTK_printJustLineColour(buffer,this->sx,this->sy+j,this->wid,LEFTJUSTIFY,fc,bc);
						}
				}
			else
				{
					this->gc->CTK_printJustLineColour(this->blank.c_str(),this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.foreCol,this->gadgetColours.backCol);
				}

		}
	SETNORMCHARSET;
	MOVETO(1,1);
	fflush(NULL);
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
	this->gadgetDirty=true;
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
* Set select item ( multiple selections ).
* \param int item
* \param bool set/unset
*/
void CTK_cursesListBoxClass::CTK_setItem(int item,bool set)
{
	if(this->multi==true)
		{
			if(item<this->selections.size())
				{
					this->selections[item]=set;
					this->gadgetDirty=true;
				}
		}
	else
		{
			this->CTK_selectItem(item);
		}
}

/**
* Select list item.
* \param int item
* \note if item out of bounds selects item 0.
*/
void CTK_cursesListBoxClass::CTK_selectItem(int item)
{
	int	tonum=item;
	int	xtra=this->hite/2;
	int xxtra=0;

	if(item==0xdeadbeef)
		{
			this->listItemNumber=-1;
			this->activeItem=-1;
			return;
		}

	if(item<0)
		{
			tonum=0;
			xtra=0;
		}
	else
		{
			if(item>this->listItems.size()-1)
				tonum=this->listItems.size()-1;
		}

	if((tonum-this->listStart)<this->hite)
		xtra=0;

	xxtra=0;
	this->listItemNumber=0;
	this->listStart=0;

	for(int j=0;j<tonum;j++)
		this->CTK_keyUpDown(false,false);

	if(tonum<(this->listItems.size()-(this->hite/2)))
		{
			xtra=this->hite/2;
			if((this->listItems.size()-tonum)<xtra)
				xtra=this->listItems.size()-tonum;
		}
	else
		{
			xtra=this->listItems.size()-tonum;
			xxtra=-1;
		}

	for(int j=0;j<xtra;j++)
		this->CTK_keyUpDown(false,false);
	
	for(int j=0;j<xtra+xxtra;j++)
		this->CTK_keyUpDown(true,false);

	this->activeItem=tonum;
}

/**
* Find item by label.
* \param const std::string needle
* \return List item index or -1 if not found.
*/
int CTK_cursesListBoxClass::CTK_findByLabel(const std::string needle)
{
	for(int j=0;j<this->listItems.size();j++)
		if(needle.compare(this->listItems.at(j)->label)==0)
			return(j);
	return(-1);
}

/**
* Get multiple selection list.
*/
const std::vector<bool> CTK_cursesListBoxClass::CTK_getSelections(void)
{
	return(this->selections);
}

/**
* Get active item.
*/
int CTK_cursesListBoxClass::CTK_getActiveItem(void)
{
	return(this->activeItem);
}
