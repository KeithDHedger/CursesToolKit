/*
 *
 * ©K. D. Hedger. Wed 11 Mar 11:42:25 GMT 2020 keithdhedger@gmail.com

 * This file (cursesReadKeyClass.cpp) is part of CursesToolKit.

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

#include <curses.h>
#include <term.h>

const char	*CTK_cursesReadKeyClass::keyNames[]={"kcuu1","kcud1","kcub1","kcuf1","khome","kend","kpp","knp","kich1","kdch1","kent","kbs","kcbt","kf1","kf2","kf3","kf4","kf5","kf6","kf7","kf8","kf9","kf10","kf11","kf12",NULL};
const char	*CTK_cursesReadKeyClass::termInfoNames[]={NULL,"xterm","linux","xterm-256color","vt100","xterm-noapp",NULL};

/**
* CTK_cursesReadKeyClass class destroy.
*/
CTK_cursesReadKeyClass::~CTK_cursesReadKeyClass()
{
	this->restoreTerminal();
}

/**
* CTK_cursesReadKeyClass.
*/
CTK_cursesReadKeyClass::CTK_cursesReadKeyClass(CTK_mainAppClass *mc)
{
	this->mc=mc;
	this->setTerminal();
	this->getKeyCodes();
	setupterm(NULL,STDOUT_FILENO,NULL);
	this->mc->clearScreenCode=tigetstr("clear");
}

