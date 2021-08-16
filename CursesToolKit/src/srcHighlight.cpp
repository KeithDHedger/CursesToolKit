
#include "cursesGlobals.h"

srcHighlight::~srcHighlight()
{
}

srcHighlight::srcHighlight()
{
	this->resetNormal=str(boost::format("%i;%i") %this->colours[BACKCOL] %this->colours[FORECOL]);
}

void srcHighlight::findFileType(const std::string filepath)
{
	FILE		*fp;
	char		*buffer=(char*)alloca(MAXBUFFER);
	//std::string	command="file --mime-type --brief " + filepath + "|awk -F \"/\" '{print $2}'";
	std::string	command="xdg-mime  query filetype " + filepath + "|awk -F \"/\" '{print $2}'";
	fp=popen(command.c_str(),"r");
	if(fp!=NULL)
		{
			buffer[0]=0;
			fgets(buffer,MAXBUFFER-1,fp);
			if(strlen(buffer)>0)
				{
					if(buffer[strlen(buffer)-1] =='\n')
						buffer[strlen(buffer)-1]=0;
				}
			pclose(fp);
		}

	this->srcType=PLAIN;

	if(strcmp(buffer,"x-shellscript")==0)
		this->srcType=SHELL;

	if(strncmp(buffer,"x-c",3)==0)
		this->srcType=CPP;

	if(strcmp(buffer,"javascript")==0)
		this->srcType=JS;

	if(strcmp(buffer,"x-php")==0)
		this->srcType=PHP;

	if(strcmp(buffer,"x-perl")==0)
		this->srcType=PERL;
}

void srcHighlight::hilight(void)
{
	switch(this->srcType)
		{
			case CPP:
				this->hilightCPP();
				break;
			case SHELL:
				this->hilightSH();
				break;
			case JS:
				this->hilightJS();
				break;
			case PHP:
				this->hilightPHP();
				break;
			case PERL:
				this->hilightPERL();
				break;
			default:
				this->hilighted=this->data;
		}
}

void srcHighlight::hilight(srcFileType st)
{
	switch(st)
		{
			case CPP:
				this->hilightCPP();
				break;
			case SHELL:
				this->hilightSH();
				break;
			case JS:
				this->hilightJS();
				break;
			case PHP:
				this->hilightPHP();
				break;
			case PERL:
				this->hilightPERL();
				break;
			default:
				this->hilighted=this->data;
		}
}

std::string srcHighlight::getData(void)
{
	return(this->data);
}

void srcHighlight::setSrcLang(srcFileType lang)
{
	this->srcType=lang;
}

void srcHighlight::setData(std::string newdata)
{
	this->data=newdata;
	this->filePath="";
	this->srcType=CPP;
}

std::string srcHighlight::getHighlightedData(void)
{
	return(this->hilighted);
}

void srcHighlight::getColourFile(std::string filepath)
{
	std::ifstream	file(filepath);
	int				colour;
	std::string		name;
	std::string		type;;
	int				cnt=0;

	while(file >> name >> type >> colour)
		this->colours[cnt++]=colour;

	this->resetNormal=str(boost::format("%i;%i") %this->colours[BACKCOL] %this->colours[FORECOL]);
}

void srcHighlight::saveColourFile(std::string filepath)
{
	std::ofstream		file(filepath);
	const std::string	colourNames[COLOURCNT]={"forcol","backcol","keyword","type","include","string","custom","comment","class","function","bracket","namespace","number","variable"};

	for(int j=0;j<COLOURCNT;j++)
		file << colourNames[j] << " int " << std::to_string(this->colours[j]) << "\n";
}

void srcHighlight::getFile(std::string filepath,bool saveandget)
{
	std::ifstream		file(filepath);
	std::stringstream	buffer;

	buffer<< file.rdbuf();
	this->data=buffer.str();
	if(saveandget==true)
		{
			this->findFileType(filepath);
			this->filePath=filepath;
		}
}

