/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:51:10 GMT 2019 keithdhedger@gmail.com

 * This file (cursesButton.h) is part of CursesToolKit.

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

#ifndef _CURSESBUTTON_
#define _CURSESBUTTON_

//#include <stdio.h>

//#include "cursesLib.h"
#include "cursesGlobals.h"
class CTK_cursesButtonClass
{
	public:
		CTK_cursesButtonClass();
		~CTK_cursesButtonClass();
		char	*label;

		void	CTK_newButton(int x,int y,int width,int hite,const char *label="");
		void	CTK_drawButton(bool hilite=false);
		void	CTK_setEnterDeselects(bool deselect);
		bool	CTK_getEnterDeselects(void);
		void	CTK_setColours(coloursStruct cs);

		void	CTK_setSelectCB(void (*select)(void *));
		void	(*selectCB)(void *);

	private:
		int		sx;
		int		sy;
		int		wid;
		int		hite;
		coloursStruct	colours;
		bool	enterDeselects=true;
};

#endif
