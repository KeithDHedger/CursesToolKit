#if 0

#©keithhedger Thu 19 Sep 11:20:00 BST 2019 kdhedger68713@gmail.com

#USEVALGRIND="valgrind --leak-check=full"

g++ -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs termkey Magick++) -lcursestoolkit "$0" -o varsexample ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./varsexample "$@"
retval=$?
rm varsexample
reset
exit $retval

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cursesGlobals.h>

int main(int argc, char **argv)
{
	CTK_mainAppClass		*mainApp=new CTK_mainAppClass();
	varsStruct				vsitem;
	std::vector<varsStruct> outvs;
	std::vector<varsStruct> invs;
	char					*filepath="/tmp/testvar.config";

	if(argc>1)
		filepath=argv[1];

//bool
	vsitem.vType=BOOLVAR;
	vsitem.boolVar=true;
	vsitem.varName="firstvar";
	outvs.push_back(vsitem);
//int
	vsitem.vType=INTVAR;
	vsitem.intVar=666;
	vsitem.varName="secondvarvar";
	outvs.push_back(vsitem);
//string
	vsitem.vType=CHARVAR;
	vsitem.charVar=filepath;
	vsitem.varName="3rdvar";
	outvs.push_back(vsitem);
//bool
	vsitem.vType=BOOLVAR;
	vsitem.boolVar=false;
	vsitem.varName="abool";
	outvs.push_back(vsitem);

//string
	vsitem.vType=CHARVAR;
	vsitem.charVar="this is string 2";
	vsitem.varName="4thvar";
	outvs.push_back(vsitem);

	mainApp->CTK_saveVars(filepath,outvs);
	invs=mainApp->CTK_loadVars(filepath);

	for(unsigned j=0;j<invs.size();j++)
		{
			fprintf(stderr,"name=%s ",invs[j].varName.c_str());
			fprintf(stderr,"type=%i ",invs[j].vType);
			switch(invs[j].vType)
				{
					case BOOLVAR:
						fprintf(stderr,"val=%i\n",invs[j].boolVar);
						break;
					case INTVAR:
						fprintf(stderr,"val=%i\n",invs[j].intVar);
						break;
					case CHARVAR:
						fprintf(stderr,"val=%s\n",invs[j].charVar.c_str());
						break;
				}					
		}

	fprintf(stderr,"\n>>>>>\n");
	vsitem=mainApp->CTK_findVar(invs,"firstvar");
	if(vsitem.vType!=BADTYPE)
		fprintf(stderr,"abool=%i\n",vsitem.boolVar);

	vsitem=mainApp->CTK_findVar(invs,"3rdvar");
	if(vsitem.vType!=BADTYPE)
		fprintf(stderr,"3rdvar=%s\n",vsitem.charVar.c_str());
	fprintf(stderr,">>>>>\n");

	SETSHOWCURS;
	delete mainApp;
	outvs.clear(); 
	invs.clear(); 
	printf("\n");
	return 0;

}