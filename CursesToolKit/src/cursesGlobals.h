/*
 *
 * ©K. D. Hedger. Wed 10 Apr 13:18:24 BST 2019 keithdhedger@gmail.com

 * This file (cursesGlobals.h) is part of CursesToolKit.

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

#ifndef _CURSESGLOBALS_
#define _CURSESGLOBALS_

#include <linux/limits.h>

#include <string>
#include <vector>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct fbData
{
	struct fb_fix_screeninfo	frameBufferInfo;
	long int					screensize=1;
	char						*frameBufferMapPtr=NULL;
	bool						fbIsMapped=false;
	int							charWidth=1;
	int							charHeight=1;
	int							screenWidth=1;
	int							screenHeight=1;
};

enum  utilVarType {BOOLVAR=0,INTVAR,CHARVAR,MULTVAR,BADTYPE,DECIMALOUT,HEXOUT,OCTALOUT};

#include "LFSTKFindClass.h"
#include "cursesLib.h"
#include "cursesUtils.h"
#include "cursesReadKeyClass.h"
#include "cursesGraphics.h"
#include "cursesGadget.h"

#include "cursesButton.h"
#include "cursesTextBox.h"
#include "cursesMenus.h"
#include "cursesInput.h"
#include "cursesListBox.h"
#include "cursesCheckBox.h"
#include "cursesEditBox.h"
#include "cursesLabel.h"
#include "cursesSourceEditBox.h"
#include "cursesChooser.h"
#include "cursesDropDown.h"
#include "cursesFBImage.h"
#include "cursesKeyboard.h"

#include "cursesApplication.h"

/**
 *
 * \brief CTK globals.
 */
#define SCREENUPDATEALL (void*)1
#define SCREENUPDATEWINDOW (void*)2
#define SCREENUPDATEUNHILITE (void*)3
#define SCREENUPDATEBASIC NULL

#define	THISPAGE this->pages[this->pageNumber]
#define	CURRENTGADGET THISPAGE.gadgets[THISPAGE.currentGadget]
#define CURRENTPAGE(x) x->pages[x->pageNumber]

enum {BUTTON=0,TXTBOX,INPUT,LIST,CHKBOX,EDBOX,SRCBOX};

/**
* Page struct
* \note Caller is responsible for allocating/freeing userData.
* \note Other objects are handled by application class.
*/
struct pageStruct
{
	std::vector<CTK_cursesGadgetClass*>		gadgets;
	int										currentGadget=0;
	void									*userData=NULL;
	bool									menusActive=true;
	bool									menuBarVisible=true;
	bool									ignoreFirstTab=false;
	bool									retainHighliting=false;
	bool									fancyWindow=false;
	std::string								windowName;
	std::string								dialogName;
	int										boxX;
	int										boxY;
	int										boxW;
	int										boxH;
	int										boxWM;
	int										boxHM;
};

enum menuStyle {FLATNORM=0,FLATINVERT,DISABLED,BLANK};

#endif
