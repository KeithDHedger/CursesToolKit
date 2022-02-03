#if 0

#
#//©keithhedger Mon 9 Mar 12:50:13 GMT 2020 kdhedger68713@gmail.com
#

#USEVALGRIND="valgrind --leak-check=full"

g++ "$0" -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit -o oskbexample ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./oskbexample
retval=$?
rm oskbexample
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	CTK_cursesKeyboardClass	*oskb=new CTK_cursesKeyboardClass(mainApp);
	varsStruct				vsitem;
	std::vector<varsStruct> outvs;
	std::vector<varsStruct> invs;
	char					*filepath="/tmp/testvar.config";

	if(argc>1)
		filepath=argv[1];

	system("clear");
	oskb->CTK_newKeyboard(10,10, 20,10,"OS Keyboard Example");

	oskb->CTK_drawGadget(false);
	oskb->CTK_doInput();

	SETSHOWCURS;
	delete mainApp;
	outvs.clear(); 
	invs.clear(); 
	printf("\n");
	return 0;

}