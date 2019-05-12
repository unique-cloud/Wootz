/**
*	Source : Grammar.h
*	Description: Provides the grammar to be used to parse the program. Provides interface for each non-terminal in the grammar. 
*				 Non-Terminal interfaces will validate the program usign the terminals of the grammar.
*/
#ifndef _GRAMMAR_H
#define _GRAMMAR_H
#include <fstream>
#include "Parser.hh"
#include "Network.hh"
#include "Layer.hh"
//#include "Function.hh"
//#include "ASTNode.hh"


class Parser;
class Network;
class Layer;

class Grammar
{
public:
	/*Interfaces for each of the non-terminal in the grammar*/
	bool prototxt();

	bool input_def();

	bool input_shape_def();

	bool input_shape_opt();

	bool layers();

	bool layer_def();

	bool layer_opt();

	bool bottoms();

	bool params();

	bool layer_param();

	bool param();

	bool param_def();

	bool convolution_def();

	bool batch_norm_def();

	bool scale_def();

	bool pooling_def();

	bool dropout_def();

	bool reshape_def();
	
	bool inner_product_def();

	bool convolution_opt();

	bool convolution_opt_tail();

	bool pads();

	bool kernels();

	bool batch_norm_opt();

	bool scale_opt();

	bool pooling_opt();

	bool pooling_opt_tail();

	bool pooling_opt_tail_tail();

	bool dropout_opt();

	bool reshape_opt();

	bool inner_product_opt();

	bool weight_filler_def();

	bool weight_filler_opt();

	bool bias_filler_def();

	bool bias_filler_opt();

	bool shape_def();

	bool shape_opt();

	bool name();

	bool input();

	bool dim();

	bool layer_type();

	bool bottom();

	bool top();

	bool lr_mult();

	bool decay_mult();

	bool bias_term();

	bool num_output();

	bool pad();

	bool kernel_size();

	bool use_global_stats();

	bool eps();

	bool pool();

	bool stride();

	bool global_pooling();

	bool dropout_ratio();

	bool weight_filler_type();

	bool std();

	bool group();

	bool bias_filler_type();

	bool value();

	bool pad_h();
;
	bool pad_w();
;
	bool kernel_h();
;
	bool kernel_w();

	void instanciateParser(Parser* newParser);

	Grammar();

private:

	Parser *parse;
	Network *network;
	Layer *myLayer;

};
#endif
