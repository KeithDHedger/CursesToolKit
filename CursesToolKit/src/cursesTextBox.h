/*
 *
 * ©K. D. Hedger. Mon 18 Mar 14:42:59 GMT 2019 keithdhedger@gmail.com

 * This file (cursesTextBox.h) is part of CursesMenus.

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

#ifndef _CURSESTEXTBOX_
#define _CURSESTEXTBOX_

#include <string>
#include <vector>

#include "cursesLib.h"

class CTK_cursesTextBoxClass
{
	public:
		CTK_cursesTextBoxClass();
		~CTK_cursesTextBoxClass();

		void						CTK_newBox(int x,int y,int width,int hite,const char *txt="",bool selectable=true);
		void						CTK_updateText(const char *txt);
		void						CTK_drawBox(bool hilite=false);
		void						CTK_scrollLine(bool scrollup);
		void						CTK_scrollPage(bool scrollup);
		void						CTK_setSelectable(bool canselect);
		bool						CTK_getSelectable(void);
		const std::string			CTK_getText(void);

	private:
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		int							startLine=0;
		std::string					text;
		std::string					blank;
		std::vector<std::string>	txtstrings;
		bool						canSelect=true;
		int							foreCol=FORE_WHITE;
		int							backCol=BACK_BLACK;
		int							hiliteForeCol=FORE_BLACK;
		int							hiliteBackCol=BACK_CYAN;

		void						scroll(bool scrollup,int numlines);
};

#endif
