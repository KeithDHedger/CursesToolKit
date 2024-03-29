/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:54:51 GMT 2019 keithdhedger@gmail.com

 * This file (LFSTKFindClass.cpp) is part of CursesToolKit.

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

#include <dirent.h>
#include <libgen.h>

#include "LFSTKFindClass.h"

LFSTK_findClass *fc;

void LFSTK_findClass::deleteData(void)
{
	if(!this->data.empty())
		{
			for(int j=0;j<this->data.size();j++)
				{
					this->data.at(j).path.clear();
					this->data.at(j).name.clear();
				}
			this->data.clear();
		}
}

/**
* Find class destructor.
*/
LFSTK_findClass::~LFSTK_findClass()
{
	this->deleteData();
	if(this->fileTypes!=NULL)
		free(this->fileTypes);
}

/**
* Find class constructor.
* \note NOT thread safe.
*/
LFSTK_findClass::LFSTK_findClass()
{
	this->data.clear();
}

/**
* Set search depth
* \param int,int min,max
*/
void LFSTK_findClass::LFSTK_setDepth(int min,int max)
{
	this->minDepth=min;
	this->maxDepth=max;
}

/**
* Get minimum search depth
* \return int.
*/
int LFSTK_findClass::LFSTK_getMinDepth(void)
{
	return(this->minDepth);
}

/**
* Get maximum search depth
* \return int.
*/
int LFSTK_findClass::LFSTK_getMaxDepth(void)
{
	return(this->maxDepth);
}

/**
* Set find type
* \param int type.
* \note type is same as ntfw flags.
*/
void LFSTK_findClass::LFSTK_setFindType(int type)
{
	this->findType=type;
}

/**
* Get find type
* \return int type.
* \note type is same as ntfw flags.
*/
int LFSTK_findClass::LFSTK_getFindType(void)
{
	return(this->findType);
}

/**
* Set follow links
* \param bool follow.
*/
void LFSTK_findClass::LFSTK_setFollowLinks(bool follow)
{
	this->followLinks=follow;
}

/**
* Get follow links
* \return bool.
*/
bool LFSTK_findClass::LFSTK_getFollowlinks(void)
{
	return(this->followLinks);
}

/**
* Set include hiiden files
* \param bool hidden.
*/
void LFSTK_findClass::LFSTK_setIncludeHidden(bool hidden)
{
	this->includeHidden=hidden;
}

/**
* Get include hiiden files
* \return bool.
*/
bool LFSTK_findClass::LFSTK_getIncludeHidden(void)
{
	return(this->includeHidden);
}

/**
* Set sort direction
* \param bool down.
*/
void LFSTK_findClass::LFSTK_setSort(bool down)
{
	this->sortDescending=down;
}

/**
* Get sort direction
* \return bool.
*/
bool LFSTK_findClass::LFSTK_getSort(void)
{
	return(this->sortDescending);
}

/**
* Get ignore boroken links
* \return bool.
*/
bool LFSTK_findClass::LFSTK_getIgnoreBroken(void)
{
	return(this->ignoreBroken);
}

/**
* Set ignore boroken links
* \param bool ignore.
*/
void LFSTK_findClass::LFSTK_setIgnoreBroken(bool ignore)
{
	this->ignoreBroken=ignore;
}

/**
* Set file types filter
* \param const char *types.
*/
void LFSTK_findClass::LFSTK_setFileTypes(const char *suffix)
{
	this->fileTypes=strdup(suffix);
}

/**
* Get file types filter
* \return const char *.
*/
const char * LFSTK_findClass::LFSTK_getFileTypes(void)
{
	return(this->fileTypes);
}

static bool sortDataN(dataStruct i,dataStruct j)
{	
	if(fc->LFSTK_getSort()==true)
		return (i.name<j.name);
	else
		return (i.name>j.name);
}

static bool sortDataP(dataStruct i,dataStruct j)
{
	if(fc->LFSTK_getSort()==true)
		return (i.path<j.path);
	else
		return (i.path>j.path);
}

static bool sortDataT(dataStruct i,dataStruct j)
{
	if(fc->LFSTK_getSort()==true)
		return (i.fileType<j.fileType);
	else
		return (i.fileType>j.fileType);
}

static bool sortDataTN(dataStruct i,dataStruct j)
{
	if(fc->LFSTK_getSort()==true)
		{
			if(i.fileType<j.fileType)
				return(true);

			if((i.fileType==j.fileType) && (i.name<j.name))
				return(true);
			return(false);
		}
	else
		{
			if(i.fileType>j.fileType)
				return(true);

			if((i.fileType==j.fileType) && (i.name<j.name))
				return(true);
			return(false);
		}
	return(false);
}

/**
* Sort data by name
*/
void LFSTK_findClass::LFSTK_sortByName(void)
{
	std::sort(this->data.begin(),this->data.end(),sortDataN);
}

/**
* Sort data by path
*/
void LFSTK_findClass::LFSTK_sortByPath(void)
{
	std::sort(this->data.begin(),this->data.end(),sortDataP);
}

/**
* Sort data by type
*/
void LFSTK_findClass::LFSTK_sortByType(void)
{
	std::sort(this->data.begin(),this->data.end(),sortDataT);
}

/**
* Sort data LFSTK_sortByTypeAndName
*/
void LFSTK_findClass::LFSTK_sortByTypeAndName(void)
{
	std::sort(this->data.begin(),this->data.end(),sortDataTN);
}

/**
* Get number of items.
* \return int.
*/
int LFSTK_findClass::LFSTK_getDataCount(void)
{
	return((int)this->dataCnt);
}

