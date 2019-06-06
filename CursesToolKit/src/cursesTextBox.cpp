/*
 *
 * ©K. D. Hedger. Sat 30 Mar 16:50:24 GMT 2019 keithdhedger@gmail.com

 * This file (cursesTextBox.cpp) is part of CursesToolKit.

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
* Text box class destroy.
*/
CTK_cursesTextBoxClass::~CTK_cursesTextBoxClass()
{
	delete this->gc;
}

/**
* Text box class.
*/
CTK_cursesTextBoxClass::CTK_cursesTextBoxClass()
{
	this->gc=new CTK_cursesGraphicsClass;
	this->gc->CTK_setColours(this->colours);
}

/**
* New text box.
*/
void CTK_cursesTextBoxClass::CTK_newBox(int x,int y,int width,int hite,const char *txt,bool selectable)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->canSelect=selectable;

	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt);
}


/**
* Update the text in the gadget.
*/
void CTK_cursesTextBoxClass::CTK_updateText(const char *txt,bool isfilename,bool reset)
{
	const char					*ptr=NULL;
	char						*buffer=NULL;
	int							startchr=0;
	std::vector<std::string>	array;
	std::string					str;
	CTK_cursesUtilsClass		cu;
	long						fsize;
	FILE						*f;
	bool						flag=true;
	char						*txtbuffer;

	this->txtStrings.clear();
	if(isfilename==false)
		this->text=txt;
	else
		{
			f=fopen(txt,"rb");
			fseek(f,0,SEEK_END);
			fsize=ftell(f);
			if(fsize==0)
				{
					txtbuffer=(char*)malloc(2);
					sprintf(txtbuffer,"\n");
				}
			else
				{
					fseek(f,0,SEEK_SET);
					txtbuffer=(char*)malloc(fsize+1);
					fread(txtbuffer,1,fsize,f);
					txtbuffer[fsize]=0;
				}
			fclose(f);
			this->text=txtbuffer;
			free(txtbuffer);
		}

	str=this->text;
	this->txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',this->wid-1,this->tabWidth);
}

/**
* Draw gadget.
*/
void CTK_cursesTextBoxClass::CTK_drawBox(bool hilite)
{
	int xcnt=0;
	int ycnt=0;
	int j=0;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.textBoxType,false);

	if(this->txtStrings.size()==0)
		return;

	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
	else
		setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);

	while(j<this->hite)
		{
			if(j<this->txtStrings.size())
				{
					this->gc->CTK_printLine(this->txtStrings[j+this->startLine].c_str(),this->blank.c_str(),this->sx,this->sy+j,this->wid);
					j++;
				}
			else
				return;
		}		
	MOVETO(this->sx,this->sy+100);
	fflush(NULL);
}

/**
* Scroll text by 1 line.
* \param bool true=scroll up.
*/
void CTK_cursesTextBoxClass::CTK_scrollLine(bool scrollup)
{
	this->TBscroll(scrollup,1);
}

/**
* Scroll text by 1 page.
* \param bool true=scroll up.
*/
void CTK_cursesTextBoxClass::CTK_scrollPage(bool scrollup)
{
	this->TBscroll(scrollup,this->hite);
}

/**
* Private.
*/
void CTK_cursesTextBoxClass::TBscroll(bool scrollup,int numlines)
{
	if(scrollup==true)
		{
			this->startLine-=numlines;
			if(this->startLine<0)
				this->startLine=0;
		}
	else
		{
			this->startLine+=numlines;
			if(this->txtStrings.size()-this->startLine<this->hite)
				this->startLine=this->txtStrings.size()-this->hite;
		}
}

/**
* Set whether gadget is selectable.
*/
void CTK_cursesTextBoxClass::CTK_setSelectable(bool canselect)
{
	this->canSelect=canselect;
}

/**
* Get whether gadget is selectable.
*/
bool CTK_cursesTextBoxClass::CTK_getSelectable(void)
{
	return(this->canSelect);
}

/**
* Get text of gadget.
*/
const std::string CTK_cursesTextBoxClass::CTK_getText(void)
{
	return(this->text);
}

/**
* Set gadget colours etc.
*/
void CTK_cursesTextBoxClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
	this->gc->CTK_setColours(this->colours);
}


