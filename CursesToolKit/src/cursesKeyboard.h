/*
 *
 * ©K. D. Hedger. Fri 20 Sep 11:25:12 BST 2019 keithdhedger@gmail.com

 * This file (cursesKeyboard.h) is part of CursesToolKit.

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

#ifndef _CURSESKEYBOARD_
#define _CURSESKEYBOARD_

static const char	*osKLine[][5]={{"1234567890","qwertyuiop","asdfghjkl;","\\zxcvbnm,.","^<     >yk"},{"+=-_{}^#@/","QWERTYUIOP","ASDFGHJKL:","|ZXCVBNM<>","^<     >yk"}};

class CTK_cursesKeyboardClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesKeyboardClass(CTK_mainAppClass *mc);
		~CTK_cursesKeyboardClass();

		void					CTK_newKeyboard(int x,int y, int w,int h,const char *txt="");
		void					CTK_doInput(void);
		void					CTK_setText(const char *txt);
		const char				*CTK_getText(void);

//over ridden virtual funcs and data
		void					CTK_drawGadget(bool hilite);

	private:
		bool					osKeyboard=false;
		int						curs=0;
		int						startChar=0;
		std::string				text;
		std::string				blank;
		int						cx=0;
		int						cy=0;
		int						cPage=0;
		void					updateText(const char *txt);
};


#endif
