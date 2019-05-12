#include <algorithm>
#include <string>
#include <sstream>
#include "Util.hh"
#include "Token.hh"


std::pair<bool, int> Util::isSymbol(std::string::iterator pos)
{
	switch (*pos)
	{
	case '{':
		return std::pair<bool, int>(true, Token::SYMTYPE_LEFT_BRACE);
	case '}':
		return std::pair<bool, int>(true, Token::SYMTYPE_RIGHT_BRACE);
	case ':':
		return std::pair<bool, int>(true, Token::SYMTYPE_COLON);
	default:
		return std::pair<bool, int>(false, Token::SYMTYPE_INVALID);
	}
}

bool Util::isLetter(std::string::iterator pos)
{
	if ((*pos >= 'a' && *pos <= 'z')
		|| (*pos >= 'A' && *pos <= 'Z')
		|| (*pos == '_'))
	{
		return true;
	}
	return false;
}

bool Util::isDigit(std::string::iterator pos)
{
	if (*pos >= '0' && *pos <= '9')
	{
		return true;
	}
	return false;
}

bool Util::isDot(std::string::iterator pos)
{
    if (*pos == '.')
    {
        return true;
    }
    return false;
}

bool Util::isType(std::string curString)
{
	if (!curString.compare("\"Convolution\"")
		|| !curString.compare("\"BatchNorm\"")
		|| !curString.compare("\"Scale\"")
		|| !curString.compare("\"ReLU\"")
		|| !curString.compare("\"Pooling\"")
		|| !curString.compare("\"Dropout\"")
		|| !curString.compare("\"Reshape\"")
		|| !curString.compare("\"Softmax\"")
		|| !curString.compare("\"Concat\"")
		|| !curString.compare("\"InnerProduct\""))
	{
		return true;
	}
	return false;
}

bool Util::isWeightFillerType(std::string curString)
{
	if (!curString.compare("\"xavier\"")
		|| !curString.compare("\"gaussian\""))
	{
		return true;
	}
	return false;
}

bool Util::isBiasFillerType(std::string curString)
{
	if (!curString.compare("\"constant\""))
	{
		return true;
	}
	return false;
}

bool Util::isSubstrOf(std::string str1, std::string str2)
{
	if (str2.find(str1) != std::string::npos)
	{
		return true;
	}
	return false;
}

bool Util::isOption(std::string curString)
{
	if (!curString.compare("input")
		|| !curString.compare("dim")
		|| !curString.compare("input_shape")
		|| !curString.compare("layer")
		|| !curString.compare("name")
		|| !curString.compare("type")
		|| !curString.compare("bottom")
		|| !curString.compare("top")
		|| !curString.compare("param")
		|| !curString.compare("convolution_param")
		|| !curString.compare("batch_norm_param")
		|| !curString.compare("scale_param")
		|| !curString.compare("pooling_param")
		|| !curString.compare("dropout_param")
		|| !curString.compare("reshape_param")
		|| !curString.compare("weight_filler")
		|| !curString.compare("shape")
		|| !curString.compare("lr_mult")
		|| !curString.compare("decay_mult")
		|| !curString.compare("bias_term")
		|| !curString.compare("num_output")
		|| !curString.compare("pad")
		|| !curString.compare("kernel_size")
		|| !curString.compare("use_global_stats")
		|| !curString.compare("eps")
		|| !curString.compare("pool")
		|| !curString.compare("stride")
		|| !curString.compare("global_pooling")
		|| !curString.compare("dropout_ratio")
		|| !curString.compare("std")
		|| !curString.compare("group")
		|| !curString.compare("inner_product_param")
		|| !curString.compare("bias_filler")
		|| !curString.compare("value")
		|| !curString.compare("pad_h")
		|| !curString.compare("pad_w")
		|| !curString.compare("kernel_h")
		|| !curString.compare("kernel_w"))
	{
		return true;
	}
	return false;
}

