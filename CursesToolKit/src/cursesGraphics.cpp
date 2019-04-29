/*
 *
 * ©K. D. Hedger. Mon  1 Apr 16:45:28 BST 2019 keithdhedger@gmail.com

 * This file (cursesGraphics.cpp) is part of CursesToolKit.

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

#include "cursesGraphics.h"

CTK_cursesGraphicsClass::~CTK_cursesGraphicsClass()
{
}

CTK_cursesGraphicsClass::CTK_cursesGraphicsClass()
{
}

void CTK_cursesGraphicsClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}

void CTK_cursesGraphicsClass::CTK_drawBox(int x,int y,int w,int h,int type,bool fill,bool shadow)
{
	int	topcol;
	int	botcol;

	switch(type)
		{
			case PLAINBOX:
				topcol=FORE_BLACK;
				botcol=FORE_BLACK;
				break;
			case OUTBOX:
				topcol=FORE_BOLD_WHITE;
				botcol=FORE_BLACK;
				break;
			case INBOX:
				topcol=FORE_BLACK;
				botcol=FORE_BOLD_WHITE;
				break;
			case NOBOX:
				return;
				break;
		}

	setBackColour(this->colours.backCol,this->colours.use256Colours);
	setForeColour(topcol,this->colours.use256Colours);
//FLICKER//
	if(fill==true)
		{
			for(int j=0;j<h;j++)
				{
					MOVETO(x,y+j);
					printf("%*s",w," ");
				}
		}

	SETALTCHARSET;
	MOVETO(x,y);
	printf("%s",TOPLEFT);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);
	setForeColour(botcol,this->colours.use256Colours);
	printf("%s",TOPRITE);

	for(int j=1;j<h;j++)
		{
			setForeColour(topcol,this->colours.use256Colours);
			MOVETO(x,y+j);
			printf(VBAR);
			setForeColour(botcol,this->colours.use256Colours);
			MOVETO(x+w,y+j);
			printf(VBAR);
		}

	setForeColour(topcol,this->colours.use256Colours);
	MOVETO(x,y+h);
	printf("%s",BOTTOMLEFT);
	setForeColour(botcol,this->colours.use256Colours);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);
	printf("%s",BOTTOMRITE);

	if(shadow==true)
		{
			setBackColour(BACK_BLACK,false);
			for(int j=1;j<h+2;j++)
				{
					MOVETO(x+w+1,y+j);
					printf("  ");
				}
			MOVETO(x+1,y+h+1);
			printf("%*s",w);
		}

	SETNORMCHARSET;
	fflush(NULL);
}





