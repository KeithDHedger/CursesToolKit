#if 0

#
#//©keithhedger Mon 9 Mar 12:50:13 GMT 2020 kdhedger68713@gmail.com
#

#Run eg:
#(cd ../;make -j4);./querybox.cpp "$(echo -e "Body String\nLine 2\nLine 3\nLine 4\n\e[30;43;51mLine 5\e[30;47m")" "Window name" "Dialog title" 7

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ "$0" -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit -o querybox ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./querybox "$@"
retval=$?
rm querybox
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	*cu;

	if(argc<5)
		{
			printf("USAGE:\nquerybox \"$(echo -e \"Body String\\nLine 2\\nLine 3\\nLine 4\\n\\e[30;43;51mLine 5\\e[30;47m\")\" \"Window name\" \"Dialog title\" Show buttons (1='YES', 2='NO', 4='CANCEL'), OR values together for combinations.\n");
			exit(1);
		}

	cu=new CTK_cursesUtilsClass();

	if(cu->CTK_queryDialog(argv[1],argv[2],argv[3],atoi(argv[4])))
		fprintf(stderr,"Button pressed=%i\n",cu->dialogReturnData.intValue);

	delete cu;
	SETSHOWCURS;
	return(0);
}
