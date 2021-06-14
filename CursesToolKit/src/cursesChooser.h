/*
 *
 * ©K. D. Hedger. Wed 26 Jun 11:38:09 BST 2019 keithdhedger@gmail.com

 * This file (cursesChooser.h) is part of CursesToolKit.

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

#ifndef _CURSESCHOOSER_
#define _CURSESCHOOSER_

class CTK_cursesChooserClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesChooserClass(CTK_mainAppClass *mc,int x,int y,int width,int hite);
		~CTK_cursesChooserClass();

		LFSTK_findClass			*files;
		std::string				folderPath;
		std::string				fileName;
		std::string				filePath;

		CTK_cursesListBoxClass	*lb;

		void					CTK_selectFolder(CTK_mainAppClass *app,const char *folder);
		void					CTK_setShowHidden(bool show);
		void					CTK_setShowTypes(int show);
		void					CTK_setShowFileTypes(const char *filetypes);
		void					CTK_updateList(void);
		void					CTK_setCanChangeFolder(bool canchange);

	protected:

	private:
		bool					canChangeFolder=true;
		bool					showHidden=false;
		int						types=FOLDERTYPE;
		const char				*fileTypes=NULL;
};

#endif
