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

#ifndef _CURSESGRAPHICS_
#define _CURSESGRAPHICS_

#include <stdio.h>

#include "cursesLib.h"

class CTK_cursesGraphicsClass
{
	public:
		CTK_cursesGraphicsClass();
		~CTK_cursesGraphicsClass();

		void	CTK_setColours(coloursStruct cs);
		void	CTK_drawBox(int x,int y,int w,int h,int type,bool fill);

	private:
		coloursStruct	colours;
};

#endif
