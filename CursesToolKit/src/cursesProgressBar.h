/*
 *
 * ©K. D. Hedger. Fri 19 Apr 16:10:00 BST 2019 keithdhedger@gmail.com

 * This file (cursesLabel.h) is part of CursesToolKit.

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

#ifndef _CURSESPROGRESSBAR_
#define _CURSESPROGRESSBAR_

enum fillStyle {BAR=0,INDICATOR,PULSE,FILLEDBAR,FILLEDPULSE,FILLEDINDICATOR};

class CTK_cursesProgressBarClass : public CTK_cursesGadgetClass
{
	public:
		CTK_cursesProgressBarClass(CTK_mainAppClass *mc);
		~CTK_cursesProgressBarClass();

		void	CTK_newBar(int x,int y,int width,double min,double max, double val,char fill='#');
		void	CTK_drawGadget(bool hilite=false);

		void	CTK_setValue(double val);
		double	CTK_getValue(void);
		void	CTK_setMinValue(double val);
		double	CTK_getMinValue(void);
		void	CTK_setMaxValue(double val);
		double	CTK_getMaxValue(void);

		void	CTK_setFillStyle(fillStyle style);
		void	CTK_setPulseStyle(bool pulsecol,bool pulschar,const char *pulsechars="#*");
		void	CTK_pulseBar(void);

		int		blockColour=BACK_RED;
		int		blockPulseColours[2]={BACK_RED,BACK_GREEN};

	private:
		double		minvalue=1.0;
		double		maxvalue=100.0;
		double		value=1.0;
		char		fillchar='#';
		fillStyle	style=BAR;
		bool		pulseColour=false;
		bool		pulseCharacter=true;
		std::string	pulseCharString="#*";
		int			pulseCnt=0;
		int			pulseColCnt=0;
};

#endif
