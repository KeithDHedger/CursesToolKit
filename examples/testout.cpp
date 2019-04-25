[00;34m/*[m
[00;34m *[m
[00;34m * ©K. D. Hedger. Tue 23 Apr 20:50:19 BST 2019 keithdhedger@gmail.com[m
[00;34m[m
[00;34m * This file (cursesSourceEditBox.cpp) is part of CursesToolKit.[m
[00;34m[m
[00;34m * CursesToolKit is free software: you can redistribute it and/or modify[m
[00;34m * it under the terms of the GNU General Public License as published by[m
[00;34m * the Free Software Foundation, either version 3 of the License, or[m
[00;34m * at your option) any later version.[m
[00;34m[m
[00;34m * CursesToolKit is distributed in the hope that it will be useful,[m
[00;34m * but WITHOUT ANY WARRANTY; without even the implied warranty of[m
[00;34m * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the[m
[00;34m   GNU General Public License for more details.[m
[00;34m[m
[00;34m * You should have received a copy of the GNU General Public License[m
[00;34m * along with CursesToolKit.  If not, see <http://www.gnu.org/licenses/>.[m
[00;34m */[m
 
[00;35m#include[m [00;31m"cursesSourceEditBox.h"[m[00;35m[m

[00;32mCTK_cursesSourceEditBoxClass[m[00;00m::~[m[00;32mCTK_cursesSourceEditBoxClass[m[00;00m()[m
[00;00m{[m
	[00;32mfree[m[00;00m([m[00;33mthis[m[00;00m->[mtxtBuffer[00;00m);[m
	[00;33mdelete this[m[00;00m->[mgc[00;00m;[m
	[00;32mtermkey_destroy[m[00;00m([m[00;33mthis[m[00;00m->[mtk[00;00m);[m
[00;00m}[m

[00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_cursesSourceEditBoxClass[m[00;00m()[m
[00;00m{[m
	[00;33mthis[m[00;00m->[mtk [00;00m=[m [00;32mtermkey_new[m[00;00m([m[00;34m0[m[00;00m,[mTERMKEY_FLAG_CTRLC[00;00m);[m
	[00;33mif[m[00;00m(![m[00;33mthis[m[00;00m->[mtk[00;00m)[m
		[00;00m{[m
			[00;32mfprintf[m[00;00m([mstderr[00;00m,[m [00;31m"Cannot allocate termkey instance[m[00;35m\n[m[00;31m"[m[00;00m);[m
			[00;32mexit[m[00;00m([m[00;34m1[m[00;00m);[m
		[00;00m}[m

	[00;33mthis[m[00;00m->[mgc[00;00m=[m[00;33mnew[m CTK_cursesGraphicsClass[00;00m;[m
	[00;33mthis[m[00;00m->[mgc[00;00m->[m[00;32mCTK_setColours[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m);[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_setColours[m[00;00m([mcoloursStruct cs[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[mcolours[00;00m=[mcs[00;00m;[m
	[00;33mthis[m[00;00m->[mgc[00;00m->[m[00;32mCTK_setColours[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m);[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_newBox[m[00;00m([m[00;32mint[m x[00;00m,[m[00;32mint[m y[00;00m,[m[00;32mint[m width[00;00m,[m[00;32mint[m hite[00;00m,[m[00;32mbool[m isfilename[00;00m,[m[00;32mconst char[m [00;00m*[mtxt[00;00m,[m[00;32mbool[m selectable[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[msx[00;00m=[mx[00;00m;[m
	[00;33mthis[m[00;00m->[msy[00;00m=[my[00;00m;[m
	[00;33mthis[m[00;00m->[mwid[00;00m=[mwidth[00;00m;[m
	[00;33mthis[m[00;00m->[mhite[00;00m=[mhite[00;00m-[m[00;34m1[m[00;00m;[m
	[00;33mthis[m[00;00m->[mcanSelect[00;00m=[mselectable[00;00m;[m

	[00;33mthis[m[00;00m->[mblank[00;00m.[m[00;32minsert[m[00;00m([m[00;33mthis[m[00;00m->[mblank[00;00m.[m[00;32mbegin[m[00;00m(),[mwidth[00;00m,[m[00;31m' '[m[00;00m);[m
	[00;33mthis[m[00;00m->[m[00;32mCTK_updateText[m[00;00m([mtxt[00;00m,[misfilename[00;00m);[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_updateText[m[00;00m([m[00;32mconst char[m [00;00m*[mtxt[00;00m,[m[00;32mbool[m isfilename[00;00m,[m[00;32mbool[m reset[00;00m)[m
[00;00m{[m
	[00;32mconst char[m					[00;00m*[mptr[00;00m=[mNULL[00;00m;[m
	[00;32mchar[m						[00;00m*[mbuffer[00;00m=[mNULL[00;00m;[m
	[00;32mint[m							startchr[00;00m=[m[00;34m0[m[00;00m;[m
	[00;32mchar[m						[00;00m*[mcpybuf[00;00m=([m[00;32mchar[m[00;00m*)[m[00;32malloca[m[00;00m([m[00;33mthis[m[00;00m->[mwid[00;00m+[m[00;34m1[m[00;00m+[m[00;34m1000[m[00;00m);[m
	[00;32mstd[m[00;00m::[mvector[00;00m<[m[00;32mstd[m[00;00m::[mstring[00;00m>[m	array[00;00m;[m
	[00;32mstd[m[00;00m::[mstring					str[00;00m;[m
	CTK_cursesUtilsClass		cu[00;00m;[m
	[00;32mlong[m						fsize[00;00m;[m
	[00;32mFILE[m						[00;00m*[mf[00;00m;[m

	[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32mclear[m[00;00m();[m
	[00;32mfreeAndNull[m[00;00m(&[m[00;33mthis[m[00;00m->[mtxtBuffer[00;00m);[m
	[00;33mif[m[00;00m([mreset[00;00m==[m[00;33mtrue[m[00;00m)[m
		[00;00m{[m
			[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;34m0[m[00;00m;[m
			[00;33mthis[m[00;00m->[mcurrentY[00;00m=[m[00;34m0[m[00;00m;[m
			[00;33mthis[m[00;00m->[mstartLine[00;00m=[m[00;34m0[m[00;00m;[m
		[00;00m}[m

	[00;33mif[m[00;00m([misfilename[00;00m==[m[00;33mfalse[m[00;00m)[m
		[00;33mthis[m[00;00m->[mtxtBuffer[00;00m=[m[00;32mstrdup[m[00;00m([mtxt[00;00m);[m
	[00;33melse[m
		[00;00m{[m
			f[00;00m=[m[00;32mfopen[m[00;00m([mtxt[00;00m,[m[00;31m"rb"[m[00;00m);[m
			[00;32mfseek[m[00;00m([mf[00;00m,[m[00;34m0[m[00;00m,[mSEEK_END[00;00m);[m
			fsize[00;00m=[m[00;32mftell[m[00;00m([mf[00;00m);[m
			[00;33mif[m[00;00m([mfsize[00;00m==[m[00;34m0[m[00;00m)[m
				[00;00m{[m
					[00;33mthis[m[00;00m->[mtxtBuffer[00;00m=([m[00;32mchar[m[00;00m*)[m[00;32mmalloc[m[00;00m([m[00;34m2[m[00;00m);[m
					[00;32msprintf[m[00;00m([m[00;33mthis[m[00;00m->[mtxtBuffer[00;00m,[m[00;31m"[m[00;35m\n[m[00;31m"[m[00;00m);[m
				[00;00m}[m
			[00;33melse[m
				[00;00m{[m
					[00;32mfseek[m[00;00m([mf[00;00m,[m[00;34m0[m[00;00m,[mSEEK_SET[00;00m);[m
					[00;33mthis[m[00;00m->[mtxtBuffer[00;00m=([m[00;32mchar[m[00;00m*)[m[00;32mmalloc[m[00;00m([mfsize[00;00m+[m[00;34m1[m[00;00m);[m
					[00;32mfread[m[00;00m([m[00;33mthis[m[00;00m->[mtxtBuffer[00;00m,[m[00;34m1[m[00;00m,[mfsize[00;00m,[mf[00;00m);[m
					[00;33mthis[m[00;00m->[mtxtBuffer[00;00m[[mfsize[00;00m]=[m[00;34m0[m[00;00m;[m
				[00;00m}[m
			[00;32mfclose[m[00;00m([mf[00;00m);[m
		[00;00m}[m

	str[00;00m=[m[00;33mthis[m[00;00m->[mtxtBuffer[00;00m;[m
	array[00;00m=[mcu[00;00m.[m[00;32mCTK_cursesUtilsClass[m[00;00m::[m[00;32mCTK_explode[m[00;00m([mstr[00;00m,[m[00;31m'[m[00;35m\n[m[00;31m'[m[00;00m);[m
	[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;34m0[m[00;00m;[mj[00;00m<[marray[00;00m.[m[00;32msize[m[00;00m();[mj[00;00m++)[m
		[00;00m{[m
			ptr[00;00m=[marray[00;00m[[mj[00;00m].[m[00;32mc_str[m[00;00m();[m
			[00;32mint[m numchars[00;00m=[m[00;34m0[m[00;00m;[m
			[00;32mint[m cnt[00;00m=[m[00;34m0[m[00;00m;[m
			startchr[00;00m=[m[00;34m0[m[00;00m;[m
			[00;32masprintf[m[00;00m(&[mbuffer[00;00m,[m[00;31m"%s[m[00;35m\n[m[00;31m"[m[00;00m,[mptr[00;00m);[m
			[00;33mwhile[m[00;00m([mcnt[00;00m<[m[00;32mstrlen[m[00;00m([mbuffer[00;00m))[m
				[00;00m{[m
					[00;33mwhile[m[00;00m(([mnumchars[00;00m<[m[00;33mthis[m[00;00m->[mwid[00;00m) && ([mcnt[00;00m<[m[00;32mstrlen[m[00;00m([mbuffer[00;00m)))[m
						[00;00m{[m
							cpybuf[00;00m[[mstartchr[00;00m]=[mbuffer[00;00m[[mcnt[00;00m++];[m
							[00;33mif[m[00;00m([mcpybuf[00;00m[[mstartchr[00;00m]==[m[00;34m0x1b[m[00;00m)[m
								[00;00m{[m
									startchr[00;00m++;[m
									[00;33mwhile[m[00;00m([mbuffer[00;00m[[mcnt[00;00m]!=[m[00;31m'm'[m[00;00m)[m
										[00;00m{[m
											cpybuf[00;00m[[mstartchr[00;00m++]=[mbuffer[00;00m[[mcnt[00;00m++];[m
										[00;00m}[m
									[00;34m//cpybuf[startchr]=buffer[cnt++];[m
									[00;34m//continue;[m
								[00;00m}[m
							[00;33melse[m
								[00;00m{[m
							[00;33mif[m[00;00m([mcpybuf[00;00m[[mstartchr[00;00m]==[m[00;31m'[m[00;35m\t[m[00;31m'[m[00;00m)[m
								numchars[00;00m+=[m[00;34m8[m[00;00m;[m
							[00;33melse[m
								numchars[00;00m++;[m
							startchr[00;00m++;[m
								[00;00m}[m
						[00;00m}[m
					cpybuf[00;00m[[mstartchr[00;00m]=[m[00;34m0[m[00;00m;[m
					[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32mpush_back[m[00;00m([mcpybuf[00;00m);[m
					startchr[00;00m=[m[00;34m0[m[00;00m;[m
					numchars[00;00m=[m[00;34m0[m[00;00m;[m
				[00;00m}[m
			[00;32mfree[m[00;00m([mbuffer[00;00m);[m
		[00;00m}[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_drawBox[m[00;00m([m[00;32mbool[m hilite[00;00m,[m[00;32mbool[m showcursor[00;00m)[m
[00;00m{[m
	[00;32mint[m	startchr[00;00m=[m[00;34m0[m[00;00m;[m
	[00;32mint[m j[00;00m;[m
	[00;32mint[m	linenum[00;00m=[m[00;34m0[m[00;00m;[m
	[00;32mint[m	boxline[00;00m=[m[00;34m0[m[00;00m;[m

	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mfancyGadgets[00;00m==[m[00;33mtrue[m[00;00m)[m
		[00;33mthis[m[00;00m->[mgc[00;00m->[m[00;32mCTK_drawBox[m[00;00m([m[00;33mthis[m[00;00m->[msx[00;00m-[m[00;34m1[m[00;00m,[m[00;33mthis[m[00;00m->[msy[00;00m-[m[00;34m1[m[00;00m,[m[00;33mthis[m[00;00m->[mwid[00;00m+[m[00;34m1[m[00;00m,[m[00;33mthis[m[00;00m->[mhite[00;00m+[m[00;34m1[m[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[mtextBoxType[00;00m,[m[00;33mfalse[m[00;00m);[m

	[00;32msetBackColour[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mbackCol[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[muse256Colours[00;00m);[m
	[00;32msetForeColour[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mforeCol[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[muse256Colours[00;00m);[m
	[00;32mMOVETO[m[00;00m([m[00;33mthis[m[00;00m->[msx[00;00m,[m[00;33mthis[m[00;00m->[msy[00;00m+[mhite[00;00m+[m[00;34m1[m[00;00m);[m
	[00;32mprintf[m[00;00m([m[00;31m"%s"[m[00;00m,[m[00;33mthis[m[00;00m->[mblank[00;00m.[m[00;32mc_str[m[00;00m());[m
	[00;32mMOVETO[m[00;00m([m[00;33mthis[m[00;00m->[msx[00;00m,[m[00;33mthis[m[00;00m->[msy[00;00m+[mhite[00;00m+[m[00;34m1[m[00;00m);[m
	[00;32mprintf[m[00;00m([m[00;31m"COL %i, LINE %i, MODE %s"[m[00;00m,[m[00;33mthis[m[00;00m->[mcurrentX[00;00m+[m[00;34m1[m[00;00m,[m[00;33mthis[m[00;00m->[mcurrentY[00;00m+[m[00;34m1[m[00;00m,[m[00;33mthis[m[00;00m->[meditStatus[00;00m);[m

	[00;33mif[m[00;00m([mhilite[00;00m==[m[00;33mtrue[m[00;00m)[m
		[00;00m{[m
			[00;32msetBackColour[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mhiliteBackCol[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[muse256Colours[00;00m);[m
			[00;32msetForeColour[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mhiliteForeCol[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[muse256Colours[00;00m);[m
		[00;00m}[m
	[00;33melse[m
		[00;00m{[m
			[00;32msetBackColour[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mbackCol[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[muse256Colours[00;00m);[m
			[00;32msetForeColour[m[00;00m([m[00;33mthis[m[00;00m->[mcolours[00;00m.[mforeCol[00;00m,[m[00;33mthis[m[00;00m->[mcolours[00;00m.[muse256Colours[00;00m);[m
		[00;00m}[m

[00;34m//	for(int j=0;j<this->hite;j++)[m
[00;34m//		{[m
[00;34m//			MOVETO(this->sx,this->sy+j);[m
[00;34m//			printf("%s",this->blank.c_str());[m
[00;34m//		}[m

	[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;34m1[m[00;00m)-[m[00;33mthis[m[00;00m->[mstartLine[00;00m<[m[00;33mthis[m[00;00m->[mhite[00;00m)[m
		[00;33mthis[m[00;00m->[mstartLine[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;33mthis[m[00;00m->[mhite[00;00m;[m

	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mstartLine[00;00m<[m[00;34m0[m[00;00m)[m
		[00;33mthis[m[00;00m->[mstartLine[00;00m=[m[00;34m0[m[00;00m;[m


	[00;33mwhile[m[00;00m(([mboxline[00;00m<[m[00;33mthis[m[00;00m->[mhite[00;00m) && ([mboxline[00;00m<[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()))[m
		[00;00m{[m
			[00;32mMOVETO[m[00;00m([m[00;33mthis[m[00;00m->[msx[00;00m,[m[00;33mthis[m[00;00m->[msy[00;00m+[mboxline[00;00m);[m
	[00;34m//		printf("%s",this->txtstrings[boxline+this->startLine].c_str());[m
			[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m]==[m[00;31m'[m[00;35m\n[m[00;31m'[m[00;00m)[m
				[00;32mprintf[m[00;00m([m[00;31m"%s%s"[m[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32msubstr[m[00;00m([m[00;34m0[m[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m).[m[00;32mc_str[m[00;00m(),[m[00;33mthis[m[00;00m->[mblank[00;00m.[m[00;32msubstr[m[00;00m([m[00;34m0[m[00;00m,[m[00;33mthis[m[00;00m->[mwid[00;00m-[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mlength[m[00;00m()+[m[00;34m1[m[00;00m).[m[00;32mc_str[m[00;00m());[m
			[00;33melse[m
				[00;32mprintf[m[00;00m([m[00;31m"%s%s"[m[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m(),[m[00;33mthis[m[00;00m->[mblank[00;00m.[m[00;32msubstr[m[00;00m([m[00;34m0[m[00;00m,[m[00;33mthis[m[00;00m->[mwid[00;00m-[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mlength[m[00;00m()).[m[00;32mc_str[m[00;00m());[m

			[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mcurrentY[00;00m==[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m) && ([mshowcursor[00;00m==[m[00;33mtrue[m[00;00m))[m
				[00;00m{[m
					[00;32mMOVETO[m[00;00m([m[00;33mthis[m[00;00m->[msx[00;00m,[m[00;33mthis[m[00;00m->[msy[00;00m+[mboxline[00;00m);[m
					[00;33mfor[m[00;00m([mj[00;00m=[m[00;34m0[m[00;00m;[mj[00;00m<[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[mj[00;00m++)[m
						[00;00m{[m
							[00;32mprintf[m[00;00m([m[00;31m"%c"[m[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m]);[m
						[00;00m}[m

					[00;33mswitch[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m])[m
						[00;00m{[m
							[00;33mcase[m [00;34m0x1b[m[00;00m:[m
								[00;33mwhile[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m]==[m[00;34m0x1b[m[00;00m)[m
									[00;00m{[m
										[00;33mwhile[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m++]!=[m[00;31m'm'[m[00;00m);[m
										j[00;00m++;[m
									[00;00m}[m
								[00;32mprintf[m[00;00m([m INVERSEON [00;31m"%c"[m INVERSEOFF[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m]);[m
								[00;33mbreak[m[00;00m;[m
							[00;33mcase[m [00;31m'[m[00;35m\t[m[00;31m'[m[00;00m:[m
								[00;32mprintf[m[00;00m([mINVERSEON SPACETAB INVERSEOFF[00;00m);[m
								[00;33mbreak[m[00;00m;[m
							[00;33mcase[m [00;31m'[m[00;35m\n[m[00;31m'[m[00;00m:[m
								[00;32mprintf[m[00;00m([mINVERSEON SPACENL INVERSEOFF[00;00m);[m
								[00;33mbreak[m[00;00m;[m
							[00;33mdefault[m[00;00m:[m
								[00;32mprintf[m[00;00m([m INVERSEON [00;31m"%c"[m INVERSEOFF[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m]);[m
								[00;33mbreak[m[00;00m;[m
						[00;00m}[m
					j[00;00m++;[m
					[00;33mwhile[m[00;00m([mj[00;00m<[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mlength[m[00;00m())[m
						[00;32mprintf[m[00;00m([m[00;31m"%c"[m[00;00m,[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mboxline[00;00m+[m[00;33mthis[m[00;00m->[mstartLine[00;00m].[m[00;32mc_str[m[00;00m()[[mj[00;00m++]);[m
				[00;00m}[m
			boxline[00;00m++;[m
		[00;00m}[m
	[00;32mMOVETO[m[00;00m([m[00;33mthis[m[00;00m->[msx[00;00m,[m[00;33mthis[m[00;00m->[msy[00;00m+[mboxline[00;00m);[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_doEditEvent[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	TermKeyResult	ret[00;00m;[m
	TermKeyKey		key[00;00m;[m
	[00;32mint[m				lineadd[00;00m=[m[00;34m1[m[00;00m;[m

	[00;32mchar[m			tstr[00;00m[[m[00;34m3[m[00;00m]={[m[00;31m'_'[m[00;00m,[m[00;34m0[m[00;00m,[m[00;34m0[m[00;00m};[m

	[00;33mthis[m[00;00m->[meditStatus[00;00m=[m[00;31m"Edit Mode"[m[00;00m;[m
	[00;33mthis[m[00;00m->[m[00;32mCTK_drawBox[m[00;00m([m[00;33mfalse[m[00;00m,[m[00;33mtrue[m[00;00m);[m
	[00;32mfflush[m[00;00m([mNULL[00;00m);[m
	[00;33mthis[m[00;00m->[mrunLoop[00;00m=[m[00;33mtrue[m[00;00m;[m

	[00;33mwhile[m[00;00m([m[00;33mthis[m[00;00m->[mrunLoop[00;00m==[m[00;33mtrue[m[00;00m)[m
		[00;00m{[m
			ret[00;00m=[m[00;32mtermkey_waitkey[m[00;00m([m[00;33mthis[m[00;00m->[mtk[00;00m,&[mkey[00;00m);[m
			lineadd[00;00m=[m[00;34m1[m[00;00m;[m
			[00;33mswitch[m[00;00m([mkey[00;00m.[mtype[00;00m)[m
				[00;00m{[m
					[00;33mcase[m TERMKEY_TYPE_UNICODE[00;00m:[m
						[00;33mif[m[00;00m([mkey[00;00m.[mmodifiers[00;00m==[mTERMKEY_KEYMOD_CTRL[00;00m)[m
							[00;00m{[m
								tstr[00;00m[[m[00;34m1[m[00;00m]=[m[00;32mtoupper[m[00;00m([mkey[00;00m.[mcode[00;00m.[mcodepoint[00;00m);[m
								[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;34m0[m[00;00m;[mj[00;00m<[m[00;33mthis[m[00;00m->[mmc[00;00m->[mmenuBar[00;00m->[mmenuNames[00;00m.[m[00;32msize[m[00;00m();[mj[00;00m++)[m
									[00;00m{[m
										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mmc[00;00m->[mmenuBar[00;00m->[m[00;32mCTK_doShortCutKey[m[00;00m([mtstr[00;00m[[m[00;34m1[m[00;00m],[mj[00;00m)==[m[00;33mtrue[m[00;00m)[m
											[00;00m{[m
												[00;33mthis[m[00;00m->[mmc[00;00m->[mmenuBar[00;00m->[mmenuNumber[00;00m=[mj[00;00m;[m
												[00;33mthis[m[00;00m->[mmc[00;00m->[mmenuBar[00;00m->[m[00;32mselectCB[m[00;00m([m[00;33mthis[m[00;00m->[mmc[00;00m->[mmenuBar[00;00m);[m
												[00;33mbreak[m[00;00m;[m
											[00;00m}[m
									[00;00m}[m
								[00;33mbreak[m[00;00m;[m
							[00;00m}[m

						[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32minsert[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m,[m[00;34m1[m[00;00m,[mkey[00;00m.[mcode[00;00m.[mcodepoint[00;00m);[m
						[00;33mthis[m[00;00m->[mcurrentX[00;00m++;[m
						[00;33mbreak[m[00;00m;[m
					[00;33mcase[m TERMKEY_TYPE_KEYSYM[00;00m:[m
						[00;00m{[m
							[00;33mswitch[m[00;00m([mkey[00;00m.[mcode[00;00m.[msym[00;00m)[m
								[00;00m{[m
									[00;33mcase[m TERMKEY_SYM_BACKSPACE[00;00m:[m
										[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mcurrentY[00;00m==[m[00;34m0[m[00;00m) && ([m[00;33mthis[m[00;00m->[mcurrentX[00;00m==[m[00;34m0[m[00;00m))[m
											[00;33mbreak[m[00;00m;[m
										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m>[m[00;34m0[m[00;00m)[m
											[00;00m{[m
												[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32merase[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m-[m[00;34m1[m[00;00m,[m[00;34m1[m[00;00m);[m
												[00;33mthis[m[00;00m->[mcurrentX[00;00m--;[m
												[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
												[00;33mbreak[m[00;00m;[m
											[00;00m}[m				

										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentY[00;00m>[m[00;34m0[m[00;00m)[m
											[00;00m{[m
												[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;34m1[m[00;00m].[m[00;32merase[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;34m1[m[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m,[m[00;34m1[m[00;00m);[m
												[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;34m1[m[00;00m].[m[00;32mlength[m[00;00m()>[m[00;34m0[m[00;00m)[m
													[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;34m1[m[00;00m].[m[00;32mlength[m[00;00m();[m

												[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;34m1[m[00;00m].[m[00;32mappend[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m]);[m
												[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32merase[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32mbegin[m[00;00m()+[m[00;33mthis[m[00;00m->[mcurrentY[00;00m);[m
												[00;33mthis[m[00;00m->[mcurrentY[00;00m--;[m
												[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
												[00;33mbreak[m[00;00m;[m
											[00;00m}[m
										[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
										[00;33mthis[m[00;00m->[m[00;32mCTK_drawBox[m[00;00m([m[00;33mfalse[m[00;00m,[m[00;33mtrue[m[00;00m);[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m  TERMKEY_SYM_DELETE[00;00m:[m
										[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32merase[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m,[m[00;34m1[m[00;00m);[m
										[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_ENTER[00;00m:[m
										[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32minsert[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m,[m[00;34m1[m[00;00m,[m[00;31m'[m[00;35m\n[m[00;31m'[m[00;00m);[m
										[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;34m0[m[00;00m;[m
										[00;33mthis[m[00;00m->[mcurrentY[00;00m++;[m

										[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;33mthis[m[00;00m->[mstartLine[00;00m)>=[m[00;33mthis[m[00;00m->[mhite[00;00m)[m
											[00;33mthis[m[00;00m->[mstartLine[00;00m++;[m

										[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_TAB[00;00m:[m
										[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32minsert[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m,[m[00;34m1[m[00;00m,[m[00;31m'[m[00;35m\t[m[00;31m'[m[00;00m);[m
										[00;33mthis[m[00;00m->[mcurrentX[00;00m++;[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_ESCAPE[00;00m:[m
										[00;33mthis[m[00;00m->[mrunLoop[00;00m=[m[00;33mfalse[m[00;00m;[m
										[00;33mcontinue[m[00;00m;[m
										[00;33mbreak[m[00;00m;[m
								[00;33mcase[m TERMKEY_SYM_HOME[00;00m:[m
									[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;34m0[m[00;00m;[m
									[00;33mbreak[m[00;00m;[m
								[00;33mcase[m TERMKEY_SYM_END[00;00m:[m
									[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m;[m
									[00;33mbreak[m[00;00m;[m

									[00;33mcase[m TERMKEY_SYM_PAGEUP[00;00m:[m
										lineadd[00;00m=[m[00;33mthis[m[00;00m->[mhite[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_UP[00;00m:[m
										[00;33mthis[m[00;00m->[mcurrentY[00;00m-=[mlineadd[00;00m;[m
										[00;33mif[m[00;00m([mcurrentY[00;00m<[m[00;33mthis[m[00;00m->[mstartLine[00;00m)[m
											[00;33mthis[m[00;00m->[mstartLine[00;00m-=[mlineadd[00;00m;[m
										[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mcurrentY[00;00m<[m[00;34m0[m[00;00m) || ([m[00;33mthis[m[00;00m->[mstartLine[00;00m<[m[00;34m0[m[00;00m))[m
											[00;00m{[m
												[00;33mthis[m[00;00m->[mcurrentY[00;00m=[m[00;34m0[m[00;00m;[m
												[00;33mthis[m[00;00m->[mstartLine[00;00m=[m[00;34m0[m[00;00m;[m
											[00;00m}[m
										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m>=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m())[m
											[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m;[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_PAGEDOWN[00;00m:[m
										lineadd[00;00m=[m[00;33mthis[m[00;00m->[mhite[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_DOWN[00;00m:[m
										[00;33mthis[m[00;00m->[mcurrentY[00;00m+=[mlineadd[00;00m;[m
										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentY[00;00m>=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m())[m
											[00;33mthis[m[00;00m->[mcurrentY[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;34m1[m[00;00m;[m
										[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mcurrentY[00;00m-[m[00;33mthis[m[00;00m->[mstartLine[00;00m)>=[m[00;33mthis[m[00;00m->[mhite[00;00m)[m
											[00;33mthis[m[00;00m->[mstartLine[00;00m+=[mlineadd[00;00m;[m
										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m>=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m())[m
											[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m;[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_LEFT[00;00m:[m
										[00;33mthis[m[00;00m->[mcurrentX[00;00m--;[m
										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m<[m[00;34m0[m[00;00m)[m
											[00;00m{[m
												[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentY[00;00m>[m[00;34m0[m[00;00m)[m
													[00;00m{[m
														[00;33mthis[m[00;00m->[mcurrentY[00;00m--;[m
														[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32msize[m[00;00m()-[m[00;34m1[m[00;00m;[m
													[00;00m}[m
												[00;33melse[m
													[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;34m0[m[00;00m;[m
											[00;00m}[m
										[00;33mbreak[m[00;00m;[m
									[00;33mcase[m TERMKEY_SYM_RIGHT[00;00m:[m
										[00;33mthis[m[00;00m->[mcurrentX[00;00m++;[m
[00;34m//										if(this->txtstrings[this->currentY].c_str()[this->currentX]==0x1b)[m
[00;34m//											{[m
[00;34m//											fprintf(stderr,"currentx=%i\n%c",this->txtstrings[this->currentY].c_str()[this->currentX]);[m
[00;34m//												while(this->txtstrings[this->currentY].c_str()[this->currentX]!='m')[m
[00;34m//													{[m
[00;34m//														this->currentX++;[m
[00;34m//													fprintf(stderr,"%c",this->txtstrings[this->currentY].c_str()[this->currentX]);[m
[00;34m//													}[m
[00;34m//												this->currentX++;[m
[00;34m//												fprintf(stderr,"%c\n",this->txtstrings[this->currentY].c_str()[this->currentX]);[m
[00;34m//											}[m

										[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m>=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mcurrentY[00;00m].[m[00;32mlength[m[00;00m())[m
											[00;00m{[m
												[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentY[00;00m<[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;34m1[m[00;00m)[m
													[00;00m{[m
														[00;33mthis[m[00;00m->[mcurrentY[00;00m++;[m
														[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;34m0[m[00;00m;[m
													[00;00m}[m
												[00;33melse[m
													[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m;[m
											[00;00m}[m
										[00;33mbreak[m[00;00m;[m
								[00;00m}[m
						[00;00m}[m
				[00;00m}[m
			[00;33mthis[m[00;00m->[m[00;32mskipAnsi[m[00;00m();[m
			[00;33mthis[m[00;00m->[m[00;32mCTK_drawBox[m[00;00m([m[00;33mfalse[m[00;00m,[m[00;33mtrue[m[00;00m);[m
		[00;00m}[m
	[00;33mthis[m[00;00m->[meditStatus[00;00m=[m[00;31m"Normal"[m[00;00m;[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mupdateBuffer[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;32mstd[m[00;00m::[mstring buff[00;00m;[m
	buff[00;00m.[m[00;32mclear[m[00;00m();[m

	[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;34m0[m[00;00m;[mj[00;00m<[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m();[mj[00;00m++)[m
		buff[00;00m.[m[00;32mappend[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[mj[00;00m]);[m

	[00;33mthis[m[00;00m->[m[00;32mCTK_updateText[m[00;00m([mbuff[00;00m.[m[00;32mc_str[m[00;00m(),[m[00;33mfalse[m[00;00m,[m[00;33mfalse[m[00;00m);[m
[00;00m}[m

[00;32mconst char[m [00;00m*[m[00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_getBuffer[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
	[00;33mreturn[m[00;00m([m[00;33mthis[m[00;00m->[mtxtBuffer[00;00m);[m
[00;00m}[m

[00;32mconst[m [00;32mstd[m[00;00m::[mstring [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_getCurrentLine[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;33mreturn[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m]);[m
[00;00m}[m

[00;32mconst[m [00;32mstd[m[00;00m::[mstring [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_getCurrentWord[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;32mint[m startchr[00;00m=[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[m
	[00;32mint[m endchr[00;00m=[mstartchr[00;00m;[m

	[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[mj[00;00m>=[m[00;34m0[m[00;00m;[mj[00;00m--)[m
		[00;33mif[m[00;00m([m[00;32misalnum[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m][[mj[00;00m])==[m[00;33mfalse[m[00;00m)[m
			[00;33mbreak[m[00;00m;[m
		[00;33melse[m
			startchr[00;00m=[mj[00;00m;[m

	[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[mj[00;00m<[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m();[mj[00;00m++)[m
		[00;33mif[m[00;00m([m[00;32misalnum[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m][[mj[00;00m])==[m[00;33mfalse[m[00;00m)[m
			[00;33mbreak[m[00;00m;[m
		[00;33melse[m
			endchr[00;00m=[mj[00;00m;[m
			
	[00;33mreturn[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32msubstr[m[00;00m([mstartchr[00;00m,[mendchr[00;00m-[mstartchr[00;00m+[m[00;34m1[m[00;00m));[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_deleteCurrentWord[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;32mint[m startchr[00;00m=[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[m
	[00;32mint[m endchr[00;00m=[mstartchr[00;00m;[m

	[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[mj[00;00m>=[m[00;34m0[m[00;00m;[mj[00;00m--)[m
		[00;33mif[m[00;00m([m[00;32misalnum[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m][[mj[00;00m])==[m[00;33mfalse[m[00;00m)[m
			[00;33mbreak[m[00;00m;[m
		[00;33melse[m
			startchr[00;00m=[mj[00;00m;[m

	[00;33mfor[m[00;00m([m[00;32mint[m j[00;00m=[m[00;33mthis[m[00;00m->[mcurrentX[00;00m;[mj[00;00m<[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m();[mj[00;00m++)[m
		[00;33mif[m[00;00m([m[00;32misalnum[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m][[mj[00;00m])==[m[00;33mfalse[m[00;00m)[m
			[00;33mbreak[m[00;00m;[m
		[00;33melse[m
			endchr[00;00m=[mj[00;00m;[m
	[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32merase[m[00;00m([mstartchr[00;00m,[mendchr[00;00m-[mstartchr[00;00m+[m[00;34m1[m[00;00m);[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_deleteCurrentLine[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32merase[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32mbegin[m[00;00m()+[m[00;33mthis[m[00;00m->[mcurrentY[00;00m);[m
[00;00m}[m


[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_insertText[m[00;00m([m[00;32mconst char[m [00;00m*[mtxt[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32minsert[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m,[mtxt[00;00m);[m
	[00;33mthis[m[00;00m->[m[00;32mupdateBuffer[m[00;00m();[m
	[00;33mthis[m[00;00m->[mcurrentX[00;00m+=[m[00;32mstrlen[m[00;00m([mtxt[00;00m);[m
	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m>=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m())[m
		[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m;[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_gotoXY[m[00;00m([m[00;32mint[m x[00;00m,[m[00;32mint[m y[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[mcurrentX[00;00m=[mx[00;00m;[m
	[00;33mthis[m[00;00m->[mcurrentY[00;00m=[my[00;00m;[m
	[00;33mthis[m[00;00m->[mstartLine[00;00m=[my[00;00m;[m
	[00;33mthis[m[00;00m->[m[00;32madjustXY[m[00;00m();[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mCTK_setRunLoop[m[00;00m([m[00;32mbool[m loop[00;00m)[m
[00;00m{[m
	[00;33mthis[m[00;00m->[mrunLoop[00;00m=[mloop[00;00m;[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32mskipAnsi[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
[00;34m//this->adjustXY();[m
[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mcurrentX[00;00m]==[m[00;31m'm'[m[00;00m) && ([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mcurrentX[00;00m-[m[00;34m2[m[00;00m]==[mESCCHAR[00;00m))[m
	[00;00m{[m
		[00;33mthis[m[00;00m->[mcurrentX[00;00m-=[m[00;34m3[m[00;00m;[m
		[00;33mthis[m[00;00m->[m[00;32mskipAnsi[m[00;00m();[m
		[00;33mreturn[m[00;00m;[m
	[00;00m}[m
[00;33mif[m[00;00m(([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mcurrentX[00;00m]==[m[00;31m'm'[m[00;00m) && ([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mcurrentX[00;00m-[m[00;34m7[m[00;00m]==[mESCCHAR[00;00m))[m
	[00;00m{[m
		[00;33mthis[m[00;00m->[mcurrentX[00;00m-=[m[00;34m8[m[00;00m;[m
		[00;33mthis[m[00;00m->[m[00;32mskipAnsi[m[00;00m();[m
		[00;33mreturn[m[00;00m;[m
	[00;00m}[m
	[00;34m//if(this->txtstrings[this->currentY].c_str()[this->currentX]==0x1b)[m
	[00;33mwhile[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mcurrentX[00;00m]==[m[00;34m0x1b[m[00;00m)[m
		[00;00m{[m
			[00;34m//fprintf(stderr,"currentx=%i\n%c",this->txtstrings[this->currentY].c_str()[this->currentX]);[m
			[00;33mwhile[m[00;00m([m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mc_str[m[00;00m()[[m[00;33mthis[m[00;00m->[mcurrentX[00;00m]!=[m[00;31m'm'[m[00;00m)[m
				[00;00m{[m
					[00;33mthis[m[00;00m->[mcurrentX[00;00m++;[m
					[00;34m//fprintf(stderr,"%c",this->txtstrings[this->currentY].c_str()[this->currentX]);[m
				[00;00m}[m
			[00;33mthis[m[00;00m->[mcurrentX[00;00m++;[m
			[00;34m//fprintf(stderr,"%c\n",this->txtstrings[this->currentY].c_str()[this->currentX]);[m
		[00;00m}[m

[00;34m//	this->skipAnsi();[m
[00;00m}[m

[00;32mvoid[m [00;32mCTK_cursesSourceEditBoxClass[m[00;00m::[m[00;32madjustXY[m[00;00m([m[00;32mvoid[m[00;00m)[m
[00;00m{[m
	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentY[00;00m<[m[00;34m0[m[00;00m)[m
		[00;33mthis[m[00;00m->[mcurrentY[00;00m=[m[00;34m0[m[00;00m;[m
	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentY[00;00m>[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;34m2[m[00;00m)[m
		[00;00m{[m
			[00;33mthis[m[00;00m->[mcurrentY[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;34m1[m[00;00m;[m
			[00;33mthis[m[00;00m->[mstartLine[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m.[m[00;32msize[m[00;00m()-[m[00;33mthis[m[00;00m->[mhite[00;00m;[m
		[00;00m}[m
	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m<[m[00;34m0[m[00;00m)[m
		[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;34m0[m[00;00m;[m
	[00;33mif[m[00;00m([m[00;33mthis[m[00;00m->[mcurrentX[00;00m>[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m)[m
		[00;33mthis[m[00;00m->[mcurrentX[00;00m=[m[00;33mthis[m[00;00m->[mtxtstrings[00;00m[[m[00;33mthis[m[00;00m->[mcurrentY[00;00m].[m[00;32mlength[m[00;00m()-[m[00;34m1[m[00;00m;[m
[00;00m}[m


