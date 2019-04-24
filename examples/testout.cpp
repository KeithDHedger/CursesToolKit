[00;35m#if 0[m

[00;35m#©keithhedger Sun 24 Mar 19:15:22 GMT 2019 kdhedger68713@gmail.com[m
[00;35m#Run eg:[m
[00;35m#(cd ../;make -j4);./inputbox.cpp[m [00;31m"Body text"[m[00;35m[m [00;31m"Default input"[m[00;35m[m [00;31m"Window name"[m[00;35m[m [00;31m"Dialog title"[m[00;35m 1[m

g[00;00m++ -[mWall [00;00m-[mI[00;00m../[mCursesToolKit[00;00m/[msrc [00;00m-[mL[00;00m../[mCursesToolKit[00;00m/[mlib[00;00m/.[mlibs $[00;00m([mpkg[00;00m-[mconfig [00;00m--[mcflags [00;00m--[mlibs termkey[00;00m) -[mlcursestoolkit [00;31m"$0"[m  [00;00m-[mo inputbox [00;00m||[mexit [00;34m1[m
LD_LIBRARY_PATH[00;00m=../[mCursesToolKit[00;00m/[mlib[00;00m/.[mlibs [00;00m./[minputbox [00;31m"$@"[m
retval[00;00m=[m$[00;00m?[m
rm inputbox
reset
exit $retval

[00;35m#endif[m

[00;35m#include <cursesGlobals.h>[m

[00;32mint[m [00;32mmain[m[00;00m([m[00;32mint[m argc[00;00m,[m [00;32mchar[m [00;00m**[margv[00;00m)[m
[00;00m{[m
	CTK_mainAppClass		[00;00m*[mmainApp[00;00m;[m
	[00;32mstd[m[00;00m::[mstring				str[00;00m;[m
	CTK_cursesUtilsClass	[00;00m*[mcu[00;00m;[m

	[00;33mif[m[00;00m([margc[00;00m<[m[00;34m6[m[00;00m)[m
		[00;00m{[m
			[00;32mprintf[m[00;00m([m[00;31m"USAGE:[m[00;35m\n[m[00;31minputbox[m [00;35m\"[m[00;31mBody String[m[00;35m\" \"[m[00;31mDefault Input String[m[00;35m\" \"[m[00;31mWindow name[m[00;35m\" \"[m[00;31mDialog title[m[00;35m\" \"[m[00;31mShow 'CANCEL' button (0=no show, 1=show)[m[00;35m\"\n[m[00;31m"[m[00;00m);[m
			[00;32mexit[m[00;00m([m[00;34m1[m[00;00m);[m
		[00;00m}[m

	mainApp[00;00m=[m[00;33mnew[m CTK_mainAppClass[00;00m;[m
	cu[00;00m=[m[00;33mnew[m CTK_cursesUtilsClass[00;00m;[m

	[00;33mif[m[00;00m([mcu[00;00m->[m[00;32mCTK_entryDialog[m[00;00m([mmainApp[00;00m,[margv[00;00m[[m[00;34m1[m[00;00m],[margv[00;00m[[m[00;34m2[m[00;00m],[margv[00;00m[[m[00;34m3[m[00;00m],[margv[00;00m[[m[00;34m4[m[00;00m],[m[00;32matoi[m[00;00m([margv[00;00m[[m[00;34m5[m[00;00m]))==[m[00;33mtrue[m[00;00m)[m
		[00;32mfprintf[m[00;00m([mstderr[00;00m,[m[00;31m"Input box=%s, Button pressed=%i[m[00;35m\n[m[00;31m"[m[00;00m,[mcu[00;00m->[mstringResult[00;00m.[m[00;32mc_str[m[00;00m(),[mcu[00;00m->[mintResult[00;00m);[m
	[00;33melse[m
		[00;32mfprintf[m[00;00m([mstderr[00;00m,[m[00;31m"Canceled ... Button pressed=%i[m[00;35m\n[m[00;31m"[m[00;00m,[mcu[00;00m->[mintResult[00;00m);[m

	SETSHOWCURS[00;00m;[m
	[00;33mdelete[m mainApp[00;00m;[m
	[00;33mdelete[m cu[00;00m;[m
	[00;33mreturn[m[00;00m([m[00;34m0[m[00;00m);[m
[00;00m}[m
