/*
 *
 * ©K. D. Hedger. Fri 15 Mar 11:39:15 GMT 2019 keithdhedger@gmail.com

 * This file (cursesLib.h) is part of CursesMenus.

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

#ifndef _CURSESLIB_
#define _CURSESLIB_

#define ESCCHAR 0x1b
#define CURSFORWARD "\e[1C"
#define TABCHAR		"\t"
#define NEXTSCRLINE "\n\e[8C"

#define	FOREBLACK "\e[30m"
#define FORERED "\e[31m"
#define FOREGREEN "\e[32m"
#define FOREYELLOW "\e[33m"
#define FOREBLUE "\e[34m"
#define FOREMAGENTA "\e[35m"
#define FORECYAN "\e[36m"
#define FOREWHITE "\e[37m"

#define	BACKBLACK "\e[40m"
#define BACKRED "\e[41m"
#define BACKGREEN "\e[42m"
#define BACKYELLOW "\e[43m"
#define BACKBLUE "\e[44m"
#define BACKMAGENTA "\e[45m"
#define BACKCYAN "\e[46m"
#define BACKWHITE "\e[47m"

//#define INVERTON "\e[7m"
//#define INVERTOFF "\e[27m"

#define BLACK "0"
#define RED "1"
#define GREEN "2"
#define YELLOW "3"
#define BLUE "4"
#define MAGENTA "5"
#define CYAN "6"
#define WHITE "7"
#define FORECOL(x) "\e[3" x "m"
#define BACKCOL(x) "\e[4" x "m"
#define SETFORECOL(x) printf(FORECOL(x));
#define SETBACKCOL(x) printf(BACKCOL(x));
#define	SETNORMAL printf("\e[39;49m\e[0m")

#define NORMAL "\e[0m"

#define UNDERSCOREON "\e[4m"
#define UNDERSCOREOFF "\e[24m"
#define INVERSEON "\e[7m"
#define INVERSEOFF "\e[27m"

#define SETINVERSEON printf(INVERSEON);
#define SETINVERSEOFF printf(INVERSEOFF);

#define FORECOL256(col) "\e[38;5;" #col "m"
#define BACKCOL256(col) "\e[48;5;" #col "m"

#define CLEARTOEOL "\e[0K"
#define CLEARTOSOL "\e[1K"
#define CLEARTOEOS "\e[0J"

#define GETCURSPOS	"\e[6n"

#define SETHIDECURS printf("\e[?25l")
#define SETSHOWCURS printf("\e[?25h")

#define MOVETO(x,y) printf("\e[%i;%iH",y,x);fflush(NULL);

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

enum FORECOL8 {NBLACK=30,NRED,NGREEN,NYELLOW,NBLUE,NMAGENTA,NCYAN,NWHITE,BBLACK=90,BRED,BGREEN,BYELLOW,BBLUE,BMAGENTA,BCYAN,BWHITE};
enum BACKCOL8 {NBBLACK=40,NBRED,NBGREEN,NBYELLOW,NBBLUE,NBMAGENTA,NBCYAN,NBWHITE,BBBLACK=100,BBRED,BBGREEN,BBYELLOW,BBBLUE,BBMAGENTA,BBCYAN,BBWHITE};

static void setForeColour(int fc)
{
	printf("\e[%im",fc);
}

static void setBackColour(int fc)
{
	printf("\e[%im",fc);
}


#endif

