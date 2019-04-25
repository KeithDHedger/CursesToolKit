/*
 *
 * ©K. D. Hedger. Tue 23 Apr 20:50:29 BST 2019 keithdhedger@gmail.com

 * This file (cursesSourceEditBox.h) is part of CursesToolKit.

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

#ifndef _CURSESSOURCEEDITBOX_
#define _CURSESSOURCEEDITBOX_

#include "cursesGlobals.h"

class CTK_cursesSourceEditBoxClass
{
	public:
		CTK_cursesSourceEditBoxClass();
		~CTK_cursesSourceEditBoxClass();

		CTK_mainAppClass			*mc=NULL;
		std::vector<std::string>	txtstrings;


		void						CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt="",bool selectable=true);
		void						CTK_setColours(coloursStruct cs);
		void						CTK_updateText(const char *txt,bool isfilename=false,bool reset=true);
		void						CTK_drawBox(bool hilite=false,bool showcursor=false);
		void						CTK_doEditEvent(void);
		const char					*CTK_getBuffer(void);
		const std::string			CTK_getCurrentLine(void);
		const std::string			CTK_getCurrentWord(void);
		void						CTK_deleteCurrentWord(void);
		void						CTK_deleteCurrentLine(void);
		void						CTK_insertText(const char *txt);
		void						CTK_gotoXY(int x,int y);
		void						CTK_setRunLoop(bool loop);

	private:
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		bool						canSelect=true;
		int							startLine=0;
		int							currentX;
		int							currentY;
		int							virtualX;
		bool						runLoop=false;

		char						*txtBuffer=NULL;
		std::string					blank;
		std::string					status;
		const char					*editStatus="Normal";
		TermKey						*tk;
		coloursStruct				colours;
		CTK_cursesGraphicsClass		*gc;

		void						updateBuffer(void);
		void						adjustXY(void);
		void						skipAnsi(void);
};

#endif
