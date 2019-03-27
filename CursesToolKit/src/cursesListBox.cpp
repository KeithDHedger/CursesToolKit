/*
 *
 * ©K. D. Hedger. Thu 14 Mar 16:11:26 GMT 2019 keithdhedger@gmail.com

 * This file (cursesLists.cpp) is part of CursesLists.

 * CursesLists is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesLists is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesLists.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cursesListBox.h"

CTK_cursesListBox::~CTK_cursesListBox()
{
//	for(int j=0;j<listNames.size();j++)
//		{
//			this->CTK_clearList(j);
//		}
//	this->listNames.clear();
//	termkey_destroy(this->tk);
}

CTK_cursesListBox::CTK_cursesListBox()
{
//	winsize	w;
//	tk = termkey_new(0,TERMKEY_FLAG_CTRLC);
//	if(!tk)
//		{
//			fprintf(stderr, "Cannot allocate termkey instance\n");
//			exit(1);
//		}
//
//    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
//	rows=w.ws_row;
//	maxRows=rows-mBarHite+1;
//	listHite=rows-mBarHite;
//	this->listNames.clear();
}

//void CTK_cursesListBox::CTK_drawListBar(void)
//{
//	int	x=1;
//	int y=1;
//
//	for(int j=0;j<this->listCnt;j++)
//		{
//			this->listWidth=strlen(this->listNames[j]->listName);
//			this->drawListStyle(j,-1,x,y,FLATNORM,true,false);
//			x+=strlen(this->listNames[j]->listName);
//		}
//	SETNORMAL;
//}
//
//char CTK_cursesListBox::setShortCut(const char *name)
//{
//	for(int j=0;j<strlen(name);j++)
//		{
//			if(name[j]=='_')
//				return(toupper(name[j+1]));
//		}
//	return(0);
//}

void CTK_cursesListBox::CTK_addListItem(const char *itemtxt)
{
//	listStruct	*list=new listStruct;
//
//	list->listName=strdup(name);
//	list->key=setShortCut(name);
//	list->maxWidth=strlen(name);
//	if(list->key!=0)
//		if(this->listNames[listnum]->maxWidth<strlen(name)+strlen("Ctrl+X"))
//			this->listNames[listnum]->maxWidth=strlen(name)+strlen("Ctrl+X");
//	list->listItem.clear();
//	this->listNames[listnum]->listItem[this->listNames[listnum]->listItemCnt++]=list;
	this->listItems.push_back(itemtxt);
}
//
//void CTK_cursesListBox::CTK_addListToBar(const char *name)
//{
//	listStruct	*list=new listStruct;
//
//	list->listName=strdup(name);
//	list->key=0;
//	list->startCol=this->listNamesStartX+1;
//	this->listNamesStartX+=strlen(name);
//	list->listItem.clear();
//
//	this->listNames[this->listCnt++]=list;
//}

void CTK_cursesListBox::drawListStyle(int listnum,int listitem,int x,int y,int style,bool doshortcut,bool dopad)
{
//	bool		gotus=false;
//	listStruct	*thelist;
//
//	MOVETO(x,y);
//	switch(style)
//		{
//			case FLATINVERT:
//				setBackColour(this->hiliteBackCol);
//				setForeColour(this->hiliteForeCol);
//				break;
//			case FLATNORM:
//				setBackColour(this->backCol);
//				setForeColour(this->foreCol);
//				break;
//		}
//
//	if(listitem<0)
//		thelist=this->listNames[listnum];
//	else
//		thelist=this->listNames[listnum]->listItem[listitem];
//
//	for(unsigned j=0;j<this->listWidth;j++)
//		{
//			if(j<strlen(thelist->listName))
//				{
//					if((thelist->listName[j]=='_') && (doshortcut==true))
//						{
//							j++;
//							printf("%s%c%s",UNDERSCOREON,thelist->listName[j],UNDERSCOREOFF);
//							gotus=true;
//						}
//					else
//						printf("%c",thelist->listName[j]);
//				}
//			else
//				printf(" ");
//		}
//
//	if((gotus==true) && (dopad==true))
//		{
//			MOVETO(x+this->listWidth-6,y);
//			printf(" Ctrl+%c ",thelist->key);
//		}
//
//	if(gotus==false)
//		printf("  ");
//	fflush(NULL);
}

void CTK_cursesListBox::CTK_drawListWindow(bool hilite)
{
	for(int j=0;j<this->listItems.size();j++)
		{
			//if((hilite==true) && (this->listItemNumber==j))
			if(hilite==true)
				{
					setBackColour(this->hiliteBackCol);
					setForeColour(this->hiliteForeCol);
				}
			else
				{
					setBackColour(this->backCol);
					setForeColour(this->foreCol);
				}
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->blank.c_str());
			MOVETO(this->sx,this->sy+j);
			printf("%s",this->listItems[j].c_str());
			//MOVETO(this->sx+this->curs,this->sy);
		}

//		}
//	int	y=sy+1;
//	int	maxitems=0;
//	int msx=this->listNames[listnum]->startCol;
//
//	this->listWidth=0;
//
//	maxitems=this->listNames[listnum]->listItemCnt;
//	this->listWidth=this->listNames[listnum]->maxWidth;
//	if(doshortcut==true)
//		this->listWidth--;
//
//	for(int cnt=0;cnt<this->listNames[listnum]->listItem.size();cnt++)
//		{
//			if(prelight==cnt)
//				this->drawListStyle(listnum,cnt+this->listStart,msx,y++,FLATINVERT,doshortcut,true);
//			else
//				this->drawListStyle(listnum,cnt+this->listStart,msx,y++,FLATNORM,doshortcut,true);
//		}
//	SETNORMAL;
//	return(maxitems);
}

int CTK_cursesListBox::CTK_doListEvent(int sx,int sy,bool doshortcut)
{
#if 0
	bool			loop=true;
	bool			mainloop=true;
	int				selection=0;
	int				maxitems;
	int				cnt=0;
	char			tstr[3]={'_',0,0};
	char			buffer[32];

	TermKeyResult	ret;
	TermKeyKey		key;
	TermKeyFormat	format=TERMKEY_FORMAT_VIM;

	if(this->listNumber<0)
		return(CONT);

	while(mainloop==true)
		{
			if(this->listNames[this->listStart]!=NULL)
				maxitems=this->drawListWindow(this->listNumber,sx,1,-1,doshortcut);

			this->CTK_drawListBar();
			SETHIDECURS;
			fflush(NULL);
			loop=true;
			while(loop==true)
				{
					ret=termkey_waitkey(tk,&key);
					termkey_strfkey(tk,buffer,32,&key,format);

					switch(key.type)
						{
							case TERMKEY_TYPE_KEYSYM:
								{
									switch(key.code.sym)
										{
											case TERMKEY_SYM_ESCAPE:
												loop=false;
												selection=BRAKE;
												return(selection);
												break;
											case TERMKEY_SYM_UP:
												selection--;
												if(selection<1)
													{
														selection=1;
														if(listStart>0)
															listStart--;
													}
												this->drawListWindow(this->listNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_DOWN:
												selection++;
												if((selection>maxitems) || (selection>listHite))
													selection=listHite-mBarHite;
												if(selection>maxitems)
													selection=maxitems;

												if(selection+listStart<=maxitems)
													{
														if(selection>listHite-mBarHite)
															{
																selection=listHite-mBarHite;
																listStart++;
															}
													}
												else
													selection--;
												this->drawListWindow(this->listNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_LEFT:
												listStart=0;
												this->listNumber--;
												if(this->listNumber<0)
													this->listNumber=0;
												loop=false;
												selection=0;
												continue;
												break;
											case TERMKEY_SYM_RIGHT:
												listStart=0;
												selection=0;
												loop=false;
												this->listNumber++;
												if(this->listNumber>=this->listCnt)
													this->listNumber--;
												continue;
												break;
											case TERMKEY_SYM_HOME:
												selection=1;
												listStart=0;
												this->drawListWindow(this->listNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_END:
												if(maxitems>rows-2)
													{
														selection=rows-2;
														listStart=maxitems-rows+2;
														if((listStart+rows-2)>maxitems)
															listStart=maxitems-rows+2;
													}
												else
													selection=maxitems;
												this->drawListWindow(this->listNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_PAGEUP:
												listStart-=listHite;
												if(listStart<0)
													listStart=0;
												this->drawListWindow(this->listNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_PAGEDOWN:
												if(maxitems<listHite)
													continue;
												listStart+=listHite-1;
												if((listStart+listHite)>maxitems)
													listStart=maxitems-listHite+1;
												this->drawListWindow(this->listNumber,sx,1,selection-1,doshortcut);
												continue;
												break;
											case TERMKEY_SYM_ENTER:
												this->listItemNumber=selection+this->listStart-1;
												this->selectCB(this);
												return(SELECTED);
												break;
											default:
												break;
										}
								}

							case TERMKEY_TYPE_UNICODE:
								if(key.modifiers==0)
									{
										tstr[1]=toupper(key.code.codepoint);
										if(this->CTK_doShortCutKey(tstr[1],this->listNumber)==true)
											{
												this->selectCB(this);
												return(SELECTED);
											}
									}
								break;

							default:
								break;
						}
				}
			this->updateCB((void*)this->mainApp,NULL);
		}
	SETNORMAL;
	return(selection);

#endif
}

void CTK_cursesListBox::CTK_clearList(void)
{
//	for(int j=0;j<this->listNames[listnum]->listItem.size();j++)
//		{
//			free(this->listNames[listnum]->listItem[j]->listName);
//			this->listNames[listnum]->listItem.clear();
//		}
//	this->listNames[listnum]->listItemCnt=0;
}
//
//bool CTK_cursesListBox::CTK_doShortCutKey(char key,int listnum)
//{
//	for(int k=0;k<this->listNames[listnum]->listItem.size();k++)
//		{
//			if(key==this->listNames[listnum]->listItem[k]->key)
//				{
//					this->listItemNumber=k;
//					return(true);
//				}
//		}
//	return(false);
//}
//
//void CTK_cursesListBox::CTK_setUpdateCB(void (*update)(void *,void*),void* mainapp)
//{
//	this->updateCB=update;
//	this->mainApp=mainapp;
//}

void CTK_cursesListBox::CTK_setSelectCB(void (*select)(void *))
{
	this->selectCB=select;
}

void CTK_cursesListBox::CTK_newListBox(int x,int y,int width,int hite)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
//	this->canSelect=selectable;

	this->blank.insert(this->blank.begin(),width,' ');
}


