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

#include "cursesLabel.h"

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

	this->txtstrings.clear();
	this->text=txt;

	str=this->text;
	array=cu.CTK_cursesUtilsClass::CTK_explode(str,'\n');

	for(int j=0;j<array.size();j++)
		{
			ptr=array[j].c_str();
			int numchars=0;
			int cnt=0;
			startchr=0;
			asprintf(&buffer,"%s\n",ptr);
			while(cnt<strlen(buffer))
				{
					while(numchars<this->wid)
						{
							cpybuf[startchr]=buffer[cnt++];
							if(cpybuf[startchr]=='\t')
								numchars+=8;
							else
								numchars++;
							startchr++;
						}
					cpybuf[startchr]=0;
					this->txtstrings.push_back(cpybuf);
					startchr=0;
					numchars=0;
				}
			free(buffer);
		}
}

void CTK_cursesLabelClass::CTK_drawLabel(void)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

	if(this->txtstrings.size()==0)
		return;

	setBackColour(this->colours.backCol,this->colours.use256Colours);
	setForeColour(this->colours.foreCol,this->colours.use256Colours);

	MOVETO(this->sx,this->sy);
	for(int j=0;j<this->hite;j++)
		{
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());
		}

	for(int j=0;j<this->hite;j++)
		{
			if(j<this->txtstrings.size())
				{
					MOVETO(this->sx,this->sy+j);
					printf("%s",this->txtstrings[j].c_str());
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


