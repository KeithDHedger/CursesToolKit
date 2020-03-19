#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./openfile.cpp ~

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit "$0"  -o fileselect ||exit 1
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
	char					*folder=NULL;

	if(argc>1)
		folder=argv[1];

//just pics
//bool CTK_fileChooserDialog(const char *startdir,bool open,const char *filename,const char *filetypes)
	//cu.CTK_openFile(mainApp,"Open File Example",folder,true,NULL,".png;.tiff;");
	cu.CTK_fileChooserDialog(getenv("HOME"),true);
	if(cu.isValidFile==true)
		fprintf(stderr,"%s\n",cu.stringResult.c_str());

	SETSHOWCURS;
	delete mainApp;
	return(0);
}
