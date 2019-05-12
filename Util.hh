#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <queue>
#include <fstream>

class Util
{
public:
	static std::pair<bool, int> isSymbol(std::string::iterator pos);
	
	static bool isLetter(std::string::iterator pos);
	
	static bool isDigit(std::string::iterator pos);

    static bool isDot(std::string::iterator pos);
	
	static bool isType(std::string curString);

    static bool isWeightFillerType(std::string curString);

	static bool isBiasFillerType(std::string curString);
	
	static bool isSubstrOf(std::string str1, std::string str2);

	static bool isOption(std::string curString);
	
	static bool isOptionCandidate(std::string curString);

	static bool isBool(std::string curString);

	static bool isBoolCandidate(std::string curString);

	static bool isPoolingMethod(std::string curString);

	static bool isPoolingMethodCandidate(std::string curString);

	static std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value);

	static std::vector<std::string> split(std::string str,std::string pattern);
	
};
#endif
