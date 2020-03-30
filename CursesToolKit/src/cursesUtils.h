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
#define CHOOSERBTNWIDTH 12

enum {CUENTRYOK=5000,CUENTRYCANCEL,CUENTRYINPUTENTER,CUABOUTCLOSE,CUABOUTCREDITS,CUABOUTLICENCE,CUABOUTDONE,CUCHOOSEROK,CUCHOOSERSHOW,CUCHOOSERCANCEL,CUCHOOSERINPUT,CUQUERYOK,CUQUERYNO,CUQUERYCANCEL};
enum {CUOPENFILE,CUOPENFOLDER,CUSAVEFILE};

class CTK_mainAppClass;
class CTK_cursesInputClass;
class CTK_cursesGadgetClass;
class CTK_cursesChooserClass;
class CTK_cursesTextBoxClass;

struct varsStruct
{
	std::string	varName="";
	int			vType=BADTYPE;
	int			outIntType=DECIMALOUT;
	bool		boolVar=false;
	int			intVar=-1;
	std::string	charVar="";	
};

struct utilDialogStruct
{
	CTK_mainAppClass		*mc=NULL;
	CTK_cursesInputClass	*input=NULL;
	CTK_cursesGadgetClass	*defaultGadget=NULL;
	CTK_cursesChooserClass	*chooser=NULL;
	CTK_cursesTextBoxClass	*results=NULL;

	bool					boolValue=false;
	int						intValue=-1;
	std::string				stringValue="";
	bool					isValidData=false;
	utilVarType				varType=BADTYPE;
};

class CTK_cursesUtilsClass
{
	public:
		CTK_cursesUtilsClass();
		CTK_cursesUtilsClass(CTK_mainAppClass *mc);
		~CTK_cursesUtilsClass();

		utilDialogStruct			dialogReturnData;
		std::string					inFolder=".";

		std::vector<std::string>	CTK_explodeWidth(const std::string str,const char termchar,int width,int tabwidth,int offset,bool incdelim);

		std::vector<std::string>	CTK_explodeWidth(const std::string s,const char c,int width,int tw,bool incdelim=true);
		std::vector<std::string>	CTK_explode(const std::string s,const char c);

		bool						CTK_entryDialog(const char *bodytxt,const char *defaulttxt,const char *windowname,const char *dialogtitle,bool hascancel,int dialogwidth=ENTRYWIDTH);
		bool						CTK_queryDialog(const char *bodytxt,const char *name,const char *title,int buttons,int dialogwidth=QUERYBOXWIDTH);
		void						CTK_aboutDialog(const char *appname,const char *appinfo,const char *copyright,const char *email,const char *website,const char *credits,const char *licence,int dialogwidth=ABOUTWIDTH);
		bool						CTK_fileChooserDialog(const char *startdir,int choosertype=CUOPENFILE,const char *filetypes=NULL,const char *savename="");

		void						CTK_splashScreen(CTK_mainAppClass *app,const char *text);

		int							CTK_getGadgetPosX(int sx,int wid,int gadgetcnt,int gadgetwid,int gadgetnum);
		std::string					CTK_padString(std::string label,int maxlen);
//prefs
		void						CTK_saveVars(const char *filepath,std::vector<varsStruct> vs);
		std::vector<varsStruct>		CTK_loadVars(const char *filepath);
		varsStruct					CTK_findVar(std::vector<varsStruct> vs,const char *varname);

	private:
		CTK_mainAppClass			*mc=NULL;
};

#endif
