/*
 *
 * ©K. D. Hedger. Fri 19 Apr 16:09:52 BST 2019 keithdhedger@gmail.com

 * This file (cursesLabel.cpp) is part of CursesToolKit.

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

CTK_cursesLabelClass::~CTK_cursesLabelClass()
{
	delete this->gc;
}

CTK_cursesLabelClass::CTK_cursesLabelClass()
{
	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
}

void CTK_cursesLabelClass::CTK_newLabel(int x,int y,int width,int hite,const char *txt)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;

	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt);
}

void CTK_cursesLabelClass::CTK_updateText(const char *txt)
{
	const char					*ptr=NULL;
	char						*buffer=NULL;
	int							startchr=0;
	char						*cpybuf=(char*)alloca(this->wid+1);
	std::vector<std::string>	array;
	std::string					str;
	CTK_cursesUtilsClass		cu;

	this->txtStrings.clear();
	this->text=txt;

	str=this->text;
	this->txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',this->wid-1,this->tabWidth);
}

void CTK_cursesLabelClass::CTK_drawLabel(void)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.labelBoxType,true);

	if(this->txtStrings.size()==0)
		return;

	setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);

	MOVETO(this->sx,this->sy);
	for(int j=0;j<this->hite;j++)
		{
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());
		}

	for(int j=0;j<this->hite;j++)
		{
			if(j<this->txtStrings.size())
				{
					MOVETO(this->sx,this->sy+j);
					printf("%s",this->txtStrings[j].c_str());
				}
			else
				return;
		}

	MOVETO(this->sx,this->sy+100);
	fflush(NULL);
}

void CTK_cursesLabelClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
	this->gc->CTK_setColours(this->colours);
}


