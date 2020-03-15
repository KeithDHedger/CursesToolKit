/*
 *
 * ©K. D. Hedger. Wed 11 Mar 11:42:14 GMT 2020 keithdhedger@gmail.com

 * This file (cursesReadKeyClass.h) is part of CursesToolKit.

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

#ifndef _CURSESREADKEY_
#define _CURSESREADKEY_

enum	{CTK_KEY_NONE=1000,CTK_KEY_TAB,CTK_KEY_ESC,CTK_KEY_UP,CTK_KEY_DOWN,CTK_KEY_LEFT,CTK_KEY_RIGHT,CTK_KEY_HOME,CTK_KEY_END,CTK_KEY_PAGEUP,CTK_KEY_PAGEDOWN,CTK_KEY_INSERT,CTK_KEY_DELETE,CTK_KEY_ENTER,CTK_KEY_BACKSPACE,CTK_KEY_BACKTAB,CTK_KEY_F1,CTK_KEY_F2,CTK_KEY_F3,CTK_KEY_F4,CTK_KEY_F5,CTK_KEY_F6,CTK_KEY_F7,CTK_KEY_F8,CTK_KEY_F9,CTK_KEY_F10,CTK_KEY_F11,CTK_KEY_F12};

#define CTK_KEY_RETURN 13

class CTK_cursesReadKeyClass
{
	public:
														CTK_cursesReadKeyClass(CTK_mainAppClass *mc);
														~CTK_cursesReadKeyClass();
		void											CTK_getInput(void);

		std::string										inputBuffer;
		int												specialKeyName=CTK_KEY_NONE;
		int												controlKeyNumber=-1;
		bool											isHexString=false;
		bool											tabIsSpecial=false;
		bool											isControlKey=false;


	protected:
	private:
		void											restoreTerminal(void);
		void											setTerminal(void);
		void											getKeyCodes(void);
		void											setSpecialKeyName(void);

		struct termios									ttyOldAttr;
		const char										*thisTerm="xterm";
		static const char								*keyNames[];
		static const char								*termInfoNames[];
		std::map<const char*,std::map<int,std::string>>	allCodes;
		CTK_mainAppClass								*mc;
};

#endif