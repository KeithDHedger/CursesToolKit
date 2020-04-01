/*
 *
 * ©K. D. Hedger. Tue 31 Mar 13:19:50 BST 2020 keithdhedger@gmail.com

 * This file (cursesShell.h) is part of CursesToolKit.

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

#ifndef _CURSESSHELL_
#define _CURSESSHELL_

#include "cursesGlobals.h"

class CTK_cursesShellClass
{
	public:
		CTK_cursesShellClass(CTK_mainAppClass *mc);
		~CTK_cursesShellClass();

		void				CTK_startShell(void);
	private:
		CTK_mainAppClass	*mc;
};

#endif
