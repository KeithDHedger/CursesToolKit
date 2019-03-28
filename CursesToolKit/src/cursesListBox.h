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

struct listItemStruct
{
	std::string	label;
	void		*userData;
};

class CTK_cursesListBoxClass
{
	public:
		CTK_cursesListBoxClass();
		~CTK_cursesListBoxClass();
		int							listStart=0;
		int							listItemNumber=0;
		void						CTK_newListBox(int x,int y,int width,int hite);
		
		void						CTK_setSelectCB(void (*select)(void *));
		void						(*selectCB)(void *);

		std::vector<listItemStruct*>	listItems;

		void						CTK_addListItem(const char *label,void *ud=NULL);
		void						CTK_clearList(void);
		void						CTK_drawListWindow(bool hilite);
		void						CTK_keyUpDown(bool doup);

	private:
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		std::string					blank;

		int							foreCol=FORE_BLACK;
		int							backCol=BACK_WHITE;
		int							hiliteForeCol=FORE_BLACK;
		int							hiliteBackCol=BACK_CYAN;
};

#endif
