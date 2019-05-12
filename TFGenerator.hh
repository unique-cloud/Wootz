#ifndef _TFGENERATOR_H
#define _TFGENERATOR_H

#include "Network.hh"
#include "Parser.hh"

class Parser;

class TFGenerator
{
public:

    TFGenerator(std::string inFileName);

    bool genTFCode(bool genMultiPlex);

    bool genAll(std::string &allCode);

    bool genNode(TFNode* myNode, std::string cur_scope, std::string &last_scope, std::string &nodeCode);

    bool genSingle(TFNode* myNode, std::string cur_scope, std::string &last_scope, std::string &singleCode);

    bool genMultiple(TFNode* myNode, std::string cur_scope, std::string &last_scope, std::string &multiCode);

    bool genConcat(std::vector<TFNode*> &myNodeVec, std::string cur_scope, std::string &last_scope, std::string &mixedCode);

    bool genLayer(Layer* myLayer, std::string cur_scope, std::string &last_scope, std::string &layerCode);

    void instanciateParser(Parser* newParser);

private:

    Network* myNetwork;

    Parser* parse;

	std::string fileName;

    bool isMultiplex;

    bool useMultiplex;
};

#endif