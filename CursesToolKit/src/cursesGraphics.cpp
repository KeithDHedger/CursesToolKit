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
CTK_cursesGraphicsClass::CTK_cursesGraphicsClass()
{
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
* Print a single line, blanking first.
*/
void CTK_cursesGraphicsClass::CTK_printLine(const char *line,const char *blnk,int sx,int sy,int boxwidth)
{
//	printf("\e[%i;%iH%*s\n\e[%i;%iH%s\n",sy,sx,boxwidth,blnk,sy,sx,line);
//	printf("\e[%i;%iH%*s\e[%iG%s\n",sy,sx,boxwidth,blnk,sx,line);
	printf("\e[%i;%iH%*s\e[%iG%s\n",sy,sx,boxwidth,blnk,sx,line);
}

/**
* Print a single line, remainder of line.
*/
void CTK_cursesGraphicsClass::CTK_printLinePostBlanks(const char *line,int sx,int sy,int boxwidth)
{
	char	*bline=strdup(line);
	if((strstr(bline,"e\[")==NULL) && (strstr(bline,"e\(")==NULL))
		printf("\e[%i;%iH%-*s\n",sy,sx,boxwidth,line);
//	fprintf(stderr,">>>%-*s<<<<\n",boxwidth,line);
//	else
//	printf("\e[%i;%iH%*s\n\e[%i;%iH%s\n",sy,sx,boxwidth,blnk,sy,sx,line);
//	printf("\e[%i;%iH%*s\e[%iG%s\n",sy,sx,boxwidth,blnk,sx,line);
//		printf("\e[%i;%iH%*s\e[%iG%s\n",sy,sx,boxwidth,blnk,sx,line);
	free(bline);
}

/**
* Print a single line, justified.
* \param const char *line to print.

*/
void CTK_cursesGraphicsClass::CTK_printJustLine(const char *line,int sx,int sy,int boxwidth,int just)
{
	char	*buffer=(char*)alloca(boxwidth+1);
	int		slen=strlen(line);
	int		virtlen=slen;
	char	*linecpy=strdup(line);

	if(strchr(linecpy,'\e')!=NULL)
		{
			virtlen=0;
			for(int j=0;j<slen;j++)
				{
					if(linecpy[j]=='\e')
						{
							while(linecpy[j]!='m')
								j++;
							j++;
						}
					else
						virtlen++;
				}
			slen=virtlen+1;
		}

	sprintf(buffer,"%*s",boxwidth," ");
	if(linecpy[strlen(linecpy)-1]=='\n')
		linecpy[strlen(linecpy)-1]=0;

	switch(just)
		{
			case LEFTJUSTIFY:
				sprintf(buffer,"%-*s",boxwidth,linecpy);
				break;
			case CENTREJUSTIFY:
				sprintf(&buffer[(boxwidth/2)-(slen/2)],"%-*s",(int)(boxwidth/2)+(slen/2),linecpy);
				break;
			case RIGHTJUSTIFY:
				sprintf(buffer,"%*s",boxwidth,linecpy);
				break;
		}


	printf("\e[%i;%iH%.*s",sy,sx,boxwidth,buffer);
	free(linecpy);
}