/**
* Use full path
* \param bool.
*/
void LFSTK_findClass::LFSTK_setFullPath(bool usefull)
{
	this->fullPath=usefull;
}

/**
* Get use full path
* \return bool.
*/
bool LFSTK_findClass::LFSTK_getFullPath(void)
{
	return(this->fullPath);
}

bool LFSTK_findClass::fileTypeTest(int filetype)
{
	if(this->LFSTK_getFindType()==ANYTYPE)
		return(true);

	switch(this->LFSTK_getFindType())
		{
			case FOLDERTYPE:
				if((filetype==FOLDERTYPE) || (filetype==FOLDERLINKTYPE))
					return(true);
				break;

			case FILETYPE:
				if((filetype==FILETYPE) || (filetype==FILELINKTYPE))
					return(true);
				break;
		}
	return(false);
}

/**
* Set ignore hard folder links (".", "..").
* \param bool true=ignore.
*/
void LFSTK_findClass::LFSTK_setIgnoreNavLinks(bool ignore)
{
	this->ignoreNavLinks=ignore;
}

/**
* Get ignore hard folder links (".", "..").
* \return bool.
*/
bool LFSTK_findClass::LFSTK_getIgnoreNavLinks(void)
{
	return(this->ignoreNavLinks);
}

/**
* Test for file types by suffix ( list seperated by semi-colon must end in semi-colon).
* \return bool.
*/
bool LFSTK_findClass::fileTypesTest(const char *name)
{
	char	*ptr=this->fileTypes;
	char	*tmptypes=(char*)alloca(256);
	bool	loop=true;
	bool	gotone=false;
	char	*oldptr;

	if(this->LFSTK_getFileTypes()!=NULL)
		{
			strncpy(tmptypes,this->fileTypes,255);
			tmptypes[255]=0;
			ptr=tmptypes;
			while(loop==true)
				{					
					oldptr=ptr;
					ptr=strstr(ptr,";");
					if(ptr!=NULL)
						{
							*ptr=0;
							if(strcasestr(name,oldptr)!=NULL)
								{
									loop=false;
									return(true);
								}
							ptr++;
						}
					else
						loop=false;
				}
		}
	else
		return(true);
	return(false);
}

/**
* Main search function.
* \param const char *dir Path to search.
* \param bool Tru=Add this search to last, False=New search.
* \note If LFSTK_getIgnoreBroken()==true broken links not reported.
*/
void LFSTK_findClass::LFSTK_findFiles(const char *dir,bool multi)
{
	DIR				*dirhandle;
	dirent			*entry;
	struct stat		filestat;
	int				retstat;
	dataStruct		datas;
	char			*filepath;
	struct dirent	**namelist=NULL; 
	int n=0;

	fc=this;
	if(multi==false)
		this->deleteData();
	filepath=(char*)alloca(PATH_MAX);

    n=scandir(dir,&namelist,NULL,versionsort);
    if(n==-1)
    	{
	        fprintf(stderr,"scandir fail >>%s<<",dir); 
			return;
		}

	for (int j=0;j<n;j++)
		{
			entry=namelist[j];

			if(strcmp(entry->d_name,".")==0)
				continue;
			if((this->LFSTK_getIncludeHidden()==false) && ((strlen(entry->d_name)>2) && ((entry->d_name[0]=='.') && (entry->d_name[1]!='.') )))
				continue;

			if((this->ignoreNavLinks==true) && ((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0)))
				continue;

			if(entry->d_type==DT_UNKNOWN)//for fuse mounted files
				{
					sprintf(filepath,"%s/%s",dir,entry->d_name);
					stat(filepath,&filestat);
					entry->d_type=DT_REG;
					if((filestat.st_mode & S_IFMT)==S_IFDIR)
						entry->d_type=DT_DIR;
					if((filestat.st_mode & S_IFMT)==S_IFLNK)
						entry->d_type=DT_LNK;
				}

			datas.name=entry->d_name;
			sprintf(filepath,"%s/%s",dir,entry->d_name);
			if(entry->d_type==DT_LNK)
				{
					retstat=stat(filepath,&filestat);
					if(retstat!=0)
						{
							if(this->LFSTK_getIgnoreBroken()==true)
								continue;
							datas.fileType=BROKENLINKTYPE;
						}
					else
						{
							if(this->LFSTK_getFollowlinks()==false)
								{
									datas.fileType=FILELINKTYPE;
								}
							else
								{
									switch(filestat.st_mode & S_IFMT)
										{
											case S_IFREG:
												if(this->fileTypeTest(FILELINKTYPE)==false)
													continue;
												if(this->fileTypesTest(entry->d_name)==false)
													continue;
												datas.fileType=FILELINKTYPE;
												break;
											case S_IFDIR:
												if(this->fileTypeTest(FOLDERLINKTYPE)==false)
													continue;
												datas.fileType=FOLDERLINKTYPE;
												break;
										}
								}
						}
				}
			else
				{
					switch(entry->d_type)
						{
							case DT_REG:
								if(this->fileTypeTest(FILETYPE)==false)
									continue;
								if(this->fileTypesTest(entry->d_name)==false)
									continue;			
								datas.fileType=FILETYPE;
								break;
							case DT_DIR:
								if(this->fileTypeTest(FOLDERTYPE)==false)
									continue;
								datas.fileType=FOLDERTYPE;
								break;
						}
				}

			if(this->LFSTK_getFullPath()==true)
				datas.path=filepath;
			else
				datas.path="";
					
			this->data.push_back(datas);
			datas.name.clear();
			datas.path.clear();
		}
	for (int j=0;j<n;j++)
		free(namelist[j]);
	free(namelist); 
	this->dataCnt=this->data.size();
}

