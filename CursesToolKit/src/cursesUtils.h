/*
 *
 * ©K. D. Hedger. Sat  6 Apr 17:05:19 BST 2019 keithdhedger@gmail.com

 * This file (cursesUtils.h) is part of CursesToolKit.

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
 
#include <string>
#include <termkey.h>
#include <vector>
#include <string.h>
#include <unistd.h>

//#include "cursesLib.h"
//#include "cursesGraphics.h"
//class CTK_mainAppClass;

#include "cursesGlobals.h"

#ifndef _CURSESUTILS_
#define _CURSESUTILS_

//#include "cursesApplication.h"

class CTK_cursesUtilsClass
{
	public:
		CTK_cursesUtilsClass();
		~CTK_cursesUtilsClass();

		std::vector<std::string>	CTK_explode(const std::string s,const char c);
		std::string					CTK_openFile(CTK_mainAppClass *app);

	private:
		void						runOpenFile(CTK_mainAppClass *app);
		std::string					inFolder="./";
};

#endif
