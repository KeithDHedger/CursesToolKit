#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ..;make -j4);./inputbox.cpp "$(echo -ne "Body String line 1\nLine 2\nLine 3\nLine 4")" "Default Input String" "Window name" "Dialog title"  1 40

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit "$0"  -o inputbox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./inputbox "$@"
retval=$?
rm inputbox
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	*cu=new CTK_cursesUtilsClass;

	if(argc<7)
		{
			printf("USAGE:\ninputbox \"Body String\" \"Default Input String\" \"Window name\" \"Dialog title\" \"Show 'CANCEL' button (0=no show, 1=show)\" Dialog Width\n");
			exit(1);
		}
	
	if(cu->CTK_entryDialog(argv[1],argv[2],argv[3],argv[4],atoi(argv[5]),atoi(argv[6]))==true)
		fprintf(stderr,"Input box=%s, Button pressed=%i\n",cu->dialogReturnData.stringValue.c_str(),cu->dialogReturnData.intValue);

	SETSHOWCURS;
	delete cu;
	return(0);
}
