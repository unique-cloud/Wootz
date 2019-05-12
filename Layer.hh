#ifndef _LAYER_H
#define _LAYER_H

#include <vector>
#include <string>


class Layer
{
public: 

    Layer();

    void setType(std::string const &newType);
    void setName(std::string const &newName);
    void addBottom(std::string const &newBottom);
    void setTop(std::string const &newTop);

    void setOutput(std::string const &newOutput);
    void setKernelSize(std::string const &newKZ);
    void setStride(std::string const &newStride);
    void setDropoutRatio(std::string const &newDropR);
    void setPool(std::string const &newPool);

	int getType();
    std::string getName();
    std::vector<std::string> getBottom();
    std::string getTop();

    std::string getOutput();
    std::string getKernelSize();
    std::string getStride();
    std::string getDropoutRatio();
    std::string getPool();

    static enum LAYER_TYPE{
		LAYER_TYPE_CONV, 
        LAYER_TYPE_BN,
        LAYER_TYPE_SCALE, 
        LAYER_TYPE_RELU,
        LAYER_TYPE_POOLING,
        LAYER_TYPE_DROPOUT, 
        LAYER_TYPE_RESHAPE, 
        LAYER_TYPE_SOFTMAX, 
        LAYER_TYPE_CONCAT,
        LAYER_TYPE_INNERPRO,
        LAYER_TYPE_EMPTY
    }LTYPE;

private:

    int type;
    std::string name;
    std::vector<std::string> bottom;
    std::string top;

    // Follow attributes only validate in CONV layer
    std::string output;
    std::string kernel_size;
    std::string stride;
    std::string dropout_ratio;
    std::string pool;
};

#endif