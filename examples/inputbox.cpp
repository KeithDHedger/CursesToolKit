#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey) -lcursestoolkit "$0"  -o inputbox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./inputbox "$@"
retval=$?
rm inputbox
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{

	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	std::string				str;
	CTK_cursesUtilsClass	cu;

	if(argc<4)
		{
			printf("USAGE:\ninputbox \"Body String\" \"Default Input String\" \"Show 'CANCEL' button (0=no show, 1=show)\"\n");
			exit(1);
		}

	if(cu.CTK_entryDialog(mainApp,argv[1],argv[2],atoi(argv[3]))==true)
		fprintf(stderr,"%s",cu.results.c_str());

	SETSHOWCURS;
	delete mainApp;
	return(0);
}
