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

struct pageStruct;
class CTK_cursesButtonClass;
class CTK_cursesTextBoxClass;
class CTK_cursesCheckBoxClass;
class CTK_cursesGraphicsClass;
class CTK_cursesInputClass;
class CTK_cursesEditBoxClass;
class CTK_cursesMenuClass;
class CTK_cursesListBoxClass;
class CTK_cursesLabelClass;
class CTK_cursesSourceEditBoxClass;

enum {HLNONE=-1,HLBUTTONS,HLTEXT,HLINPUTS,HLLISTS,HLCHKBOXS,HLEDITBOXES,HLSRCEDITBOXES,HLNOMORE};

class CTK_mainAppClass
{
	public:
		CTK_mainAppClass();
		~CTK_mainAppClass();

		int										maxCols;
		int										maxRows;
		int										tabWidth=8;
		int										pageNumber=0;
		std::vector<pageStruct>					pages;
		CTK_cursesMenuClass						*menuBar=NULL;
		coloursStruct							colours;
		bool									runEventLoop=false;

		void									CTK_clearScreen(void);
		void									CTK_setColours(coloursStruct cs);
		int										CTK_addPage(void);
		int										CTK_removePage(int pagenum);
		void									CTK_setPage(int pagenum);
		int										CTK_previousPage(void);
		int										CTK_nextPage(void);
		void									CTK_setPageUserData(int pagenum,void *userdata);
		void									CTK_appWindow(int x,int y,int w,int h,const char *windowname,const char *title);
		void									CTK_setTabWidth(int width);

		void									(*eventLoopCB)(void*,void*)=NULL;

//build new gadgets
		void									CTK_addNewMenuBar(void);
		void									CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable=true);
		void									CTK_addNewButton(int x,int y,int width,int hite,const char *label);
		void									CTK_addNewInput(int x,int y,int width,int hite,const char *label);
		void									CTK_addNewListBox(int x,int y,int width,int hite);
		void									CTK_addNewCheckBox(int x,int y,int width,const char *label);
		void									CTK_addNewEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable=true);
		void									CTK_addNewLabel(int x,int y,int width,int hite,const char *txt);
		void									CTK_addNewSourceEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable=true);
//add existing gadgets
		void									CTK_addMenuBar(CTK_cursesMenuClass *mb);
		void									CTK_addTextBox(CTK_cursesTextBoxClass *txtbox);
		void									CTK_addButton(CTK_cursesButtonClass *btn);
		void									CTK_addInput(CTK_cursesInputClass *inp);
		void									CTK_addListBox(CTK_cursesListBoxClass *lb);
		void									CTK_addCheckBox(CTK_cursesCheckBoxClass *cb);
		void									CTK_addEditBox(CTK_cursesEditBoxClass *edbox);
		void									CTK_addLabel(CTK_cursesLabelClass *label);
		void									CTK_addSourceEditBox(CTK_cursesSourceEditBoxClass *edbox);

		static void								CTK_updateScreen(void *object,void* userdata);
		void									CTK_mainEventLoop(void);
		void									CTK_emptyIPBuffer(void);

		int										x;
		int										y;
		int										wid;
		int										hite;
		const char								*windowName;
		const char								*title;
		bool									useAppWindow=false;


	private:
		TermKey									*tk;
		int										hiliteBtnNum=-1;
		int										hiliteTxtBoxNum=-1;
		int										hiliteInputNum=-1;
		int										hiliteListNum=-1;
		int										hiliteCheckBoxNum=-1;
		int										hiliteEditBoxNum=-1;
		int										hiliteSourceEditBoxNum=-1;
		int										hiliting=HLNONE;

		void									setHilite(bool forward);

};

#endif
