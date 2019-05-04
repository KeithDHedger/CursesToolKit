/*
 *
 * ©K. D. Hedger. Tue 23 Apr 20:50:29 BST 2019 keithdhedger@gmail.com

 * This file (cursesSourceEditBox.h) is part of CursesToolKit.

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

#ifndef _CURSESSOURCEEDITBOX_
#define _CURSESSOURCEEDITBOX_

class CTK_cursesSourceEditBoxClass : public CTK_cursesEditBoxClass
{
	public:
		CTK_cursesSourceEditBoxClass();
		~CTK_cursesSourceEditBoxClass();

		CTK_mainAppClass			*mc=NULL;
		void						CTK_updateText(const char *txt,bool isfilename=false,bool reset=true);
		void						CTK_drawBox(bool hilite=false,bool showcursor=false);
		void						CTK_doEditEvent(void);
		bool						needsRefresh=false;

	private:
		int							sourceX;
		std::string					filePath;
		std::vector<std::string>	srcStrings;
		bool						fullUpdate=true;
		void						setScreenX(void);

	protected:
		void						updateBuffer(void);
};

#endif
