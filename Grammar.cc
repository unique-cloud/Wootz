#include <vector>
#include <cstring>
#include <cstdlib>
#include "Grammar.hh"
#include "Token.hh"
#include "Util.hh"
#include <iostream>


bool Grammar::prototxt()
{
	/**
	* Production:	<prototxt> --> <name> <input_def> <layers> 
	*/
	return name() && input_def() && layers();
}

bool Grammar::input_def()
{
	/**
	* Production:	<input_def> --> <input> <input_shape_def>
	*/
	return input() && input_shape_def();
}

bool Grammar::input_shape_def()
{
	/**
	* Production:	<input_shape_def> --> input_shape left_brace <input_shape_opt> right_brace 
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("input_shape"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (input_shape_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	
	return false;
}

bool Grammar::input_shape_opt()
{
	/**
	* Production:	<input_shape_opt> --> <dim> <dim> <dim> <dim>
	*/
	if (dim() && dim() && dim())
	{
		network->foundInputSize = true;
		return dim();
	}
	return false;
}

bool Grammar::layers()
{
	/**
	* Production:	<layers> --> <layer_def> <layers> 		FIRST_PLUS = { layer }
    *                           | EMPTY						FIRST_PLUS = { eof }					
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("layer"))
	{
		return layer_def() && layers();
	}
	else if(!parse->curToken())
	{
		return true;
	}

	return false;
}

bool Grammar::layer_def()
{
	/**
	* Production:	<layer_def> --> layer left_brace <layer_opt> right_brace 
	*/
	myLayer = new Layer();

	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("layer"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (layer_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				// Add a new layer here
				network->addLayer(myLayer);
				// std::cout << "Added layer name: " << myLayer->getName() << std::endl;
				// If we add the layer to network, we don't need to delete it
				// cause we will handle it when we delete the network
				myLayer = nullptr;

				parse->nextToken();
				return true;
			}
		}
	}

	delete myLayer;
	myLayer = nullptr;
	return false;
}

bool Grammar::layer_opt()
{
	/**
	* Production:	<layer_opt> --> <name> <layer_type> <bottom> <bottoms> <top> <params>
	*/
	return name() && layer_type() && bottom() && bottoms() && top() && params();

}

bool Grammar::bottoms()
{
	/**
	* Production:	<bottoms> --> <bottom> <bottoms>        FIRST_PLUS = { bottom }                                                
	*                			 | EMPTY           			FIRST_PLUS = { top }
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("bottom"))
	{
		return bottom() && bottoms();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("top"))
	{
		return true;
	}

	return false;
}

bool Grammar::params()
{
	/**
	* Production:	<params> --> <param> <layer_def>
	*/
	return param() && layer_param();
}

