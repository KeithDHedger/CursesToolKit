/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:53:38 GMT 2019 keithdhedger@gmail.com

 * This file (cursesCheckBox.h) is part of CursesToolKit.

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

#ifndef _CURSESCHECKBOX_
#define _CURSESCHECKBOX_

class CTK_cursesCheckBoxClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesCheckBoxClass(CTK_mainAppClass *mc);
		~CTK_cursesCheckBoxClass();

		char			*label=NULL;

		void			CTK_newCheckBox(int x,int y,int width,const char *label="");
		void			CTK_setValue(bool val);
		bool			CTK_getValue(void);

//		void			CTK_setSelectKey(TermKeySym key);
//		TermKeySym		CTK_getSelectKey(void);

//over ridden virtual funcs
		void			CTK_drawGadget(bool hilite=false);

	private:
//		TermKeySym		selectKey=TERMKEY_SYM_ENTER;
		bool			value=false;
};

#endif