/**
* Set up terminal.
*/
void CTK_cursesReadKeyClass::setTerminal(void)
{
	struct termios	tty_attr;
	int				flags;

	if(getenv("TERM")!=NULL)
		this->thisTerm=getenv("TERM");
	this->termInfoNames[0]=this->thisTerm;

	/* make stdin non-blocking */
	flags = fcntl(STDIN_FILENO, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(0, F_SETFL, flags);

	tcgetattr(0, &this->ttyOldAttr);

	/* turn off buffering, echo and key processing */
	tty_attr = this->ttyOldAttr;

	tty_attr.c_iflag &= ~(ICRNL | IXON);
	tty_attr.c_lflag &= ~(ICANON | ECHO| ISIG|BRKINT | ICRNL | INPCK | ISTRIP | IXON);


	tcsetattr(0, TCSANOW, &tty_attr);
	setupterm(NULL,STDOUT_FILENO,NULL);
	putp(tigetstr("smkx"));
	fflush(NULL);
}

/**
* Restore terminal.
*/
void CTK_cursesReadKeyClass::restoreTerminal(void)
{
	tcsetattr(0,TCSAFLUSH,&this->ttyOldAttr);
	setupterm(NULL,STDOUT_FILENO,NULL);
	putp(tigetstr("rmkx"));
	fflush(NULL);
}

/**
* Get input.
* \note If tabIsSpecial=true ^I is reported as CTK_KEY_TAB
* \note Else ^I is normal tab char, ^I is never reported as Control I
*/
void CTK_cursesReadKeyClass::getInput(void)
{
	char	buffer[1];
	int		result;
	char	*hexpair[3]= {0,0,0};

	this->inputBuffer.clear();
	this->isHexString=false;
	this->isControlKey=false;
	this->specialKeyName=CTK_KEY_NONE;
	this->controlKeyNumber=-1;

	while(true)
		{
			result=read(STDIN_FILENO,&buffer[0],1);
			if(result>=0)
				break;
		}

	if(buffer[0]!='\t')
		{
			if((iscntrl(buffer[0])) && (buffer[0]<27) && (buffer[0]>0))
				{
					this->isControlKey=true;
					this->inputBuffer=buffer[0]+64;
					this->controlKeyNumber=buffer[0];
					return;
				}
		}

	if(buffer[0]==0x1b)
		this->isHexString=true;

	if(buffer[0]==0x7f)
		this->isHexString=true;

	result=1;
	while(true)
		{
			if(result==-1)
				continue;
			do
				{
					sprintf((char*)&hexpair[0],"%x",buffer[0]);
					if(this->isHexString==true)
						this->inputBuffer+=(char*)&hexpair[0];
					else
						this->inputBuffer+=buffer[0];
				}
			while(read(STDIN_FILENO,&buffer[0],1)>=0);
			break;
		}

	if(this->isHexString==true)
		{
			if(this->inputBuffer.compare("1b")==0)
				{
					this->specialKeyName=CTK_KEY_ESC;
					this->inputBuffer="";
					return;
				}
			this->setSpecialKeyName();
		}
	else
		{
			if((this->inputBuffer.compare("\t")==0) && (this->tabIsSpecial==true))
				{
					this->specialKeyName=CTK_KEY_TAB;
					this->inputBuffer="";
					this->isHexString=true;
					return;
				}
			this->specialKeyName=CTK_KEY_NONE;
		}
}

/**
* Get special key codes.
*/
void CTK_cursesReadKeyClass::getKeyCodes(void)
{
	char		*strresult=NULL;
	char		*hexpair[3]= {0,0,0};
	std::string	specialcodes;
	int			keycode=0;
	int			entnames=0;

	while(termInfoNames[entnames]!=NULL)
		{
			keycode=0;
			while(keyNames[keycode]!=NULL)
				{
					setupterm(termInfoNames[entnames],STDOUT_FILENO,NULL);
					specialcodes="";
					strresult=NULL;
					strresult=tigetstr(keyNames[keycode]);
					if(strresult!=NULL)
						{
							for(unsigned  j=0;j<strlen(strresult);j++)
								{
									sprintf((char*)&hexpair[0],"%x",strresult[j]);
									specialcodes+=(char*)&hexpair[0];
								}
							this->allCodes[keyNames[keycode]][entnames]=specialcodes;
						}
					keycode++;
				}
			entnames++;
		}

//	entnames=0;
//	while(termInfoNames[entnames]!=NULL)
//		{
//			keycode=0;
//			while(keyNames[keycode]!=NULL)
//				{
//					fprintf(stderr,"term=%s key %s=%s\n",this->termInfoNames[entnames],this->keyNames[keycode],this->allCodes[keyNames[keycode]][entnames].c_str());
//					keycode++;
//				}
//			entnames++;
//		}
}

/**
* Set Special Key Name.
*/
void CTK_cursesReadKeyClass::setSpecialKeyName(void)
{
	int			keycode=0;
	int			entnames=0;
//fprintf(stderr,">>>>>>>>>>>>>>>>>>\n");
	while(termInfoNames[entnames]!=NULL)
		{
//fprintf(stderr,">>>>>>>>>%s>>>>>>>>>\n",termInfoNames[entnames]);
			keycode=0;
			while(keyNames[keycode]!=NULL)
				{
//				fprintf(stderr,">>>>>>>>>%s>>>>>>>>>\n",keyNames[keycode]);
//					fprintf(stderr,"key=%s,inp=%s keycode=%i\n",this->allCodes[keyNames[keycode]][entnames].c_str(),this->inputBuffer.c_str(),keycode);
//					fprintf(stderr,"--->%s %s<---\n",this->allCodes[keyNames[keycode]][entnames].c_str(),this->inputBuffer.c_str());
					if(this->allCodes[keyNames[keycode]][entnames].compare(this->inputBuffer)==0)
						{
//							fprintf(stderr,"--------term=%s key %s=%s\n",this->termInfoNames[entnames],this->keyNames[keycode],this->allCodes[keyNames[keycode]][entnames].c_str());
//							fprintf(stderr,"keycode=%i keyup=%i\n",keycode,CTK_KEY_UP);
							this->specialKeyName=keycode+CTK_KEY_UP;
							return;
						}
					keycode++;
				}
			entnames++;
		}
	this->specialKeyName=CTK_KEY_NONE;
}



