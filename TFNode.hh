#ifndef _TFNODE_H
#define _TFNODE_H

#include <string>
#include <vector>
#include "Layer.hh"

class TFNode
{
public:

    TFNode(std::string newTop);

    void addLayer(Layer* newLayer);

    std::vector<std::string> getBottom(); 

    void addNext(TFNode* newNext);

    // bool reorderLayers();

    std::string getTop();

    std::vector<TFNode*> getNext();

    void setTop(std::string const &newName);

    void setBottom(std::vector<std::string> const &newBottom);

    bool isConv2D();

    bool isInnerPro();

    bool isMaxPool2D();

    bool isAvgPool2D();

    bool isSoftmax();

    bool isConcat();

    Layer* getLayer(int layerType);

    std::vector<Layer*> getLayers();

    int getNumOfLayers();

private:

    std::string top;
    std::vector<std::string> bottom;
    std::vector<Layer*> myLayers;
    std::vector<TFNode*> next;
};

#endif