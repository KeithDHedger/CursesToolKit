/*
 *
 * ©K. D. Hedger. Thu 14 Mar 16:11:16 GMT 2019 keithdhedger@gmail.com

 * This file (cursesMenus.h) is part of CursesMenus.

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

#ifndef _CURSESMENUS_
#define _CURSESMENUS_

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <map>

#include <termkey.h>
#include <termios.h>
#include <termcap.h>
#include <sys/ioctl.h>


#include "cursesLib.h"

struct	menuStruct
{
	char							*menuName=NULL;
	char							key=0;
	int								menuItemCnt=0;
	int								startCol=0;
	int								maxWidth=0;
	std::map<int,menuStruct*>		menuItem;
};

enum menuStyle {FLATNORM=0,FLATINVERT};

class CTK_cursesMenuClass
{
	public:
		CTK_cursesMenuClass();
		~CTK_cursesMenuClass();
		int							menuNumber=0;
		int							menuStart=0;
		int							menuCnt=0;
		int							menuItemNumber=-1;

		void						CTK_setUpdateCB(void (*update)(void *,void*),void* mainapp);
		void						CTK_setSelectCB(void (*select)(void *));
		void						(*selectCB)(void *);

		std::map<int,menuStruct*>	menuNames;

		void						CTK_drawMenuBar(void);
		void						CTK_addMenuToBar(const char *name);
		void						CTK_addMenuItem(int menunum,const char *name);
		void						CTK_clearMenu(int menunum);
		int							CTK_doMenuEvent(int sx,int sy,bool doshortcut);//togo private
		bool						CTK_doShortCutKey(char key,int menunum);

	private:
		void						(*updateCB)(void *,void*);
		void						*mainApp=NULL;
		unsigned					menuWidth=0;
		int							menuNamesStartX=0;
		int							pad=1;

		void						drawMenuStyle(int menunum,int menuitem,int x,int y,int style,bool doshortcut,bool dopad);
		int							drawMenuWindow(int menunum,int sx,int sy,int prelight,bool doshortcut);
		char						setShortCut(const char *name);
//termkey
		TermKey						*tk;
		int							mBarHite=1;
		int							menuHite=1;
		int							rows=0;
		unsigned					maxRows=0;
};

#endif
