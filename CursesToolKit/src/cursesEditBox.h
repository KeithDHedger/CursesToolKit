/*
 *
 * ©K. D. Hedger. Thu  4 Apr 12:38:53 BST 2019 keithdhedger@gmail.com

 * This file (cursesEditBox.h) is part of CursesToolKit.

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

#ifndef _CURSESEDITBOX_
#define _CURSESEDITBOX_

#include "cursesGlobals.h"

class CTK_cursesEditBoxClass
{
	public:
		CTK_cursesEditBoxClass();
		~CTK_cursesEditBoxClass();

		std::vector<std::string>	txtstrings;

		void						CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt="",bool selectable=true);
		void						CTK_setColours(coloursStruct cs);
		void						CTK_updateText(const char *txt,bool isfilename=false,bool reset=true);
		void						CTK_drawBox(bool hilite=false,bool showcursor=false);
		void						CTK_doEditEvent(void);
		const char					*CTK_getBuffer(void);

	private:
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		bool						canSelect=true;
		int							startLine=0;
		int							currentX;
		int							currentY;

		char						*txtBuffer=NULL;
		std::string					blank;

		TermKey						*tk;
		coloursStruct				colours;
		CTK_cursesGraphicsClass		*gc;

		void						updateBuffer(void);
};

#endif
