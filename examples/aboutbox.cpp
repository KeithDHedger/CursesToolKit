#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./aboutbox.cpp  "About Example" "An example about box" "Copyright 2019 K.D.Hedger" "keithdhedger@gmail.com" "http://keithhedger.freeddns.org" "$(echo -e "K.D.Hedger\nJoe Bloggs")" "/media/LinuxData/Development64/Projects/CursesToolKit/LICENSE"

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit "$0"  -o aboutbox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./aboutbox "$@"
retval=$?
rm aboutbox
reset
exit $retval
#endif

#include <cursesGlobals.h>

enum {INFOAPPNAME=1,INFOAPPINFO,INFOCOPYRITE,INFOEMAIL,INFOWEBSITE,INFOCREDITS,INFOLICENCE};

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	*cu;
	CTK_mainAppClass		*mainApp;

	if(argc<8)
		{
			printf("USAGE:\naboutbox \"App name\" \"App info\" \"Copyright\" \"Email\" \"Website\" \"Credits\" \"Licence File\".\n");
			exit(1);
		}

	mainApp=new CTK_mainAppClass();
	cu=new CTK_cursesUtilsClass();

	cu->CTK_aboutDialog(argv[INFOAPPNAME],argv[INFOAPPINFO],argv[INFOCOPYRITE],argv[INFOEMAIL],argv[INFOWEBSITE],argv[INFOCREDITS],argv[INFOLICENCE]);

	SETSHOWCURS;
	delete mainApp;
	delete cu;
	return(0);

}
