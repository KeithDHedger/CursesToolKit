/*
 *
 * ©K. D. Hedger. Fri 19 Apr 16:10:00 BST 2019 keithdhedger@gmail.com

 * This file (cursesLabel.h) is part of CursesToolKit.

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

#ifndef _CURSESLABEL_
#define _CURSESLABEL_

#include "cursesGlobals.h"

class CTK_cursesLabelClass
{
	public:
		CTK_cursesLabelClass();
		~CTK_cursesLabelClass();

		int							tabWidth=8;

		void						CTK_newLabel(int x,int y,int width,int hite,const char *txt="");
		void						CTK_updateText(const char *txt);
		void						CTK_drawLabel(void);
		void						CTK_setColours(coloursStruct cs);

	private:
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		std::string					text;
		std::string					blank;
		std::vector<std::string>	txtStrings;
		coloursStruct				colours;
		CTK_cursesGraphicsClass		*gc;
};

#endif
