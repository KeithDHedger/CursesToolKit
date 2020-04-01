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

//#include <curses.h>
//#include <term.h>

void CTK_cursesShellClass::CTK_startShell(void)
{
	SETSHOWCURS;
	fprintf(stderr,">>>>>>\n");
//	struct termios	tty_attr;
//	int				flags;

//	fcntl(0, F_SETFL, 33794);

//	this->mc->readKey->restoreTerminal();
	//tty_attr.c_iflag &= (ICRNL | IXON);
	//tty_attr.c_lflag &= (ICANON | ECHO| ISIG|BRKINT | ICRNL | INPCK | ISTRIP | IXON);

	//tcsetattr(0,TCSAFLUSH,&this->mc->readKey->ttyOldAttr);
	//setupterm(NULL,STDOUT_FILENO,NULL);
	//restartterm(NULL,STDOUT_FILENO,NULL);
	//putp(tigetstr("rmkx"));

	//printf("\e[?1049h");
	//fflush(NULL);
	system("/bin/bash 2>&1");
	//sleep(5);
	//system("/bin/bash");
	//printf("\e[?1049l");
	//fflush(NULL);
//	this->mc->readKey->setTerminal();
	fprintf(stderr,"<<<<<<<\n");
	SETHIDECURS;
	//fflush(NULL);
	this->mc->CTK_clearScreen();
	this->mc->CTK_updateScreen(this->mc,NULL);
}



