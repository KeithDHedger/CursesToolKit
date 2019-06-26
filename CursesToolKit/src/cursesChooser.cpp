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
	delete this->lb;
	CTK_freeAndNull(&this->folderPath);
}

/**
* Constructor.
*/
CTK_cursesChooserClass::CTK_cursesChooserClass(CTK_mainAppClass *app,int x,int y,int width,int hite)
{
	this->x=x;
	this->y=y;
	this->width=width;
	this->hite=hite;
	this->mc=app;
	this->files=new LFSTK_findClass();
	this->lb=new CTK_cursesListBoxClass();
	this->lb->CTK_newListBox(this->x,this->y,this->width,this->hite);
}


/**
* Set chooser callback.
*/
void CTK_cursesChooserClass::CTK_setSelectCB(void (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

/**
* Private internal callback.
*/
static void chooserSelectCB(void *inst,void *ud)
{
	char						*buffer=(char*)alloca(PATH_MAX);
	CTK_cursesListBoxClass		*ls=static_cast<CTK_cursesListBoxClass*>(inst);
	CTK_cursesChooserClass		*ch=static_cast<CTK_cursesChooserClass*>(ud);
	//fprintf(stderr,">>%s<<\n",ch->files->data[ls->listItemNumber].path.c_str());

	if((ch->files->data[ls->listItemNumber].fileType==FOLDERTYPE) || (ch->files->data[ls->listItemNumber].fileType==FOLDERLINKTYPE))
		{
			sprintf(buffer,"%s",ch->files->data[ls->listItemNumber].path.c_str());
			chdir(buffer);
			CTK_freeAndNull(&ch->folderPath);
			ch->folderPath=get_current_dir_name();
			ch->CTK_updateList();
		}

	if(ch->selectCB!=NULL)
		ch->selectCB((void*)ch,(void*)ch->selectCBUserData);
}

/**
* Upate the file list.
*/
void CTK_cursesChooserClass::CTK_updateList(void)
{
	char	*buffer=(char*)alloca(PATH_MAX);

	this->files->LFSTK_setIncludeHidden(this->showHidden);
	this->files->LFSTK_findFiles(this->folderPath);
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

	this->lb->CTK_setEnterDeselects(false);
	app->CTK_addListBox(this->lb);
	this->lb->CTK_setSelectCB(chooserSelectCB,this);
	CTK_freeAndNull(&this->folderPath);
	this->folderPath=strdup(folder);
	return;
}
