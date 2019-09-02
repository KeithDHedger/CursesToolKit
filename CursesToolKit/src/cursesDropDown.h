/*
 *
 * ©K. D. Hedger. Thu 27 Jun 20:34:23 BST 2019 keithdhedger@gmail.com

 * This file (cursesDropDown.h) is part of CursesToolKit.

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


#ifndef _CURSESDROPDOWN_
#define _CURSESDROPDOWN_

/**
* Structure for drop down button.
*/
struct	dropDownStruct
{
	std::string						label;
	int								maxWidth=0;
	bool							enabled=true;
};

class CTK_cursesDropClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesDropClass(CTK_mainAppClass *mc);
		~CTK_cursesDropClass();

		std::string					label;
		int							selectedItem=-1;

		void						CTK_newDropButton(int x,int y,int width,int hite,const char *label="");//TODO//?
		void						CTK_setItemEnabled(int item,bool enable);
		bool						CTK_getItemEnabled(int item);
		void						CTK_addDropItem(const char *name);
		void						CTK_clearList(void);

		void						CTK_doDropDownEvent(void);
		void						CTK_drawList(int selection);

//over ridden virtual funcs
		void						CTK_drawGadget(bool hilite=false);//TODO//?

	private:
		std::vector<dropDownStruct>	items;
		int							maxWidth=0;
		TermKey						*tk;
		std::string					holdLabel;
	protected:
};

#endif
