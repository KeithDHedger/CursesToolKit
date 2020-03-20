#if 0

#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com
#Run eg:
#(cd ../;make -j4);./openfile.cpp ~

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit "$0"  -o fileselect ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./fileselect "$@"
retval=$?
rm fileselect
reset
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	cu;

//just pics
//	cu.CTK_fileChooserDialog(getenv("HOME"),CUOPENFILE,".png;.tiff;.jpeg;.jpg;");
//all
	cu.CTK_fileChooserDialog(getenv("HOME"),CUOPENFILE);
	if(cu.dialogReturnData.isValidData==true)
		fprintf(stderr,"%s\n",cu.dialogReturnData.stringValue.c_str());

	SETSHOWCURS;
	return(0);
}
