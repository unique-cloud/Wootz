#include "TFNode.hh"
#include "Layer.hh"
#include <iostream>
#include <algorithm>

TFNode::TFNode(std::string newTop) : top(newTop)
{
    
}

std::vector<std::string> TFNode::getBottom()
{
    return bottom;
}

void TFNode::addNext(TFNode* newNext)
{
    next.push_back(newNext);
}

void TFNode::addLayer(Layer* newLayer)
{
    myLayers.push_back(newLayer);
}

std::string TFNode::getTop()
{
    return top;
}

std::vector<TFNode*> TFNode::getNext()
{
    return next;
}

void TFNode::setTop(std::string const &newName)
{
    top = newName;
}

void TFNode::setBottom(std::vector<std::string> const &newBottom)
{
    bottom = newBottom;
}

bool TFNode::isConv2D()
{
    int conv2dTypes[4] = {
        Layer::LAYER_TYPE_CONV,
        Layer::LAYER_TYPE_BN,
        Layer::LAYER_TYPE_SCALE,
        Layer::LAYER_TYPE_RELU
    };

    if (myLayers.size() != 4)
        return false;

    for (int i = 0; i < 4; ++i)
    {
        if (myLayers[i]->getType() != conv2dTypes[i])
        {
            return false;
        }
    }
    
    return true;
}

bool TFNode::isMaxPool2D()
{
    if(myLayers.size() != 1)
        return false;

    return (myLayers[0]->getType() == Layer::LAYER_TYPE_POOLING)
        && (myLayers[0]->getPool() == "MAX");
}

bool TFNode::isAvgPool2D()
{
    if(myLayers.size() != 1)
        return false;

    return (myLayers[0]->getType() == Layer::LAYER_TYPE_POOLING)
        && (myLayers[0]->getPool() == "AVE");
}

bool TFNode::isSoftmax()
{
    if(myLayers.size() != 1)
        return false;

    return myLayers[0]->getType() == Layer::LAYER_TYPE_SOFTMAX;
}

bool TFNode::isInnerPro()
{
    if(myLayers.size() != 1)
        return false;

    return myLayers[0]->getType() == Layer::LAYER_TYPE_INNERPRO;
}

bool TFNode::isConcat()
{
    if(myLayers.size() != 1)
        return false;

    return myLayers[0]->getType() == Layer::LAYER_TYPE_CONCAT;
}

Layer* TFNode::getLayer(int layerType)
{
    for (auto iter = myLayers.begin(); iter != myLayers.end(); ++iter)
    {
        if ((*iter)->getType() == layerType)
        {
            return *iter;
        }
    }

    return nullptr;
}

std::vector<Layer*> TFNode::getLayers()
{
    return myLayers;
}

int TFNode::getNumOfLayers()
{
    return myLayers.size();
}