/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:52:35 GMT 2019 keithdhedger@gmail.com

 * This file (cursesListBox.h) is part of CursesToolKit.

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

#ifndef _CURSESLISTBOX_
#define _CURSESLISTBOX_

/**
* Struct for list item.
* \note Caller is responsible for allocating/freeing userData.
*/
struct listItemStruct
{
	std::string	label;
	void		*userData;
};

class CTK_cursesListBoxClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesListBoxClass(CTK_mainAppClass *mc);
		~CTK_cursesListBoxClass();

		int								listStart=0;
		int								listItemNumber=0;
		std::vector<listItemStruct*>	listItems;

		void							CTK_newListBox(int x,int y,int width,int hite);
		void							CTK_addListItem(const char *label,void *userdata=NULL);
		void							CTK_clearList(void);
		void							CTK_keyUpDown(bool doup,bool page=false);
		void							CTK_selectAll(void);
		void							CTK_selectNone(void);
		void							CTK_setMultipleSelect(bool multi);
		bool							CTK_getMultipleSelect(void);
		const std::vector<bool>			CTK_getSelections(void);
		void							CTK_toggleItem(int item);
		void							CTK_setItem(int item,bool set);
		void							CTK_selectItem(int item);
		int								CTK_findByLabel(const std::string);

//over ridden virtual funcs and data
		void							CTK_drawGadget(bool hilite=false);

	private:
		std::string						blank;
		bool							multi=false;
		std::vector<bool>				selections;
};

#endif
