/*
 *
 * ©K. D. Hedger. Tue 27 Aug 11:22:06 BST 2019 keithdhedger@gmail.com

 * This file (cursesGadget.h) is part of CursesToolKit.

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

#ifndef _CURSESGADGET_
#define _CURSESGADGET_

enum gadgetType {BASEGADGET=0,BUTTONGADGET,CHECKGADGET,CHOOSERGADGET,DROPGADGET,EDITGADGET,IMAGEGADGET,INPUTGADGET,LABELGADGET,LISTGADGET,MENUGADGET,SRCGADGET,TEXTGADGET};

/**
 *
 * \brief Base class for CTK_curses gadgets.
 *
 * Base class for CTK_curses gadgets.
 *
 */
class CTK_cursesGadgetClass
{
	public:
		CTK_cursesGadgetClass();
		virtual ~CTK_cursesGadgetClass();
		
	private:
		
	gadgetType	type;
};

#endif
