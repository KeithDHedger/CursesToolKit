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
#define ABOUTWIDTH 60
#define ABOUTHITE 10
#define QUERYBOXWIDTH 60
#define ENTRYWIDTH 40
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

		void						CTK_openFile(CTK_mainAppClass *app,const char *wname,const char *startdir,bool open=true,const char *filename="",const char *filetypes=NULL);
		void						CTK_selectFolder(CTK_mainAppClass *app,const char *wname,const char *startdir);
		bool						CTK_entryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *defaulttxt,const char *name,const char *title,bool hascancel,int dialogwidth=ENTRYWIDTH);
		int							CTK_queryDialog(CTK_mainAppClass *app,const char *bodytxt,const char *name,const char *title,int buttons,int dialogwidth=QUERYBOXWIDTH);
		void						CTK_aboutDialog(CTK_mainAppClass *app,const char *appname,const char *appinfo,const char *copyright,const char *email,const char *website,const char *credits,const char *licence,int dialogwidth=ABOUTWIDTH);
		void						CTK_splashScreen(CTK_mainAppClass *app,const char *text);

		int							CTK_getGadgetPosX(int sx,int wid,int gadgetcnt,int gadgetwid,int gadgetnum);
		std::string					CTK_padString(std::string label,int maxlen);

	private:
		bool						runOpenFile(CTK_mainAppClass *app,const char *wname,bool open,const char *filename,const char *filetypes);
		bool						runSelectFolder(CTK_mainAppClass *app,const char *wname,const char *folder);
};

#endif
