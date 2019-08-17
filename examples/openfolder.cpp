#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./openfolder.cpp ~

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey) -lcursestoolkit "$0"  -o folderselect ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs ./folderselect "$@"
retval=$?
rm folderselect
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	std::string				str;
	CTK_cursesUtilsClass	cu;
	char					*folder=NULL;

	if(argc>1)
		folder=argv[1];

//fprintf(stderr,"folder=%s\n",folder);
	cu.CTK_selectFolder(mainApp,"Select Folder Example",folder);
	if(cu.isValidFile==true)
		fprintf(stderr,"%s\n",cu.stringResult.c_str());

	SETSHOWCURS;
	delete mainApp;
	return(0);
}
