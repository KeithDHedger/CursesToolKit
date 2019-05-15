/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:49:29 GMT 2019 keithdhedger@gmail.com

 * This file (cursesLib.h) is part of CursesToolKit.

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

#ifndef _CURSESLIB_
#define _CURSESLIB_

#define ESCCHAR 0x1b
#define CURSFORWARD "\e[1C"
#define TABCHAR		"\t"
#define SPACETAB	" \t"
#define SPACENL	" "
#define INVSPACE 	INVERSEON " " INVERSEOFF
#define NEXTSCRLINE "\n\e[8C"

#define	SETNORMAL printf("\e[0m")
#define NORMAL "\e[0m"

#define UNDERSCOREON "\e[4m"
#define UNDERSCOREOFF "\e[24m"
#define INVERSEON "\e[7m"
#define INVERSEOFF "\e[27m"
#define BRIGHTON "\e[1m"
#define BRIGHTOFF "\e[21m"

#define SETHIDECURS printf("\e[?25l")
#define SETSHOWCURS printf("\e[?25h")

#define CLEARTOEOL "\e[0K"
#define CLEARTOSOL "\e[1K"
#define CLEARTOEOS "\e[0J"

#define MOVETO(x,y) printf("\e[%i;%iH",y,x);fflush(NULL);

#define ALTCHARSET "\e(0"
#define NORMCHARSET "\e(B"
#define SETALTCHARSET printf("\e(0")
#define SETNORMCHARSET printf("\e(B")
#define TOPLEFT "l"
#define TOPRITE "k"
#define BOTTOMLEFT "m"
#define BOTTOMRITE "j"
#define VBAR "x"
#define HBAR "q"
#define TDOWN "w"
#define TUP "v"
#define TLEFT "u"
#define TRIGHT "t"
#define CROSS "n"

//ui
#define BRAKE -1
#define MENURITE -2
#define MENULEFT -3
#define MENUREFRESH -4
#define CONT -5
#define SELECTED -6

//keys
//#define KEYUP 'A'
//#define KEYDOWN 'B'
//#define KEYRITE 'C'
//#define KEYLEFT 'D'
//#define PAGEUP '5'
//#define PAGEDOWN '6'
//#define BACKSPACE 0x7f
//#define DELETEKEY '3'
//#define RETURNKEY 0xa
//#define TABKEY 0x9
//#define CURSHOME 'H'
//#define CURSEND 'F'
//#define CURSHOMECONS '1'
//#define CURSENDCONS '4'

enum FORECOL8 {FORE_BLACK=30,FORE_RED,FORE_GREEN,FORE_YELLOW,FORE_BLUE,FORE_MAGENTA,FORE_CYAN,FORE_WHITE,FORE_BOLD_BLACK=90,FORE_BOLD_RED,FORE_BOLD_GREEN,FORE_BOLD_YELLOW,FORE_BOLD_BLUE,FORE_BOLD_MAGENTA,FORE_BOLD_CYAN,FORE_BOLD_WHITE};
enum BACKCOL8 {BACK_BLACK=40,BACK_RED,BACK_GREEN,BACK_YELLOW,BACK_BLUE,BACK_MAGENTA,BACK_CYAN,BACK_WHITE,BACK_BOLD_BLACK=100,BACK_BOLD_RED,BACK_BOLD_GREEN,BACK_BOLD_YELLOW,BACK_BOLD_BLUE,BACK_BOLD_MAGENTA,BACK_BOLD_CYAN,BACK_BOLD_WHITE};

enum {PLAINBOX=0,OUTBOX,INBOX,NOBOX};

struct coloursStruct
{
	int		foreCol=FORE_BLACK;
	int		backCol=BACK_WHITE;
	int		hiliteForeCol=FORE_BLACK;
	int		hiliteBackCol=BACK_CYAN;
	int		cursBackCol=BACK_GREEN;
	int		cursForeCol=FORE_BLACK;
	int		menuBackCol=BACK_GREEN;
	int		menuForeCol=FORE_BLACK;
	int		lineNumBackCol=BACK_BLACK;
	int		lineNumForeCol=FORE_YELLOW;
	int		windowBackCol=BACK_BLACK;
	int		windowForeCol=FORE_WHITE;
	int		buttonBackCol=BACK_BLUE;
	int		buttonForeCol=FORE_WHITE;
	int		boxType=PLAINBOX;
	int		textBoxType=OUTBOX;
	int		inputBoxType=INBOX;
	int		labelBoxType=PLAINBOX;
	int		listBoxType=INBOX;
	bool	use256Colours=false;
	bool	fancyGadgets=false;
};

static inline void setForeColour(int fc,bool use256=false)
{
	if(use256==false)
		printf("\e[%im",fc);
	else
		printf("\e[38;5;%im",fc);
	fflush(NULL);
}

static inline void setBackColour(int fc,bool use256=false)
{
	if(use256==false)
		printf("\e[%im",fc);
	else
		printf("\e[48;5;%im",fc);
	fflush(NULL);
}

static inline void setBothColours(int fc,int bc,bool use256=false)
{
	if(use256==false)
		printf("\e[%i;%im",fc,bc);
	else
		printf("\e[38;5;%i;48;5;%im",fc,bc);
	fflush(NULL);
}

static inline void freeAndNull(char **data)
{
	if((data!=NULL) && (*data!=NULL))
		{
			free(*data);
			*data=NULL;
		}
}

static inline bool willFitLine(std::string str,int tabwidth,int linelen)
{
	int	column=0;
	int	next_tab_column;

	for(int j=0;j<str.length();j++)
		{
			if(str[j]=='\t')
				{
					next_tab_column=column + (tabwidth-column % tabwidth);
					while(++column<next_tab_column);
				} 
			else
				column++;
	}

	if(column<linelen)
		return(true);
	return(false);
}

static inline int getColForXpos(std::string str,int tabwidth,int cx,int pad)
{
/*
	int maxcol=offset+width;

//	column=offset;
	for(unsigned int j=0;j<str.length();j++)
		{
			if(str[j]=='\t')
				column=((column-1+tabwidth)/tabwidth)*tabwidth;
			//else
			column++;			

			if(column>maxcol)

*/

	int	column=pad;

	for(unsigned int j=0;j<cx;j++)
		{
			if(str[j]=='\t')
				column=((column-1+tabwidth)/tabwidth)*tabwidth;

			column++;			
		}
	return(column);
#if 0
	int	column=pad;

	for(int j=0;j<cx;j++)
		{
			if(str[j]=='\t')
				column=(column+tabwidth/4);
			else
				column++;
		}
	return(column);
return -1;
#if 0
	int	column=pad;
	int	next_tab_column;

	for(int j=0;j<cx;j++)
		{
			if(str[j]=='\t')
				{
					next_tab_column=column + (tabwidth-column % tabwidth);
					while(++column<next_tab_column);
				} 
			//else
				column++;
	}

	return(column++);
#endif
#endif
}

#endif

