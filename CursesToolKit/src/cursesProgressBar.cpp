/*
 *
 * ©K. D. Hedger. Fri 19 Apr 16:09:52 BST 2019 keithdhedger@gmail.com

 * This file (cursesLabel.cpp) is part of CursesToolKit.

 * CursesToolKit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * CursesToolKit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CursesToolKit.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cursesGlobals.h"

#include <chrono>
#include <math.h>

/**
* Progress Bar class destroy.
*/
CTK_cursesProgressBarClass::~CTK_cursesProgressBarClass()
{
}

/**
* Progress Bar class.
*/
CTK_cursesProgressBarClass::CTK_cursesProgressBarClass(CTK_mainAppClass *mc)
{
	varsStruct	vsitem;

	this->CTK_setCommon(mc);

	
	this->gadgetColours.foreCol=this->mc->gc->CTK_getColourFromNamedVar("barforecol",this->gadgetColours.foreCol);
	this->gadgetColours.backCol=this->mc->gc->CTK_getColourFromNamedVar("barbackcol",this->gadgetColours.backCol);
	this->gadgetColours.hiliteForeCol=this->mc->gc->CTK_getColourFromNamedVar("barthiliteforecol",this->gadgetColours.hiliteForeCol);
	this->gadgetColours.hiliteBackCol=this->mc->gc->CTK_getColourFromNamedVar("barhilitebackcol",this->gadgetColours.hiliteBackCol);
	this->gadgetColours.disabledForeCol=this->mc->gc->CTK_getColourFromNamedVar("bardisabledforecol",this->gadgetColours.disabledForeCol);
	this->gadgetColours.disabledBackCol=this->mc->gc->CTK_getColourFromNamedVar("bardisabledbackcol",this->gadgetColours.disabledBackCol);

	this->gadgetColours.gadgetCustom1ForeCol=this->mc->gc->CTK_getColourFromNamedVar("barfillforecol",this->mc->windowColours.foreCol);
	this->gadgetColours.gadgetCustom1BackCol=this->mc->gc->CTK_getColourFromNamedVar("barfillbackcol",this->mc->windowColours.backCol);

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"barfancy");
	if(vsitem.vType==BOOLVAR)
		this->gadgetColours.useFancy=vsitem.boolVar;

	vsitem=this->mc->utils->CTK_findVar(this->mc->newAppColours,"barboxtype");
	if(vsitem.vType==INTVAR)
		this->gadgetColours.boxType=vsitem.intVar;
	else
		this->gadgetColours.boxType=INBOX;

	this->isSelectable=false;
	this->type=BARGADGET;
}

/**
* New Progress Bar.
*/
void CTK_cursesProgressBarClass::CTK_newBar(int x,int y,int width,double min,double max, double val,char fill)
{
	this->sx=x;
	this->sy=y;
	this->wid=width;
	this->hite=1;
	this->gadgetDirty=true;
	this->maxvalue=max;
	this->minvalue=min;
	this->value=val;
	this->fillchar=fill;
}
			
std::string CTK_cursesProgressBarClass::convertValueToTime(double value)
{
	char						buffer[256]={0};
	std::string					retstr="";
	int							s=value*(1000);
	std::chrono::milliseconds	msec(s);
	double						intpart;
	const char					*zeros="";

	if(this->valuesAsTime==false)
		{
			snprintf(buffer,255,"%.*f",this->scale,value);
			retstr=buffer;
			return(retstr);
		}
	else
		{
			if(value>=3600)
				{
					retstr+=std::to_string(std::chrono::duration_cast<std::chrono::hours>(msec).count());
					retstr+=":";
				}
			if(value>=60)
				{
					retstr+=std::to_string(std::chrono::duration_cast<std::chrono::minutes>(msec).count() % 60);
					retstr+=":";
				}

			if((this->showZeroMinutes==true) && (value<60))
				zeros="0:";

			if(value>=1)
				{
					retstr+=zeros;
					retstr+=std::to_string(std::chrono::duration_cast<std::chrono::seconds>(msec).count() % 60);
					if(this->scale>0)
						{
							retstr+=".";
							snprintf(buffer,255,"%.*f",this->scale,modf (value , &intpart));
							retstr+=&buffer[2];
						}
				}
			else
				{
					snprintf(buffer,255,"%s%.*f",zeros,this->scale,modf(value,&intpart));
					retstr+=buffer;
				}
		}

	return(retstr);
}