bool Grammar::layer_param()
{
	/**
	* Production:	<params> -->| <convolution_def>	 			FIRST_PLUS = { convolution_param }
    *                           | <batch_norm_def>				FIRST_PLUS = { batch_norm_param }
    *                           | <scale_def>					FIRST_PLUS = { scale_param }
    *                           | <pooling_def>					FIRST_PLUS = { pooling_param }
    *                           | <dropout_def>					FIRST_PLUS = { dropout_param }
    *                           | <reshape_def>					FIRST_PLUS = { reshape_param }
    *                           | <inner_product_def>			FIRST_PLUS = { inner_product_param }
	* 							| EMPTY							FIRST_PLUS = { right_brace }
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("convolution_param"))
	{
		return convolution_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("batch_norm_param"))
	{
		return batch_norm_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("scale_param"))
	{
		return scale_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pooling_param"))
	{
		return pooling_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("dropout_param"))
	{
		return dropout_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("reshape_param"))
	{
		return reshape_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("inner_product_param"))
	{
		return inner_product_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}

	return false;
}

bool Grammar::param()
{
	/**
	* Production:	<param> --> <param_def> <param>			FIRST_PLUS = { param }
	*						   | EMPTY             			FIRST_PLUS = { convolution_param, 
    *                                              				batch_norm_param,
    *                                              				scale_param,
    *                                              				pooling_param,
    *                                              				dropout_param,
    *                                              				reshape_param,
    *                                              				inner_product_param,
    *                                              				right_brace,}
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("param"))
	{
		return param_def() && param();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& (!parse->curToken()->getTokenName().compare("convolution_param")
		|| !parse->curToken()->getTokenName().compare("batch_norm_param")
		|| !parse->curToken()->getTokenName().compare("scale_param")
		|| !parse->curToken()->getTokenName().compare("pooling_param")
		|| !parse->curToken()->getTokenName().compare("dropout_param")
		|| !parse->curToken()->getTokenName().compare("reshape_param")
		|| !parse->curToken()->getTokenName().compare("inner_product_param")
		|| !parse->curToken()->getTokenName().compare("right_brace")))
	{
		return true;
	}
}

bool Grammar::param_def()
{
	/**
	* Production:	<param_def> --> param left_brace <lr_mult> <decay_mult> right_brace 
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (lr_mult()
				&& decay_mult()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::convolution_def()
{
	/**
	* Production:	<convolution_def> --> convolution_param left_brace <convolution_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("convolution_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (convolution_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::batch_norm_def()
{
	/**
	* Production:	<batch_norm_def> --> batch_norm_param left_brace <batch_norm_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("batch_norm_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (batch_norm_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::scale_def()
{
	/**
	* Production:	<scale_def> --> scale_param left_brace <scale_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("scale_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (scale_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::pooling_def()
{
	/**
	* Production:	<pooling_def> --> pooling_param left_brace <pooling_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pooling_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (pooling_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::dropout_def()
{
	/**
	* Production:	<dropout_def> --> dropout_param left_brace <dropout_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("dropout_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (dropout_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::reshape_def()
{
	/**
	* Production:	<reshape_def> --> reshape_param left_brace <reshape_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("reshape_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (reshape_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::inner_product_def()
{
	/**
	* Production:	<inner_product_def> --> inner_product_param left_brace <inner_product_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("inner_product_param"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (inner_product_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::convolution_opt()
{
	/**
	* Production:	<convolution_opt> --> <bias_term> <num_output> <convolution_opt_tail>
	*/
	return bias_term() 
		&& num_output() 
		&& convolution_opt_tail();
}

bool Grammar::convolution_opt_tail()
{
	/**
	* Production:	<convolution_opt_tail> --> <pad> <kernel_size> <stride> <group> <weight_filler_def> <bias_filler_def>	FIRST_PLUS = { pad }
    *                                         | <stride> <group> <weight_filler_def> <bias_filler_def> <pads> <kernels>		FIRST_PLUS = { stride }
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pad"))
	{
		return pad() 
			&& kernel_size()
			&& stride()
			&& group()
			&& weight_filler_def()
			&& bias_filler_def();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("stride"))
	{
		return stride() 
			&& group()
			&& weight_filler_def()
			&& bias_filler_def()
			&& pads()
			&& kernels();
	}	
}

bool Grammar::pads()
{
	/**
	* Production:	<pads> --> <pad_h> <pad_w>
	*/
	return pad_h() && pad_w();
}

bool Grammar::kernels()
{
	/**
	* Production:	<kernels> --> <kernel_h> <kernel_w>
	*/
	return kernel_h() && kernel_w();
}

bool Grammar::batch_norm_opt()
{
	/**
	* Production:	<batch_norm_opt> --> <use_global_stats> <eps>
	*/
	return use_global_stats() && eps();
}

bool Grammar::scale_opt()
{
	/**
	* Production:	<scale_opt> --> <bias_term>
	*/
	return bias_term();
}

bool Grammar::pooling_opt()
{
	/**
	* Production:	<pooling_opt> --> <pool> <pooling_opt_tail>
	*/
	return pool() && pooling_opt_tail();
}

