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

CTK_cursesSourceEditBoxClass::~CTK_cursesSourceEditBoxClass()
{
}

CTK_cursesSourceEditBoxClass::CTK_cursesSourceEditBoxClass()
{
	this->tk = termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}

	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
}

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
	this->txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',this->wid-1-this->lineReserve,this->tabWidth);
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

void CTK_cursesSourceEditBoxClass::setScreenX(void)
{
	this->sourceX=0;
	while(this->srcStrings[this->currentY][this->sourceX]==0x1b)
		while(this->srcStrings[this->currentY][this->sourceX++]!='m');

	for(int j=0;j<this->currentX;j++)
		{
			while(this->srcStrings[this->currentY][this->sourceX]==0x1b)
				while(this->srcStrings[this->currentY][this->sourceX++]!='m');

			switch(this->txtStrings[this->currentY][j])
				{
					case '\t':
						this->sourceX++;
						break;
					default:
						this->sourceX++;
						break;
				}
		}

	while(this->srcStrings[this->currentY][this->sourceX]==0x1b)
		{
			while(this->srcStrings[this->currentY][this->sourceX]!='m')
				this->sourceX++;
			this->sourceX++;
		}

	MOVETO(this->sx+this->lineReserve,this->sy+this->currentY-this->startLine);
	printf("%s",this->srcStrings[this->currentY].substr(0,this->sourceX).c_str());
	switch(this->txtStrings[this->currentY][this->currentX])
		{
			case '\t':
			case '\n':
				printf( "\e[%im\e[%im " ,this->colours.cursBackCol,this->colours.cursForeCol);
				break;
			default:
				printf("\e[%im\e[%im%c",this->colours.cursBackCol,this->colours.cursForeCol,this->txtStrings[this->currentY][this->currentX]);
				break;
			}
}

void CTK_cursesSourceEditBoxClass::CTK_drawBox(bool hilite,bool showcursor)
{
	int	startchr=0;
	int j;
	int	linenum=0;
	int	boxline=0;
	std::string tclip;
	const char *mark=INVERSEON "M" INVERSEOFF;

	SETNORMAL;
	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.textBoxType,false);
	if((this->txtStrings.size()-1)-this->startLine<this->hite)
		this->startLine=this->txtStrings.size()-this->hite;

	if(this->startLine<0)
		this->startLine=0;

	while((boxline<this->hite) && (boxline<this->txtStrings.size()))
		{
			MOVETO(this->sx,this->sy+boxline);
			if(this->showLineNumbers>0)
				{
					setBothColours(this->colours.lineNumForeCol,this->colours.lineNumBackCol,this->colours.use256Colours);
					if(this->lineNumbers[boxline+this->startLine]!=-1)
						printf("%.*i",this->showLineNumbers,this->lineNumbers[boxline+this->startLine]);
					else
						printf("%*s",this->showLineNumbers," ");

					setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
					if(this->bookMarks[boxline+this->startLine]==true)
						printf(mark);
					else
						printf("  ");
				}

			setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
			this->gc->CTK_printLine(this->srcStrings[boxline+this->startLine].c_str(),this->blank.c_str(),this->sx+this->lineReserve,this->sy+boxline,this->wid-this->lineReserve);
			boxline++;
		}

	if(hilite==true)
		{
			setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
		}

	tclip=this->CTK_getCurrentWord();
	if(tclip.back()=='\n')
		tclip.pop_back();
	//printf("COL %i, LINE %i, MODE %s SELECTION %s",this->currentX+1,this->currentY+1,this->editStatus,tclip.c_str());
	MOVETO(this->sx,this->sy+hite+1);
	printf("\e[%iXCOL %i, LINE %i, MODE %s SELECTION %s",this->wid,this->currentX+1,this->currentY+1,this->editStatus,tclip.c_str());

	this->setScreenX();
	fflush(NULL);
}

void CTK_cursesSourceEditBoxClass::updateBuffer(void)
{
	std::string buff;
	buff.clear();
	for(int j=0;j<this->txtStrings.size();j++)
		buff.append(this->txtStrings[j]);

	this->CTK_updateText(buff.c_str(),false,false);
	this->needsRefresh=false;
}

std::vector<std::string> &CTK_cursesSourceEditBoxClass::CTK_getSrcStrings(void)
{
	return((this->srcStrings));
}

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

