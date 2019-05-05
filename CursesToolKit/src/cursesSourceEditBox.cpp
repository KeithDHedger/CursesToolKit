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
	std::string					inputLang="nohilite.lang";

	this->txtStrings.clear();
	freeAndNull(&this->txtBuffer);
	if(reset==true)
		{
			this->currentX=0;
			this->currentY=0;
			this->startLine=0;
		}

	if(isfilename==false)
		this->txtBuffer=strdup(txt);
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
	std::string lang=langMap.getMappedFileNameFromFileName(this->filePath.c_str());

	if(lang != "")
		inputLang=lang;
	else
		{
			lang=inf.infer(this->filePath.c_str());
			if(lang != "")
				{
					langMap.open();
					inputLang=langMap.getFileName(lang);
				}
		}

	if(inputLang=="")
		inputLang="nohilite.lang";
	sourceHighlight.setStyleFile("esc.style");

	sourceHighlight.highlight(inpstream,oputstream,inputLang,"");
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

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.textBoxType,false);
	if((this->txtStrings.size()-1)-this->startLine<this->hite)
		this->startLine=this->txtStrings.size()-this->hite;

	if(this->startLine<0)
		this->startLine=0;

	while((boxline<this->hite) && (boxline<this->txtStrings.size()))
		{
			if(this->showLineNumbers==true)
				{
					MOVETO(this->sx,this->sy+boxline);
//					setBackColour(this->colours.lineBackCol,this->colours.use256Colours);
//					setForeColour(this->colours.lineForeCol,this->colours.use256Colours);
					setBothColours(this->colours.lineForeCol,this->colours.lineBackCol,this->colours.use256Colours);
					if(this->lineNumbers[boxline+this->startLine]!=-1)
						printf("%.*i",4,this->lineNumbers[boxline+this->startLine]);
					else
						printf("%*s",4," ");
				}

//			setBackColour(this->colours.backCol,this->colours.use256Colours);
//			setForeColour(this->colours.foreCol,this->colours.use256Colours);
			setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
			this->gc->CTK_printLine(this->srcStrings[boxline+this->startLine].c_str(),this->sx+this->lineReserve,this->sy+boxline,this->wid-this->lineReserve);
			boxline++;
		}

	if(hilite==true)
		{
			setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
		}

	MOVETO(this->sx,this->sy+hite+1);
	printf("%s",this->blank.c_str());
	MOVETO(this->sx,this->sy+hite+1);
	printf("COL %i, LINE %i, MODE %s",this->currentX+1,this->currentY+1,this->editStatus);

	this->setScreenX();
	MOVETO(this->sx,this->sy+boxline);
}

