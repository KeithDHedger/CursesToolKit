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

class CTK_cursesCheckBoxClass
{
	public:
		CTK_cursesCheckBoxClass();
		~CTK_cursesCheckBoxClass();
		char			*label=NULL;
		void			*selectCBUserData=NULL;

		void			CTK_newCheckBox(int x,int y,int width,const char *label="");
		void			CTK_drawCheckBox(bool hilite=false);
		void			CTK_setValue(bool val);
		bool			CTK_getValue(void);
		void			CTK_setEnterDeselects(bool deselect);
		bool			CTK_getEnterDeselects(void);
		void			CTK_setColours(coloursStruct cs);

		void			CTK_setSelectKey(TermKeySym key);
		TermKeySym		CTK_getSelectKey(void);
		void			CTK_setSelectCB(void (*select)(void *,void *),void *userdata=NULL);
		void			(*selectCB)(void *,void *)=NULL;
	private:
		TermKeySym		selectKey=TERMKEY_SYM_ENTER;
		int				sx;
		int				sy;
		int				wid;
		int				hite;

		coloursStruct	colours;
		bool			value=false;
		bool			enterDeselects=true;
};

#endif
