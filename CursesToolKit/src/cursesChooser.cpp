/*
 *
 * ©K. D. Hedger. Wed 26 Jun 11:38:02 BST 2019 keithdhedger@gmail.com

 * This file (cursesChooser.cpp) is part of CursesToolKit.

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
 
#include "cursesGlobals.h"

/**
* Destructor.
*/
CTK_cursesChooserClass::~CTK_cursesChooserClass()
{
	delete this->files;
}

/**
* Constructor.
*/
CTK_cursesChooserClass::CTK_cursesChooserClass(CTK_mainAppClass *mc,int x,int y,int width,int hite)
{
	this->CTK_setCommon(mc);
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->files=new LFSTK_findClass();
	this->lb=new CTK_cursesListBoxClass(mc);
	this->lb->CTK_newListBox(this->sx,this->sy,this->wid,this->hite);
	this->type=CHOOSERGADGET;
	this->selectDeselects=false;
	this->isSelectable=false;
}

/**
* Private internal callback.
*/
static void chooserSelectCB(void *inst,void *ud)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);
	CTK_cursesChooserClass		*ch=static_cast<CTK_cursesChooserClass*>(ud);
	char						*cwd;
	//fprintf(stderr,">>%s<<\n",ch->files->data[ls->listItemNumber].path.c_str());

	ch->fileName=ch->files->data[ls->listItemNumber].name;
	ch->filePath=ch->files->data[ls->listItemNumber].path;
	if((ch->files->data[ls->listItemNumber].fileType==FOLDERTYPE) || (ch->files->data[ls->listItemNumber].fileType==FOLDERLINKTYPE))
		{
			sprintf(buffer,"%s",ch->files->data[ls->listItemNumber].path.c_str());
			chdir(buffer);
			getcwd(buffer,PATH_MAX);
			ch->folderPath=buffer;
			ch->fileName=ch->files->data[ls->listItemNumber].name;
			ch->filePath=ch->files->data[ls->listItemNumber].path;
			ch->CTK_updateList();
		}

//	fprintf(stderr,"folder=%s\nname=%s\npath=%s\n",ch->folderPath.c_str(),ch->fileName.c_str(),ch->filePath.c_str());
	if(ch->selectCB!=NULL)
		ch->selectCB((void*)ch,(void*)ch->CTK_getCBUserData());
}

/**
* Upate the file list.
*/
void CTK_cursesChooserClass::CTK_updateList(void)
{
	char	*buffer=(char*)alloca(PATH_MAX);

	this->files->LFSTK_setIncludeHidden(this->showHidden);
	this->files->LFSTK_findFiles(this->folderPath.c_str());
	this->files->LFSTK_setSort(false);
	this->files->LFSTK_sortByTypeAndName();
	this->lb->CTK_clearList();
	for(int j=0;j<this->files->data.size();j++)
		{
			setPathAndType(buffer,this->files->data[j].name.c_str(),this->files->data[j].fileType);
			this->lb->CTK_addListItem(buffer,NULL);
		}
}

/**
* Set show hidden files/folders.
*/
void CTK_cursesChooserClass::CTK_setShowHidden(bool show)
{
	this->showHidden=show;
}

/**
* Set show files.
*/
void CTK_cursesChooserClass::CTK_setShowTypes(int show)
{
	this->types=show;
	this->files->LFSTK_setFindType(this->types);
}

/**
* Set show file types ( uses suffix eg *.jpg ).
*/
void CTK_cursesChooserClass::CTK_setShowFileTypes(const char *filetypes)
{
	if(filetypes==NULL)
		return;
	this->fileTypes=filetypes;
	this->files->LFSTK_setFileTypes(this->fileTypes);
}

/**
* Build selector.
*/
void CTK_cursesChooserClass::CTK_selectFolder(CTK_mainAppClass *app,const char *folder)
{
	char	*buffer=(char*)alloca(PATH_MAX);

	this->lb->CTK_setColours(app->colours);

	this->files->LFSTK_setFollowLinks(true);
	this->files->LFSTK_setFindType(this->types);
	this->files->LFSTK_setFullPath(true);
	this->files->LFSTK_setIncludeHidden(this->showHidden);
	this->files->LFSTK_findFiles(folder);
	this->files->LFSTK_setSort(false);
	this->files->LFSTK_sortByTypeAndName();

	for(int j=0;j<files->data.size();j++)
		{
			setPathAndType(buffer,files->data[j].name.c_str(),files->data[j].fileType);
			this->lb->CTK_addListItem(buffer,NULL);
		}

	this->lb->CTK_setSelectDeselects(false);
	app->CTK_addListBox(this->lb);
	this->lb->CTK_setSelectCB(chooserSelectCB,this);
	this->folderPath=folder;
	return;
}
