/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:50:24 GMT 2019 keithdhedger@gmail.com

 * This file (cursesTextBox.cpp) is part of CursesToolKit.

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

#include "cursesTextBox.h"

CTK_cursesTextBoxClass::~CTK_cursesTextBoxClass()
{
}

CTK_cursesTextBoxClass::CTK_cursesTextBoxClass()
{
}

void CTK_cursesTextBoxClass::CTK_newBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->canSelect=selectable;

	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt);
}

void CTK_cursesTextBoxClass::CTK_updateText(const char *txt)
{
	char		*line=(char*)alloca(this->wid+1);
	int			xcnt=0;
	int			ycnt=0;
	std::string	str;

	line[0]=0;
	this->text=txt;
	this->txtstrings.clear();

	for(int j=0;j<this->text.length();j++)
		{
			if(this->text.c_str()[j]=='\n')
				{
					ycnt++;
					sprintf(&line[xcnt],"%c",0);
					str=line;
					this->txtstrings.push_back(str);
					xcnt=0;
					continue;
				}

			sprintf(&line[xcnt],"%c",this->text.c_str()[j]);
			xcnt++;

			if(xcnt>=this->wid)
				{
					ycnt++;
					sprintf(&line[xcnt],"%c",0);
					str=line;
					this->txtstrings.push_back(str);
					xcnt=0;
				}
		}

	str=line;
	if(xcnt>0)
		this->txtstrings.push_back(str);
}

void CTK_cursesTextBoxClass::CTK_drawBox(bool hilite)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

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

	MOVETO(this->sx,this->sy);
	for(int j=0;j<this->hite;j++)
		{
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());
		}

	while(j<this->hite)
		{
			if(j<this->txtstrings.size())
				{
					MOVETO(this->sx,this->sy+j);
					printf( "%s" ,this->txtstrings[j+this->startLine].c_str());
					j++;
				}
			else
				return;
		}
}

void CTK_cursesTextBoxClass::CTK_scrollLine(bool scrollup)
{
	this->scroll(scrollup,1);
}

void CTK_cursesTextBoxClass::CTK_scrollPage(bool scrollup)
{
	this->scroll(scrollup,this->hite);
}

void CTK_cursesTextBoxClass::scroll(bool scrollup,int numlines)
{
	if(scrollup==true)
		{
			this->startLine-=numlines;
			if(this->startLine<0)
				this->startLine=0;
		}
	else
		{
			this->startLine+=numlines;
			if(this->txtstrings.size()-this->startLine<this->hite)
				this->startLine=this->txtstrings.size()-this->hite;
		}
}

void CTK_cursesTextBoxClass::CTK_setSelectable(bool canselect)
{
	this->canSelect=canselect;
}

bool CTK_cursesTextBoxClass::CTK_getSelectable(void)
{
	return(this->canSelect);
}

const std::string CTK_cursesTextBoxClass::CTK_getText(void)
{
	return(this->text);
}

void CTK_cursesTextBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
//	this->foreCol=cs.foreCol;
//	this->backCol=cs.backCol;
//	this->hiliteForeCol=cs.hiliteForeCol;
//	this->hiliteBackCol=cs.hiliteBackCol;
//	this->use256=cs.use256Colours;
}





