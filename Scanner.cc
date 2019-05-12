#include "Scanner.hh"
#include "Token.hh"
#include "Util.hh"


Scanner::Scanner(const char* inputProgram)
{
	inpPgm.open(inputProgram, std::ifstream::in);
	curLine.erase();
}

bool Scanner::hasNextLine()
{
	curLine.erase();
	if (std::getline(inpPgm, curLine))
	{
		lineIt = curLine.begin();
		return true;
	}
	else
	{
		curLine.erase();
		return false;
	}
}

bool Scanner::hasNextToken()
{
	if (curLine.empty() || lineIt == curLine.end())
	{
		if (!hasNextLine())
		{
			return false;
		}
		/*std::cout << "\t\tRead next Line\n";*/
	}

	curToken = new Token();
	populateToken();
	return true;
}

Token* Scanner::getNextToken()
{
	return curToken;
}

void Scanner::populateToken()
{
	while (lineIt != curLine.end())
	{
		if (curToken->getID() == Token::IDTYPE_NAME)
		{
			/*std::cout << "STRING\t" << *lineIt << "\n";*/
			if (*lineIt != '"')
			{
				curToken->push(*lineIt);
			}
			else
			{
				curToken->push(*lineIt);
				curToken->isStringComplete = true;

                if (Util::isType(curToken->getTokenName()))
                {
                    curToken->setID(Token::IDTYPE_LAYER_TYPE);
                }
                else if (Util::isWeightFillerType(curToken->getTokenName()))
                {
                    curToken->setID(Token::IDTYPE_WEIGHT_FILLER_TYPE);    
                }
				else if (Util::isBiasFillerType(curToken->getTokenName()))
				{
					curToken->setID(Token::IDTYPE_BIAS_FILLER_TYPE);
				}
                ++lineIt;
				break;
			}
		}
        else if (*lineIt == '"')
		{
			/*std::cout << "QUOTES\t" << *lineIt << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)				/*Start of a string so intialize new token*/
			{
				curToken->setID(Token::IDTYPE_NAME);
				curToken->push(*lineIt);
				curToken->isString = true;
                curToken->isStringComplete = false;
			}
			else
			{
				break;								/*Start of string so save the previous token*/
			}
		}
		else if (Util::isDigit(lineIt))
		{
			/*std::cout << "DIGIT\t" << *lineIt << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)
			{
				curToken->setID(Token::IDTYPE_NUMBER);
				curToken->push(*lineIt);
                curToken->isDecimal = false;
			}
			else if (curToken->getID() == Token::IDTYPE_NUMBER)
			{
				curToken->push(*lineIt);
			}
			else
			{
				break;
			}
		}
        else if (Util::isDot(lineIt))
        {
            if (curToken->getID() == Token::IDTYPE_NUMBER && !curToken->isDecimal)
            {
                curToken->push(*lineIt);
                curToken->isDecimal = true;
            }
            else
            {
                break;
            }
        }
		else if (Util::isSymbol(lineIt).first)
		{
			/*std::cout << "SYMBOL\t" << *lineIt  << "\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)				/*Start of a symbol so intialize new token*/
			{
				curToken->setID(Token::IDTYPE_SYMBOL);			/*Initialize the token to symbol*/
				curToken->push(*lineIt);
                curToken->setSymType(Util::isSymbol(lineIt).second);
				++lineIt;
				break;
			}
			else
			{
				break;
			}
		}
        else if (Util::isLetter(lineIt))
        {
            if (curToken->getID() == Token::IDTYPE_EMPTY 
				|| curToken->getID() == Token::IDTYPE_CANDIDATE)
			{	
				curToken->push(*lineIt);

				if (Util::isOptionCandidate(curToken->getTokenName()) 
					|| Util::isBoolCandidate(curToken->getTokenName())
					|| Util::isPoolingMethodCandidate(curToken->getTokenName()))
				{
					// If this is the end letter
					if (!Util::isLetter(lineIt + 1))
					{
						if (Util::isOption(curToken->getTokenName()))
						{
							curToken->setID(Token::IDTYPE_OPTION);
							++lineIt;
							break;
						}
						else if (Util::isBool(curToken->getTokenName()))
						{
							curToken->setID(Token::IDTYPE_BOOL);
							++lineIt;
							break;
						}
						else if (Util::isPoolingMethod(curToken->getTokenName()))
						{
							curToken->setID(Token::IDTYPE_POOLING_METHOD);
							++lineIt;
							break;
						}
						else
						{
							curToken->setID(Token::IDTYPE_INVALID);
							++lineIt;
							break;
						}
					}
					else
					{
						curToken->setID(Token::IDTYPE_CANDIDATE);
					}
				}
				else
				{
					curToken->setID(Token::IDTYPE_INVALID);	
					++lineIt;
					break;
				}
			}
			else
			{
				break;
			}
        }
		else if (*lineIt == '\t' || *lineIt == ' ')
		{
			/*std::cout << "TABSPACE\t[" << *lineIt << "]\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY || curToken->getID() == Token::IDTYPE_TABSPACE)
			{
				curToken->setID(Token::IDTYPE_TABSPACE);
				curToken->push(*lineIt);
			}
			else
			{
				break;
			}
		}
		else
		{	/* This branch will execute when the character is not a digit,
			 * letter, symbol, or part of a string
			 */
			/*std::cout<<"ELSE PART\t [" << *lineIt << "]\n";*/
			if (curToken->getID() == Token::IDTYPE_EMPTY)
			{
				++lineIt;
				curToken->setID(Token::IDTYPE_INVALID);
			} 
			break;
		}
		++lineIt;
	}

	if (curToken->isString && !curToken->isStringComplete)
	{
		/*std::cout<<"INCOMPLETE STRING\n";*/
		curToken->setID(Token::IDTYPE_INVALID);
	}
}
