#if 0

#
#//©keithhedger Mon 9 Mar 12:50:13 GMT 2020 kdhedger68713@gmail.com
#

#USEVALGRIND="valgrind --leak-check=full --suppressions=./ignorelibleaks -s"

g++ "$0" -Wall -ggdb -O0 -I.. -I../CursesToolKit/src -L../CursesToolKit/lib/.libs $(pkg-config --cflags --libs Magick++) -lcursestoolkit -o varsexample ||exit 1
LD_LIBRARY_PATH=../CursesToolKit/lib/.libs $USEVALGRIND ./varsexample
retval=$?
rm varsexample
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
	std::string				filepath="/tmp/testvar.config";

	if(argc>1)
		filepath=argv[1];

//bool
	vsitem.vType=BOOLVAR;
	vsitem.boolVar=true;
	vsitem.varName="firstboolvar";
	outvs.push_back(vsitem);
//int
	vsitem.vType=INTVAR;
	vsitem.intVar=666;
	vsitem.varName="secondvarvar";
	outvs.push_back(vsitem);
//hex
	vsitem.vType=INTVAR;
	vsitem.intVar=0xfe;
	vsitem.outIntType=HEXOUT;
	vsitem.varName="hexvar";
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

	mainApp->utils->CTK_saveVars(filepath.c_str(),outvs);
	invs=mainApp->utils->CTK_loadVars(filepath.c_str());

	for(unsigned j=0;j<invs.size();j++)
		{
			fprintf(stderr,"name=%s ",invs[j].varName.c_str());

			switch(invs[j].vType)
				{
					case BOOLVAR:
						fprintf(stderr,"type=bool ");
						fprintf(stderr,"val=%i\n",invs[j].boolVar);
						break;
					case INTVAR:
						fprintf(stderr,"type=int ");
						fprintf(stderr,"val=%i\n",invs[j].intVar);
						break;
					case CHARVAR:
						fprintf(stderr,"type=string ");
						fprintf(stderr,"val=%s\n",invs[j].charVar.c_str());
						break;
				}					
		}

	vsitem=mainApp->utils->CTK_findVar(invs,"firstboolvar");
	if(vsitem.vType!=BADTYPE)
		fprintf(stderr,"firstboolvar=%i\n",vsitem.boolVar);

	vsitem=mainApp->utils->CTK_findVar(invs,"3rdvar");
	if(vsitem.vType!=BADTYPE)
		fprintf(stderr,"3rdvar=%s\n",vsitem.charVar.c_str());

	vsitem=mainApp->utils->CTK_findVar(invs,"unkownvar");
	if(vsitem.vType==BADTYPE)
		fprintf(stderr,"var 'unkownvar' doesn't exist\n");

	vsitem=mainApp->utils->CTK_findVar(invs,"hexvar");
	if(vsitem.vType!=BADTYPE)
		fprintf(stderr,"hexvar=%x\n",vsitem.intVar);

	SETSHOWCURS;
	delete mainApp;
	outvs.clear(); 
	invs.clear(); 
	printf("\n");
	return 0;

}