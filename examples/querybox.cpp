#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./querybox.cpp "$(echo -e "Body String\nLine 2\nLine 3\nLine 4\n\e[30;43;51mLine 5\e[30;47m")" "Window name" "Dialog title" 7

USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit "$0"  -o querybox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./querybox "$@"
retval=$?
rm querybox
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	*cu;

	if(argc<5)
		{
			printf("USAGE:\nquerybox \"$(echo -e \"Body String\\nLine 2\\nLine 3\\nLine 4\\n\\e[30;43;51mLine 5\\e[30;47m\")\" \"Window name\" \"Dialog title\" Show buttons (1='YES', 2='NO', 4='CANCEL'), or values together for combinations.\n");
			exit(1);
		}

	cu=new CTK_cursesUtilsClass();

	if(cu->CTK_queryDialog(argv[1],argv[2],argv[3],atoi(argv[4])))
		fprintf(stderr,"Button pressed=%i\n",cu->dialogReturnData.intValue);

	//fprintf(stderr,"Button pressed=%i\n",cu->intResult);

	SETSHOWCURS;
	delete cu;
	return(0);
}
