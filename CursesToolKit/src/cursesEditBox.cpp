/*
 *
 * ©K. D. Hedger. Thu  4 Apr 12:38:45 BST 2019 keithdhedger@gmail.com

 * This file (cursesEditBox.cpp) is part of CursesToolKit.

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
 
#include "cursesEditBox.h"

CTK_cursesEditBoxClass::~CTK_cursesEditBoxClass()
{
	delete this->gc;
}

CTK_cursesEditBoxClass::CTK_cursesEditBoxClass()
{
	this->tk = termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}

	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
}

void CTK_cursesEditBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
	this->gc->CTK_setColours(this->colours);
}

void CTK_cursesEditBoxClass::CTK_newBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->canSelect=selectable;

	this->CTK_updateText(txt);
}

void CTK_cursesEditBoxClass::CTK_updateText(const char *txt)
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

void CTK_cursesEditBoxClass::CTK_drawBox(bool hilite)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.textBoxType,true);

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


