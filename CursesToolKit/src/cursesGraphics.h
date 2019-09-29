/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:51:10 GMT 2019 keithdhedger@gmail.com

 * This file (cursesButton.h) is part of CursesToolKit.

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

#ifndef _CURSESGRAPHICS_
#define _CURSESGRAPHICS_

enum {LEFTJUSTIFY=0,CENTREJUSTIFY,RIGHTJUSTIFY};

class CTK_cursesGraphicsClass
{
	public:
		CTK_cursesGraphicsClass();
		~CTK_cursesGraphicsClass();

		void	CTK_setColours(coloursStruct cs);
		void	CTK_drawBox(int x,int y,int w,int h,int type,bool fill,bool shadow=false);
		//void	CTK_printLine(const char *line,const char *blank,int width);
		//void	CTK_printLine(const char *line,int boxwidth);
		void	CTK_printLine(const char *line,int sx,int sy,int boxwidth);
		void	CTK_printLine(const char *line,const char *blnk,int sx,int sy,int boxwidth);
		void	CTK_printLinePostBlanks(const char *line,int sx,int sy,int boxwidth);
		void	CTK_printJustLine(const char *line,int sx,int sy,int boxwidth,int just=LEFTJUSTIFY);
	private:
		coloursStruct	colours;
};

#endif
