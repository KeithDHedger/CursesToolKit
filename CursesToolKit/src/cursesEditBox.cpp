/*
 *
 * ©K. D. Hedger. Thu  4 Apr 12:38:45 BST 2019 keithdhedger@gmail.com

 * This file (cursesEditBox.cpp) is part of CursesToolKit.

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
* Edit box class destroy.
*/
CTK_cursesEditBoxClass::~CTK_cursesEditBoxClass()
{
	free(this->txtBuffer);
	delete this->gc;
	termkey_destroy(this->tk);
}

/**
* Edit box class.
*/
CTK_cursesEditBoxClass::CTK_cursesEditBoxClass()
{
	this->tk = termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}
	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
	this->bookMarks.clear();
	this->thisType=EDITBOXCLASS;
	this->userData=this;
}

/**
* Set colours etc.
*/
void CTK_cursesEditBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
	this->gc->CTK_setColours(this->colours);
}

/**
*  New edit box.
*/
void CTK_cursesEditBoxClass::CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite-1;
	this->canSelect=selectable;

	this->blank.insert(0,width,' ');
	this->CTK_updateText(txt,isfilename);
}

/**
* Update the text in the edit box.
*/
void CTK_cursesEditBoxClass::CTK_updateText(const char *txt,bool isfilename,bool reset)
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
	bool						flag=true;
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
					this->txtBuffer=(char*)malloc(fsize+2);
					fread(this->txtBuffer,1,fsize,f);
					this->txtBuffer[fsize]='\n';
					this->txtBuffer[fsize+1]=0;
				}
			fclose(f);
		}

	str=this->txtBuffer;
	this->txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',this->wid-this->lineReserve,this->tabWidth,this->sx+this->lineReserve,true);

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
					flag=true;
				}
			else
				{
					flag=false;
				}
		}
}

/**
* Draw edit box.
*/
void CTK_cursesEditBoxClass::CTK_drawBox(bool hilite,bool showcursor,bool shortupdate)
{
	int							startchr=0;
	int							j;
	int							linenum=0;
	int							boxline=0;
	std::string					 tclip;
	const char					*mark=INVERSEON "M" INVERSEOFF;
	char						charundercurs=0;
	std::vector<std::string>	edstrings;

	if(this->thisType==EDITBOXCLASS)
		edstrings=this->CTK_getStrings();
	else
		edstrings=dynamic_cast<CTK_cursesSourceEditBoxClass*>(this)->CTK_getSrcStrings();

	if((shortupdate==false) || (this->needsRefresh==true))
		{
			this->needsRefresh=false;
		//fprintf(stderr,"long draw\n");
			if(this->colours.fancyGadgets==true)
				this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.textBoxType,false);

			if((this->txtStrings.size()-1)-this->startLine<this->hite)
				this->startLine=this->txtStrings.size()-this->hite;

			if(this->startLine<0)
				this->startLine=0;

			setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
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
					this->gc->CTK_printLine(edstrings[boxline+this->startLine].c_str(),this->blank.c_str(),this->sx+this->lineReserve,this->sy+boxline,this->wid-this->lineReserve);
					boxline++;
				}
		}
	else
		{
	//	fprintf(stderr,"short draw\n");
			this->refreshLine();
		}

	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);

	tclip=this->CTK_getCurrentWord();
	if(tclip.back()=='\n')
		tclip.pop_back();
	MOVETO(this->sx,this->sy+hite+1);
	printf("\e[%iXCOL %.*i, LINE %.*i, MODE %s SELECTION %s",this->wid,this->statusCLPad,this->currentX+1,this->statusCLPad,this->currentY+1,this->editStatus,tclip.c_str());

//print cursor
	MOVETO(getColForXpos(this->txtStrings[this->currentY],this->tabWidth,this->currentX,this->sx+this->lineReserve),this->sy+this->currentY-this->startLine);
	switch(this->txtStrings[this->currentY][this->currentX])
		{
			case '\t':
			case '\n':
				charundercurs=' ';
				break;
			default:
				charundercurs=this->txtStrings[this->currentY][this->currentX];
				break;
		}	
	printf("%s%c",getBothColours(this->colours.cursBackCol,this->colours.cursForeCol,this->colours.use256Colours),charundercurs);

