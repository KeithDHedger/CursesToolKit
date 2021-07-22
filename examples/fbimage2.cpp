#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

#Needs to be run on the console NOT in a graphical terminal ( ie not in X ).
#USEVALGRIND="valgrind --leak-check=full"
#Run eg:
#(cd ../;make -j4);./fbimage2.cpp /path/to/folder/with/images

g++ -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++ ) -lcursestoolkit "$0" -o fbimage2example ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./fbimage2example "$@"
retval=$?
rm fbimage2example
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesGlobals.h>
CTK_mainAppClass		*mainApp=new CTK_mainAppClass();

bool imgselctCB(void *inst,void *userdata)
{
	fprintf(stderr,"image selected\n");
	return(true);
}

bool buttonselctCB(void *inst,void *userdata)
{
	char					*buffer=(char*)alloca(256);
	CTK_cursesButtonClass	*bc=static_cast<CTK_cursesButtonClass*>(inst);
	long					ud=(long)userdata;

	fprintf(stderr,"Button '%s' clicked, Userdata %p\n",bc->label,userdata);
	mainApp->runEventLoop=false;
	return(true);
}

int main(int argc, char **argv)
{

	//coloursStruct			cs;
	CTK_cursesLabelClass	*lab;
	CTK_cursesButtonClass	*button;


	const char				*label="Simple Framebuffer Image Example.";
	int						labellen=strlen(label);
	struct fbData			*fbinf=mainApp->CTK_getFBData();
	CTK_cursesUtilsClass	cu;
	char					*folder=NULL;

	if(argc>1)
		folder=argv[1];

	//cs.windowBackCol=BACK_WHITE;
//	cs.fancyGadgets=true;
//	mainApp->CTK_setColours(&cs,true);

	lab=mainApp->CTK_addNewLabel((mainApp->maxCols/2)-(labellen/2),2,labellen,1,label);
	lab->CTK_setJustify(CENTREJUSTIFY);

	CTK_cursesFBImageClass	*img=mainApp->CTK_addNewFBImage(2,4,64,32,"LFSTux.png");
	img->sx=(mainApp->maxCols/2)-(img->wid/2/fbinf->charWidth);
	img->CTK_setSelectCB(imgselctCB,(void*)1);
	button=mainApp->CTK_addNewButton((mainApp->maxCols/2)-(8/2),48,16,1,"  QUIT  ");
	button->CTK_setSelectCB(buttonselctCB,(void*)1);

	mainApp->CTK_mainEventLoop(0);

	SETSHOWCURS;

	delete mainApp;
	printf("\n");
	return 0;
}
