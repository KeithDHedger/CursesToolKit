/*
 *
 * ©K. D. Hedger. Mon 18 Mar 14:43:06 GMT 2019 keithdhedger@gmail.com

 * This file (cursesTextBox.cpp) is part of CursesMenus.

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

#if 0
#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cctype>
#include<sstream>

const std::string str="I need to write \nsomething that passes\n a value\n from! one program to another.\n  This is a test (with a 20 character word) - ottffssentettffssent - end test." ;

int main(int argc, char **argv)
{
	std::istringstream			stream(str);
	std::string					line;
	std::size_t					line_sz=30;
	std::vector<std::string>	result(1);
	std::string					word;
	std::istringstream			stm;

	while(std::getline(stream,line))
		{
			if(line.length()>line_sz)
				{
					stm.str(line);
					while(stm>>word)
						{
							if((result.back().size()+word.size())<=line_sz -1)
								result.back()+=word+' ';

							else
								{
									result.push_back(word+' '); // and place this new word on the next line
								}
						}
				}
			else
				result.push_back(line);
		}
	result.back().pop_back();
	std::cout<< " 12345678901234567890123456789012345678901234567890\n------------------------------------------------------------\n";
	for(int j=0; j<result.size(); j++)
		printf("\e[7m%s\e[27m\n",result[j].c_str());

}
#endif

#if 0
void cursesTextBoxClass::updateText(const char *txt)
{
	std::string			line;
	std::size_t			line_sz=30;
//	std::vector<std::string>	result(1);
	std::string			word;
	std::istringstream	stm;

	this->text=txt;
	std::istringstream	stream(this->text);
	this->txtstrings.clear();
	this->txtstrings.push_back("");

	while(std::getline(stream,line))
		{
							fprintf(stderr,">>%s<<\n",line.c_str());
		if(line.length()>line_sz)
				{
					stm.str(line);
					while(stm>>word)
						{
						//fprintf(stderr,">>%s<<\n",word.c_str());
							if((this->txtstrings.back().size()+word.size())<=line_sz -1)
								this->txtstrings.back()+=word+' ';

							else
								{
									this->txtstrings.push_back(word+' '); // and place this new word on the next line
								}
						}
				}
			else
				this->txtstrings.push_back(line);
		}
	this->txtstrings.back().pop_back();
	this->startLine=0;

}
#else
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
#endif
void CTK_cursesTextBoxClass::CTK_drawBox(bool hilite)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

	if(hilite==true)
		{
			SETFORECOL(BLACK);
			SETBACKCOL(CYAN);
		}
	else
		{
			SETFORECOL(WHITE);
			SETBACKCOL(BLACK);
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