//higlite selection
	if(this->isSelecting==true)
		{
			for(int j=0;j<this->multiLineSels.size();j++)
				{
					MOVETO(getColForXpos(this->txtStrings[this->multiLineSels[j].line],this->tabWidth,this->multiLineSels[j].sx,this->sx+this->lineReserve),this->sy+this->multiLineSels[j].line-this->startLine);
					setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
					printf("%s",this->txtStrings[this->multiLineSels[j].line].substr(this->multiLineSels[j].sx,this->multiLineSels[j].ex-this->multiLineSels[j].sx).c_str());
				}
		}

	fflush(NULL);
}

/**
* Insert char at current position.
*/
void CTK_cursesEditBoxClass::CTK_insertChar(std::string &str,char chr)
{
//TODO//Last line
	int col;
	str.insert(this->currentX,1,chr);

	col=getColForXpos(str,this->tabWidth,str.length(),this->lineReserve+this->sx);
	this->currentX++;
	if(col>this->wid+this->sx)
		{
			col=getColForXpos(str,this->tabWidth,this->currentX,this->lineReserve+this->sx);
			if(col>=this->wid+this->sx)
				{
					this->updateBuffer();
					if(chr!='\t')
						this->currentX=0;
					else
						this->currentX=1;
					this->currentY++;
					return;
				}
			this->updateBuffer();
			return;
		}
	return;
}

