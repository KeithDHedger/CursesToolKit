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
 
#include "cursesSourceEditBox.h"

CTK_cursesSourceEditBoxClass::~CTK_cursesSourceEditBoxClass()
{
	free(this->txtBuffer);
	delete this->gc;
	termkey_destroy(this->tk);
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

void CTK_cursesSourceEditBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
	this->gc->CTK_setColours(this->colours);
}

void CTK_cursesSourceEditBoxClass::CTK_newBox(int x,int y,int width,int hite,bool isfilename,const char *txt,bool selectable)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite-1;
	this->canSelect=selectable;

	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt,isfilename);
}

void CTK_cursesSourceEditBoxClass::CTK_updateText(const char *txt,bool isfilename,bool reset)
{
	const char					*ptr=NULL;
	char						*buffer=NULL;
	int							startchr=0;
	char						*cpybuf=(char*)alloca(this->wid+1+1000);
	std::vector<std::string>	array;
	std::string					str;
	CTK_cursesUtilsClass		cu;
	long						fsize;
	FILE						*f;

	this->txtstrings.clear();
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

	str=this->txtBuffer;
	array=cu.CTK_cursesUtilsClass::CTK_explode(str,'\n');
	for(int j=0;j<array.size();j++)
		{
			ptr=array[j].c_str();
			int numchars=0;
			int cnt=0;
			startchr=0;
			asprintf(&buffer,"%s\n",ptr);
			while(cnt<strlen(buffer))
				{
					while((numchars<this->wid) && (cnt<strlen(buffer)))
						{
							cpybuf[startchr]=buffer[cnt++];
							if(cpybuf[startchr]==0x1b)
								{
									startchr++;
									while(buffer[cnt]!='m')
										{
											cpybuf[startchr++]=buffer[cnt++];
										}
									//cpybuf[startchr]=buffer[cnt++];
									//continue;
								}
							else
								{
							if(cpybuf[startchr]=='\t')
								numchars+=8;
							else
								numchars++;
							startchr++;
								}
						}
					cpybuf[startchr]=0;
					this->txtstrings.push_back(cpybuf);
					startchr=0;
					numchars=0;
				}
			free(buffer);
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

	setBackColour(this->colours.backCol,this->colours.use256Colours);
	setForeColour(this->colours.foreCol,this->colours.use256Colours);
	MOVETO(this->sx,this->sy+hite+1);
	printf("%s",this->blank.c_str());
	MOVETO(this->sx,this->sy+hite+1);
	printf("COL %i, LINE %i, MODE %s",this->currentX+1,this->currentY+1,this->editStatus);

	if(hilite==true)
		{
			setBackColour(this->colours.hiliteBackCol,this->colours.use256Colours);
			setForeColour(this->colours.hiliteForeCol,this->colours.use256Colours);
		}
	else
		{
			setBackColour(this->colours.backCol,this->colours.use256Colours);
			setForeColour(this->colours.foreCol,this->colours.use256Colours);
		}

	for(int j=0;j<this->hite;j++)
		{
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());
		}

	if((this->txtstrings.size()-1)-this->startLine<this->hite)
		this->startLine=this->txtstrings.size()-this->hite;

	if(this->startLine<0)
		this->startLine=0;


	while((boxline<this->hite) && (boxline<this->txtstrings.size()))
		{
			MOVETO(this->sx,this->sy+boxline);
			printf("%s",this->txtstrings[boxline+this->startLine].c_str());
			if((this->currentY==boxline+this->startLine) && (showcursor==true))
				{
					MOVETO(this->sx,this->sy+boxline);
					for(j=0;j<this->currentX;j++)
						{
							printf("%c",this->txtstrings[boxline+this->startLine].c_str()[j]);
						}

					switch(this->txtstrings[boxline+this->startLine].c_str()[j])
						{
							case 0x1b:
								while(this->txtstrings[boxline+this->startLine].c_str()[j]==0x1b)
									{
										while(this->txtstrings[boxline+this->startLine].c_str()[j++]!='m');
										j++;
									}
								printf( INVERSEON "%c" INVERSEOFF,this->txtstrings[boxline+this->startLine].c_str()[j]);
								break;
							case '\t':
								printf(INVERSEON SPACETAB INVERSEOFF);
								break;
							case '\n':
								printf(INVERSEON SPACENL INVERSEOFF);
								break;
							default:
								printf( INVERSEON "%c" INVERSEOFF,this->txtstrings[boxline+this->startLine].c_str()[j]);
								break;
						}
					j++;
					while(j<this->txtstrings[boxline+this->startLine].length())
						printf("%c",this->txtstrings[boxline+this->startLine].c_str()[j++]);
				}
			boxline++;
		}
	MOVETO(this->sx,this->sy+boxline);
}

