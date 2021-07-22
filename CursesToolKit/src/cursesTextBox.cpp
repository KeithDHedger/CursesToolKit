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
}

/**
* Text box class.
*/
CTK_cursesTextBoxClass::CTK_cursesTextBoxClass(CTK_mainAppClass *mc)
{
	varsStruct	vsitem;

	this->CTK_setCommon(mc);
	
	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("textforecol",this->mc->windowColours.foreCol);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("textbackcol",this->mc->windowColours.backCol);
	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("texthiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("texthilitebackcol",this->gadgetColours.hiliteBackCol);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"textfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"textboxtype");
	if(vsitem.vType==INTVAR)
		this->gadgetColours.boxType=vsitem.intVar;
	else
		this->gadgetColours.boxType=OUTBOX;

	this->type=TEXTGADGET;
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
	this->blank.insert(this->blank.begin(),width,' ');
	this->CTK_updateText(txt,false,false);
	this->gadgetDirty=true;
	this->isSelectable=selectable;
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

	this->gadgetDirty=true;
	this->txtStrings.clear();
	if(isfilename==false)
		this->text=txt;
	else
		{
			f=fopen(txt,"rb");
			if(f!=NULL)
				{
					fseek(f,0,SEEK_END);
					fsize=ftell(f);
					if(fsize==0)
						asprintf(&txtbuffer,"\n");
					else
						{
							fseek(f,0,SEEK_SET);
							txtbuffer=(char*)malloc(fsize+1);
							fread(txtbuffer,1,fsize,f);
							txtbuffer[fsize]=0;
						}
					fclose(f);
				}
			else
				asprintf(&txtbuffer,"%s",txt);

			this->text=txtbuffer;
			free(txtbuffer);
		}

	str=this->text;
	this->txtStrings=cu.CTK_cursesUtilsClass::CTK_explodeWidth(str,'\n',this->wid-1,this->tabWidth);
	if(reset==true)
		this->CTK_drawGadget();
}

/**
* Draw gadget.
*/
void CTK_cursesTextBoxClass::CTK_drawGadget(bool hilite)
{
	int xcnt=0;
	int ycnt=0;
	int j;

	if(this->gadgetDirty==false)
		return;

	this->hiLited=hilite;

	if(this->gadgetColours.useFancy==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->gadgetColours.boxType,false);

	if(this->txtStrings.size()==0)
		return;

	j=0;
	while(j<this->hite)
		{
			if(j<this->txtStrings.size())
				{
					if(hilite==true)
						this->gc->CTK_printJustLineColour(this->txtStrings[j+this->startLine].c_str(),this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.hiliteForeCol,this->gadgetColours.hiliteBackCol);
					else
						this->gc->CTK_printJustLineColour(this->txtStrings[j+this->startLine].c_str(),this->sx,this->sy+j,this->wid,LEFTJUSTIFY,this->gadgetColours.foreCol,this->gadgetColours.backCol);
					j++;
				}
			else
				break;
		}
	MOVETO(this->sx,this->sy);
}

/**
* Scroll text by 1 line.
* \param bool true=scroll up.
*/
void CTK_cursesTextBoxClass::CTK_scrollLine(bool scrollup)
{
	this->scrollTBox(scrollup,1);
}

/**
* Scroll text by 1 page.
* \param bool true=scroll up.
*/
void CTK_cursesTextBoxClass::CTK_scrollPage(bool scrollup)
{
	this->scrollTBox(scrollup,this->hite);
}

/**
* Private.
*/
void CTK_cursesTextBoxClass::scrollTBox(bool scrollup,int numlines)
{
	if(this->txtStrings.size()<=this->hite)
		return;

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
* Get text of gadget.
*/
const std::string CTK_cursesTextBoxClass::CTK_getText(void)
{
	return(this->text);
}