/**
* Main edit/source box event loop
* \note If the mainApp class CB is set it will be called with this->userData.
* \note The default is to set this->userData to this
*/
void CTK_cursesEditBoxClass::CTK_doEvent(bool usesrc,std::vector<std::string> &lines,std::vector<std::string> &srclines)
{
	TermKeyResult	ret;
	TermKeyKey		key;
	int				lineadd=1;
	bool			shortdraw=false;
	char			tstr[3]={'_',0,0};

	if(this->canEdit==false)
		return;
	this->editStatus="Edit Mode";
	this->CTK_drawBox(false,true,shortdraw);
	fflush(NULL);
	this->runLoop=true;

	while(this->runLoop==true)
		{
			ret=termkey_waitkey(this->tk,&key);
			if(this->mc->eventLoopCBIn!=NULL)
				this->mc->eventLoopCBIn(this->mc,this->userData);

			lineadd=1;
			switch(key.type)
				{
					case TERMKEY_TYPE_UNICODE:
						if(key.modifiers==TERMKEY_KEYMOD_CTRL)
							{
								if(this->mc->menuBar->enableShortcuts==false)
									break;
								tstr[1]=toupper(key.code.codepoint);
								//tstr[1]=key.code.codepoint;
								//fprintf(stderr,"key=%c\n",tstr[1]);
								for(int j=0;j<this->mc->menuBar->menuNames.size();j++)
									{
										if(this->mc->menuBar->CTK_doShortCutKey(tstr[1],j)==true)
											{
												this->mc->menuBar->menuNumber=j;
												this->mc->menuBar->selectCB(this->mc->menuBar);
												this->mc->CTK_updateScreen(this->mc,NULL);
												
												break;
											}
									}
								break;
							}

						this->CTK_insertChar(lines[this->currentY],key.code.codepoint);
						if(usesrc==true)
							srclines[this->currentY]=lines[this->currentY];
						this->isDirty=true;
						this->needsRefresh=true;
						break;
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_BACKSPACE:
									case TERMKEY_SYM_DEL:
										if((this->currentY==0) && (this->currentX==0))
											break;
										this->isDirty=true;
										if(this->currentX>0)
											{
												lines[this->currentY].erase(this->currentX-1,1);
												if(usesrc==true)
													srclines[this->currentY]=lines[this->currentY];
												this->currentX--;
												this->needsRefresh=true;
												break;
											}				

										if(this->currentY>0)
											{
												shortdraw=false;
												lines[this->currentY-1].erase(lines[this->currentY-1].length()-1,1);
												if(lines[this->currentY-1].length()>0)
													this->currentX=lines[this->currentY-1].length();

												lines[this->currentY-1].append(lines[this->currentY]);
												lines.erase(lines.begin()+this->currentY);
												this->currentY--;
												this->updateBuffer();
												this->needsRefresh=true;
												break;
											}
										break;
									case  TERMKEY_SYM_DELETE:
										{
											char	hold=lines[this->currentY][this->currentX];
											lines[this->currentY].erase(this->currentX,1);
											if(usesrc==true)
												srclines[this->currentY]=lines[this->currentY];
											this->isDirty=true;
											this->needsRefresh=true;
											if(hold=='\n')
												{
													shortdraw=false;
													this->updateBuffer();
												}
										}
										break;
									case TERMKEY_SYM_ENTER:
										lines[this->currentY].insert(this->currentX,1,'\n');
										shortdraw=false;
										this->currentX=0;
										this->currentY++;

										if((this->currentY-this->startLine)>=this->hite)
											this->startLine++;

										this->updateBuffer();
										this->isDirty=true;
										break;
									case TERMKEY_SYM_TAB:
										this->CTK_insertChar(lines[this->currentY],'\t');
										if(usesrc==true)
											srclines[this->currentY]=lines[this->currentY];
										break;
//exit loop
									case TERMKEY_SYM_ESCAPE:
										this->runLoop=false;
										shortdraw=false;
										this->updateBuffer();
										continue;
										break;
//start selecting
									case TERMKEY_SYM_INSERT:
										if(this->isSelecting==false)
											this->CTK_startSelecting();
										else
											this->CTK_finishSelecting();
										break;

									case TERMKEY_SYM_HOME:
									case TERMKEY_SYM_FIND://console?
										this->currentX=0;
										break;
									case TERMKEY_SYM_END:
									case TERMKEY_SYM_SELECT://console?
										this->currentX=lines[this->currentY].length()-1;
										break;

									case TERMKEY_SYM_PAGEUP:
										shortdraw=false;
										lineadd=this->hite;
									case TERMKEY_SYM_UP:
										this->CTK_finishSelecting();
										this->refreshLine();
										if(this->needsRefresh==true)
											this->updateBuffer();
										this->currentY-=lineadd;
										if(currentY<this->startLine)
											{
												this->startLine-=lineadd;
												shortdraw=false;
											}
										if((this->currentY<0) || (this->startLine<0))
											{
												this->currentY=0;
												this->startLine=0;
											}
										if(this->currentX>=lines[this->currentY].length())
											this->currentX=lines[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_PAGEDOWN:
										this->CTK_finishSelecting();
										shortdraw=false;
										lineadd=this->hite;
									case TERMKEY_SYM_DOWN:
										this->refreshLine();
										if(this->needsRefresh==true)
											this->updateBuffer();
										this->currentY+=lineadd;
										if(this->currentY>=lines.size())
											this->currentY=lines.size()-1;
										if((this->currentY-this->startLine)>=this->hite)
											{
												shortdraw=false;
												this->startLine+=lineadd;
											}
										if(this->currentX>=lines[this->currentY].length())
											this->currentX=lines[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_LEFT:
										this->currentX--;
										if(this->currentX<0)
											{
												if(this->currentY>0)
													{
														this->refreshLine();
														this->currentY--;
														this->currentX=lines[this->currentY].size()-1;
													}
												else
													this->currentX=0;
											}
										break;

									case TERMKEY_SYM_RIGHT:
										this->currentX++;
										if(this->currentX>=lines[currentY].length())
											{
												if(this->currentY<lines.size()-1)
													{
														this->refreshLine();
														this->currentY++;
														this->currentX=0;
													}
												else
													this->currentX=lines[currentY].length()-1;
											}
										break;
								}
						}
				}

			if(this->isSelecting==true)
				{
					if(this->multiLineSels.back().line!=this->currentY)
						{
							this->multiLineSels.back().ex=this->txtStrings[this->multiLineSels.back().line].length();
							this->multiLineSels.push_back({this->currentY,0,this->currentX});
						}
					this->multiLineSels.back().ex=this->currentX;
				}

			this->CTK_drawBox(false,true,shortdraw);
			this->mc->CTK_emptyIPBuffer();
			if(this->eventLoopCBOut!=NULL)
				this->eventLoopCBOut(this->mc,this->userData);
			shortdraw=true;
		}
	this->editStatus="Normal";
}

/**
* Private.
*/
void CTK_cursesEditBoxClass::updateBuffer(void)
{
	std::string buff;
	buff.clear();

	for(int j=0;j<this->txtStrings.size();j++)
		buff.append(this->txtStrings[j]);
//	buff.append("\n");

	this->CTK_updateText(buff.c_str(),false,false);
}

/**
* Get the current text buffer.
*/
const char *CTK_cursesEditBoxClass::CTK_getBuffer(void)
{
	this->updateBuffer();
	return(this->txtBuffer);
}

/**
* Get current line of text.
*/
const std::string CTK_cursesEditBoxClass::CTK_getCurrentLine(void)
{
	return(this->txtStrings[this->currentY]);
}

