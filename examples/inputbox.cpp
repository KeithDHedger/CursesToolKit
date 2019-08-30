#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./inputbox.cpp "Body text" "Default input" "Window name" "Dialog title" 1 64

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey Magick++) -lcursestoolkit "$0"  -o inputbox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./inputbox "$@"
retval=$?
rm inputbox
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_mainAppClass		*mainApp;
	std::string				str;
	CTK_cursesUtilsClass	*cu;
	if(argc<7)
		{
			printf("USAGE:\ninputbox \"Body String\" \"Default Input String\" \"Window name\" \"Dialog title\" \"Show 'CANCEL' button (0=no show, 1=show)\" Dialog Width\n");
			exit(1);
		}

	mainApp=new CTK_mainAppClass;
	cu=new CTK_cursesUtilsClass;
		
	if(cu->CTK_entryDialog(mainApp,argv[1],argv[2],argv[3],argv[4],atoi(argv[5]),atoi(argv[6])))
		fprintf(stderr,"Input box=%s, Button pressed=%i\n",cu->stringResult.c_str(),cu->intResult);
	else
		fprintf(stderr,"Canceled ... Button pressed=%i\n",cu->intResult);

	SETSHOWCURS;
	delete mainApp;
	delete cu;
	return(0);
}
