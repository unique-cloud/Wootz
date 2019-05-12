#include <vector>
#include "Parser.hh"
#include "Util.hh"
#include "TFNode.hh"
#include <iostream>

Token* Parser::nextToken()
{
	++tokenIT;

	// Skip blank space
	while ((tokenIT != tokenVector->end()) 
		&& ((*tokenIT)->getID() == Token::IDTYPE_TABSPACE 
		|| (*tokenIT)->getID() == Token::IDTYPE_EMPTY))
	{
		++tokenIT;
	}

	if (tokenIT == tokenVector->end())
	{
		return NULL;
	}

	// std::cout << "Token : " << (*tokenIT)->getTokenName() << std::endl;
	return *tokenIT;
}

Token* Parser::curToken()
{
	if (tokenIT == tokenVector->end())
	{
		return NULL;
	}

	return *tokenIT;
}

void Parser::addToken(Token *newToken)
{
	tokenVector->push_back(newToken);
}

bool Parser::parsePrototxt()
{
	tokenIT = tokenVector->begin();
	if (tokenIT != tokenVector->end())
	{
		bool temp = myGrammar->prototxt();

		// Debug
		std::cout << std::endl;
		std::cout << "Token type\t\tToken name" << std::endl;
		int count = 100;
		while (tokenIT != tokenVector->end() && count)
		{
			std::cout << (*tokenIT)->getID() << "\t\t" << (*tokenIT)->getTokenName() << std::endl;
			++tokenIT;
			--count;
		}

		return temp;
	}
	return true;
}

bool Parser::analyzeNetwork()
{
	return myNetwork->formLayerNode()&& myNetwork->linkNodes();
}

Parser::Parser(Grammar *newGrammar, TFGenerator* newTFGen) 
				: myGrammar(newGrammar), myTFGen(newTFGen)
{
	tokenVector = new std::vector<Token*>();
	myNetwork = new Network();

	newGrammar->instanciateParser(this);
	newTFGen->instanciateParser(this);
}

Parser::~Parser()
{
	for (std::vector<Token *>::iterator it = tokenVector->begin(); it != tokenVector->end(); ++it)
	{
		delete *it;
	}
	delete tokenVector;
	delete myNetwork;
}

Network* Parser::getNetwork()
{
	return myNetwork;
}