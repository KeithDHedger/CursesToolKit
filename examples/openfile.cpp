#if 0

#
#//©keithhedger Mon 9 Mar 12:50:13 GMT 2020 kdhedger68713@gmail.com
#

#Run eg:
#(cd ../;make -j4);./openfile.cpp

if [ "${USEVAL}" != "" ];then
	USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"
fi

pushd ..
	make||exit 100
popd

g++ "$0" -Wall -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit -o fileselect ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./fileselect
retval=$?
rm fileselect
exit $retval

#endif

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_cursesUtilsClass	cu;

//just pics
	//cu.CTK_fileChooserDialog(getenv("HOME"),CUOPENFILE,true,".png;.tiff;.jpeg;.jpg;");
//all
	cu.CTK_fileChooserDialog(getenv("HOME"),CUOPENFILE);
//	cu.CTK_fileChooserDialog("/media/Kodi/media/Movies3/Movies/Drama",CUOPENFILE);
	if(cu.dialogReturnData.isValidData==true)
		fprintf(stderr,"%s\n",cu.dialogReturnData.stringValue.c_str());

	SETSHOWCURS;
	return(0);
}
