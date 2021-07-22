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

#include "cursesGlobals.h"

/**
*  Graphics class destroy.
*/
CTK_cursesGraphicsClass::~CTK_cursesGraphicsClass()
{
}

/**
* Graphics class.
*/
CTK_cursesGraphicsClass::CTK_cursesGraphicsClass(CTK_mainAppClass *mc)
{
	this->tabWidth=mc->tabWidth;
	this->mc=mc;
}


/**
* Set colour var
*/
int CTK_cursesGraphicsClass::CTK_getColourFromNamedVar(const char *varname,int defaultcolour)
{
	const char	*colournames[]={"black","red","green","yellow","blue","magenta","cyan","white",NULL};
	int			colouroffset=0;
	varsStruct	vsitem;
	char		*type=(char*)alloca(256);
	char		*colour=(char*)alloca(256);
	int			colournamecnt=0;
	int			basecol=-1;

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,varname);
	if(vsitem.vType!=BADTYPE)
		{
			std::string::size_type found = vsitem.charVar.find_last_of(";");
			std::string col=vsitem.charVar.substr(found+1);
			while(colournames[colournamecnt]!=NULL)
				{
					if(col.compare(colournames[colournamecnt])==0)
						{
							basecol=colournamecnt;
							if(strstr(varname,"fore")!=NULL)
								colouroffset=30;
							else
								colouroffset=40;
							if(strstr(vsitem.charVar.c_str(),"bold")!=NULL)
								colouroffset+=60;
							return(basecol+colouroffset);
							break;
						}
					colournamecnt++;
				}
		}
	return(defaultcolour);
}

/**
* Draw dialog window.
* \param int boxx=X pos of internal box. 
* \param int boxy=Y pos of internal box. 
* \param int boxw=Width of internal box. 
* \param int boxh=Height of internal box. 
* \param int shadow=Draw drop shadow ( default draw ).
*/
void CTK_cursesGraphicsClass::CTK_drawDialogWindow(void)
{
	pageStruct	page=this->mc->pages[this->mc->pageNumber];
	if(this->mc->menuBar!=NULL)
		this->mc->menuBar->CTK_setMenuBarVisible(false);

	MOVETO(1,1)
	setBothColours(FORE_BOLD_WHITE,BACK_BLUE);
	printf("%s",CLEARTOEOS);
	this->drawDialogBox(page.boxX,page.boxY,page.boxW,page.boxH);

	MOVETO(1,1);
	setBothColours(FORE_BOLD_WHITE,BACK_BLUE);
	if(page.windowName.length()>0)
		{
			printf("%s",page.windowName.c_str());
			SETALTCHARSET;
			MOVETO(1,2)
			for(int j=0;j<this->mc->maxCols;j++)
				printf(HBAR);
			SETNORMCHARSET;
		}

	if(page.dialogName.length()>0)
		{
			SETNORMCHARSET;
			MOVETO((page.boxWM)-(page.dialogName.length()/2)+1,page.boxY)
			setBothColours(FORE_BLACK,BACK_WHITE);
			printf("%s",page.dialogName.c_str());
		}
}

/**
* Draw a box a la dialog.
*/
void CTK_cursesGraphicsClass::drawDialogBox(int x,int y,int w,int h)
{
	int	topcol=FORE_BOLD_WHITE;
	int	botcol=FORE_BOLD_BLACK;
	int fillcol=BACK_WHITE;

	setBothColours(FORE_BOLD_WHITE,fillcol);

	for(int j=0;j<h;j++)
		{
			MOVETO(x,y+j);
			printf("%*s",w," ");
		}

	MOVETO(x,y);
	setBothColours(topcol,fillcol);
	SETALTCHARSET;
	printf("%s",TOPLEFT);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);

	setForeColour(botcol);
	printf("%s",TOPRITE);

	for(int j=1;j<h;j++)
		{
			setForeColour(topcol);
			MOVETO(x,y+j);
			printf(VBAR);
			setForeColour(botcol);
			MOVETO(x+w,y+j);
			printf(VBAR);
		}

	setForeColour(topcol);
	MOVETO(x,y+h);
	printf("%s",BOTTOMLEFT);
	setForeColour(botcol);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);
	printf("%s",BOTTOMRITE);

	setBackColour(BACK_BLACK);
	for(int j=1;j<h+2;j++)
		{
			MOVETO(x+w+1,y+j);
			printf("  ");
		}
	MOVETO(x+1,y+h+1);
	printf("%*s",w," ");
	SETNORMCHARSET;
	fflush(NULL);
}