std::string srcHighlight::clearUpString(boost::regex re,std::string data,int newcolour)
{
	boost::match_results<std::string::const_iterator>	what;
	long int											off;
	std::string::const_iterator							start;
	std::string::const_iterator							end;
	bool												flag=true;
	std::string											sub;
	boost::regex										reg;
	boost::regex										subreg;
	start=data.begin();
	end=data.end();
	off=0;

	while (flag==true)
		{
			flag=boost::regex_search(start,end,what,re,boost::match_default);
				if(flag==true)
					{
						if((long unsigned int)(what.position()+off+what.length())>=(long unsigned int)data.length())
								break;

						sub=data.substr(what.position()+off,what.length());
						data.erase(what.position()+off,what.length());
						reg="\\e\\[.*?m";
						sub=boost::regex_replace(sub,reg,"",boost::format_sed| boost::format_all);
						subreg.assign("^(.*)$",boost::regex_constants::no_mod_s);
						sub=boost::regex_replace(sub,subreg,str(boost::format("\e[%im\\1\e[%sm") %this->colours[newcolour] %this->resetNormal),boost::format_sed| boost::format_all);
						data.insert(what.position()+off,sub);
						off+=what.position()+sub.length();
						start=data.begin()+off;
						end=data.end();
					}
		}
	return(data);
}

