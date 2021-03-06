/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:50:50 GMT 2019 keithdhedger@gmail.com

 * This file (cursesTextBox.h) is part of CursesToolKit.

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

#ifndef _CURSESTEXTBOX_
#define _CURSESTEXTBOX_

class CTK_cursesTextBoxClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesTextBoxClass(CTK_mainAppClass *mc);
		~CTK_cursesTextBoxClass();

		int							tabWidth=8;

		void						CTK_newBox(int x,int y,int width,int hite,const char *txt="",bool selectable=true);
		void						CTK_updateText(const char *txt,bool isfilename=false,bool reset=true);
		void						CTK_scrollLine(bool scrollup);
		void						CTK_scrollPage(bool scrollup);
		const std::string			CTK_getText(void);

//over ridden virtual funcs and data
		void						CTK_drawGadget(bool hilite=false);

	private:
		void						scrollTBox(bool scrollup,int numlines);

		int							startLine=0;
		std::string					text;
		std::string					blank;
		std::vector<std::string>	txtStrings;
};

#endif
