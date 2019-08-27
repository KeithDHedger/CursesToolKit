/*
 *
 * ©K. D. Hedger. Sat 17 Aug 15:16:33 BST 2019 keithdhedger@gmail.com

 * This file (cursesFBImage.h) is part of CursesToolKit.

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

#ifndef _CURSESFBIMAGE_
#define _CURSESFBIMAGE_

#include "cursesGlobals.h"

class CTK_cursesFBImageClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesFBImageClass();
		~CTK_cursesFBImageClass();

		void				CTK_newFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect=true);
		void				CTK_drawFBImage(void);

		CTK_mainAppClass	*mc=NULL;
		int					sx=0;
		int					sy=0;
		int					wid=-1;
		int					hite=-1;
	private:
		void				*image=NULL;
		void				*blob=NULL;
};


#endif
