/*
 *
 * ©K. D. Hedger. Thu 14 Mar 16:11:26 GMT 2019 keithdhedger@gmail.com

 * This file (cursesLists.cpp) is part of CursesLists.

 * CursesLists is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesLists is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesLists.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cursesListBox.h"

CTK_cursesListBoxClass::~CTK_cursesListBoxClass()
{
	this->CTK_clearList();
}

CTK_cursesListBoxClass::CTK_cursesListBoxClass()
{
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
	for(int j=0;j<this->hite;j++)
		{
			if(hilite==true)
				{
					setBackColour(this->hiliteBackCol);
					setForeColour(this->hiliteForeCol);
				}
			else
				{
					setBackColour(this->backCol);
					setForeColour(this->foreCol);
				}

			if((hilite==true) && (this->listItemNumber==(j+this->listStart)))
				{
					setBackColour(BACK_WHITE);
					setForeColour(FORE_BLACK);
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
	this->listItems.clear();
	this->listStart=0;
	this->listItemNumber=0;
}

void CTK_cursesListBoxClass::CTK_setSelectCB(void (*select)(void *))
{
	this->selectCB=select;
}

void CTK_cursesListBoxClass::CTK_newListBox(int x,int y,int width,int hite)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->blank.insert(this->blank.begin(),width,' ');
}

void CTK_cursesListBoxClass::CTK_keyUpDown(bool doup)
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
					if(this->listItemNumber>=this->hite)
						this->listStart++;
				}
			else
				this->listItemNumber--;
		}
}

//TODO page up/down
