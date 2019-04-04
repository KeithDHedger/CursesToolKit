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
	termkey_destroy(this->tk);
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

	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt);
}

void CTK_cursesEditBoxClass::CTK_updateText(const char *txt)
{
	char	*ptr=NULL;
	char	*txtcpy=strdup(txt);

	this->txtstrings.clear();

	ptr=strtok(txtcpy,"\n");
	while(ptr!=NULL)
		{
			this->txtstrings.push_back(ptr);
			ptr=strtok(NULL,"\n");
		}
	free(txtcpy);
}
#if 0
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
#endif

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

int startchr=0;
int linenum=0;
int chrsused=0;
int boxline=0;
	while(boxline<this->hite)
		{
			chrsused=0;
			startchr=0;
			
			while(this->txtstrings[linenum+this->startLine].length()-chrsused>this->wid)
				{
					MOVETO(this->sx,this->sy+boxline);
					fprintf(stderr,"%s\n" ,this->txtstrings[linenum+this->startLine].substr(startchr,this->wid).c_str());
					printf( "%s" ,this->txtstrings[linenum+this->startLine].substr(startchr,this->wid).c_str());
					startchr+=this->wid;
					chrsused+=this->wid;
					boxline++;
					if(boxline>=this->hite)
						return;
				}
			
			if(chrsused<this->txtstrings[linenum+this->startLine].length())
				{
					MOVETO(this->sx,this->sy+boxline);
					printf( "%s" ,this->txtstrings[linenum+this->startLine].substr(startchr,this->wid).c_str());
					fprintf(stderr,">>%s<<\n" ,this->txtstrings[linenum+this->startLine].substr(startchr,this->wid).c_str());
					boxline++;
					if(boxline>=this->hite)
						return;
				}
			linenum++;
					//printf( "%s" ,this->txtstrings[j+this->startLine].c_str());
		}		
return;
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

void CTK_cursesEditBoxClass::CTK_doEditEvent(void)
{
	bool			loop=true;
	TermKeyResult	ret;
	TermKeyKey		key;
	TermKeyFormat	format=TERMKEY_FORMAT_VIM;
	char			buffer[32];

	this->CTK_drawBox(false);
	SETSHOWCURS;
	fflush(NULL);
	while(loop==true)
		{
			ret=termkey_waitkey(this->tk,&key);
			termkey_strfkey(this->tk,buffer,32,&key,format);
			switch(key.type)
				{
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_ESCAPE:
										loop=false;
										continue;
										break;
									case TERMKEY_SYM_UP:
										this->scrollLine(true);
										break;
									case TERMKEY_SYM_DOWN:
										this->scrollLine(false);
										break;
								}
						}
				}
			this->CTK_drawBox(false);
		}
	SETHIDECURS;
}

void CTK_cursesEditBoxClass::scrollLine(bool scrollup)
{
	this->scroll(scrollup,1);
}

void CTK_cursesEditBoxClass::scrollPage(bool scrollup)
{
	this->scroll(scrollup,this->hite);
}

void CTK_cursesEditBoxClass::scroll(bool scrollup,int numlines)
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