/**
* Draw Progress Bar.
*/
void CTK_cursesProgressBarClass::CTK_drawGadget(bool hilite)
{
	double		absscale=(double)this->wid/(this->maxvalue-this->minvalue);
	double		abswid=absscale*(this->value-this->minvalue);
	int			absx;
	int			jx;
	std::string	timevalue;

	if(this->gadgetColours.useFancy==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->gadgetColours.boxType,false);

//show gauge
	if((this->showValues==SHOWGAUGE) ||(this->showValues==SHOWGAUGEVALUE))
		{
			timevalue=this->convertValueToTime(this->value);
			MOVETO(this->sx+1,this->sy-1);
			printf("%s\n",timevalue.c_str());
			if(this->showValues==SHOWGAUGEVALUE)
				{
					timevalue=this->convertValueToTime(this->value);
					MOVETO((this->sx+this->wid/2)-(timevalue.length()/2),this->sy-1);
					printf("%s\n",timevalue.c_str());
				}
			timevalue=this->convertValueToTime(this->maxvalue-this->value);
			MOVETO(this->sx+this->wid-timevalue.length()-1,this->sy-1);
			printf("%s\n",timevalue.c_str());
		}

//min
	if((this->showValues==SHOWMINMAX) || (this->showValues==SHOWALL))
		{
			timevalue=this->convertValueToTime(this->minvalue);
			MOVETO(this->sx+1,this->sy-1);
			printf("%s\n",timevalue.c_str());
//max
			timevalue=this->convertValueToTime(this->maxvalue);
			MOVETO(this->sx+this->wid-timevalue.length()-1,this->sy-1);
			printf("%s\n",timevalue.c_str());
		}
//value
	if((this->showValues==SHOWVALUE) || (this->showValues==SHOWALL))
		{
			if(this->valueAsReal==true)
				{
					timevalue=this->convertValueToTime(this->value);
					MOVETO((this->sx+this->wid/2)-(timevalue.length()/2),this->sy-1);
					printf("%s\n",timevalue.c_str());
				}
			else
				{
					timevalue=this->convertValueToTime(((this->value-this->minvalue)/(this->maxvalue-this->minvalue))*100);
					MOVETO((this->sx+this->wid/2)-(timevalue.length()/2),this->sy-1);
					printf("%s%%\n",timevalue.c_str());
				}
		}

	setBothColours(this->gadgetColours.gadgetCustom1ForeCol,this->gadgetColours.gadgetCustom1BackCol);
	MOVETO(this->sx,this->sy);
	printf("%*s", this->wid,"");

	switch(this->style)
		{
			case FILLEDBAR:
				setBothColours(this->gadgetColours.foreCol,this->gadgetColours.backCol);
				MOVETO(this->sx,this->sy);
				printf("%*s", this->wid,"");
			case BAR:
				setBothColours(this->gadgetColours.foreCol,this->blockColour);
				MOVETO(this->sx,this->sy);
				for(jx=0;jx<abswid-1;jx++)
					printf(" ");
				if(this->gadgetColours.useFancy==true)
					{
						MOVETO(this->sx+jx,this->sy);
						setBothColours(this->gadgetColours.foreCol,this->gadgetColours.backCol);
						printf("%*s",this->wid-jx,"");
					}
				break;
			case FILLEDINDICATOR:
				setBothColours(this->gadgetColours.foreCol,this->gadgetColours.backCol);
				MOVETO(this->sx,this->sy);
				printf("%*s", this->wid,"");
			case INDICATOR:
				setBothColours(this->gadgetColours.foreCol,this->blockColour);
				absx=(int)(abswid+this->sx);
				if(absx>this->wid+this->sx-1)
					absx=this->wid+this->sx-1;
				MOVETO(absx,this->sy);
				printf(" ");
				break;

			case FILLEDPULSE:
				setBothColours(this->gadgetColours.foreCol,this->gadgetColours.backCol);
				MOVETO(this->sx,this->sy);
				printf("%*s", this->wid,"");
			case PULSE:
				setBothColours(this->gadgetColours.foreCol,this->blockColour);
				this->pulseColCnt+=1;
				if(this->pulseCharacter==true)
					this->pulseCnt+=1;
				if(this->pulseCnt>this->pulseCharString.length()-1)
					this->pulseCnt=0;
				if(this->pulseColour==true)
					{
						if((this->pulseColCnt & 1)==0)
							setBothColours(this->gadgetColours.backCol,this->blockPulseColours[0]);
						else
							setBothColours(this->gadgetColours.backCol,this->blockPulseColours[1]);
					}
				MOVETO(this->sx,this->sy);
				for(int j=0;j<abswid;j++)
					printf("%c",this->pulseCharString.at(this->pulseCnt));
				break;
		}
	//fflush(NULL);
}