bool Util::isOptionCandidate(std::string curString)
{
	if (Util::isSubstrOf(curString, "input")
		|| Util::isSubstrOf(curString, "dim")
		|| Util::isSubstrOf(curString, "input_shape")
		|| Util::isSubstrOf(curString, "layer")
		|| Util::isSubstrOf(curString, "name")
		|| Util::isSubstrOf(curString, "type")
		|| Util::isSubstrOf(curString, "bottom")
		|| Util::isSubstrOf(curString, "top")
		|| Util::isSubstrOf(curString, "param")
		|| Util::isSubstrOf(curString, "convolution_param")
		|| Util::isSubstrOf(curString, "batch_norm_param")
		|| Util::isSubstrOf(curString, "scale_param")
		|| Util::isSubstrOf(curString, "pooling_param")
		|| Util::isSubstrOf(curString, "dropout_param")
		|| Util::isSubstrOf(curString, "reshape_param")
		|| Util::isSubstrOf(curString, "weight_filler")
		|| Util::isSubstrOf(curString, "shape")
		|| Util::isSubstrOf(curString, "lr_mult")
		|| Util::isSubstrOf(curString, "decay_mult")
		|| Util::isSubstrOf(curString, "bias_term")
		|| Util::isSubstrOf(curString, "num_output")
		|| Util::isSubstrOf(curString, "pad")
		|| Util::isSubstrOf(curString, "kernel_size")
		|| Util::isSubstrOf(curString, "use_global_stats")
		|| Util::isSubstrOf(curString, "eps")
		|| Util::isSubstrOf(curString, "pool")
		|| Util::isSubstrOf(curString, "stride")
		|| Util::isSubstrOf(curString, "global_pooling")
		|| Util::isSubstrOf(curString, "dropout_ratio")
		|| Util::isSubstrOf(curString, "std")
		|| Util::isSubstrOf(curString, "group")
		|| Util::isSubstrOf(curString, "inner_product_param")
		|| Util::isSubstrOf(curString, "bias_filler")
		|| Util::isSubstrOf(curString, "value")
		|| Util::isSubstrOf(curString, "pad_h")
		|| Util::isSubstrOf(curString, "pad_w")
		|| Util::isSubstrOf(curString, "kernel_h")
		|| Util::isSubstrOf(curString, "kernel_w"))
	{
		return true;
	}
	return false;
}

bool Util::isBool(std::string curString)
{
	if (!curString.compare("true")
		|| !curString.compare("false"))
	{
		return true;
	}
	return false;
}

bool Util::isBoolCandidate(std::string curString)
{
	if (Util::isSubstrOf(curString, "true")
		|| Util::isSubstrOf(curString, "false"))
	{
		return true;
	}
	return false;
}

bool Util::isPoolingMethod(std::string curString)
{
	if (!curString.compare("MAX")
		|| !curString.compare("AVE")
		|| !curString.compare("STOCHASTIC"))
	{
		return true;
	}
	return false;
}

bool Util::isPoolingMethodCandidate(std::string curString)
{
	if (Util::isSubstrOf(curString, "MAX")
		|| Util::isSubstrOf(curString, "AVE")
		|| Util::isSubstrOf(curString, "STOCHASTIC"))
	{
		return true;
	}
	return false;
}

std::string& Util::replace_all(
	std::string& str, const std::string& old_value, const std::string& new_value)
{   
    for(std::string::size_type pos(0); pos!=std::string::npos; pos+=new_value.length())   
	{   
        if((pos=str.find(old_value,pos))!=std::string::npos)   
            str.replace(pos,old_value.length(),new_value);   
        else  
			break;   
    }   
    return str;   
} 

std::vector<std::string> Util::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();

    for(int i = 0; i < size; i++)
    {
        pos = str.find(pattern,i);
        if(pos < size)
        {
            std::string s = str.substr(i,pos-i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}