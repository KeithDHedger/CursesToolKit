#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey) -lcursestoolkit "$0"  -o querybox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./querybox "$@"
retval=$?
rm querybox
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
//
//printf("y=%i n=%i c=%i\n",YESBUTTON,NOBUTTON,CANCELBUTTON);
//return 0;

	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	std::string				str;
	CTK_cursesUtilsClass	cu;

	if(argc<3)
		{
			printf("USAGE:\ninputbox \"Body String\" \"Show buttons (1='YES', 2='NO', 4='CANCEL')\", or values together for combinations.\n");
			exit(1);
		}

	cu.CTK_queryDialog(mainApp,argv[1],atoi(argv[2]));
	fprintf(stderr,"button pressed=%i",cu.intResult);

	SETSHOWCURS;
	delete mainApp;
	return(0);
}
