#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./querybox.cpp "$(echo -e "Body String\nLine 2\nLine 3\nLine 4\n          \e[30;43;51mLine 5")" "Window name" "Dialog title" 7

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey) -lcursestoolkit "$0"  -o querybox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./querybox "$@"
retval=$?
rm querybox
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_mainAppClass		*mainApp;
	std::string				str;
	CTK_cursesUtilsClass	*cu;

	if(argc<5)
		{
			printf("USAGE:\ninputbox \"Body String\" \"Window name\" \"Dialog title\" \"Show buttons (1='YES', 2='NO', 4='CANCEL')\", or values together for combinations.\n");
			exit(1);
		}

	mainApp=new CTK_mainAppClass();
	cu=new CTK_cursesUtilsClass();

	cu->CTK_queryDialog(mainApp,argv[1],argv[2],argv[3],atoi(argv[4]));
	fprintf(stderr,"Button pressed=%i\n",cu->intResult);

	SETSHOWCURS;
	delete mainApp;
	delete cu;
	return(0);
}
