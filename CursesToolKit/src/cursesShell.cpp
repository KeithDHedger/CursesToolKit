/*
 *
 * ©K. D. Hedger. Tue 31 Mar 13:19:43 BST 2020 keithdhedger@gmail.com

 * This file (cursesShell.cpp) is part of CursesToolKit.

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

#include "cursesShell.h"


/**
* Shell class destroy.
*/
CTK_cursesShellClass::~CTK_cursesShellClass()
{
}

/**
* Shell class.
*/
CTK_cursesShellClass::CTK_cursesShellClass(CTK_mainAppClass *mc)
{
	this->mc=mc;
}

void CTK_cursesShellClass::CTK_startShell(void)
{
	this->mc->readKey->CTK_restoreTerminal();
	SETSHOWCURS;
	system("/bin/bash -c \"setterm --background default --foreground default --clear all;printf \"\e[?25h\"\" 2>&1;/bin/bash 2>&1");
	this->mc->readKey->CTK_setTerminal();
	SETHIDECURS;
	this->mc->CTK_clearScreen();
	this->mc->CTK_updateScreen(this->mc,NULL);
	fflush(NULL);
}



