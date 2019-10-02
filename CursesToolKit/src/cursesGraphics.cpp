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
}

/**
* Set colours etc from coloursStruct.
*/
void CTK_cursesGraphicsClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
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

	if(this->colours.use256Colours==true)
		{
			foreblack=0;
			forewhite=15;
		}

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

	setBothColours(topcol,this->colours.backCol,this->colours.use256Colours);

//FLICKER//TODO//
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
			printf("%*s",w," ");
		}
	SETNORMCHARSET;
	fflush(NULL);
}

/**
* Print a single line.
*/
void CTK_cursesGraphicsClass::CTK_printLine(const char *line,int sx,int sy,int boxwidth)
{
	//printf("\e[%i;%iH%*s\n",sy,sx,boxwidth,line);
	printf("\e[%i;%iH\e[%iX%.*s\n",sy,sx,boxwidth,boxwidth,line);
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

/**
* Print a single line, blanking first.
*/
void CTK_cursesGraphicsClass::CTK_printLine(const char *line,const char *blnk,int sx,int sy,int boxwidth)
{
//	printf("\e[%i;%iH%*s\n",sy,sx,boxwidth,buffer);
//	printf("\e[%i;%iH%*s\e[%iG%s\n",sy,sx,boxwidth,blnk,sx,line);
	printf("\e[%i;%iH%*s\e[%iG%s\n",sy,sx,boxwidth,blnk,sx,line);
}

/**
* Print a single line, justified.
* \param const char *line to print.
* \param int sx,sy print at.
* \param int max width of line.
* \param int justification ( default=LEFTJUSTIFY ).
*/
void CTK_cursesGraphicsClass::CTK_printJustLine(const char *line,int sx,int sy,int boxwidth,int just)
{
	int		slen=strlen(line);
	char	*buffer;
	char	*outp;
	char	*linecpy=strdup(line);
	int		maxlen=boxwidth;
	int		printablelen=0;

	maxlen=strlen(line)*this->tabWidth*boxwidth;
	buffer=(char*)calloc(1,maxlen+1);
	outp=(char*)calloc(1,maxlen+1);

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
				sprintf(&buffer[(boxwidth/2)-(slen/2)],"%-*s",(int)(boxwidth/2)+(slen/2)-1,outp);
				break;
			case RIGHTJUSTIFY:
				sprintf(buffer,"%*s",boxwidth,outp);
				break;
		}

	printf("\e[%i;%iH%s",sy,sx,buffer);
	free(linecpy);
}

