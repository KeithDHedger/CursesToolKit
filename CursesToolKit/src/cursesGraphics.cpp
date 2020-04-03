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
* Set colours etc from user defined colours.
*/
void CTK_cursesGraphicsClass::setUserColours(coloursStruct *cs)
{
	const char	*names[]={
	"forecolour",
	"backcol",
	"hiliteforecol",
	"hilitebackcol",
	"markbackcol",
	"markforecol",
	"cursbackcol",
	"cursforecol",
	"menubackcol",
	"menuforecol",
	"menuhilitebackcol",
	"menuhiliteforecol",
	"linenumbackcol",
	"linenumforecol",
	"windowbackcol",
	"windowforecol",
	"buttonbackcol",
	"buttonforecol",
	"buttondisabledforecol",
	"disabledforecol",
	"dialogbackcol",
	"dialogforecol",
	"dialogboxbackcol",
	"dialogboxforecol",
	"dialogboxtlfore",
	"dialogboxbrfore",
	"dialogboxshadowbackcol",
	"boxtype",
	"textboxtype",
	"inputboxtype",
	"labelboxtype",
	"listboxtype",
	NULL};

	int			cnt=0;
	varsStruct	vsitem;
	int			*ptr;
	const char	*colournames[]={"black","red","green","yellow","blue","magenta","cyan","white",NULL};
	char		*type=(char*)alloca(256);
	char		*colour=(char*)alloca(256);
	int			colouroffset;
	int			colournamecnt=0;

	if(this->mc->gotUserColours==false)
		return;

	ptr=(int*)cs;
	while(names[cnt]!=NULL)
		{
			vsitem=this->mc->utils->CTK_findVar(this->mc->appColours,names[cnt]);
			if(vsitem.vType==INTVAR)
				{
					ptr[cnt]=vsitem.intVar;
				}
			if(vsitem.vType==CHARVAR)
				{
					colournamecnt=0;
					sscanf(vsitem.charVar.c_str(),"%[^;];%[^\n]",type,colour);
					while(colournames[colournamecnt]!=NULL)
						{
							if(strcmp(colournames[colournamecnt],colour)==0)
								{
									if(strstr(names[cnt],"fore")!=NULL)
										colouroffset=30;
									else
										colouroffset=40;
									if(strcmp(type,"bold")==0)
										colouroffset+=60;
									ptr[cnt]=colournamecnt+colouroffset;
									break;			
								}
							colournamecnt++;
						}
				}
			cnt++;
		}
}

/**
* Set colours etc from coloursStruct.
* \note if force==true override user colours.
* \note user colours are set 1st then overridden by cs.
* \note if force==false colours are set from cs then overridden by user colours.
*/
void CTK_cursesGraphicsClass::CTK_setColours(coloursStruct *srccs,coloursStruct *dstcs,bool force)
{
	if(force==true)
		{
			
			this->setUserColours(dstcs);
			memcpy(dstcs,srccs,sizeof(coloursStruct));
		}
	else
		{
			memcpy(dstcs,srccs,sizeof(coloursStruct));
			this->setUserColours(dstcs);
		}
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
		{
			this->mc->pages[this->mc->pageNumber].menuBarVisible=false;
			this->mc->menuBar->CTK_setMenuBarVisible(false);
		}
	MOVETO(1,1)
	setBothColours(this->mc->colours.dialogForeCol,this->mc->colours.dialogBackCol,false);
	printf("%s",CLEARTOEOS);
	this->drawDialogBox(page.boxX,page.boxY,page.boxW,page.boxH);

	MOVETO(1,1);
	setBothColours(this->mc->colours.dialogForeCol,this->mc->colours.dialogBackCol,false);
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
			setBothColours(this->mc->colours.dialogBoxForecol,this->mc->colours.dialogBoxBackcol,false);//TODO//
			printf("%s",page.dialogName.c_str());
		}
}

/**
* Draw a box a la dialog.
*/
void CTK_cursesGraphicsClass::drawDialogBox(int x,int y,int w,int h)
{
	int	topcol=this->mc->colours.dialogBoxTL;
	int	botcol=this->mc->colours.dialogBoxBR;
	int fillcol=this->mc->colours.dialogBoxBackcol;

	setBothColours(this->mc->colours.dialogBoxForecol,fillcol,false);

	for(int j=0;j<h;j++)
		{
			MOVETO(x,y+j);
			printf("%*s",w," ");
		}

	MOVETO(x,y);
	setBothColours(topcol,fillcol,false);
	SETALTCHARSET;
	printf("%s",TOPLEFT);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);

	setForeColour(botcol,false);
	printf("%s",TOPRITE);

	for(int j=1;j<h;j++)
		{
			setForeColour(topcol,false);
			MOVETO(x,y+j);
			printf(VBAR);
			setForeColour(botcol,false);
			MOVETO(x+w,y+j);
			printf(VBAR);
		}

	setForeColour(topcol,false);
	MOVETO(x,y+h);
	printf("%s",BOTTOMLEFT);
	setForeColour(botcol,false);
	for(int j=1;j<w;j++)
		printf("%s",HBAR);
	printf("%s",BOTTOMRITE);

	setBackColour(this->mc->colours.dialogBoxShadowCol,false);
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

	if(this->mc->colours.use256Colours==true)
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

	setBothColours(topcol,this->mc->colours.backCol,this->mc->colours.use256Colours);

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
	setForeColour(botcol,this->mc->colours.use256Colours);
	printf("%s",TOPRITE);

	for(int j=1;j<h;j++)
		{
			setForeColour(topcol,this->mc->colours.use256Colours);
			MOVETO(x,y+j);
			printf(VBAR);
			setForeColour(botcol,this->mc->colours.use256Colours);
			MOVETO(x+w,y+j);
			printf(VBAR);
		}

	setForeColour(topcol,this->mc->colours.use256Colours);
	MOVETO(x,y+h);
	printf("%s",BOTTOMLEFT);
	setForeColour(botcol,this->mc->colours.use256Colours);
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



