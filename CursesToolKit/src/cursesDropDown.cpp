/*
 *
 * ©K. D. Hedger. Thu 27 Jun 20:34:15 BST 2019 keithdhedger@gmail.com

 * This file (cursesDropDown.cpp) is part of CursesToolKit.

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
* Menus class destroy.
*/
CTK_cursesDropClass::~CTK_cursesDropClass()
{
	this->items.clear();
	termkey_destroy(this->tk);
}

/**
* Menus class constuctor.
*/
CTK_cursesDropClass::CTK_cursesDropClass()
{
	this->tk=termkey_new(0,TERMKEY_FLAG_CTRLC);
	if(!this->tk)
		{
			fprintf(stderr, "Cannot allocate termkey instance\n");
			exit(1);
		}
}

/**
* New drop button.
*/
void CTK_cursesDropClass::CTK_newDropButton(int x,int y,int width,int hite,const char *label)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=hite;
	this->label=label;
	this->holdLabel=label;
}

/**
* Draw drop button.
* \note hilite=true draw in highlight colour.
*/
void CTK_cursesDropClass::CTK_drawDropButton(bool hilite)
{
	CTK_cursesGraphicsClass	gc;
	std::string				str=this->label;

	MOVETO(this->sx,this->sy);
	if(hilite==true)
		setBothColours(this->colours.hiliteForeCol,this->colours.hiliteBackCol,this->colours.use256Colours);
	else
		setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);

	gc.CTK_printLine(str.c_str(),this->sx,sy,this->wid);

	if(this->colours.fancyGadgets==true)
		gc.CTK_printLine(">",this->sx+this->wid,sy,1);

	fflush(NULL);
}

/**
* Set colours etc from coloursStruct.
*/
void CTK_cursesDropClass::CTK_setColours(coloursStruct cs)
{
	this->colours=cs;
}

/**
* Add item to drop down.
*/
void CTK_cursesDropClass::CTK_addDropItem(const char *name)
{
	dropDownStruct dss;

	dss.label=name;
	if(strlen(name)>this->maxWidth)
		this->maxWidth=strlen(name);
	this->items.push_back(dss);
}

/**
* Handle drop down events.
*/
void CTK_cursesDropClass::CTK_doDropDownEvent(void)
{
	int						iy=this->sy+this->hite;
	bool					loop=true;
	TermKeyResult			ret;
	TermKeyKey				key;
	TermKeyFormat			format=TERMKEY_FORMAT_VIM;
	int						selection=this->selectedItem;

	if(this->items.size()==0)
		return;

	this->CTK_drawList(selection);

	while(loop==true)
		{
			ret=termkey_waitkey(this->tk,&key);

			switch(key.type)
				{
					case TERMKEY_TYPE_KEYSYM:
						{
							switch(key.code.sym)
								{
									case TERMKEY_SYM_ESCAPE:
										loop=false;
										continue;
										break;

									case TERMKEY_SYM_UP:
										selection--;
//skip disabled menu items
										while((selection>-1) && (this->items[selection].enabled==false))
											selection--;
										if(selection<0)
											selection=0;
										break;
									case TERMKEY_SYM_DOWN:
										selection++;
//skip disabled menu items
										while((selection<this->items.size()) && (this->items[selection].enabled==false))
											selection++;
										if(selection==this->items.size())
											selection=this->items.size()-1;
										break;

									case TERMKEY_SYM_HOME:
										selection=0;
										break;

									case TERMKEY_SYM_END:
										selection=this->items.size()-1;
										break;

									case TERMKEY_SYM_ENTER:
										if(selection>-1)
											this->selectedItem=selection;
										loop=false;
										continue;
										break;

									default:
										break;
										}
								}
					}
			this->CTK_drawList(selection);
		}
	if(this->selectedItem>-1)
		this->label=this->items[this->selectedItem].label;
	this->mc->CTK_clearScreen();
	this->mc->CTK_updateScreen(this->mc,NULL);
}

void CTK_cursesDropClass::CTK_drawList(int selection)
{
	CTK_cursesGraphicsClass	gc;
	int						iy=this->sy+this->hite;

	for(int j=0;j<this->items.size();j++)
		{
			if(this->items[j].enabled==false)
				setBothColours(this->colours.disabledForeCol,this->colours.menuBackCol,this->colours.use256Colours);
			else
				{
					if(selection==j)
						setBothColours(this->colours.menuHiliteForeCol,this->colours.menuHiliteBackCol,this->colours.use256Colours);
					else
						setBothColours(this->colours.menuForeCol,this->colours.menuBackCol,this->colours.use256Colours);
				}
			gc.CTK_printLine(this->items[j].label.c_str(),this->sx,iy+j,this->maxWidth);
		}
}

/**
*  Set whether to deselect gadget after 'pressing'.
*/
void CTK_cursesDropClass::CTK_setEnterDeselects(bool deselect)
{
	this->enterDeselects=deselect;
}

/**
* Get whether to deselect gadget after 'pressing'.
*/
bool CTK_cursesDropClass::CTK_getEnterDeselects(void)
{
	return(this->enterDeselects);
}

/**
* Clear drop down list.
*/
void CTK_cursesDropClass::CTK_clearList(void)
{
	this->selectedItem=-1;
	this->items.clear();
	this->label=this->holdLabel;
}

/**
*  Set drop down 'pressed' callback.
*/
void CTK_cursesDropClass::CTK_setSelectCB(void (*select)(void *,void *),void *userdata)
{
	this->selectCB=select;
	this->selectCBUserData=userdata;
}

void CTK_cursesDropClass::CTK_setItemEnabled(int item,bool enable)
{
	 this->items[item].enabled=enable;
}

bool CTK_cursesDropClass::CTK_getItemEnabled(int item)
{
	return(this->items[item].enabled);
}



