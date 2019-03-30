/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:52:35 GMT 2019 keithdhedger@gmail.com

 * This file (cursesListBox.h) is part of CursesToolKit.

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
		void						CTK_setColours(coloursStruct cs);
		
		void						CTK_setSelectCB(void (*select)(void *));
		void						(*selectCB)(void *);

		std::vector<listItemStruct*>	listItems;

		void						CTK_addListItem(const char *label,void *ud=NULL);
		void						CTK_clearList(void);
		void						CTK_drawListWindow(bool hilite);
		void						CTK_keyUpDown(bool doup,bool page=false);
		void						CTK_setEnterDeselects(bool deselect);
		bool						CTK_getEnterDeselects(void);

	private:
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		bool						enterDeselects=true;
		std::string					blank;

		int							foreCol=FORE_BLACK;
		int							backCol=BACK_WHITE;
		int							hiliteForeCol=FORE_BLACK;
		int							hiliteBackCol=BACK_CYAN;
};

#endif
