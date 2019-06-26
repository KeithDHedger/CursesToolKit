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
#include <termkey.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "LFSTKFindClass.h"
#include "cursesLib.h"
#include "cursesUtils.h"
#include "cursesGraphics.h"

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

#include "cursesApplication.h"

/**
 *
 * \brief CTK globals.
 */

enum {BUTTON=0,TXTBOX,INPUT,LIST,CHKBOX,EDBOX,SRCBOX};

/**
* Private structure for util dialogs.
*/
struct fileUDStruct
{
	CTK_cursesUtilsClass	*inst=NULL;
	//LFSTK_findClass			*find=NULL;
	CTK_mainAppClass		*app=NULL;
	//CTK_cursesListBoxClass	*list=NULL;
	CTK_cursesChooserClass	*chooser=NULL;
	bool					isValid=false;
	bool					isOpenDialog=false;
	bool					isSelectFolder=false;
	int						buttonsToShow=YESBUTTON|NOBUTTON;
	int						buttonPressed=NOBUTTONPRESSED;
	const char				*licenceFilename=NULL;
	const char				*credits=NULL;
};

/**
* Page struct
* \note Caller is responsible for allocating/freeing userData.
* \note Other objects are handled by application class.
*/
struct pageStruct
{
	std::vector<CTK_cursesButtonClass*>			buttons;
	std::vector<CTK_cursesTextBoxClass*>		textBoxes;
	std::vector<CTK_cursesInputClass*>			inputs;
	std::vector<CTK_cursesListBoxClass*>		lists;
	std::vector<CTK_cursesCheckBoxClass*>		checkBoxes;
	std::vector<CTK_cursesEditBoxClass*>		editBoxes;
	std::vector<CTK_cursesSourceEditBoxClass*>	srcEditBoxes;
	std::vector<CTK_cursesLabelClass*>			labels;
	std::vector<CTK_cursesChooserClass*>		choosers;
	void										*userData=NULL;
	bool										menusActive=true;
};

#endif
