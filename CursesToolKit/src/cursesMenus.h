/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:52:11 GMT 2019 keithdhedger@gmail.com

 * This file (cursesMenus.h) is part of CursesToolKit.

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

#ifndef _CURSESMENUS_
#define _CURSESMENUS_

/**
* \note Sub menus not implemented yet.
*/
struct	menuStruct
{
	char							*menuName=NULL;
	char							shortcutKey=0;
	char							menuKey=0;
	int								menuItemCnt=0;
	int								startCol=0;
	int								maxWidth=0;
	bool							itemsHaveKey=false;
	bool							menuEnabled=true;
	std::map<int,menuStruct*>		menuItem;
};

class CTK_cursesMenuClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesMenuClass(CTK_mainAppClass *mc);
		~CTK_cursesMenuClass();
		int							menuNumber=0;
		int							menuStart=0;
		int							menuCnt=0;
		int							menuItemNumber=-1;
		bool						enableShortcuts=true;
		void						CTK_setUpdateCB(void (*update)(void *,void*),void* mainapp);

		std::map<int,menuStruct*>	menuNames;

		void						CTK_drawDefaultMenuBar(void);
		void						CTK_addMenuToBar(const char *name);
		void						CTK_addMenuItem(int menunum,const char *name,bool shortcut=true);
		void						CTK_clearMenu(int menunum,bool full=false);
		int							CTK_doMenuEvent(int sx,int sy,bool xdoshortcut);//TODO// private
		bool						CTK_doShortCutKey(char key,int menunum);
		bool						CTK_doMenuKey(char key,int menunum);
		void						CTK_setMenuShortCut(int menunum,int menuitem,char key);
		void						CTK_setMenuBarEnable(bool enable);
		bool						CTK_getMenuBarEnable(void);
		void						CTK_setMenuBarVisible(bool page,bool global);
		void						CTK_setMenuBarVisible(bool page);
		bool						CTK_getMenuBarVisible(void);

//over ridden virtual funcs and data
		void						CTK_drawGadget(bool hilite=true);//TODO//

	private:
		void						(*updateCB)(void *,void*);
		unsigned					menuWidth=0;
		int							menuNamesStartX=1;
		int							pad=1;
		bool						menuShowing=false;
		bool						enableMenuBar=true;
		bool						menuBarVisibleGlobal=true;
		//bool						menuBarVisible=true;
		void						drawMenuStyle(int menunum,int menuitem,int x,int y,int style,bool doshortcut,bool dopad);
		int							drawMenuWindow(int menunum,int sx,int sy,int prelight,bool doshortcut);
		char						setShortCut(const char *name);

		int							mBarHite=1;
		int							menuHite=1;
		int							rows=0;
		unsigned					maxRows=0;
};

#endif
