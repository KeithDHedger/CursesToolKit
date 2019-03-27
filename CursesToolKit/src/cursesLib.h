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

#define	SETNORMAL printf("\e[0m")

#define NORMAL "\e[0m"

#define UNDERSCOREON "\e[4m"
#define UNDERSCOREOFF "\e[24m"
#define INVERSEON "\e[7m"
#define INVERSEOFF "\e[27m"

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

enum FORECOL8 {FORE_BLACK=30,FORE_RED,FORE_GREEN,FORE_YELLOW,FORE_BLUE,FORE_MAGENTA,FORE_CYAN,FORE_WHITE,FORE_BOLD_BLACK=90,FORE_BOLD_RED,FORE_BOLD_GREEN,FORE_BOLD_YELLOW,FORE_BOLD_BLUE,FORE_BOLD_MAGENTA,FORE_BOLD_CYAN,FORE_BOLD_WHITE};
enum BACKCOL8 {BACK_BLACK=40,BACK_RED,BACK_GREEN,BACK_YELLOW,BACK_BLUE,BACK_MAGENTA,BACK_CYAN,BACK_WHITE,BACK_BOLD_BLACK=100,BACK_BOLD_RED,BACK_BOLD_GREEN,BACK_BOLD_YELLOW,BACK_BOLD_BLUE,BACK_BOLD_MAGENTA,BACK_BOLD_CYAN,BACK_BOLD_WHITE};

static void setForeColour(int fc)
{
	printf("\e[%im",fc);
}

static void setBackColour(int fc)
{
	printf("\e[%im",fc);
}


#endif

