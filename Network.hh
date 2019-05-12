#ifndef _NETWORK_H
#define _NETWORK_H

#include <vector>
#include <string>
#include "Layer.hh"
#include "TFNode.hh"

class Network
{
public: 

    Network();

    ~Network();

    void setName(std::string const &newName);

    /* Get network name */
    std::string getName();

    /* Set input name */
    std::string setInputName(std::string const &newName);

    /* Add a layer to layerVector */
    void addLayer(Layer* const newLayer);

    /* Get a pointer to layerVecotr */
    std::vector<Layer*>* getLayerVec();

    /* Get input size */
    std::string getInputSize();

    /* Set input size */
    void setInputSize(std::string const &newSize);

    /* Get number of predicted classes */
    std::string getNumClasses();

    /* Set number of predicted classes */
    void setNumClasses(std::string const &newNum);

    /* Form layer nodes from layers in layerVector */
    bool formLayerNode();

    /* Link all nodes using bottom */
    bool linkNodes();

    /* Find particular node in layerNodes by its top */
    TFNode* findNodeWithTop(std::string myTop);

    /* Get InputNode */
    TFNode* getInputNode();

    void printLayers();

    void printNodes();

    bool foundInputSize;

private:

    /* Network name, defined in prototxt */
    std::string name;

    /* Network input name, defined in prototxt */
    std::string inputName;

    /* Network input size */
    std::string inputSize;

    /* Number of predicted classes */
    std::string numClasses;

    /* Header of TFNode link list*/
    TFNode* inputNode;

    /* All layers parsed by parser. */
    std::vector<Layer*> *layerVector;

    /* All nodes formed from layers */
    std::vector<TFNode*> *layerNodes;
};

#endif