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

void CTK_cursesEditBoxClass::CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->canSelect=selectable;

	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt,isfilename);
}

#if 1
void CTK_cursesEditBoxClass::CTK_updateText(const char *txt,bool isfilename)
{
	char	*ptr=NULL;
	char	*txtcpy=NULL;
	char	*buffer=NULL;
	int		startchr=0;
	char	*cpybuf=(char*)alloca(this->wid+1);

	this->txtstrings.clear();
	if(isfilename==false)
		txtcpy=strdup(txt);
	else
		{
			FILE *f=fopen(txt,"rb");
			fseek(f,0,SEEK_END);
			long fsize=ftell(f);
			fseek(f,0,SEEK_SET);
			txtcpy=(char*)malloc(fsize+1);
			fread(txtcpy,1,fsize,f);
			fclose(f);
			txtcpy[fsize]=0;
		}

	ptr=strtok(txtcpy,"\n");
	while(ptr!=NULL)
		{
			int numchars=0;
			int cnt=0;
			startchr=0;
			asprintf(&buffer,"%s",ptr);
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
				fprintf(stderr,"%s\n",cpybuf);
			}
			free(buffer);
			ptr=strtok(NULL,"\n");
		}
//		return;
//			while(strlen(buffer)-startchr>this->wid)
//				{
//					buffer[startchr+this->wid]=0;
//					//fprintf(stderr,">>%s<<\n",&buffer[startchr]);
//					this->txtstrings.push_back(&buffer[startchr]);
//					startchr+=this->wid;
//					sprintf(buffer,"%s",ptr);
//				}
//
//			if(startchr<strlen(buffer))
//				{
//					this->txtstrings.push_back(&buffer[startchr]);
//					//fprintf(stderr,">>%s<<\n",&buffer[startchr]);
//				}
//
//			free(buffer);
//			ptr=strtok(NULL,"\n");
//		}
}
#else
void CTK_cursesEditBoxClass::CTK_updateText(const char *txt,bool isfilename)
{
	char	*ptr=NULL;
	char	*txtcpy=NULL;
	char	*buffer=NULL;
	int		startchr=0;

	this->txtstrings.clear();
	if(isfilename==false)
		txtcpy=strdup(txt);
	else
		{
			FILE *f=fopen(txt,"rb");
			fseek(f,0,SEEK_END);
			long fsize=ftell(f);
			fseek(f,0,SEEK_SET);
			txtcpy=(char*)malloc(fsize+1);
			fread(txtcpy,1,fsize,f);
			fclose(f);
			txtcpy[fsize]=0;
		}

	ptr=strtok(txtcpy,"\n");
	while(ptr!=NULL)
		{
			startchr=0;
			asprintf(&buffer,"%s",ptr);
			while(strlen(buffer)-startchr>this->wid)
				{
					buffer[startchr+this->wid]=0;
					//fprintf(stderr,">>%s<<\n",&buffer[startchr]);
					this->txtstrings.push_back(&buffer[startchr]);
					startchr+=this->wid;
					sprintf(buffer,"%s",ptr);
				}

			if(startchr<strlen(buffer))
				{
					this->txtstrings.push_back(&buffer[startchr]);
					//fprintf(stderr,">>%s<<\n",&buffer[startchr]);
				}

			free(buffer);
			ptr=strtok(NULL,"\n");
		}
}
#endif

void CTK_cursesEditBoxClass::CTK_drawBox(bool hilite,bool showcursor)
{
	int	startchr=0;
	int j;
	int	linenum=0;
	int	boxline=0;

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

	for(int j=0;j<this->hite;j++)
		{
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());
		}

	while(boxline<this->hite)
		{
			MOVETO(this->sx,this->sy+boxline);
			printf( "%s" ,this->txtstrings[boxline+this->startLine].c_str());
			if((this->currentY==boxline+this->startLine) && (showcursor==true))
				{
					MOVETO(this->sx,this->sy+boxline);
					for(j=0;j<this->currentX;j++)
						{
							printf("%c",this->txtstrings[boxline+this->startLine].c_str()[j]);
						}
					if(this->txtstrings[boxline+this->startLine].c_str()[j]!='\t')
						printf( INVERSEON "%c" INVERSEOFF,this->txtstrings[boxline+this->startLine].c_str()[j]);
					else
						//printf( INVERSEON "\t\e[D " INVERSEOFF);
						printf( INVERSEON TABSPACE INVERSEOFF);
				}
			boxline++;
		}
	MOVETO(this->sx,this->sy+boxline);
}

void CTK_cursesEditBoxClass::CTK_doEditEvent(void)
{
	bool			loop=true;
	TermKeyResult	ret;
	TermKeyKey		key;
	TermKeyFormat	format=TERMKEY_FORMAT_VIM;
	char			buffer[32];

	this->CTK_drawBox(false,true);
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
								case TERMKEY_SYM_HOME:
									this->currentX=0;
									break;
								case TERMKEY_SYM_END:
									this->currentX=this->txtstrings[this->currentY].length()-1;
									break;

									case TERMKEY_SYM_UP:
										this->currentY--;
										if(currentY<this->startLine)
											this->startLine--;
										if(this->currentY<0)
											{
												this->currentY=0;
												this->startLine=0;
											}
										if(this->currentX>=this->txtstrings[this->currentY].length())
											this->currentX=this->txtstrings[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_DOWN:
										this->currentY++;
										if(this->currentY>=this->txtstrings.size())
											this->currentY=this->txtstrings.size()-1;
										if((this->currentY-this->startLine)>=this->hite)
											this->startLine++;
										if(this->currentX>=this->txtstrings[this->currentY].length())
											this->currentX=this->txtstrings[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_LEFT:
										this->currentX--;
										if(this->currentX<0)
											this->currentX=0;
										break;
									case TERMKEY_SYM_RIGHT:
										//if(this->txtstrings[currentY][this->currentX+1]=='\t')
										//	this->currentX+=8;
										//else
										this->currentX++;
										if(this->currentX>=this->txtstrings[currentY].length())
											this->currentX=this->txtstrings[currentY].length()-1;
										break;
								}
						}
				}
			this->CTK_drawBox(false,true);
		}
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
