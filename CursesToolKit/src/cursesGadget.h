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

enum gadgetType {BASEGADGET=0,BUTTONGADGET,CHECKGADGET,CHOOSERGADGET,DROPGADGET,EDITGADGET,IMAGEGADGET,INPUTGADGET,LABELGADGET,LISTGADGET,MENUGADGET,SRCGADGET,TEXTGADGET,OSKEYBOARD,BARGADGET};

class CTK_mainAppClass;
class CTK_cursesInputClass;

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

		bool					gadgetDirty=false;
		bool					hiLited=false;
		CTK_mainAppClass		*mc=NULL;
		void					*userData=NULL;
		int						sx=1;
		int						sy=1;
		int						wid=1;
		int						hite=1;

		gadgetColourStruct		gadgetColours;

		void					CTK_setCommon(CTK_mainAppClass *mc);
		const gadgetType		CTK_getGadgetType(void);
		void					*CTK_getCBUserData(void);
		void					CTK_setCBUserData(void*);
		bool					(*selectCB)(void *,void *)=NULL;
		void					CTK_setSelectDeselects(bool deselect);
		bool					CTK_getSelectDeselects(void);
		void					CTK_setEnabled(bool enable);
		bool					CTK_getEnabled(void);
		void					CTK_setSelectable(bool enable);
		bool					CTK_getSelectable(void);
		void					CTK_setSelectKey(int key);
		int						CTK_getSelectKey(void);
		void					CTK_setVisible(bool visible);
		bool					CTK_getVisible(void);

//virtual functions
		virtual void			CTK_drawGadget(bool hilite=false);
		virtual void			CTK_setSelectCB(bool (*select)(void *,void *),void *userdata=NULL);

	protected:
		void					*selectCBUserData=NULL;
		gadgetType				type=BASEGADGET;
		CTK_cursesGraphicsClass	*gc;
		bool					isSelectable=true;
		bool					selectDeselects=true;
		bool					enabled=true;
		int						selectKey=CTK_KEY_NONE;
		bool					visible=true;
	private:	
};

#endif