/**
* Draw a box a la dialog.
*/
void CTK_cursesGraphicsClass::CTK_drawBox(int x,int y,int w,int h,int type,bool fill,bool shadow)
{
	int	topcol;
	int	botcol;
	int	foreblack=FORE_BLACK;
	int	forewhite=FORE_BOLD_WHITE;

	switch(type)
		{
			case PLAINBOX:
				topcol=foreblack;
				botcol=foreblack;
				break;
			case OUTBOX:
				topcol=forewhite;
				botcol=foreblack;
				break;
			case INBOX:
				topcol=foreblack;
				botcol=forewhite;
				break;
			case NOBOX:
				return;
				break;
		}

	setBothColours(topcol,this->mc->windowColours.backCol);

	if(fill==true)
		{
			for(int fy=0;fy<h;fy++)
				{
					MOVETO(x,y+fy);
					printf("%*s",w,"");
				}
		}

	SETALTCHARSET;
	MOVETO(x,y);
	printf("%s",TOPLEFT);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);
	setForeColour(botcol);
	printf("%s",TOPRITE);

	for(int j=1;j<h;j++)
		{
			setForeColour(topcol);
			MOVETO(x,y+j);
			printf(VBAR);
			setForeColour(botcol);
			MOVETO(x+w,y+j);
			printf(VBAR);
		}

	setForeColour(topcol);
	MOVETO(x,y+h);
	printf("%s",BOTTOMLEFT);
	setForeColour(botcol);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);
	printf("%s",BOTTOMRITE);

	if(shadow==true)
		{
			setBackColour(BACK_BLACK);
			for(int j=1;j<h+2;j++)
				{
					MOVETO(x+w+1,y+j);
					printf("  ");
				}
			MOVETO(x+1,y+h+1);
			printf("%*s",w," ");
		}
	SETNORMCHARSET;
}

/**
* private.
*/
void CTK_cursesGraphicsClass::detab(char *in,char *out,int maxlen,int sx)
{
	int	i=sx-1;
	int chr=0;

	while((*in!=0) && (chr<maxlen-1))
		{
			if(*in=='\e')
				{
					while(*in!='m')
						out[chr++]=*in++;
					out[chr++]=*in++;
				}

			if(*in=='\t')
				{
					in++;
					out[chr++]=' ';
					i++;
					while((i % this->tabWidth) && (chr<maxlen-1))
						{
							out[chr++]=' ';
							i++;
						}
				}
			else
				{
					out[chr++]=*in++;
					i++;
				}
		}
	out[chr]=0;
}

void CTK_cursesGraphicsClass::CTK_printJustLineColour(const char *line,int sx,int sy,int boxwidth,int just,int fg,int bg)
{
	int		slen=strlen(line);
	char	*buffer;
	char	*outp;
	char	*linecpy=strdup(line);
	int		maxlen=boxwidth;
	int		printablelen=0;

	maxlen=strlen(line)*this->tabWidth*boxwidth;
	buffer=(char*)calloc(1,maxlen+boxwidth+1);
	outp=(char*)calloc(1,maxlen+boxwidth+1);

	if(linecpy[strlen(linecpy)-1]=='\n')
		linecpy[strlen(linecpy)-1]=0;

	this->detab(linecpy,&outp[0],maxlen,sx);
	printablelen=strlen(outp);

	if(strchr(outp,'\e')!=NULL)
		{
			printablelen=0;
			for(int j=0;j<strlen(outp);j++)
				{
					if(outp[j]=='\e')
						{
							while(outp[j]!='m')
								j++;
						}
					else
						{
							printablelen++;
						}
				}
			slen=printablelen+1;
		}

	switch(just)
		{
			case LEFTJUSTIFY:
				if(printablelen>boxwidth)
					sprintf(buffer,"%.*s",boxwidth,outp);
				else
					sprintf(buffer,"%s%-*s",outp,boxwidth-printablelen,"");
				
				break;
			case CENTREJUSTIFY:
				sprintf(buffer,"%*s",maxlen," ");
				sprintf(&buffer[(boxwidth/2)-(slen/2)],"%-*s",(int)(boxwidth/2)+(slen/2),outp);//TODO//
				break;
			case RIGHTJUSTIFY:
				sprintf(buffer,"%*s",boxwidth,outp);
				break;
		}

	printf("\e[%i;%iH\e[0m\e[%i;%im%s\e[0m",sy,sx,fg,bg,buffer);
	free(linecpy);
	free(buffer);
	free(outp);
}
