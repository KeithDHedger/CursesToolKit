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
#ifdef _USEFRAMBUFFER_
	if(this->image!=NULL)
		delete this->image;
	if(this->blob!=NULL)
		delete this->blob;
#endif
}

/**
* FB Image class.
*/
CTK_cursesFBImageClass::CTK_cursesFBImageClass()
{
}

/**
* Create image
*/
void CTK_cursesFBImageClass::CTK_newFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect)
{
#ifdef _USEFRAMBUFFER_
	char	buffer[256];
	struct fbData	*fbinfo=this->mc->CTK_getFBData();

	if(this->image!=NULL)
		delete this->image;
	if(this->blob!=NULL)
		delete this->blob;

	this->image=new Magick::Image;
	this->blob=new Magick::Blob;
	this->image->read(filepath);
	this->image->magick("BGRA");
	if(width!=-1)
		{
			if(keepaspect==true)
				snprintf(buffer,255,"%ix%i",width*fbinfo->charWidth,hite*fbinfo->charHeight);
			else
				snprintf(buffer,255,"!%ix%i",width*fbinfo->charWidth,hite*fbinfo->charHeight);
			buffer[255]=0;
			this->image->resize(buffer);
		}

	this->image->write((this->blob));
	this->wid=(int)image->columns();
	this->hite=(int)image->rows();
	this->sx=x;
	this->sy=y;
#endif
}

/**
* Draw image
*/
void CTK_cursesFBImageClass::CTK_drawFBImage(void)
{
#ifdef _USEFRAMBUFFER_
	unsigned char	*datptr;
	unsigned int	pixoffset;
	struct fbData	*fbinfo=this->mc->CTK_getFBData();

	datptr=(unsigned char *)blob->data();
	for(int y=(this->sy-1)*fbinfo->charHeight;y<this->hite+((this->sy-1)*fbinfo->charHeight);y++)
		{
			pixoffset=((this->sx-1)*4*fbinfo->charWidth)+(y*fbinfo->frameBufferInfo.line_length);
			memcpy(&(fbinfo->frameBufferMapPtr[pixoffset]),&datptr[0],this->wid*4);
			datptr+=this->wid*4;
		}
#endif
}




