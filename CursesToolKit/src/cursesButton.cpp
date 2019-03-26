/*
 *
 * ©K. D. Hedger. Wed 20 Mar 16:54:15 GMT 2019 keithdhedger@gmail.com

 * This file (cursesButton.cpp) is part of CursesMenus.

 * CursesMenus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesMenus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesMenus.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cursesButton.h"

CTK_cursesButtonClass::~CTK_cursesButtonClass()
{
}

CTK_cursesButtonClass::CTK_cursesButtonClass()
{
}

void CTK_cursesButtonClass::CTK_drawButton(bool hilite)
{
	MOVETO(this->sx,this->sy);
	if(hilite==true)
		{
			//SETFORECOL(CYAN);
			//setForeColour(this->hiliteCol);
			setForeColour(this->hiliteCol);
			//SETBACKCOL(BLACK);
			setBackColour(this->backCol);
		}
	else
		{
			
			//SETFORECOL(RED);
			//SETBACKCOL(BLACK);
			setBackColour(this->backCol);
			setForeColour(this->forCol);
		}
	printf("\e[7m%s\e[27m",this->label);
}

void CTK_cursesButtonClass::CTK_newButton(int x,int y,int width,int hite,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;

	asprintf(&(this->label),"%s",label);
}

void CTK_cursesButtonClass::CTK_setSelectCB(void (*select)(void *))
{
	this->selectCB=select;
}

void CTK_cursesButtonClass::CTK_setColours(int forcol,int backcol,int hilite)
{
	this->forCol=forcol;
}
