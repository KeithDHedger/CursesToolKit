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
class CTK_cursesChooserClass;
class CTK_cursesDropClass;
class CTK_cursesFBImageClass;
class CTK_cursesReadKeyClass;

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
		CTK_cursesUtilsClass					*utils;
		CTK_cursesGraphicsClass					*gc;
		bool									noHiliteChange=false;

		void									CTK_clearScreen(void);
		void									CTK_setColours(coloursStruct cs);
		int										CTK_addPage(void);
		int										CTK_removePage(int pagenum);
		void									CTK_setPage(int pagenum);
		int										CTK_previousPage(void);
		int										CTK_nextPage(void);
		void									CTK_setPageUserData(int pagenum,void *userdata);
		void									CTK_appWindow(int x,int y,int w,int h,const char *windowname,const char *title);
//////////
		void									CTK_setDialogWindow(const char *windowname,const char *dialogname,int dialogwidth=-1,int dialoghite=-1);
//////////////////
		void									CTK_setTabWidth(int width);

		void									(*eventLoopCBIn)(void*,void*)=NULL;
		void									(*eventLoopCBOut)(void*,void*)=NULL;

//build new gadgets
		void									CTK_addNewMenuBar(void);
		CTK_cursesTextBoxClass*					CTK_addNewTextBox(int x,int y,int width,int hite,const char *txt,bool selectable=true);
		CTK_cursesTextBoxClass*					CTK_addNewTextBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable=true);
		
		CTK_cursesButtonClass*					CTK_addNewButton(int x,int y,int width,int hite,const char *label);
		CTK_cursesInputClass*					CTK_addNewInput(int x,int y,int width,int hite,const char *label);
		CTK_cursesListBoxClass*					CTK_addNewListBox(int x,int y,int width,int hite);//TO BE REMOVED?
		CTK_cursesCheckBoxClass*				CTK_addNewCheckBox(int x,int y,int width,const char *label);
		CTK_cursesEditBoxClass*					CTK_addNewEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable=true);
		CTK_cursesLabelClass*					CTK_addNewLabel(int x,int y,int width,int hite,const char *txt);
		CTK_cursesSourceEditBoxClass*			CTK_addNewSourceEditBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable=true);
		CTK_cursesDropClass*					CTK_addNewDropDownBox(CTK_mainAppClass *mc,int x,int y,int width,int hite,const char *label);
		CTK_cursesFBImageClass*					CTK_addNewFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect=true);
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
		void									CTK_addChooserBox(CTK_cursesChooserClass *cb);
		void									CTK_addDropBox(CTK_cursesDropClass *cb);

		static void								CTK_updateScreen(void *object,void* userdata);
//////////////////
		int										CTK_mainEventLoop_New(int runcnt=0,bool docls=true,bool leavehilited=false);
		int										CTK_mainEventLoop(int runcnt=0,bool docls=true);
//////////////////
		void									CTK_emptyIPBuffer(void);
		void									CTK_setSelectKey(int key);

		void									CTK_setDefaultGadget(CTK_cursesGadgetClass *gadget,bool unhilite=true);
		CTK_cursesGadgetClass*					CTK_getGadgetNum(int page,gadgetType type,int nth);

		struct fbData							*CTK_getFBData(void);

		int										x;
		int										y;
		int										wid;
		int										hite;
		const char								*windowName;
		const char								*title;
		bool									useAppWindow=false;
		void									*userData=NULL;
		bool									showHilighting=true;
		std::string								clearScreenCode="\e[H\e[2J";
		CTK_cursesReadKeyClass					*readKey=NULL;

	private:
		void									scrollGadget(bool pagescrill,bool lineup);
		void									runMenus(void);
		void									activateGadget(void);
		void									highLiteGadget(bool forward);
		void									drawAllGadgets(void);
		void									resetAllGadgets(void);
		void									setHilite(bool forward);

		struct fbData							frameBufferData;
		int										selectKey=CTK_KEY_RETURN;
};

#endif
