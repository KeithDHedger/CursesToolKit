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
 
#ifdef _SOURCEHIGHLIGHT_
#define SRCDATADIR "/usr/share/source-highlight"
#include <srchilite/sourcehighlight.h>
#include <srchilite/langmap.h>
#include <srchilite/lineranges.h>
#include <srchilite/sourcehighlight.h>
#include <srchilite/languageinfer.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>

#include "cursesGlobals.h"


/**
*  Source code box class destroy.
*/
CTK_cursesSourceEditBoxClass::~CTK_cursesSourceEditBoxClass()
{
}

/**
*  Source code box class.
*/
CTK_cursesSourceEditBoxClass::CTK_cursesSourceEditBoxClass(CTK_mainAppClass *mc,bool native)
{
	varsStruct	vsitem;

#ifndef _SOURCEHIGHLIGHT_
	this->useNative=true;
#else
	this->useNative=native;
#endif

	this->CTK_setCommon(mc);

	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("srcforecol",FORE_WHITE);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("srcbackcol",BACK_BLACK);
	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("srchiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("srchilitebackcol",this->gadgetColours.hiliteBackCol);
	this->gadgetColours.gadgetCustom1ForeCol=this->mc->gc->CTK_getColourFromNamedVar("srclineforecol",FORE_YELLOW);
	this->gadgetColours.gadgetCustom1BackCol=this->mc->gc->CTK_getColourFromNamedVar("srclinebackcol",BACK_BLACK);
	this->gadgetColours.gadgetCustom2ForeCol=this->mc->gc->CTK_getColourFromNamedVar("srccursorforecol",FORE_BLACK);
	this->gadgetColours.gadgetCustom2BackCol=this->mc->gc->CTK_getColourFromNamedVar("srccursorbackcol",BACK_GREEN);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"srcfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"srcboxtype");
	if(vsitem.vType==INTVAR)
		this->gadgetColours.boxType=vsitem.intVar;

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
#ifdef _SOURCEHIGHLIGHT_
	std::stringstream			inpstream;
	std::ostringstream			oputstream;
	srchilite::LanguageInfer	inf;
	srchilite::SourceHighlight	sourceHighlight("esc.outlang");
	srchilite::LangMap			langMap(SRCDATADIR,"lang.map");
#endif
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
					this->srcClass.findFileType(txt);
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
#ifdef _SOURCEHIGHLIGHT_
	if(this->useNative==false)
		{
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
			inpstream << std::endl; 
			sourceHighlight.setDataDir(SRCDATADIR);
			std::string lang="";
			if((this->filePath.compare("")!=0) && (this->forceLang==false))
				{
					lang=langMap.getMappedFileNameFromFileName(this->filePath);
					if(lang != "")
						this->inputLang=lang;
					else
						{
							lang=inf.infer(this->filePath);

							if(lang != "")
								{
									langMap.open();
									this->inputLang=langMap.getFileName(lang);
								}
						}
				}

			if(this->forceLang==false)
				{
					if(lang=="")
						this->inputLang="nohilite.lang";

					if(this->inputLang.length()==0)
						this->inputLang="cpp.lang";
				}

			sourceHighlight.setStyleFile(this->styleFile);
			sourceHighlight.highlight(inpstream,oputstream,this->inputLang,"");
			this->srcStrings=cu.CTK_explode(oputstream.str(),'\n');
		}
#endif

	if(this->useNative==true)
		{
			this->srcClass.data="";

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
							this->srcClass.data+=this->txtStrings[j];
							flag=true;
						}
					else
						{
							this->srcClass.data+=this->txtStrings[j];
							this->srcClass.data+='\n';
							flag=false;
						}
				}

			this->srcClass.data+="\n";
			this->srcClass.getColourFile(this->styleFile);
			this->srcClass.hilight();
			this->srcStrings=cu.CTK_explode(this->srcClass.getHighlightedData(),'\n');
		}
}

void CTK_cursesSourceEditBoxClass::CTK_setStyleFile(const char *filepath)
{
	this->styleFile=filepath;
	this->updateBuffer();
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
* \note use for the external source-highlight lib.
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
* Force a source language.
* \note use for the internal syntax hilighter.
*/
void CTK_cursesSourceEditBoxClass::CTK_setInputLang(srcFileType lang)
{
	if(lang!=(srcFileType)NULL)
		{
			this->forceLang=true;
			this->srcClass.setSrcLang(lang);
		}
	else
		{
			this->forceLang=false;
			this->srcClass.setSrcLang(PLAIN);
		}
	this->updateBuffer();
}

void CTK_cursesSourceEditBoxClass::CTK_setUseNative(bool native)
{
	this->useNative=native;
	this->updateBuffer();
}

/**
* Private.
*/
void CTK_cursesSourceEditBoxClass::refreshLine(void)
{
	this->gadgetDirty=true;
	this->gc->CTK_printJustLineColour(srcStrings[this->currentY].c_str(),this->sx+this->lineReserve,this->sy+this->currentY-this->startLine,this->wid-this->lineReserve,LEFTJUSTIFY,this->gadgetColours.foreCol,this->gadgetColours.backCol);
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