void CTK_cursesSourceEditBoxClass::CTK_doEditEvent(void)
{
	TermKeyResult	ret;
	TermKeyKey		key;
	int				lineadd=1;

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

						this->txtstrings[this->currentY].insert(this->currentX,1,key.code.codepoint);
						this->currentX++;
						break;
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_BACKSPACE:
										if((this->currentY==0) && (this->currentX==0))
											break;
										if(this->currentX>0)
											{
												this->txtstrings[this->currentY].erase(this->currentX-1,1);
												this->currentX--;
												this->updateBuffer();
												break;
											}				

										if(this->currentY>0)
											{
												this->txtstrings[this->currentY-1].erase(this->txtstrings[this->currentY-1].length()-1,1);
												if(this->txtstrings[this->currentY-1].length()>0)
													this->currentX=this->txtstrings[this->currentY-1].length();

												this->txtstrings[this->currentY-1].append(this->txtstrings[this->currentY]);
												this->txtstrings.erase(this->txtstrings.begin()+this->currentY);
												this->currentY--;
												this->updateBuffer();
												break;
											}
										this->updateBuffer();
										this->CTK_drawBox(false,true);
										break;
									case  TERMKEY_SYM_DELETE:
										this->txtstrings[this->currentY].erase(this->currentX,1);
										this->updateBuffer();
										break;
									case TERMKEY_SYM_ENTER:
										this->txtstrings[this->currentY].insert(this->currentX,1,'\n');
										this->currentX=0;
										this->currentY++;

										if((this->currentY-this->startLine)>=this->hite)
											this->startLine++;

										this->updateBuffer();
										break;
									case TERMKEY_SYM_TAB:
										this->txtstrings[this->currentY].insert(this->currentX,1,'\t');
										this->currentX++;
										break;
									case TERMKEY_SYM_ESCAPE:
										this->runLoop=false;
										continue;
										break;
								case TERMKEY_SYM_HOME:
									this->currentX=0;
									break;
								case TERMKEY_SYM_END:
									this->currentX=this->txtstrings[this->currentY].length()-1;
									break;

									case TERMKEY_SYM_PAGEUP:
										lineadd=this->hite;
									case TERMKEY_SYM_UP:
										this->currentY-=lineadd;
										if(currentY<this->startLine)
											this->startLine-=lineadd;
										if((this->currentY<0) || (this->startLine<0))
											{
												this->currentY=0;
												this->startLine=0;
											}
										if(this->currentX>=this->txtstrings[this->currentY].length())
											this->currentX=this->txtstrings[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_PAGEDOWN:
										lineadd=this->hite;
									case TERMKEY_SYM_DOWN:
										this->currentY+=lineadd;
										if(this->currentY>=this->txtstrings.size())
											this->currentY=this->txtstrings.size()-1;
										if((this->currentY-this->startLine)>=this->hite)
											this->startLine+=lineadd;
										if(this->currentX>=this->txtstrings[this->currentY].length())
											this->currentX=this->txtstrings[this->currentY].length()-1;
										break;
									case TERMKEY_SYM_LEFT:
										this->currentX--;
										if(this->currentX<0)
											{
												if(this->currentY>0)
													{
														this->currentY--;
														this->currentX=this->txtstrings[this->currentY].size()-1;
													}
												else
													this->currentX=0;
											}
										break;
									case TERMKEY_SYM_RIGHT:
										this->currentX++;
//										if(this->txtstrings[this->currentY].c_str()[this->currentX]==0x1b)
//											{
//											fprintf(stderr,"currentx=%i\n%c",this->txtstrings[this->currentY].c_str()[this->currentX]);
//												while(this->txtstrings[this->currentY].c_str()[this->currentX]!='m')
//													{
//														this->currentX++;
//													fprintf(stderr,"%c",this->txtstrings[this->currentY].c_str()[this->currentX]);
//													}
//												this->currentX++;
//												fprintf(stderr,"%c\n",this->txtstrings[this->currentY].c_str()[this->currentX]);
//											}

										if(this->currentX>=this->txtstrings[currentY].length())
											{
												if(this->currentY<this->txtstrings.size()-1)
													{
														this->currentY++;
														this->currentX=0;
													}
												else
													this->currentX=this->txtstrings[currentY].length()-1;
											}
										break;
								}
						}
				}
			this->skipAnsi();
			this->CTK_drawBox(false,true);
		}
	this->editStatus="Normal";
}

