#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

class Token
{
private:
	std::string myToken;
	int id;
	int symType;

public:
	bool isString;
	bool isStringComplete;
	bool isDecimal;

public:
	static enum IDTYPE{
		IDTYPE_NAME,
		IDTYPE_BOOL,
		IDTYPE_NUMBER,
		IDTYPE_LAYER_TYPE,
		IDTYPE_WEIGHT_FILLER_TYPE,
		IDTYPE_BIAS_FILLER_TYPE,
		IDTYPE_OPTION,
		IDTYPE_SYMBOL,
		IDTYPE_TABSPACE,
		IDTYPE_INVALID,
		IDTYPE_EMPTY,
		IDTYPE_CANDIDATE,
		IDTYPE_POOLING_METHOD
	}ID;

	static enum SYMBOLTYPE{
		SYMTYPE_LEFT_BRACE,
		SYMTYPE_RIGHT_BRACE,	
		SYMTYPE_COLON,
		SYMTYPE_INVALID
	}STYPE;

public:
	Token();
	
	void push(char c);

	void setID(int newID);

	int getID();

	void setSymType(int newSymType);

	int getSymType();

	std::string getTokenName();

	char last();
};
#endif
