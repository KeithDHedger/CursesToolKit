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

#ifndef _CURSESBUTTON_
#define _CURSESBUTTON_

#include <stdio.h>

#include "cursesLib.h"

class CTK_cursesButtonClass
{
	public:
		CTK_cursesButtonClass();
		~CTK_cursesButtonClass();
		char	*label;

		void	CTK_newButton(int x,int y,int width,int hite,const char *label="");
		void	CTK_setColours(int forcol,int backcol,int hilite);
		void	CTK_drawButton(bool hilite=false);
		void	CTK_setSelectCB(void (*select)(void *));
		void	(*selectCB)(void *);
	private:
		int		sx;
		int		sy;
		int		wid;
		int		hite;
		int		forCol=NRED;
		int		backCol=NBBLACK;
		int		hiliteCol=NCYAN;
};

#endif
