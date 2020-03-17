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

#ifdef _IMAGEMAGICK_
#include <Magick++.h>
#endif

/**
* FB Image class destroy.
*/
CTK_cursesFBImageClass::~CTK_cursesFBImageClass()
{
#ifdef _IMAGEMAGICK_
	if(this->image!=NULL)
		delete static_cast<Magick::Image*>(this->image);
	if(this->blob!=NULL)
		delete static_cast<Magick::Blob*>(this->blob);
#endif
}

/**
* FB Image class.
*/
CTK_cursesFBImageClass::CTK_cursesFBImageClass(CTK_mainAppClass *mc)
{
	this->CTK_setCommon(mc);
	this->type=IMAGEGADGET;
}

/**
* Create image
* \param x,y 1 based postition, in characters
* \param width,hite 1 based size, in characters
* \param filepath
* \param keepaspect
*/
void CTK_cursesFBImageClass::CTK_newFBImage(int x,int y,int width,int hite,const char *filepath,bool keepaspect)
{
#ifdef _IMAGEMAGICK_
	char	buffer[256];
	Magick::Image	*limage;
	Magick::Blob	*lblob;
	Magick::Blob	*hblob;
	struct fbData	*fbinfo=this->mc->CTK_getFBData();

	if(this->image!=NULL)
		delete static_cast<Magick::Image*>(this->image);
	if(this->blob!=NULL)
		delete static_cast<Magick::Blob*>(this->blob);
	if(this->blobHilite!=NULL)
		delete static_cast<Magick::Blob*>(this->blobHilite);
//for errors
    this->image=NULL;
    this->blob=NULL;
    this->blobHilite=NULL;
    this->wid=1;
	this->hite=1;
	this->sx=x;
	this->sy=y;

	if(access(filepath,F_OK|R_OK)!=(F_OK))
		return;

	limage=new Magick::Image;
	lblob=new Magick::Blob;
	hblob=new Magick::Blob;
	this->image=(void*)limage;
	this->blob=(void*)lblob;
	this->blobHilite=(void*)hblob;
	try
		{
			limage->read(filepath);
			limage->magick("BGRA");
			if(width!=-1)
				{
					if(keepaspect==true)
						snprintf(buffer,255,"%ix%i",width*fbinfo->charWidth,hite*fbinfo->charHeight);
					else
						snprintf(buffer,255,"!%ix%i",width*fbinfo->charWidth,hite*fbinfo->charHeight);
					buffer[255]=0;
					limage->resize(buffer);
				}

			limage->write((lblob));
			limage->strokeColor(this->hiliteColour);
			limage->fillColor("transparent");
			limage->strokeWidth(this->hiliteWidth);
			limage->draw(Magick::DrawableRectangle(0,0,limage->columns()-1,limage->rows()-1));
			limage->write((hblob));
		}

	catch(Magick::Exception &error_)
		{
		   	if(this->image!=NULL)
				delete static_cast<Magick::Image*>(this->image);
			if(this->blob!=NULL)
				delete static_cast<Magick::Blob*>(this->blob);
			if(this->blobHilite!=NULL)
				delete static_cast<Magick::Blob*>(this->blobHilite);
		    this->image=NULL;
		    this->blob=NULL;
		    this->blobHilite=NULL;
		    return;
		}

	this->wid=(int)limage->columns();
	this->hite=(int)limage->rows();
	this->sx=x;
	this->sy=y;
#endif
}

/**
* Draw image
*/
void CTK_cursesFBImageClass::drawFBImage(bool hilite)
{
#ifdef _IMAGEMAGICK_
	unsigned char	*datptr;
	unsigned int	pixoffset;
	struct fbData	*fbinfo=this->mc->CTK_getFBData();
	Magick::Blob	*lblob=static_cast<Magick::Blob*>(this->blob);
	Magick::Blob	*hblob=static_cast<Magick::Blob*>(this->blobHilite);

	if(lblob==NULL)
		return;
	if(hilite==false)
		datptr=(unsigned char *)lblob->data();
	else
		datptr=(unsigned char *)hblob->data();
	for(int y=(this->sy-1)*fbinfo->charHeight;y<this->hite+((this->sy-1)*fbinfo->charHeight);y++)
		{
			pixoffset=((this->sx-1)*4*fbinfo->charWidth)+(y*fbinfo->frameBufferInfo.line_length);
			memcpy(&(fbinfo->frameBufferMapPtr[pixoffset]),&datptr[0],this->wid*4);
			datptr+=this->wid*4;
		}
#endif
}

//over ridden funcs
/**
* Draw image.
* \note hilite=true draw in highlight colour. //TODO//
*/
void CTK_cursesFBImageClass::CTK_drawGadget(bool hilite)
{
	this->drawFBImage(hilite);
}


