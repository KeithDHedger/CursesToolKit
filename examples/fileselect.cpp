#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com

g++ -Wall -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey) -lcursestoolkit "$0"  -o fileselect ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./fileselect "$@"
retval=$?
rm fileselect
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{

	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	std::string				str;
	CTK_cursesUtilsClass	cu;
	char					*folder=NULL;//get_current_dir_name();

	if(argc>1)
		folder=argv[1];

	cu.CTK_openFile(mainApp,folder);
	if(cu.isValidFile==true)
		fprintf(stderr,"%s",cu.selectedFile.c_str());

	SETSHOWCURS;
	delete mainApp;
	return(0);
}