/**
* Get word under cursor.
*/
const std::string CTK_cursesEditBoxClass::CTK_getCurrentWord(void)
{
	int startchr=this->currentX;
	int endchr=startchr;

	for(int j=this->currentX;j>=0;j--)
		if((isalnum(this->txtStrings[this->currentY][j])==false) && (this->txtStrings[this->currentY][j]!='_'))
			break;
		else
			startchr=j;

	for(int j=this->currentX;j<this->txtStrings[this->currentY].length();j++)
		if((isalnum(this->txtStrings[this->currentY][j])==false) && (this->txtStrings[this->currentY][j]!='_'))
			break;
		else
			endchr=j;
			
	return(this->txtStrings[this->currentY].substr(startchr,endchr-startchr+1));
}

/**
* Delete word under cursor.
*/
void CTK_cursesEditBoxClass::CTK_deleteCurrentWord(void)
{
	int startchr=this->currentX;
	int endchr=startchr;

	for(int j=this->currentX;j>=0;j--)
		if(isalnum(this->txtStrings[this->currentY][j])==false)
			break;
		else
			startchr=j;

	for(int j=this->currentX;j<this->txtStrings[this->currentY].length();j++)
		if(isalnum(this->txtStrings[this->currentY][j])==false)
			break;
		else
			endchr=j;
	this->txtStrings[this->currentY].erase(startchr,endchr-startchr+1);
	this->currentX=startchr;
}

/**
* Delete current line.
*/
void CTK_cursesEditBoxClass::CTK_deleteCurrentLine(void)
{
	this->txtStrings.erase(this->txtStrings.begin()+this->currentY);
	this->isDirty=true;
	this->updateBuffer();
	this->CTK_drawBox(false,true,false);
}

/**
* Insert text at cursor.
*/
void CTK_cursesEditBoxClass::CTK_insertText(const char *txt)
{
	if((txt==NULL) || (strlen(txt)==0))
		return;
	this->isDirty=true;
	this->txtStrings[this->currentY].insert(this->currentX,txt);
	this->updateBuffer();
	this->currentX+=strlen(txt);
	if(this->currentX>=this->txtStrings[this->currentY].length())
		this->currentX=this->txtStrings[this->currentY].length()-1;

	if(txt[strlen(txt)-1]=='\n')
		{
			this->currentX=0;
			this->currentY++;
			if(this->currentY-this->startLine>=this->hite)
				this->startLine++;
		}
	this->CTK_drawBox(false,true,false);
}

/**
* Go to position X at line Y.
*/
void CTK_cursesEditBoxClass::CTK_gotoXY(int x,int y)
{
	this->currentX=x;
	this->currentY=y;
	this->startLine=y;
	this->adjustXY();
}

/**
* Set the event loop.
*/
void CTK_cursesEditBoxClass::CTK_setRunLoop(bool loop)
{
	this->runLoop=loop;
}

/**
* Private
*/
void CTK_cursesEditBoxClass::adjustXY(void)
{
	if(this->currentY<0)
		this->currentY=0;
	if(this->currentY>this->txtStrings.size()-2)
		{
			this->currentY=this->txtStrings.size()-1;
			this->startLine=this->txtStrings.size()-this->hite;
		}
	if(this->currentX<0)
		this->currentX=0;
	if(this->currentX>this->txtStrings[this->currentY].length()-1)
		this->currentX=this->txtStrings[this->currentY].length()-1;
}

/**
* Set tab width.
* \note should match terminal/console tab width.
*/
void CTK_cursesEditBoxClass::CTK_setTabWidth(int width)
{
	this->tabWidth=width;
}

/**
* Set whether to show line numbers.
*/
void CTK_cursesEditBoxClass::CTK_setShowLineNumbers(int show)
{
	this->showLineNumbers=show;
	if(show!=0)
		this->lineReserve=show+2;
	else
		this->lineReserve=0;

	this->blank="";
	this->blank.insert(0,this->wid-this->lineReserve,' ');
	this->updateBuffer();
}

/**
* Goto line.
* \note goes to the start of the (unwrapped) line .
*/
void CTK_cursesEditBoxClass::CTK_gotoLine(int line)
{
	int j;

	if(line==-100)
		{
			this->currentX=0;
			this->currentY=0;
			this->startLine=0;
			this->adjustXY();
			return;
		}
	for(j=0;j<this->lineNumbers.size();j++)
		if(this->lineNumbers[j]==line)
			break;
	
	this->currentX=0;
	this->currentY=j;
	this->startLine=j;
	this->adjustXY();
}