void srcHighlight::hilightCPP(void)
{
	std::string		keywordRegexCPP="(__asm|__cdecl|__declspec|__export|__far16|__fastcall|__fortran|__import|__pascal|__rtti|__stdcall|_asm|_cdecl|__except|_export|_far16|_fastcall|__finally|_fortran|_import|_pascal|_stdcall|__thread|__try|asm|auto|break|case|catch|cdecl|const|continue|default|do|[[:space:]]else|enum|extern|goto|pascal|register|return|sizeof|static|struct|switch|typedef|union|volatile|class|const_cast|delete|dynamic_cast|explicit|friend|inline|mutable|namespace|new|operator|private|protected|public|reinterpret_cast|static_cast|template|this|throw|try|typeid|typename|using|virtual)\\b";
	std::string		typeRegexCPP="\\b(bool|char|double|float|int|long|short|signed|unsigned|void|wchar_t)\\b";
	std::string		includeRegexCPP="(^[[:blank:]]*#(?:[^\\\\\\n]|\\\\[^\\n[:punct:][:word:]]*[\\n[:punct:][:word:]])*)";
	std::string		dquoteRegexCPP="\"(?:[^\\\\\"]|\\\\.)*\"";
	std::string		squoteRegexCPP="('[\\\\']*.*?')";
	std::string		customRegexCPP="\\b(NULL|nullptr|true|false|TRUE|FALSE)\\b";
	std::string		commentsRegexCPP="(//[^\\n]*|/\\*(.*?)\\*/)";
	std::string		clasRegexCPP="(class[[:blank:]]*|struct[[:blank:]]*)([[:word:]]*)";
	std::string		functionRegexCPP="(([[:alpha:]]|_)[[:word:]]*(?=[[:blank:]]*\\())";
	std::string		angleBracketsRegexCPP="([[:blank:]]*#[[:blank:]]*include[[:blank:]]*)(<[[:alnum:]\\.]*>)";
	std::string		nameSpaceRegexCPP="([[:word:]]*)::";
	std::string		numbersRegexCPP="\\<[+-]?((0x[[:xdigit:]]+)|(([[:digit:]]*\\.)?[[:digit:]]+([eE][+-]?[[:digit:]]+)?))u?((int(?:8|16|32|64))|L)?\\>";
	std::string		newtext;
	boost::regex	re;
	std::string		result=this->data;

//numbers
	re=numbersRegexCPP;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NUMBER] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//include directives
	re=includeRegexCPP;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[INCLUDE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//class
	re=clasRegexCPP;
	newtext=str(boost::format("\\1\e[%im\\2\e[%sm") %this->colours[CLASS] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//functions
	re=functionRegexCPP;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[FUNCTION] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//keywordRegexCPPs
	re=keywordRegexCPP;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[KEYWORD] %this->resetNormal);
	result=regex_replace(result,re,newtext);

//double quote string
	re=dquoteRegexCPP;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[STRING] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//single quote string
	re.assign(squoteRegexCPP,boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[STRING] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//types
	re=typeRegexCPP;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[TYPE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//angle brackets include
	re=angleBracketsRegexCPP;
	newtext=str(boost::format("\\1\e[%im\\2\e[%sm") %this->colours[BRACKET] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//comment
	re=commentsRegexCPP;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//namespace
	re=nameSpaceRegexCPP;
	newtext=str(boost::format("\e[%im\\1\e[%sm::") %this->colours[NAMESPACE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//custom
	re=customRegexCPP;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[CUSTOM] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//cleanup
//string cleanup
	re=str(boost::format("\\e\\[%im\".*?\"\\e\\[%sm") %this->colours[STRING] %this->resetNormal);
	result=this->clearUpString(re,result,STRING);

//comment cleanup
	re=str(boost::format("(\\e\\[%im//[^\\n]*|\\e\\[%im/\\*(.*?)\\*/)") %this->colours[COMMENT] %this->colours[COMMENT]);
	result=this->clearUpString(re,result,COMMENT);

	this->hilighted=result;
}

void srcHighlight::hilightSH(void)
{
	std::string		keywordRegexSH="\\b(alias|bg|bind|break|builtin|caller|case|command|compgen|complete|continue|declare|dirs|disown|do|done|elif|else|enable|esac|eval|exec|exit|export|false|fc|fg|fi|for|getopts|hash|help|history|if|in|jobs|let|local|logout|popd|printf|pushd|read|readonly|return|select|set|shift|shopt|suspend|test|then|times|trap|true|type|typeset|umask|unalias|unset|until|wait|while|echo)\\b";
	std::string		includeRegexSH="^[[:blank:]]*(\\.[^\\n]*|source[^\\n]*)";
	std::string		stringRegexSH="'(?:[^\\\\']|\\\\.)*'|\"(?:[^\\\\\"]|\\\\.)*\"";
	std::string		commentsRegexSH="([[:space:]]+)(\\#.*)$";
	std::string		functionRegexSH="^(function[[:blank:]]*[[:word:]]*|[[:word:]]*)([[:blank:]]*\\([[:blank:]]*\\))";
	std::string		angleBracketsRegexSH="(\\${[[:print:]]*}|\\$\\([[:print:]]*\\))";
	std::string		numbersRegexSH="\\<[+-]?((0x[[:xdigit:]]+)|(([[:digit:]]*\\.)?[[:digit:]]+([eE][+-]?[[:digit:]]+)?))u?((int(?:8|16|32|64))|L)?\\>";
	std::string		variablesRegexSH="([[:word:]]*)=";

	std::string		newtext;
	boost::regex	re;
	std::string		result=this->data;

	re=numbersRegexSH;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NUMBER] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);
	result.insert(0,"\e[" + this->resetNormal + "m");
//comment
	re.assign(commentsRegexSH,boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\\1\e[%im\\2\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);
//edge case
	re.assign("^(.*)(\\#.*)$",boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\\1\e[%im\\2\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_first_only);

//quote string
	re.assign("(['\"].*?[^\\\\\"][\"'])",boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[STRING] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//variables
	re=variablesRegexSH;
	newtext=str(boost::format("\e[%im\\1\e[%sm=") %this->colours[NAMESPACE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

	re="(\\$[\\{[:word:][:punct:]\\}]*)";//TODO//
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NAMESPACE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//include directives
	re=includeRegexSH;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[INCLUDE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//keywordRegex
	re=keywordRegexSH;
	newtext=str(boost::format("\e[%im$&\e[%sm") %this->colours[KEYWORD] %this->resetNormal);
	result=regex_replace(result,re,newtext);

//functions
	re=functionRegexSH;
	newtext=str(boost::format("\e[%im\\1\e[%sm\\2") %this->colours[FUNCTION] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//cleanup
//comment clean
	re.assign(str(boost::format("\\e\\[%im\\#.*\\e\\[%sm") %this->colours[COMMENT] %this->resetNormal),boost::regex_constants::no_mod_s);
	result=this->clearUpString(re,result,COMMENT);

//string cleanup
	re.assign(str(boost::format("\\e\\[%im\".*?\"\\e\\[%sm|\\e\\[%im'.*?'\\e\\[%sm") %this->colours[STRING] %this->resetNormal %this->colours[STRING] %this->resetNormal),boost::regex_constants::no_mod_s);
	result=this->clearUpString(re,result,STRING);

	this->hilighted=result;
}

void srcHighlight::hilightJS(void)
{
	std::string		keywordRegex="\\b(let|abstract|break|case|catch|const|continue|debugger|default|delete|do|else|enum|export|extends|final|finally|for|function|goto|if|implements|in|instanceof|interface|native|new|private|protected|prototype|public|return|static|super|switch|synchronized|throw|throws|this|transient|try|typeof|var|volatile|while|with)\\b";
	std::string		stringRegex="'(?:[^\\\\']|\\\\.)*'|\"(?:[^\\\\\"]|\\\\.)*\"";
	std::string		customRegex="\\b(NULL|null|true|false|TRUE|FALSE)\\b";
	std::string		commentsRegex="(//[^\\n]*|/\\*(.*?)\\*/)";
	std::string		functionRegex="((?:function)?(?:[[:word:][:blank:]]*?)?)\\(";
	std::string		numbersRegex="\\<[+-]?((0x[[:xdigit:]]+)|(([[:digit:]]*\\.)?[[:digit:]]+([eE][+-]?[[:digit:]]+)?))u?((int(?:8|16|32|64))|L)?\\>";
	std::string		clasRegex="(class)";
	std::string		newtext;
	boost::regex	re;
	std::string		result=this->data;

//numbers
	re=numbersRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NUMBER] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//class
	re=clasRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[CLASS] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//functions
	re=functionRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm\\(\\2") %this->colours[FUNCTION] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//keywordRegexCPPs
	re=keywordRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[KEYWORD] %this->resetNormal);
	result=regex_replace(result,re,newtext);

//strings
	re=stringRegex;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[STRING] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//comment
	re=commentsRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//custom
	re=customRegex;
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[CUSTOM] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//cleanup
//string cleanup
	re.assign(str(boost::format("\\e\\[%im\".*?\"\\e\\[%sm|\\e\\[%im'.*?'\\e\\[%sm") %this->colours[STRING] %this->resetNormal %this->colours[STRING] %this->resetNormal),boost::regex_constants::no_mod_s);
	result=this->clearUpString(re,result,STRING);

//comment cleanup
	re=str(boost::format("(\\e\\[%im//[^\\n]*|\\e\\[%im/\\*(.*?)\\*/)") %this->colours[COMMENT] %this->colours[COMMENT]);
	result=this->clearUpString(re,result,COMMENT);

	this->hilighted=result;
}

void srcHighlight::hilightPHP(void)
{
	std::string		keywordRegex="\\b(__FILE__|exception|php_user_filter|__LINE__|abstract|array|as|break|case|catch|cfunction|class|clone|const|continue|declare|default|do|each|echo|else|elseif|empty|enddeclare|endfor|endforeach|endif|endswitch|endwhile|eval|exit|extends|final|foreach|function|global|goto|if|implements|interface|instanceof|isset|list|namespace|new|old_function|print|private|protected|public|return|static|switch|throw|unset|use|var|__FUNCTION__|__CLASS__|__METHOD__|__DIR__|__NAMESPACE__)\\b";
	std::string		customRegex="\\b(and|or|xor|null|true|false|NULL|TRUE|FALSE)\\b";
	std::string		commentsRegex="(\\#[^\\n]*|//[^\\n]*|/\\*(.*?)\\*/)";
	std::string		functionRegex="(([[:alpha:]]|_)[[:word:]]*(?=[[:blank:]]*\\())";
	std::string		numbersRegex="\\<[+-]?((0x[[:xdigit:]]+)|(([[:digit:]]*\\.)?[[:digit:]]+([eE][+-]?[[:digit:]]+)?))u?((int(?:8|16|32|64))|L)?\\>";
	std::string		variablesRegex="(\\$[[:word:]]*)";

	std::string		newtext;
	boost::regex	re;
	std::string		result=this->data;

//numbers
	re=numbersRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NUMBER] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);
	result.insert(0,"\e[" + this->resetNormal + "m");

//comment
	re=commentsRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);


//quote string //TODO//
	re.assign("(['\"].*?[^\\\\\"][\"'])",boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[STRING] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//variables
	re=variablesRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NAMESPACE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//keywordRegex
	re=keywordRegex;
	newtext=str(boost::format("\e[%im$&\e[%sm") %this->colours[KEYWORD] %this->resetNormal);
	result=regex_replace(result,re,newtext);

//functions
	re=functionRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[FUNCTION] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//cleanup
//comment clean
	re.assign(str(boost::format("\\e\\[%im\\#.*\\e\\[%sm") %this->colours[COMMENT] %this->resetNormal),boost::regex_constants::no_mod_s);

	re=str(boost::format("(\\e\\[%im\\#[^\\n]*|\\e\\[%im//[^\\n]*|\\e\\[%im/\\*(.*?)\\*/)") %this->colours[COMMENT] %this->colours[COMMENT] %this->colours[COMMENT]);
	result=this->clearUpString(re,result,COMMENT);

	this->hilighted=result;
}

void srcHighlight::hilightPERL(void)
{
	std::string		keywordRegex="\\b(foreach|chomp|chop|chr|crypt|hex|i|index|lc|lcfirst|length|oct|ord|pack|q|qq|reverse|rindex|sprintf|substr|tr|uc|ucfirst|m|s|g|qw|abs|atan2|cos|exp|hex|int|log|oct|rand|sin|sqrt|srand|local|our|delete|each|exists|keys|values|pack|read|syscall|sysread|syswrite|unpack|vec|undef|unless|return|length|grep|sort|caller|continue|dump|eval|goto|last|next|redo|sub|wantarray|pop|push|shift|splice|unshift|switch|join|defined|last|chop|chomp|bless|dbmclose|dbmopen|ref|tie|tied|untie|next|map|eq|cmp|lc|uc|and|else|use|require|package|import|chdir|chmod|chown|chroot|fcntl|glob|ioctl|link|lstat|mkdir|opendir|readlink|rename|rmdir|stat|symlink|umask|unlink|utime|binmode|closedir|dbmclose|dbmopen|eof|fileno|flock|format|getc|read|readdir|rewinddir|seek|seekdir|select|syscall|sysread|sysseek|syswrite|tell|telldir|truncate|warn|write|alarm|exec|fork|getpgrp|getppid|getpriority|kill|pipe|qx|setpgrp|setpriority|sleep|system|times|x|wait)\\b";
	std::string		stringRegex="([\"].*?[^\\\\\"][\"])";
	std::string		commentsRegex="([[:space:]]+)(\\#.*)$";
	std::string		functionRegex="(([[:alpha:]]|_)[[:word:]]*(?=[[:blank:]]*\\())";
	std::string		variablesRegex="(\\$[[:word:]]*)";
	std::string		numbersRegex="\\<[+-]?((0x[[:xdigit:]]+)|(([[:digit:]]*\\.)?[[:digit:]]+([eE][+-]?[[:digit:]]+)?))u?((int(?:8|16|32|64))|L)?\\>";
	std::string		newtext;
	boost::regex	re;
	std::string		result=this->data;

//numbers
	re=numbersRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NUMBER] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//keywordRegexCPPs
	re=keywordRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[KEYWORD] %this->resetNormal);
	result=regex_replace(result,re,newtext);

//functions
	re=functionRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[FUNCTION] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//variables
	re=variablesRegex;
	newtext=str(boost::format("\e[%im\\1\e[%sm") %this->colours[NAMESPACE] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext);

//comment
	re.assign(commentsRegex,boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\\1\e[%im\\2\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);
//edge case
	re.assign("^(.*)(\\#.*)$",boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\\1\e[%im\\2\e[%sm") %this->colours[COMMENT] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_first_only);

//string
	re.assign(stringRegex,boost::regex_constants::no_mod_s);
	newtext=str(boost::format("\e[%im&\e[%sm") %this->colours[STRING] %this->resetNormal);
	result=boost::regex_replace(result,re,newtext,boost::format_sed| boost::format_all);

//cleanup
//comment clean
	re.assign(str(boost::format("\\e\\[%im\\#.*\\e\\[%sm") %this->colours[COMMENT] %this->resetNormal),boost::regex_constants::no_mod_s);
	result=this->clearUpString(re,result,COMMENT);

//string cleanup
	re.assign(str(boost::format("\\e\\[%im\".*?\"\\e\\[%sm|\\e\\[%im'.*?'\\e\\[%sm") %this->colours[STRING] %this->resetNormal %this->colours[STRING] %this->resetNormal),boost::regex_constants::no_mod_s);
	result=this->clearUpString(re,result,STRING);

	this->hilighted=result;
}