void CTK_cursesGraphicsClass::CTK_printJustLineColour(const char *line,int sx,int sy,int boxwidth,int just,int fg,int bg)
{
	int		slen=strlen(line);
	char	*buffer;
	char	*outp;
	char	*linecpy=strdup(line);
	int		maxlen=boxwidth;
	int		printablelen=0;

//TODO//
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
////				fprintf(stderr,"%s\e[0m\e[%i;%im%s",NORMAL,this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->txtStrings[j+this->startLine].c_str());
	switch(just)
		{
			case LEFTJUSTIFY:
				if(printablelen>boxwidth)
					sprintf(buffer,NORMAL NORMCHARSET "\e[%i;%im%.*s\e[0m",fg,bg,boxwidth,outp);
				else
					sprintf(buffer,NORMAL NORMCHARSET "\e[%i;%im%s%-*s" NORMAL,fg,bg,outp,boxwidth-printablelen,"");
				
				break;
			case CENTREJUSTIFY:
				sprintf(buffer,NORMAL NORMCHARSET "%*s" NORMAL,maxlen," ");
				sprintf(&buffer[(boxwidth/2)-(slen/2)],"%-*s",(int)(boxwidth/2)+(slen/2)-1,outp);
				break;
			case RIGHTJUSTIFY:
				sprintf(buffer,"%*s",boxwidth,outp);
				break;
		}

	printf("\e[%i;%iH%s",sy,sx,buffer);
	free(linecpy);
	free(buffer);
	free(outp);
}





/**
* Print a single line, justified.
* \param const char *line to print.
* \param int sx,sy print at.
* \param int max width of line.
* \param int justification ( default=LEFTJUSTIFY ).
*/
#if 1
void CTK_cursesGraphicsClass::CTK_printJustLine(const char *line,int sx,int sy,int boxwidth,int just)
{
	int		slen=strlen(line);
	char	*buffer;
	char	*outp;
	char	*linecpy=strdup(line);
	int		maxlen=boxwidth;
	int		printablelen=0;

//TODO//
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
				sprintf(&buffer[(boxwidth/2)-(slen/2)],"%-*s",(int)(boxwidth/2)+(slen/2)-1,outp);
				break;
			case RIGHTJUSTIFY:
				sprintf(buffer,"%*s",boxwidth,outp);
				break;
		}

	printf("\e[%i;%iH%s",sy,sx,buffer);
	free(linecpy);
	free(buffer);
	free(outp);
}

#else
void CTK_cursesGraphicsClass::CTK_printJustLine(const char *line,int sx,int sy,int boxwidth,int just)
{
	int		slen=strlen(line);
	char	*buffer;
	char	*outp;
	char	*linecpy=strdup(line);
	int		maxlen=boxwidth;
	int		printablelen=0;

fprintf(stderr,"395\n");
//TODO//
	maxlen=strlen(line)*this->tabWidth*boxwidth;
	buffer=(char*)calloc(1,maxlen+boxwidth+1);
	outp=(char*)calloc(1,maxlen+boxwidth+1);

	if(linecpy[strlen(linecpy)-1]=='\n')
		linecpy[strlen(linecpy)-1]=0;

	this->detab(linecpy,&outp[0],maxlen,sx);
	printablelen=strlen(outp);

	if(strchr(outp,'\e')!=NULL)
		{
		fprintf(stderr,"got e\n");
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

//	switch(just)
//		{
//			case LEFTJUSTIFY:
//				if(printablelen>boxwidth)
					sprintf(buffer,"%.*s",boxwidth,outp);
//				else
//					sprintf(buffer,"%s%-*s",outp,boxwidth-printablelen,"");
//				
//				break;
//			case CENTREJUSTIFY:
//				sprintf(buffer,"%*s",maxlen," ");
//				sprintf(&buffer[(boxwidth/2)-(slen/2)],"%-*s",(int)(boxwidth/2)+(slen/2)-1,outp);
//				break;
//			case RIGHTJUSTIFY:
//				sprintf(buffer,"%*s",boxwidth,outp);
//				break;
//		}

//	printf("\e[%i;%iH%.*s",sy,sx,boxwidth,"X");
	if(printablelen<boxwidth)
		printf("\e[%i;%iH%s",sy,sx,buffer);
	else
	{
		printf(buffer,"%s%-*s",outp,boxwidth-printablelen,"");
//		printf(buffer,"%.*s",outp,boxwidth-printablelen);
}
	free(linecpy);
	free(buffer);
	free(outp);
}
#endif