bool Grammar::pooling_opt_tail()
{
	/**
	* Production:	<pooling_opt_tail> --> <kernel_size> <stride> <pooling_opt_tail_tail>			FIRST_PLUS = { kernel_size }           
    *                 					  | <pad> <kernel_size> <stride>	  	FIRST_PLUS = { pad } 
	* 									  | <global_pooling>                    FIRST_PLUS = { global_pooling }	
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("kernel_size"))
	{
		return kernel_size() && stride() && pooling_opt_tail_tail();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pad"))
	{
		return pad() && kernel_size() && stride();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("global_pooling"))
	{
		return global_pooling();
	}

	return false;
}

bool Grammar::pooling_opt_tail_tail()
{
	/**
	* Production:	<pooling_opt_tail_tail> --> <pad>       FIRST_PLUS = { pad } 
    *                      						| EMPTY     FIRST_PLUS = { right_brace }
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pad"))
	{
		return pad();
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}

	return false;
}

bool Grammar::dropout_opt()
{
	/**
	* Production:	<dropout_opt> --> <dropout_ratio>
	*/
	return dropout_ratio();
}

bool Grammar::reshape_opt()
{
	/**
	* Production:	<reshape_opt> --> <shape_def>
	*/
	return shape_def();
}

bool Grammar::inner_product_opt()
{
	/**
	* Production:	<inner_product_opt> --> <num_output> <weight_filler_def> <bias_filler_def> 
	*/
	return num_output() && weight_filler_def() && bias_filler_def();
}

bool Grammar::weight_filler_def()
{
	/**
	* Production:	<weight_filler_def> --> weight_filler left_brace <weight_filler_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("weight_filler"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (weight_filler_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::weight_filler_opt()
{
	/**
	* Production:	<weight_filler_opt> --> <weight_filler_type> <std>
	*/
	return weight_filler_type() && std();
}

bool Grammar::bias_filler_def()
{
	/**
	* Production:	<bias_filler_def> --> bias_filler left_brace <bias_filler_opt> right_brace		FIRST_PLUS = { bias_filler }
	* 									 | EMPTY													FIRST_Plus = { right_brace, pad_h }																
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("bias_filler"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (bias_filler_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	else if (parse->curToken()
			&& ((parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			|| (parse->curToken()->getID() == Token::IDTYPE_OPTION
			&& !parse->curToken()->getTokenName().compare("pad_h"))))
	{
		return true;
	}

	return false;
}

bool Grammar::bias_filler_opt()
{
	/**
	* Production:	<bias_filler_opt> --> <bias_filler_type> <value> 
	*/
	return bias_filler_type() && value();
}

bool Grammar::shape_def()
{
	/**
	* Production:	<shape_def> --> shape left_brace <shape_opt> right_brace
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("shape"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_LEFT_BRACE)
		{
			parse->nextToken();
			if (shape_opt()
				&& parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
				&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::shape_opt()
{
	/**
	* Production:	<shape_opt> --> <dim> <dim>
	*/
	return dim() && dim();
}

