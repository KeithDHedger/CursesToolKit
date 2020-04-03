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
		CTK_cursesGraphicsClass(CTK_mainAppClass *mc);
		~CTK_cursesGraphicsClass();

		void				CTK_setColours(coloursStruct *srccs,coloursStruct *dstcs,bool force=false);
		void				CTK_drawBox(int x,int y,int w,int h,int type,bool fill,bool shadow=false);
		void				CTK_drawDialogWindow(void);

		void				CTK_printJustLineColour(const char *line,int sx,int sy,int boxwidth,int just,int fg,int bg);

	private:
		void				detab(char *in,char *out,int maxlen,int sx);
		void				setUserColours(coloursStruct *cs);
		int					tabWidth;
		int					reservedSpace;
		CTK_mainAppClass	*mc=NULL;
		void				drawDialogBox(int x,int y,int w,int h);
};

#endif
