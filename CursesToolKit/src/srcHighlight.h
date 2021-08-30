
#include <fstream>
#include <iostream>

#include <boost/regex.hpp>
#include <boost/format.hpp>

#ifndef _SRCHIGHLIGHT_
#define _SRCHIGHLIGHT_

#define MAXBUFFER 512
#define COLOURCNT 14

enum FORECOL {FORE_COLOUR=37,BACK_COLOUR=40};

enum colourIndex {FORECOL=0,BACKCOL,KEYWORD,TYPE,INCLUDE,STRING,CUSTOM,COMMENT,CLASS,FUNCTION,BRACKET,NAMESPACE,NUMBER,VARIABLE};
enum srcFileType {PLAIN=0,CPP,SHELL,JS,PHP,PERL};//TODO//add manpage to syntax hiliter

class srcHighlight
{
	public:
		srcHighlight();
		~srcHighlight();

		void				getFile(std::string filepath,bool saveandget=true);
		void				findFileType(const std::string filepath);
		void				getColourFile(std::string filepath);
		void				saveColourFile(std::string filepath);
		std::string			getData(void);
		void				setData(std::string newdata);
		void				setSrcLang(srcFileType lang);
		std::string			getHighlightedData(void);
		void				hilight(void);
		void				hilightCPP(void);
		void				hilightSH(void);
		void				hilightJS(void);
		void				hilightPHP(void);
		void				hilightPERL(void);
		void				hilight(srcFileType st);
		std::string			clearUpString(boost::regex re,std::string result,int newcolour);

		std::string			data;
	private:
		srcFileType			srcType=PLAIN;
		std::string			hilighted;
		std::string			filePath;
		std::string			resetNormal="";

		int					colours[COLOURCNT]={FORE_COLOUR,BACK_COLOUR,FORE_BOLD_RED,FORE_BOLD_GREEN,FORE_BOLD_BLUE,FORE_BOLD_MAGENTA,FORE_BOLD_CYAN,FORE_CYAN,FORE_YELLOW,FORE_BOLD_RED,FORE_BOLD_GREEN,FORE_YELLOW,FORE_BOLD_YELLOW,FORE_YELLOW};
};

#endif
