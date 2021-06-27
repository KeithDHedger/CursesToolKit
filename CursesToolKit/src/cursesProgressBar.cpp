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
	this->CTK_setCommon(mc);
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

/**
* Draw Progress Bar.
*/
void CTK_cursesProgressBarClass::CTK_drawGadget(bool hilite)
{
	double	absscale=(double)this->wid/this->maxvalue;
	double	abswid=absscale*this->value;
	int		absx;
int jx;

	if(this->colours.fancyGadgets==true)
		this->gc->CTK_drawBox(this->sx-1,this->sy-1,this->wid+1,this->hite+1,this->colours.barBoxType,false);

	switch(this->style)
		{
			case FILLEDBAR:
				setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);
				MOVETO(this->sx,this->sy);
				printf("%*s", this->wid,"");
			case BAR:
				setBothColours(this->colours.buttonForeCol,this->blockColour,this->colours.use256Colours);
				MOVETO(this->sx,this->sy);
				for(jx=0;jx<abswid;jx++)
					printf(" ");
				if(this->colours.fancyGadgets==true)
					{
						MOVETO(this->sx+jx,this->sy);
						setBothColours(this->colours.foreCol,this->colours.backCol,this->colours.use256Colours);
						printf("%*s",this->wid-jx,"");
					}
				break;
			case FILLEDINDICATOR:
				setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);
				MOVETO(this->sx,this->sy);
				printf("%*s", this->wid,"");
		case INDICATOR:
				setBothColours(this->colours.buttonForeCol,this->blockColour,this->colours.use256Colours);
				absx=(int)(abswid+this->sx);
				if(absx>this->wid+this->sx-1)
					absx=this->wid+this->sx-1;
				MOVETO(absx,this->sy);
				printf(" ");
				break;

			case FILLEDPULSE:
				setBothColours(this->colours.buttonForeCol,this->colours.buttonBackCol,this->colours.use256Colours);
				MOVETO(this->sx,this->sy);
				printf("%*s", this->wid,"");
			case PULSE:
				setBothColours(this->colours.buttonForeCol,this->blockColour,this->colours.use256Colours);
				this->pulseColCnt+=1;
				if(this->pulseCharacter==true)
					this->pulseCnt+=1;
				if(this->pulseCnt>this->pulseCharString.length()-1)
					this->pulseCnt=0;
				if(this->pulseColour==true)
					{
						if((this->pulseColCnt & 1)==0)
							setBothColours(this->colours.buttonForeCol,this->blockPulseColours[0],this->colours.use256Colours);
						else
							setBothColours(this->colours.buttonForeCol,this->blockPulseColours[1],this->colours.use256Colours);
					}
				MOVETO(this->sx,this->sy);
				for(int j=0;j<abswid;j++)
					printf("%c",this->pulseCharString.at(this->pulseCnt));
			
				break;
		}
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
	if(this->value>this->maxvalue)
		this->value=this->maxvalue;
	if(this->value<this->minvalue)
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
	this->minvalue=val;
}

/**
* Progress Bar Get Maximum Value.
* \return double Current Maximum value.
*/
double CTK_cursesProgressBarClass::CTK_getMaxValue(void)
{
	return(this->minvalue);
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

