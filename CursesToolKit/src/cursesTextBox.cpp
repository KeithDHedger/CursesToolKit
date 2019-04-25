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
	delete this->gc;
}

CTK_cursesTextBoxClass::CTK_cursesTextBoxClass()
{
	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
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
	const char					*ptr=NULL;
	char						*buffer=NULL;
	int							startchr=0;
	char						*cpybuf=(char*)alloca(this->wid+1);
	std::vector<std::string>	array;
	std::string					str;
	CTK_cursesUtilsClass		cu;

	this->txtstrings.clear();
	this->text=txt;

	cpybuf[0]=0;
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
					while((numchars<this->wid) && (cnt<strlen(buffer)))
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

void CTK_cursesTextBoxClass::CTK_drawBox(bool hilite)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.textBoxType,false);

	if(this->txtstrings.size()==0)
		return;

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
//	for(int j=0;j<this->hite;j++)
//		{
//			MOVETO(this->sx,this->sy+j);
//			printf("%s",this->blank.c_str());
//		}

	while(j<this->hite)
		{
			if(j<this->txtstrings.size())
				{
					MOVETO(this->sx,this->sy+j);
//printf("%s%s",this->listItems[j+this->listStart]->label.c_str(),this->blank.substr(0,wid-this->listItems[j+this->listStart]->label.length()).c_str());
				//	printf("%s%s",this->txtstrings[j+this->startLine].c_str(),this->blank.substr(0,this->wid-this->txtstrings[j+this->startLine].length()-4).c_str());
					if(this->txtstrings[j+this->startLine].c_str()[this->txtstrings[j+this->startLine].length()-1]=='\n')
						printf("%s%s",this->txtstrings[j+this->startLine].substr(0,this->txtstrings[j+this->startLine].length()-1).c_str(),this->blank.substr(0,this->wid-this->txtstrings[j+this->startLine].length()+1).c_str());
					else
						printf("%s%s",this->txtstrings[j+this->startLine].c_str(),this->blank.substr(0,this->wid-this->txtstrings[j+this->startLine].length()).c_str());
					j++;
				}
			else
				return;
		}		
	MOVETO(this->sx,this->sy+100);
	fflush(NULL);
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
	this->gc->CTK_setColours(this->colours);
}