void CTK_cursesSourceEditBoxClass::updateBuffer(void)
{
	std::string buff;
	buff.clear();

	for(int j=0;j<this->txtstrings.size();j++)
		buff.append(this->txtstrings[j]);

	this->CTK_updateText(buff.c_str(),false,false);
}

const char *CTK_cursesSourceEditBoxClass::CTK_getBuffer(void)
{
	this->updateBuffer();
	return(this->txtBuffer);
}

const std::string CTK_cursesSourceEditBoxClass::CTK_getCurrentLine(void)
{
	return(this->txtstrings[this->currentY]);
}

const std::string CTK_cursesSourceEditBoxClass::CTK_getCurrentWord(void)
{
	int startchr=this->currentX;
	int endchr=startchr;

	for(int j=this->currentX;j>=0;j--)
		if(isalnum(this->txtstrings[this->currentY][j])==false)
			break;
		else
			startchr=j;

	for(int j=this->currentX;j<this->txtstrings[this->currentY].length();j++)
		if(isalnum(this->txtstrings[this->currentY][j])==false)
			break;
		else
			endchr=j;
			
	return(this->txtstrings[this->currentY].substr(startchr,endchr-startchr+1));
}

void CTK_cursesSourceEditBoxClass::CTK_deleteCurrentWord(void)
{
	int startchr=this->currentX;
	int endchr=startchr;

	for(int j=this->currentX;j>=0;j--)
		if(isalnum(this->txtstrings[this->currentY][j])==false)
			break;
		else
			startchr=j;

	for(int j=this->currentX;j<this->txtstrings[this->currentY].length();j++)
		if(isalnum(this->txtstrings[this->currentY][j])==false)
			break;
		else
			endchr=j;
	this->txtstrings[this->currentY].erase(startchr,endchr-startchr+1);
}

void CTK_cursesSourceEditBoxClass::CTK_deleteCurrentLine(void)
{
	this->txtstrings.erase(this->txtstrings.begin()+this->currentY);
}


void CTK_cursesSourceEditBoxClass::CTK_insertText(const char *txt)
{
	this->txtstrings[this->currentY].insert(this->currentX,txt);
	this->updateBuffer();
	this->currentX+=strlen(txt);
	if(this->currentX>=this->txtstrings[this->currentY].length())
		this->currentX=this->txtstrings[this->currentY].length()-1;
}

void CTK_cursesSourceEditBoxClass::CTK_gotoXY(int x,int y)
{
	this->currentX=x;
	this->currentY=y;
	this->startLine=y;
	this->adjustXY();
}

void CTK_cursesSourceEditBoxClass::CTK_setRunLoop(bool loop)
{
	this->runLoop=loop;
}

void CTK_cursesSourceEditBoxClass::skipAnsi(void)
{
//this->adjustXY();
if((this->txtstrings[this->currentY].c_str()[this->currentX]=='m') && (this->txtstrings[this->currentY].c_str()[this->currentX-2]==ESCCHAR))
	{
		this->currentX-=3;
		this->skipAnsi();
		return;
	}
if((this->txtstrings[this->currentY].c_str()[this->currentX]=='m') && (this->txtstrings[this->currentY].c_str()[this->currentX-7]==ESCCHAR))
	{
		this->currentX-=8;
		this->skipAnsi();
		return;
	}
	//if(this->txtstrings[this->currentY].c_str()[this->currentX]==0x1b)
	while(this->txtstrings[this->currentY].c_str()[this->currentX]==0x1b)
		{
			//fprintf(stderr,"currentx=%i\n%c",this->txtstrings[this->currentY].c_str()[this->currentX]);
			while(this->txtstrings[this->currentY].c_str()[this->currentX]!='m')
				{
					this->currentX++;
					//fprintf(stderr,"%c",this->txtstrings[this->currentY].c_str()[this->currentX]);
				}
			this->currentX++;
			//fprintf(stderr,"%c\n",this->txtstrings[this->currentY].c_str()[this->currentX]);
		}

//	this->skipAnsi();
}

void CTK_cursesSourceEditBoxClass::adjustXY(void)
{
	if(this->currentY<0)
		this->currentY=0;
	if(this->currentY>this->txtstrings.size()-2)
		{
			this->currentY=this->txtstrings.size()-1;
			this->startLine=this->txtstrings.size()-this->hite;
		}
	if(this->currentX<0)
		this->currentX=0;
	if(this->currentX>this->txtstrings[this->currentY].length()-1)
		this->currentX=this->txtstrings[this->currentY].length()-1;
}