/**
* Get the current text as vector of <String>'s.
*/
std::vector<std::string> &CTK_cursesEditBoxClass::CTK_getStrings(void)
{
	return(this->txtStrings);
}

/**
* Get the line the cursor is on.
*/
int CTK_cursesEditBoxClass::CTK_getCursLine(void)
{
	int	y=this->lineNumbers[this->currentY];
	int	cnt=0;
	while(y==-1)
		y=this->lineNumbers[this->currentY-cnt++];
	return(y);
}

/**
* Get nearest line to Y (unwrapped).
*/
int CTK_cursesEditBoxClass::CTK_getLineAtY(int y)
{
	int	yy=this->lineNumbers[y];
	while(yy==-1)
		yy=this->lineNumbers[y--];
	return(yy);
}

/**
* Get bookmark at Y.
*/
bool CTK_cursesEditBoxClass::CTK_getBookMark(int y)
{
	return(this->bookMarks[y]);
}

/**
* Get current line.
*/
int CTK_cursesEditBoxClass::CTK_getCurrentY(void)
{
	return(this->currentY);
}

/**
* Get current curs.
*/
int CTK_cursesEditBoxClass::CTK_getCurrentX(void)
{
	return(this->currentX);
}

/**
* Get height of gadget.
*/
int CTK_cursesEditBoxClass::CTK_getHeight(void)
{
	return(this->hite);
}

/**
* Set whether text is editable.
*/
void CTK_cursesEditBoxClass::CTK_setEditable(bool edit)
{
	this->canEdit=edit;
	if(edit==false)
		this->editStatus="Read Only";
	else
		this->editStatus="Normal";
}

/**
* Toggle a bookmark at Y (unwrapped).
*/
void CTK_cursesEditBoxClass::CTK_toggleBookMark(int y)
{
	for(int j=0;j<this->lineNumbers.size();j++)
		{
			if(this->lineNumbers[j]==y)
				{
					this->bookMarks[j]=!this->bookMarks[j];
					return;
				}
		}
}

/**
* Set a bookmark at Y (unwrapped) to 'set'.
*/
void CTK_cursesEditBoxClass::CTK_setBookMark(int y,bool set)
{
	for(int j=0;j<this->lineNumbers.size();j++)
		{
			if(this->lineNumbers[j]==y)
				{
					this->bookMarks[j]=set;
					return;
				}
		}
}

/**
* Get total number of lines.
*/
int CTK_cursesEditBoxClass::CTK_getLineCnt(void)
{
	return(this->txtStrings.size());
}

/**
* Private
*/
void CTK_cursesEditBoxClass::refreshLine(void)
{
	setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
	this->gc->CTK_printLine(txtStrings[this->currentY].c_str(),this->blank.c_str(),this->sx+this->lineReserve,this->sy+this->currentY-this->startLine,this->wid-this->lineReserve);
}

/**
* Start selecting lines
*/
void CTK_cursesEditBoxClass::CTK_startSelecting(void)
{
	if(this->isSelecting==true)
		return;
	this->multiLineSels.clear();
	this->multiLineSels.push_back({this->currentY,this->currentX,this->currentX});
	isSelecting=true;
}

/**
* Stop selecting lines
*/
void CTK_cursesEditBoxClass::CTK_finishSelecting(void)
{
	if(this->isSelecting==false)
		return;
	this->multiLineSels.clear();
	isSelecting=false;
	this->needsRefresh=true;
}

/**
* Get selection.
*/
std::string CTK_cursesEditBoxClass::CTK_getSelection(void)
{
	std::string str="";
	for(int j=0;j<this->multiLineSels.size();j++)
		str+=this->txtStrings[this->multiLineSels[j].line].substr(this->multiLineSels[j].sx,this->multiLineSels[j].ex-this->multiLineSels[j].sx+1);
	return(str);
}

/**
* Delete selection.
*/
void CTK_cursesEditBoxClass::CTK_deleteSelection(void)
{
	for(int j=this->multiLineSels.size()-1;j>=0;j--)
		this->txtStrings[this->multiLineSels[j].line].erase(this->multiLineSels[j].sx,this->multiLineSels[j].ex-this->multiLineSels[j].sx+1);

	this->currentX=this->multiLineSels[0].sx;
	this->currentY=this->multiLineSels[0].line;
	this->adjustXY();
	this->CTK_finishSelecting();
}










