/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:51:40 GMT 2019 keithdhedger@gmail.com

 * This file (cursesInput.h) is part of CursesToolKit.

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

#ifndef _CURSESINPUT_
#define _CURSESINPUT_

#include "cursesGlobals.h"

class CTK_cursesInputClass
{
	public:
		CTK_cursesInputClass();
		~CTK_cursesInputClass();

		void					CTK_newInput(int x,int y, int w,int h,const char *txt="");
		void					CTK_drawInput(bool hilite);
		void					CTK_doInput(void);
		void					CTK_setColours(coloursStruct cs);

	private:
		int						sx;
		int						sy;
		int						wid;
		int						hite;
		int						curs=0;
		int						startChar=0;

		std::string				text;
		std::string				blank;
		TermKey					*tk;
		coloursStruct			colours;
		CTK_cursesGraphicsClass	*gc;
};

#endif