bool Grammar::name()
{
	/**
	* Production:	<name> --> name colon NAME
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("name"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NAME)
			{		
				// Here we need to check if this func is called during layer parsing
				if (myLayer)
				{	
					// Get layer name here
					myLayer->setName(parse->curToken()->getTokenName());
				}
				// Or called at the parsing of network name
				else if (network->getName() == "")
				{
					// Get network name here
					network->setName(parse->curToken()->getTokenName());
				}

				parse->nextToken();
				return true;
			}
		}
	}

	return false;	
}

bool Grammar::input()
{
	/**
	* Production:	<input> --> input colon NAME
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("input"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NAME)
			{
				// Get input name here
				network->setInputName(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::dim()
{
	/**
	* Production:	<dim> --> dim colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("dim"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				if (!network->foundInputSize)
				{
					network->setInputSize(parse->curToken()->getTokenName());
				}
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::layer_type()
{
	/**
	* Production:	<layer_type> --> type colon TYPE
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("type"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_LAYER_TYPE)
			{
				// Get layer type here
				myLayer->setType(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::bottom()
{
	/**
	* Production:	<bottom> --> bottom colon NAME
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("bottom"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NAME)
			{
				// Get layer bottom here
				myLayer->addBottom(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::top()
{
	/**
	* Production:	<top> --> top colon NAME
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("top"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NAME)
			{
				// Get layer top here
				myLayer->setTop(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::lr_mult()
{
	/**
	* Production:	<lr_mult> --> lr_mult colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("lr_mult"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::decay_mult()
{
	/**
	* Production:	<decay_mult> --> decay_mult colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("decay_mult"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::bias_term()
{
	/**
	* Production:	<bias_term> --> bias_term colon BOOL        FIRST_PLUS = { bias_term }
    *          					   | EMPTY                     	FIRST_PLUS = { num_output, right_brace}
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("bias_term"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	else if (parse->curToken()
		&& ((parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("num_output"))
		|| (parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)))
	{
		return true;
	}

	return false;
}

bool Grammar::num_output()
{
	/**
	* Production:	<num_output> --> num_output colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("num_output"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				// Get layer output here
				myLayer->setOutput(parse->curToken()->getTokenName());

				// Get number of classes here
				network->setNumClasses(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::pad()
{
	/**
	* Production:	<pad> --> pad colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pad"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::kernel_size()
{
	/**
	* Production:	<kernel_size> --> kernel_size colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("kernel_size"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				// Get layer kernel size here
				myLayer->setKernelSize(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::use_global_stats()
{
	/**
	* Production:	<use_global_stats> --> use_global_stats colon BOOL
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("use_global_stats"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::eps()
{
	/**
	* Production:	<eps> --> eps colon NUMBER		FIRST_PLUS = { eps }
	*						 | EMPTY				FIRST_PLUS = { right_brace }
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("eps"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}
	return false;
}

bool Grammar::pool()
{
	/**
	* Production:	<pool> --> pool colon POOLING_METHOD
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pool"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_POOLING_METHOD)
			{
				// get pooling method here
				myLayer->setPool(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::stride()
{
	/**
	* Production:	<stride> --> stride colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("stride"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				// Get layer stride here
				myLayer->setStride(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::global_pooling()
{
	/**
	* Production:	<global_pooling> --> global_pooling colon BOOL
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("global_pooling"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_BOOL)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::dropout_ratio()
{
	/**
	* Production:	<dropout_ratio> --> dropout_ratio colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("dropout_ratio"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				// Get dropout ratio here
				myLayer->setDropoutRatio(parse->curToken()->getTokenName());

				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::weight_filler_type()
{
	/**
	* Production:	<weight_filler_type> --> type colon WEIGHT_FILLER_TYPE
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("type"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_WEIGHT_FILLER_TYPE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::std()
{
	/**
	* Production:	<std> --> std colon NUMBER      FIRST_PLUS = { std }
    *    					 | EMPTY                FIRST_PLUS = { right_brace }
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("std"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
		&& parse->curToken()->getSymType() == Token::SYMTYPE_RIGHT_BRACE)
	{
		return true;
	}

	return false;
}

bool Grammar::group()
{
	/**
	* Production:	<group> --> group colon NUMBER		FIRST_PLUS = { group }
							   | EMPTY					FIRST_PLUS = { weight_filler }				
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("group"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}
	else if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("weight_filler"))
	{
		return true;
	}

	return false;
}

bool Grammar::bias_filler_type()
{
	/**
	* Production:	<bias_filler_type> --> type colon BIAS_FILLER_TYPE
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("type"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_BIAS_FILLER_TYPE)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::value()
{
	/**
	* Production:	<value> --> value colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("value"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::pad_h()
{
	/**
	* Production:	pad_h colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pad_h"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::pad_w()
{
	/**
	* Production:	pad_w colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("pad_w"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::kernel_h()
{
	/**
	* Production:	kernel_h colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("kernel_h"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

bool Grammar::kernel_w()
{
	/**
	* Production:	kernel_w colon NUMBER
	*/
	if (parse->curToken()
		&& parse->curToken()->getID() == Token::IDTYPE_OPTION
		&& !parse->curToken()->getTokenName().compare("kernel_w"))
	{
		parse->nextToken();
		if (parse->curToken()
			&& parse->curToken()->getID() == Token::IDTYPE_SYMBOL
			&& parse->curToken()->getSymType() == Token::SYMTYPE_COLON)
		{
			parse->nextToken();
			if (parse->curToken()
				&& parse->curToken()->getID() == Token::IDTYPE_NUMBER)
			{
				parse->nextToken();
				return true;
			}
		}
	}

	return false;
}

void Grammar::instanciateParser(Parser* newParser)
{
	parse = newParser;
	network = parse->getNetwork();
}

Grammar::Grammar()
{
	myLayer = nullptr;
}