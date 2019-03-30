/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:50:09 GMT 2019 keithdhedger@gmail.com

 * This file (cursesApplication.h) is part of CursesToolKit.

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

#ifndef _CURSESAPPLICATION_
#define _CURSESAPPLICATION_

#include <termkey.h>

#include "cursesTextBox.h"
#include "cursesMenus.h"
#include "cursesButton.h"
#include "cursesInput.h"
#include "cursesListBox.h"
#include "cursesCheckBox.h"

enum HILITING {NONE=-1,BUTTONS,TEXT,INPUTS,LISTS,CHKBOXS};
class CTK_mainAppClass
{
	public:
		CTK_mainAppClass();
		~CTK_mainAppClass();

		int										maxCols;
		int										maxRows;
		std::vector<CTK_cursesTextBoxClass*>	textBoxes;
		std::vector<CTK_cursesButtonClass*>		buttons;
		std::vector<CTK_cursesInputClass*>		inputs;
		std::vector<CTK_cursesListBoxClass*>	lists;
		std::vector<CTK_cursesCheckBoxClass*>	checkBoxes;
		CTK_cursesMenuClass						*menuBar=NULL;

		bool									runEventLoop=false;
		void									(*eventLoopCB)(void*,void*);

//build new gadgets
		void									CTK_addNewMenuBar(void);
		void									CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable=true);
		void									CTK_addNewButton(int x,int y,int width,int hite,const char *label);
		void									CTK_addNewInput(int x,int y,int width,int hite,const char *label);
		void									CTK_addNewListBox(int x,int y,int width,int hite);
		void									CTK_addNewCheckBox(int x,int y,int width,const char *label);
//add existing gadgets
		void									CTK_addMenuBar(CTK_cursesMenuClass *mb);
		void									CTK_addTextBox(CTK_cursesTextBoxClass *txtbox);
		void									CTK_addButton(CTK_cursesButtonClass *btn);
		void									CTK_addInput(CTK_cursesInputClass *inp);
		void									CTK_addListBox(CTK_cursesListBoxClass *lb);
		void									CTK_addCheckBox(CTK_cursesCheckBoxClass *cb);

		static void								CTK_updateScreen(void *object,void* userdata);
		void									CTK_mainEventLoop(void);

	private:
		int										hiliteBtnNum=-1;
		int										hiliteTxtBoxNum=-1;
		int										hiliteInputNum=-1;
		int										hiliteListNum=-1;
		int										hiliteCheckBoxNum=-1;
		HILITING								hiliting=NONE;

		int										foreCol=FORE_BLACK;
		int										backCol=BACK_WHITE;
		int										hiliteForeCol=FORE_BLACK;
		int										hiliteBackCol=BACK_CYAN;
		int										indicForeCol=FORE_BLACK;
		int										indicBackCol=BACK_WHITE;
		int										windowBackCol=BACK_BLACK;
		int										windowForeCol=FORE_WHITE;
};

#endif
