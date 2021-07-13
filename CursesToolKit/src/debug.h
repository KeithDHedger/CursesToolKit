/*
 *
 * ©K. D. Hedger. Tue 13 Jul 16:56:34 BST 2021 keithdhedger@gmail.com

 * This file (debug.h) is part of FBMediaPlayer.

 * FBMediaPlayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * FBMediaPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with FBMediaPlayer.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DEBUGH_
#define _DEBUGH_

#include <stdarg.h>

__attribute__((unused)) static bool	showFileData=true;
__attribute__((unused)) static void debugFunc(const char *file,const char *func,int line,const char *fmt, ...)
{
	va_list		ap;
	char		*buffer,*subbuffer;
	const char	*bools[]={"false","true"};

	buffer=(char*)alloca(512);
	subbuffer=(char*)alloca(512);

	buffer[0]=0;
	subbuffer[0]=0;
	va_start(ap, fmt);
	while (*fmt)
		{
			subbuffer[0]=0;
			if(fmt[0]=='%')
				{
					fmt++;
					switch(*fmt)
						{
							case 's':
								sprintf(subbuffer,"%s",va_arg(ap,char*));
								break;
							case 'x':
								sprintf(subbuffer,"%x",va_arg(ap,int));
								break;
							case 'i':
								sprintf(subbuffer,"%i",va_arg(ap,int));
								break;
							case 'p':
								sprintf(subbuffer,"%p",va_arg(ap,char*));
								break;
							case 'c':
								sprintf(subbuffer,"%c",va_arg(ap,int));
								break;
							case 'f':
								sprintf(subbuffer,"%f",va_arg(ap,double));
								break;
							case 'b':
								sprintf(subbuffer,"%s",bools[va_arg(ap,int)]);
								break;
//							case 'r':
//								{
//									geometryStruct gs=va_arg(ap,geometryStruct);
//									sprintf(subbuffer,"x=%i, y=%i, w=%i, h=%i",gs.x,gs.y,gs.w,gs.h);
//								}
//								break;
							default:
								sprintf(subbuffer,"%c",fmt[0]);
								break;
						}
				}
			else
				sprintf(subbuffer,"%c",fmt[0]);
			strcat(buffer,subbuffer);
			fmt++;
		}
	va_end(ap);

	if(showFileData==true)
		{
			fprintf(stderr,"\nFile: %s\nFunc: %s\nLine: %i\n",basename(file),func,line);
			fprintf(stderr,"----USER DATA----\n%s\n----END----\n",buffer);
		}
	else
		{
			fprintf(stderr,"%s\n",buffer);
		}
}

__attribute__((unused)) static void simpleDebug(const char *file,const char *func,int line)
{
	fprintf(stderr,"\nFile: %s\nFunc: %s\nLine: %i\n",basename(file),func,line);
}

#define DEBUGFUNC(x,...) debugFunc(__FILE__,__func__,__LINE__,(const char*)x,__VA_ARGS__);
#define DEBUG simpleDebug(__FILE__,__func__,__LINE__);

#endif