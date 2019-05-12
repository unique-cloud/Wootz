#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "Scanner.hh"
#include "Parser.hh"

void getFileName(char *input, std::string &output);

int main(int argc, char* argv[])
{
    // Check validation of input param
	if(argc != 2)
	{
		std::cout << "Invalid number of arguments." << std::endl << "Format:\t./Main <InputFileName>" << std::endl;
		return EXIT_FAILURE;
	}

	Scanner scan(argv[1]);

	std::string inFileName;
	getFileName(argv[1], inFileName);
	
	Grammar *cGrammar = new Grammar();
	TFGenerator *TFGen =  new TFGenerator(inFileName);
	Parser parse(cGrammar, TFGen);

	while (scan.hasNextToken())
	{
		Token *t = scan.getNextToken();
		if (t->getID() == Token::IDTYPE_INVALID)
		{
			delete t;
			std::cout << "Scan error" << std::endl;
			return EXIT_FAILURE;
		}
		parse.addToken(t);
	}

	if (!parse.parsePrototxt())
	{
		std::cout << "Parse error" << std::endl;
		return EXIT_FAILURE;
	}

	// Debug
	// parse.getNetwork()->printLayers();

	if(!parse.analyzeNetwork())
	{
		std::cout << "Analyze Network error" << std::endl;
		return EXIT_FAILURE;
	}

	// Debug
	// parse.getNetwork()->printNodes();

	if (!TFGen->genTFCode(false))
	{
		std::cout << "TF Code generate error" << std::endl;
		return EXIT_FAILURE;
	}

	if (!TFGen->genTFCode(true))
	{
		std::cout << "TF Code generate error" << std::endl;
		return EXIT_FAILURE;
	}	
}

void getFileName(char *input, std::string &output)
{
	while (*input != '\0')
	{
		if (*input == '.')
		{
			break;
			/*std::cout << " I [" << input << "] output [" << output << "]\n";*/
		}
		else
		{
			output.push_back(*input);
			/*std::cout << " I [" << input << "] output [" << output << "]\n";*/
		}
		input++;
	}
}
