/*
 *
 * ©K. D. Hedger. Thu  4 Apr 12:38:53 BST 2019 keithdhedger@gmail.com

 * This file (cursesEditBox.h) is part of CursesToolKit.

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

#ifndef _CURSESEDITBOX_
#define _CURSESEDITBOX_

struct	selectStruct
{
	int line;
	int sx;
	int ex;
};

//enum {EDITBOXCLASS=0,SRCBOXCLASS};

class CTK_cursesEditBoxClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesEditBoxClass();
		CTK_cursesEditBoxClass(CTK_mainAppClass *mc);
		~CTK_cursesEditBoxClass();

		int							tabWidth=8;
		bool						isDirty=false;
		bool						isSelecting=false;
		int							selSY=-1;
		void						*userData=NULL;

		virtual void				CTK_updateText(const char *txt,bool isfilename=false,bool reset=true);

//over ridden virtual funcs and data
		void						CTK_drawGadget(bool hilite=false);
		void						CTK_setStatusBarVisible(bool show);
		bool						CTK_getStatusBarVisible(void);
		void						CTK_setStatusBar(std::string str,bool hilite=false);

		void						CTK_doEvent(bool usesrc,std::vector<std::string> &lines,std::vector<std::string> &srclines);
		void						CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt="",bool selectable=true);

		virtual char				*CTK_getBuffer(void);
		const std::string			CTK_getCurrentLine(void);
		int							CTK_currentLineNumber(void);
		const std::string			CTK_getCurrentWord(void);
		void						CTK_startSelecting(void);
		void						CTK_finishSelecting(void);
		std::string					CTK_getSelection(void);
		bool						CTK_isValidSelection(void);
		void						CTK_deleteSelection(void);
		void						CTK_deleteCurrentWord(void);
		void						CTK_deleteCurrentLine(void);
		void						CTK_insertText(const char *txt);

		void						CTK_gotoXY(int x,int y);
		void						CTK_setRunLoop(bool loop);
		void						CTK_setEditable(bool edit);
		void						CTK_setTabWidth(int width);
		void						CTK_setShowLineNumbers(int show);
		void						CTK_gotoLine(int line);
		int							CTK_getCursLine(void);
		int							CTK_getLineAtY(int y);
		int							CTK_getCurrentY(void);
		int							CTK_getCurrentX(void);
		int							CTK_getHeight(void);
		void						CTK_toggleBookMark(int y);
		void						CTK_setBookMark(int y,bool set);
		bool						CTK_getBookMark(int y);
		std::vector<std::string>	&CTK_getStrings(void);
		void						CTK_insertChar(std::string &str,char chr);
		int							CTK_getLineCnt(void);

	private:
		bool						needsRefresh=false;
		void						drawBox(bool hilite=false,bool showcursor=false,bool shortupdate=false);
		bool						addedNL=false;
		bool						realAddedNL=false;

	protected:
		std::vector<selectStruct>	multiLineSels;
		std::vector<std::string>	txtStrings;
		std::vector<bool>			bookMarks;
		bool						canEdit=true;
		bool						showStatus=true;

		int							currentX;
		int							currentY;
		std::vector<int>			lineNumbers;
		int							lineReserve=0;
		int							showLineNumbers=0;
		int							statusCLPad=4;
		bool						canSelect=true;
		bool						runLoop=false;
		int							startLine=0;
		int							startLineNumber=1;
		char						*txtBuffer=NULL;
		std::string					blank;
		std::string					status;
		const char					*editStatus="Normal";
		bool						statusNeedsUpdate=true;

		virtual void				updateBuffer(void);
		void						adjustXY(void);
		virtual void				refreshLine(void);
};

#endif
