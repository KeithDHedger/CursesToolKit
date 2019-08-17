/*
 *
 * ©K. D. Hedger. Sat 17 Aug 15:16:39 BST 2019 keithdhedger@gmail.com

 * This file (cursesFBImage.cpp) is part of CursesToolKit.

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


#include "cursesFBImage.h"

/**
* FB Image class destroy.
*/
CTK_cursesFBImageClass::~CTK_cursesFBImageClass()
{

}

/**
* FB Image class.
*/
CTK_cursesFBImageClass::CTK_cursesFBImageClass()
{
#ifdef _USEFRAMBUFFER_
	int	fbfd=0;

	if(fbIsMapped==false)
		{
// Open the file for reading and writing
			fbfd=open("/dev/fb0",O_RDWR);
			if(!fbfd)
				{
					printf("Error: cannot open framebuffer device.\n");
				}

// Get fixed screen information
			if(ioctl(fbfd,FBIOGET_FSCREENINFO,&frameBufferInfo))
				{
					printf("Error reading fixed information.\n");
				}

// map fb to user mem
			screensize=frameBufferInfo.smem_len;
			frameBufferMapPtr=(char*)mmap(NULL,screensize,(PROT_READ | PROT_WRITE),MAP_SHARED, fbfd,0);
			close(fbfd);
			fbIsMapped=true;
//init imagemagick
			Magick::InitializeMagick(NULL);
		}
#endif
}

/**
* Create image
*/
void CTK_cursesFBImageClass::CTK_newFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect)
{
#ifdef _USEFRAMBUFFER_
	char	*buffer;

	this->image.read(filepath);
	this->image.magick("BGRA");
	if(wid!=-1)
		{
			if(keepaspect==true)
				asprintf(&buffer,"%ix%i",width,hite);
			else
				asprintf(&buffer,"!%ix%i",width,hite);
			this->image.resize(buffer);
		}
	this->image.write(&(this->blob));
	this->wid=(int)image.columns();
	this->hite=(int)image.rows();
#endif
}

/**
* Draw image
*/
void CTK_cursesFBImageClass::CTK_drawFBImage(void)
{
#ifdef _USEFRAMBUFFER_
#endif
}
