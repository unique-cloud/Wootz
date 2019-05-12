#include "Layer.hh"
#include <iostream>

Layer::Layer()
{
	type = LAYER_TYPE_EMPTY;
}

void Layer::setType(std::string const &newType)
{
    if (!newType.compare("\"Convolution\""))
    {   
        type = LAYER_TYPE_CONV;
    }
    else if (!newType.compare("\"BatchNorm\""))
    {
        type = LAYER_TYPE_BN;
    }
    else if (!newType.compare("\"Scale\""))
    {
        type = LAYER_TYPE_SCALE;
    }
    else if (!newType.compare("\"ReLU\""))
    {
        type = LAYER_TYPE_RELU;
    }
    else if (!newType.compare("\"Pooling\""))
    {
        type = LAYER_TYPE_POOLING;
    }
    else if (!newType.compare("\"Dropout\""))
    {
        type = LAYER_TYPE_DROPOUT;
    }
    else if (!newType.compare("\"Reshape\""))
    {
        type = LAYER_TYPE_RESHAPE;
    }
    else if (!newType.compare("\"Softmax\""))
    {
        type = LAYER_TYPE_SOFTMAX;
    }
    else if (!newType.compare("\"Concat\""))
    {
        type = LAYER_TYPE_CONCAT;
    }
    else if (!newType.compare("\"InnerProduct\""))
    {
        type = LAYER_TYPE_INNERPRO;
    }
    else
    {
        std::cout << "Unknow layer type" << std::endl;
    }
}

void Layer::setName(std::string const &newName)
{
    name = newName;
}

void Layer::addBottom(std::string const &newBottom)
{
    bottom.push_back(newBottom);
}

void Layer::setTop(std::string const &newTop)
{
    top = newTop;
}

void Layer::setOutput(std::string const &newOutput)
{
    output = newOutput;
}

void Layer::setKernelSize(std::string const &newKZ)
{
    kernel_size = newKZ;
}

void Layer::setStride(std::string const &newStride)
{
    stride = newStride;
}

void Layer::setDropoutRatio(std::string const &newDropR)
{
    dropout_ratio = newDropR;
}

void Layer::setPool(std::string const &newPool)
{
    pool = newPool;
}

int Layer::getType()
{
    return type;
}

std::string Layer::getName()
{
    return name;
}

std::vector<std::string> Layer::getBottom()
{
    return bottom;
}

std::string Layer::getTop()
{
    return top;
}

std::string Layer::getOutput()
{
    return output;
}

std::string Layer::getKernelSize()
{
    return kernel_size;
}

std::string Layer::getStride()
{
    return stride;
}

std::string Layer::getDropoutRatio()
{
    return dropout_ratio;
}

std::string Layer::getPool()
{
    return pool;
}