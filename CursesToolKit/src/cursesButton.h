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

#include "cursesGlobals.h"

class CTK_cursesButtonClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesButtonClass(CTK_mainAppClass *mc);
		~CTK_cursesButtonClass();

		char			*label=NULL;

		void			CTK_newButton(int x,int y,int width,int hite,const char *label="");

//over ridden virtual funcs
		void			CTK_drawGadget(bool hilite=false);

	private:
};

#endif
