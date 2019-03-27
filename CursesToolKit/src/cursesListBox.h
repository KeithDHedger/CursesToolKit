/*
 *
 * ©K. D. Hedger. Thu 14 Mar 16:11:16 GMT 2019 keithdhedger@gmail.com

 * This file (cursesLists.h) is part of CursesLists.

 * CursesLists is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesLists is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesLists.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CURSESLISTBOX_
#define _CURSESLISTBOX_

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include <termkey.h>
#include <termios.h>
#include <termcap.h>
#include <sys/ioctl.h>

#include <string>
#include <vector>

#include "cursesLib.h"

class CTK_cursesListBox
{
	public:
		CTK_cursesListBox();
		~CTK_cursesListBox();
		int							listStart=0;
//		int							listCnt=0;
		int							listItemNumber=-1;
		void						CTK_newListBox(int x,int y,int width,int hite);
		
//		void						CTK_setUpdateCB(void (*update)(void *,void*),void* mainapp);
		void						CTK_setSelectCB(void (*select)(void *));
		void						(*selectCB)(void *);

		std::vector<std::string>	listItems;

//		void						CTK_drawListBar(void);
//		void						CTK_addListToBar(const char *name);
		void						CTK_addListItem(const char *itemtxt);
		void						CTK_clearList(void);
		int							CTK_doListEvent(int sx,int sy,bool doshortcut);//togo private
//		bool						CTK_doShortCutKey(char key,int listnum);
		void						CTK_drawListWindow(bool hilite);

	private:
		void						(*updateCB)(void *,void*);
//		void						*mainApp=NULL;
		unsigned					listWidth=0;
		int							listNamesStartX=0;
//		int							pad=1;
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		std::string					blank;

		void						drawListStyle(int listnum,int listitem,int x,int y,int style,bool doshortcut,bool dopad);
//		char						setShortCut(const char *name);
//termkey
		TermKey						*tk;
//		int							mBarHite=1;
		int							listHite=1;
		int							rows=0;
		unsigned					maxRows=0;
		int							foreCol=FORE_BLACK;
		int							backCol=BACK_WHITE;
		int							hiliteForeCol=FORE_BLACK;
		int							hiliteBackCol=BACK_CYAN;
};

#endif
