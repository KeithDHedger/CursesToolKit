/*
 *
 * ©K. D. Hedger. Tue 23 Apr 20:50:19 BST 2019 keithdhedger@gmail.com

 * This file (cursesSourceEditBox.cpp) is part of CursesToolKit.

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

#include <srchilite/sourcehighlight.h>
#include <srchilite/langmap.h>
#include <srchilite/lineranges.h>
#include <srchilite/sourcehighlight.h>
#include <srchilite/languageinfer.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "cursesGlobals.h"

#define SRCDATADIR "/usr/share/source-highlight"

/**
*  Source code box class destroy.
*/
CTK_cursesSourceEditBoxClass::~CTK_cursesSourceEditBoxClass()
{
}

/**
*  Source code box class.
*/
CTK_cursesSourceEditBoxClass::CTK_cursesSourceEditBoxClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->thisType=SRCBOXCLASS;
	this->type=SRCGADGET;
	this->addedNL=false;
}

/**
* Update the text in the source box.
*/
void CTK_cursesSourceEditBoxClass::CTK_updateText(const char *txt,bool isfilename,bool reset)
{
	const char					*ptr=NULL;
	char						*buffer=NULL;
	int							startchr=0;
	char						*cpybuf=(char*)alloca(this->wid+1);
	std::vector<std::string>	array;
	std::string					str;
	CTK_cursesUtilsClass		cu;
	long						fsize;
	FILE						*f;
	std::stringstream			inpstream;
	std::ostringstream			oputstream;
	srchilite::LanguageInfer	inf;
	srchilite::SourceHighlight	sourceHighlight("esc.outlang");
	srchilite::LangMap			langMap(SRCDATADIR,"lang.map");
	std::string					buff;
	bool						noeol=false;

	this->gadgetDirty=true;
	this->txtStrings.clear();
	CTK_freeAndNull(&this->txtBuffer);
	if(reset==true)
		{
			this->currentX=0;
			this->currentY=0;
			this->startLine=0;
		}

	if((isfilename==true) && (access(txt,F_OK|R_OK)!=(F_OK)))
		{
			buff="File not found ...\n";
			buff+=txt;
			isfilename=false;
			txt=buff.c_str();
		}

	if(isfilename==false)
		{
			this->txtBuffer=strdup(txt);
		}
	else
		{
			this->filePath=txt;
			f=fopen(txt,"rb");
			fseek(f,0,SEEK_END);
			fsize=ftell(f);
			if(fsize==0)
				{
					this->txtBuffer=(char*)malloc(2);
					sprintf(this->txtBuffer,"\n");
				}
			else
				{
					fseek(f,0,SEEK_SET);
					this->txtBuffer=(char*)malloc(fsize+1);
					fread(this->txtBuffer,1,fsize,f);
					this->txtBuffer[fsize]=0;
				}
			fclose(f);
		}

//code
	str=this->txtBuffer;
	if(str.back()!=0xa)
		noeol=true;
	this->txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',this->wid-1-this->lineReserve,this->tabWidth);

	if(this->addedNL==false)//TODO//nasty hack!
		{
			this->addedNL=true;
			if(noeol==true)
				{
					this->txtStrings.push_back("\n\n");
					realAddedNL=true;
				}
			else
				{
					realAddedNL=false;
					this->txtStrings.push_back("\n");
				}
		}

	bool	flag=true;
	this->lineNumbers.clear();
	this->startLineNumber=1;
	for(int j=0;j<this->txtStrings.size();j++)
		{
			this->bookMarks.push_back(false);
			if(flag==true)
				this->lineNumbers.push_back(this->startLineNumber++);
			else
				this->lineNumbers.push_back(-1);

			if(this->txtStrings[j].back()=='\n')
				{
					inpstream << this->txtStrings[j];
					flag=true;
				}
			else
				{
					inpstream << this->txtStrings[j] << '\n';
					flag=false;
				}
		}
//	if(noeol==true)
//	{
//	this->srcStrings.push_back("\n\n");
//inpstream << '\n' << "printf" << '\n' << '\n' << '\n';
//	}
	inpstream << std::endl; 
//sourceHighlight.backgroundColor("white");
	sourceHighlight.setDataDir(SRCDATADIR);
	std::string lang="";
	if((this->filePath.compare("")!=0) && (this->forceLang==false))
		{
			lang=langMap.getMappedFileNameFromFileName(this->filePath.c_str());

			if(lang != "")
				this->inputLang=lang;
			else
				{
					lang=inf.infer(this->filePath.c_str());
					if(lang != "")
						{
							langMap.open();
							this->inputLang=langMap.getFileName(lang);
						}
				}
		}

	if(this->forceLang==false)
		if(lang=="")
			this->inputLang="nohilite.lang";

	sourceHighlight.setStyleFile("esc.style");

	sourceHighlight.highlight(inpstream,oputstream,this->inputLang,"");
	this->srcStrings=cu.CTK_explode(oputstream.str(),'\n');

}

/**
* Private.
*/
void CTK_cursesSourceEditBoxClass::updateBuffer(void)
{
	std::string buff;
	buff.clear();
	for(int j=0;j<this->txtStrings.size();j++)
		buff.append(this->txtStrings[j]);
	this->gadgetDirty=true;
	this->CTK_updateText(buff.c_str(),false,false);
	this->needsRefresh=false;
}

/**
* Get the current source lines as vector of <String>'s.
*/
std::vector<std::string> &CTK_cursesSourceEditBoxClass::CTK_getSrcStrings(void)
{
	return(this->srcStrings);
}

/**
* Force a source language.
*/
void CTK_cursesSourceEditBoxClass::CTK_setInputLang(const char *lang)
{
	if(lang!=NULL)
		{
			this->inputLang=lang;
			this->forceLang=true;
		}
	else
		{
			this->inputLang="default.lang";
			this->forceLang=false;
		}
	this->updateBuffer();
}

/**
* Private.
*/
void CTK_cursesSourceEditBoxClass::refreshLine(void)
{
	this->gadgetDirty=true;
	//setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
	//this->gc->CTK_printLine(srcStrings[this->currentY].c_str(),this->blank.c_str(),this->sx+this->lineReserve,this->sy+this->currentY-this->startLine,this->wid-this->lineReserve);
	/////this->gc->CTK_printJustLine(srcStrings[this->currentY].c_str(),this->sx+this->lineReserve,this->sy+this->currentY-this->startLine,this->wid-this->lineReserve);
	this->gc->CTK_printJustLineColour(srcStrings[this->currentY].c_str(),this->sx+this->lineReserve,this->sy+this->currentY-this->startLine,this->wid-this->lineReserve,LEFTJUSTIFY,this->colours.foreCol,this->colours.backCol);
}

/**
* Get the current text buffer.
* \return char*
* \note Returns a copy of the buffer, caller should free.
*/
char *CTK_cursesSourceEditBoxClass::CTK_getBuffer(void)
{
	char	*retdata;

	asprintf(&retdata,"%s",this->txtBuffer);
	if(this->realAddedNL==true)
		retdata[strlen(retdata)-2]=0;
	return(retdata);
}

