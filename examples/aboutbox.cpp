#if 0

#
#//©keithhedger Mon 9 Mar 12:50:13 GMT 2020 kdhedger68713@gmail.com
#

#Run eg:
#(cd ../;make -j4);./aboutbox.cpp  "About Example" "An example about box" "Copyright 2019 K.D.Hedger" "keithdhedger@gmail.com" "http://keithhedger.freeddns.org" "$(echo -e "K.D.Hedger\nJoe Bloggs")" "/media/LinuxData/Development64/Projects/CursesToolKit/LICENSE" -1

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ "$0" -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit -o aboutbox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./aboutbox "$@"
retval=$?
rm aboutbox
exit $retval

#endif

#include <cursesGlobals.h>

enum {INFOAPPNAME=1,INFOAPPINFO,INFOCOPYRITE,INFOEMAIL,INFOWEBSITE,INFOCREDITS,INFOLICENCE,DIALOGWIDTH};

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	*cu;
	CTK_mainAppClass		*mainApp;

	if(argc<8)
		{
			printf("USAGE:\naboutbox \"App name\" \"App info\" \"Copyright\" \"Email\" \"Website\" \"Credits\" \"Licence File\" Width.\n");
			exit(1);
		}

	mainApp=new CTK_mainAppClass();
	cu=new CTK_cursesUtilsClass();

	cu->CTK_aboutDialog(argv[INFOAPPNAME],argv[INFOAPPINFO],argv[INFOCOPYRITE],argv[INFOEMAIL],argv[INFOWEBSITE],argv[INFOCREDITS],argv[INFOLICENCE],atoi(argv[DIALOGWIDTH]));

	SETSHOWCURS;
	delete mainApp;
	delete cu;
	return(0);

}
