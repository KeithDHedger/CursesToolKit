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

#include "cursesListBox.h"

CTK_cursesListBoxClass::~CTK_cursesListBoxClass()
{
	this->CTK_clearList();
	delete this->gc;
}

CTK_cursesListBoxClass::CTK_cursesListBoxClass()
{
	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
}

void CTK_cursesListBoxClass::CTK_addListItem(const char *label,void *ud)
{
	listItemStruct	*ls=new listItemStruct;
	ls->label=label;
	ls->userData=ud;
	this->listItems.push_back(ls);
}

void CTK_cursesListBoxClass::CTK_drawListWindow(bool hilite)
{
	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.listBoxType,true);

	for(int j=0;j<this->hite;j++)
		{
			if(hilite==true)
				{
					setBackColour(this->colours.hiliteBackCol,this->colours.use256Colours);
					setForeColour(this->colours.hiliteForeCol,this->colours.use256Colours);
				}
			else
				{
					setBackColour(this->colours.backCol,this->colours.use256Colours);
					setForeColour(this->colours.foreCol,this->colours.use256Colours);
				}

			if((hilite==true) && (this->listItemNumber==(j+this->listStart)))
				{
					setBackColour(BACK_WHITE,false);
					setForeColour(FORE_BLACK,false);
				}

			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());

			if(j<this->listItems.size())
				{
					MOVETO(this->sx,this->sy+j);
					printf("%s",this->listItems[j+this->listStart]->label.c_str());
				}
		}
	MOVETO(this->sx,this->sy+this->hite);
}

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
}

void CTK_cursesListBoxClass::CTK_setSelectCB(void (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

void CTK_cursesListBoxClass::CTK_newListBox(int x,int y,int width,int hite)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->blank.insert(this->blank.begin(),width,' ');
}

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

void CTK_cursesListBoxClass::CTK_setEnterDeselects(bool deselect)
{
	this->enterDeselects=deselect;
}

bool CTK_cursesListBoxClass::CTK_getEnterDeselects(void)
{
	return(this->enterDeselects);
}

void CTK_cursesListBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
	this->gc->CTK_setColours(this->colours);
}

