#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

#Needs to be run on the console NOT in a graphical terminal ( ie not in X ).
#USEVALGRIND="valgrind --leak-check=full"
#Run eg:
#(cd ../;make -j4);./fbimage.cpp /path/to/folder/with/images

g++ -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++ ) -lcursestoolkit "$0" -o fbimageexample ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./fbimageexample "$@"
retval=$?
rm fbimageexample
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	coloursStruct			cs;
	CTK_cursesLabelClass	*lab;
	const char				*label="Simple Framebuffer Image Example, Press Any Key ...";
	int						labellen=strlen(label);
	struct fbData			*fbinf=mainApp->CTK_getFBData();
	CTK_cursesUtilsClass	cu;
	char					*folder=NULL;

	if(argc>1)
		folder=argv[1];

	cs.windowBackCol=BACK_WHITE;
	mainApp->CTK_setColours(&cs,true);

	lab=mainApp->CTK_addNewLabel((mainApp->maxCols/2)-(labellen/2),1,labellen,1,label);
	lab->CTK_setJustify(CENTREJUSTIFY);

	CTK_cursesFBImageClass	*img=mainApp->CTK_addNewFBImage(2,2,64,32,"LFSTux.png");
	img->sx=(mainApp->maxCols/2)-(img->wid/2/fbinf->charWidth);
	mainApp->CTK_mainEventLoop(1);
	img->CTK_newFBImage(1,2,-1,8,"LFSTuxRed.png");
	img->sx=(mainApp->maxCols/2)-(img->wid/2/fbinf->charWidth);

	mainApp->CTK_mainEventLoop(-5000,true);

	cu.CTK_fileChooserDialog(folder,CUOPENFILE,".png;.tiff;.jpeg;.jpg;");

	if(cu.dialogReturnData.isValidData==true)
		{
			img->CTK_newFBImage(2,2,mainApp->maxCols-2,mainApp->maxRows-2,cu.dialogReturnData.stringValue.c_str());
			img->sx=(mainApp->maxCols/2)-(img->wid/2/fbinf->charWidth);
			mainApp->CTK_mainEventLoop(1);
		}

	SETSHOWCURS;

	delete mainApp;
	printf("\n");
	return 0;
}

