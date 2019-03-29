/*
 *
 * ©K. D. Hedger. Wed 20 Mar 16:54:08 GMT 2019 keithdhedger@gmail.com

 * This file (cursesButton.h) is part of CursesMenus.

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

#ifndef _CURSESCHECKBOX_
#define _CURSESCHECKBOX_

#include <stdio.h>

#include "cursesLib.h"

class CTK_cursesCheckBoxClass
{
	public:
		CTK_cursesCheckBoxClass();
		~CTK_cursesCheckBoxClass();
		char	*label;

		void	CTK_newCheckBox(int x,int y,int width,const char *label="");
		void	CTK_setColours(int forcol,int backcol,int hilite);
		void	CTK_drawCheckBox(bool hilite=false);
		void	CTK_setValue(bool val);
		bool	CTK_getValue(void);
		void	CTK_setEnterDeselects(bool deselect);
		bool	CTK_getEnterDeselects(void);

		void	CTK_setSelectCB(void (*select)(void *));
		void	(*selectCB)(void *);
	private:
		int		sx;
		int		sy;
		int		wid;
		int		hite;
		int		foreCol=FORE_BLACK;
		int		backCol=BACK_RED;
		int		hiliteForeCol=FORE_BLACK;
		int		hiliteBackCol=BACK_CYAN;
		bool	value=false;
		bool	enterDeselects=true;
};

#endif
