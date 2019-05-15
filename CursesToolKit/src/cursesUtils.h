/*
 *
 * ©K. D. Hedger. Sat  6 Apr 17:05:19 BST 2019 keithdhedger@gmail.com

 * This file (cursesUtils.h) is part of CursesToolKit.

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

//#include "cursesGlobals.h"

#ifndef _CURSESUTILS_
#define _CURSESUTILS_

#define NOBUTTONPRESSED 0
#define YESBUTTON 1<<0
#define NOBUTTON 1<<1
#define CANCELBUTTON 1<<2
#define ALLBUTTONS (YESBUTTON) + (NOBUTTON) + (CANCELBUTTON)

class CTK_mainAppClass;

class CTK_cursesUtilsClass
{
	public:
		CTK_cursesUtilsClass();
		~CTK_cursesUtilsClass();

		std::string					stringResult="";
		int							intResult=0;
		bool						isValidFile=false;
		std::string					inFolder=".";

		std::vector<std::string>	CTK_explodeWidth(const std::string str,const char termchar,int width,int tabwidth,int offset,bool incdelim);

		std::vector<std::string>	CTK_explodeWidth(const std::string s,const char c,int width,int tw,bool incdelim=true);
		std::vector<std::string>	CTK_explode(const std::string s,const char c);
		void						CTK_openFile(CTK_mainAppClass *app,const char *wname,const char *startdir,bool open=true,const char *filename="");
		bool						CTK_entryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *defaulttxt,const char *name,const char *title,bool hascancel=false);
		int							CTK_queryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *name,const char *title,int buttons);
		void						CTK_emptyIPBuffer(void);

	private:
		bool						runOpenFile(CTK_mainAppClass *app,const char *wname,bool open,const char *filename);
};

#endif
