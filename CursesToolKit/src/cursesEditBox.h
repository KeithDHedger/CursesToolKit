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

class CTK_cursesEditBoxClass
{
	public:
		CTK_cursesEditBoxClass();
		~CTK_cursesEditBoxClass();

		CTK_mainAppClass			*mc=NULL;
		int							tabWidth=8;
		bool						isDirty=false;

		virtual void				CTK_updateText(const char *txt,bool isfilename=false,bool reset=true);
		virtual void				CTK_doEditEvent(void);

		void						CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt="",bool selectable=true);
		void						CTK_setColours(coloursStruct cs);
		void						CTK_drawBox(bool hilite=false,bool showcursor=false);
		const char					*CTK_getBuffer(void);
		const std::string			CTK_getCurrentLine(void);
		const std::string			CTK_getCurrentWord(void);
		void						CTK_deleteCurrentWord(void);
		void						CTK_deleteCurrentLine(void);
		void						CTK_insertText(const char *txt);
		void						CTK_gotoXY(int x,int y);
		void						CTK_setRunLoop(bool loop);
		void						CTK_setTabWidth(int width);
		void						CTK_setShowLineNumbers(int show);
		void						CTK_gotoLine(int line);
		
		const std::vector<std::string> CTK_getStrings(void);
	private:

	protected:
		coloursStruct				colours;
		CTK_cursesGraphicsClass		*gc;
		std::vector<std::string>	txtStrings;
		int							currentX;
		int							currentY;
		int							sx;
		int							sy;
		int							wid;
		int							hite;
		int							lineReserve=0;
		int							showLineNumbers=0;
		bool						canSelect=true;
		bool						runLoop=false;
		std::vector<int>			lineNumbers;
		int							startLine=0;
		int							startLineNumber=1;
		char						*txtBuffer=NULL;
		std::string					blank;
		std::string					status;
		const char					*editStatus="Normal";
		TermKey						*tk;

		void						updateBuffer(void);
		void						adjustXY(void);

};

#endif