void CTK_cursesSourceEditBoxClass::CTK_doEditEvent(void)
{
	TermKeyResult	ret;
	TermKeyKey		key;
	int				lineadd=1;
	bool			scrolled=true;
	char			tstr[3]={'_',0,0};

	this->editStatus="Edit Mode";
	this->CTK_drawBox(false,true);
	fflush(NULL);
	this->runLoop=true;
	while(this->runLoop==true)
		{
			ret=termkey_waitkey(this->tk,&key);
			lineadd=1;
			switch(key.type)
				{
					case TERMKEY_TYPE_UNICODE:
						if(key.modifiers==TERMKEY_KEYMOD_CTRL)
							{
								tstr[1]=toupper(key.code.codepoint);
								for(int j=0;j<this->mc->menuBar->menuNames.size();j++)
									{
										if(this->mc->menuBar->CTK_doShortCutKey(tstr[1],j)==true)
											{
												this->mc->menuBar->menuNumber=j;
												this->mc->menuBar->selectCB(this->mc->menuBar);
												break;
											}
									}
								break;
							}

						this->txtStrings[this->currentY].insert(this->currentX,1,key.code.codepoint);
						this->srcStrings[this->currentY]=this->txtStrings[this->currentY];
						this->isDirty=true;
						this->currentX++;
						this->needsRefresh=true;
						break;
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_BACKSPACE://TODO//speed
									case TERMKEY_SYM_DEL:
										if((this->currentY==0) && (this->currentX==0))
											break;
										this->isDirty=true;
										if(this->currentX>0)
											{
												this->txtStrings[this->currentY].erase(this->currentX-1,1);
												this->currentX--;
												this->srcStrings[this->currentY]=this->txtStrings[this->currentY];
												this->needsRefresh=true;
												break;
											}				

										if(this->currentY>0)
											{
												this->txtStrings[this->currentY-1].erase(this->txtStrings[this->currentY-1].length()-1,1);
												if(this->txtStrings[this->currentY-1].length()>0)
													this->currentX=this->txtStrings[this->currentY-1].length();

												this->txtStrings[this->currentY-1].append(this->txtStrings[this->currentY]);
												this->txtStrings.erase(this->txtStrings.begin()+this->currentY);
												this->currentY--;
												this->updateBuffer();
												this->needsRefresh=true;
											}
										break;
									case  TERMKEY_SYM_DELETE:
										{
											char	hold=this->txtStrings[this->currentY][this->currentX];
											this->isDirty=true;
											this->txtStrings[this->currentY].erase(this->currentX,1);
											this->srcStrings[this->currentY]=this->txtStrings[this->currentY];
											this->needsRefresh=true;
											if(hold=='\n')
												this->updateBuffer();
											}
										break;
									case TERMKEY_SYM_ENTER:
										this->isDirty=true;
										this->txtStrings[this->currentY].insert(this->currentX,1,'\n');
										this->currentX=0;
										this->currentY++;

										if((this->currentY-this->startLine)>=this->hite)
											this->startLine++;

										this->updateBuffer();
										break;
									case TERMKEY_SYM_TAB:
										this->isDirty=true;
										this->txtStrings[this->currentY].insert(this->currentX,1,'\t');
										this->currentX++;
										this->updateBuffer();
										break;
									case TERMKEY_SYM_ESCAPE:
										this->runLoop=false;
										this->updateBuffer();
										continue;
										break;
								case TERMKEY_SYM_HOME:
									this->currentX=0;
									break;
								case TERMKEY_SYM_END:
									this->currentX=this->txtStrings[this->currentY].length()-1;
									break;

									case TERMKEY_SYM_PAGEUP:
										lineadd=this->hite;
									case TERMKEY_SYM_UP:
										if(this->needsRefresh==true)
											this->updateBuffer();
										this->currentY-=lineadd;
										if(currentY<this->startLine)
											this->startLine-=lineadd;
										if((this->currentY<0) || (this->startLine<0))
											{
												this->currentY=0;
												this->startLine=0;
											}
										if(this->currentX>=this->txtStrings[this->currentY].length())
											this->currentX=this->txtStrings[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_PAGEDOWN:
										lineadd=this->hite;
									case TERMKEY_SYM_DOWN:
										if(this->needsRefresh==true)
											this->updateBuffer();
										this->currentY+=lineadd;
										if(this->currentY>=this->txtStrings.size())
											this->currentY=this->txtStrings.size()-1;
										if((this->currentY-this->startLine)>=this->hite)
											this->startLine+=lineadd;
										if(this->currentX>=this->txtStrings[this->currentY].length())
											this->currentX=this->txtStrings[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_LEFT:
										this->currentX--;
										if(this->currentX<0)
											{
												if(this->currentY>0)
													{
														this->currentY--;
														this->currentX=this->txtStrings[this->currentY].size()-1;
													}
												else
													this->currentX=0;
											}
										break;
									case TERMKEY_SYM_RIGHT:
										this->currentX++;
										if(this->currentX>=this->txtStrings[currentY].length())
											{
												if(this->currentY<this->txtStrings.size()-1)
													{
														this->currentY++;
														this->currentX=0;
													}
												else
													this->currentX=this->txtStrings[currentY].length()-1;
											}
										break;
								}
						}
				}

			this->CTK_drawBox(false,true);
			this->mc->CTK_emptyIPBuffer();
		}
	this->editStatus="Normal";
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