/**
* Progress Bar Set Style.
* \param bool Pulse colours.
* \param bool Pulse characters.
* \param const char * Character string to cycle through when pulsing ( min 2 chars, default "#*" ).
*/
void CTK_cursesProgressBarClass::CTK_setPulseStyle(bool pulsecol,bool pulschar,const char *pulsechars)
{
	this->pulseColour=pulsecol;
	this->pulseCharacter=pulschar;
	this->pulseCharString=pulsechars;
}

/**
* Progress Bar Set Value.
* \param double New value.
*/
void CTK_cursesProgressBarClass::CTK_setValue(double val)
{
	this->value=val;
	if(this->value>=this->maxvalue)
		this->value=this->maxvalue;
	if(this->value<=this->minvalue)
		this->value=this->minvalue;
}

/**
* Progress Bar Get Value.
* \return double Current value.
*/
double CTK_cursesProgressBarClass::CTK_getValue(void)
{
	return(this->value);
}

/**
* Progress Bar Set Minimum Value.
* \param double New minimum value.
*/
void CTK_cursesProgressBarClass::CTK_setMinValue(double val)
{
	this->minvalue=val;
}

/**
* Progress Bar Get Minimum Value.
* \return double Current minimum value.
*/
double CTK_cursesProgressBarClass::CTK_getMinValue(void)
{
	return(this->minvalue);
}

/**
* Progress Bar Set Maximum Value.
* \param double New Maximum value.
*/
void CTK_cursesProgressBarClass::CTK_setMaxValue(double val)
{
	this->maxvalue=val;
}

/**
* Progress Bar Get Maximum Value.
* \return double Current Maximum value.
*/
double CTK_cursesProgressBarClass::CTK_getMaxValue(void)
{
	return(this->maxvalue);
}

/**
* Progress Bar Set Scale For All Values.
* \param int New scale value ( default 2 ).
*/
void CTK_cursesProgressBarClass::CTK_setScale(int scale)
{
	this->scale=scale;
}

/**
* Progress Bar Display Value As Real.
* \param bool real
*/
void CTK_cursesProgressBarClass::CTK_setShowRealValue(bool real)
{
	this->valueAsReal=real;
}

/**
* Progress Bar Show Values.
* \param bool real
*/
void CTK_cursesProgressBarClass::CTK_setShowValues(showValueStyle show)
{
	this->showValues=show;
}

/**
* Progress Bar Set fill style.
* \param fillStyle One of:BAR,INDICATOR,PULSE,FILLEDBAR,FILLEDPULSE,FILLEDINDICATOR, ( default BAR ).
*/
void CTK_cursesProgressBarClass::CTK_setFillStyle(fillStyle style)
{
	this->style=style;
}

/**
* Progress Bar Show Values as time.
* \param bool usetime
*/
void CTK_cursesProgressBarClass::CTK_setShowValuesAsTime(bool usetime)
{
	this->valuesAsTime=usetime;
}

/**
* Progress Bar Show zero minutes.
* \param bool showzero
*/
void CTK_cursesProgressBarClass::CTK_setShowZeroMinutes(bool showzero)
{
	this->showZeroMinutes=showzero;
}

/**
* Progress Bar Pulse Bar.
* \note calling this will pulse all pulsable bars.
* \note bar must be set to one of PULSE/FILLEDPULSE.
*/
void CTK_cursesProgressBarClass::CTK_pulseBar(void)
{
	if((this->style==PULSE) || (this->style==FILLEDPULSE))
		{
			this->gadgetDirty=true;
			this->mc->CTK_updateScreen(this->mc,NULL);
		}
}

