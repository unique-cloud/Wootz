#include "Network.hh"
#include <iostream>
#include <algorithm>

Network::Network()
{
    inputNode = new TFNode(inputName);
    layerVector = new std::vector<Layer*>();
    layerNodes = new std::vector<TFNode*>();

    foundInputSize = false;
}

Network::~Network()
{
    delete inputNode;

    for (auto iter = layerVector->begin(); iter != layerVector->end(); ++iter)
    {
        delete *iter;
    }
    delete layerVector;

    for (auto iter = layerNodes->begin(); iter != layerNodes->end(); ++iter)
    {
        delete *iter;
    }
    delete layerNodes;
}

void Network::addLayer(Layer* const newLayer)
{
    layerVector->push_back(newLayer);
}

std::vector<Layer*>* Network::getLayerVec()
{
    return layerVector;
}

std::string Network::getInputSize()
{
    return inputSize;
}

void Network::setInputSize(std::string const &newSize)
{
    inputSize = newSize;
}

std::string Network::getNumClasses()
{
    return numClasses;
}

void Network::setNumClasses(std::string const &newNum)
{
    numClasses = newNum;
}

void Network::setName(std::string const &newName)
{
    name = newName;
}

std::string Network::setInputName(std::string const &newName)
{
    inputName = newName;
    inputNode->setTop(newName);
}

std::string Network::getName()
{
    return name;
}

TFNode* Network::findNodeWithTop(std::string myTop)
{
    bool isFound = false;
    TFNode* result = nullptr;

    for (auto iter = layerNodes->begin(); iter != layerNodes->end(); ++iter)
    {
        if((*iter)->getTop() == myTop)
        {
            if (!isFound)
            {
                result = (*iter);
                isFound = true;
            }
            else
            {
                std::cout << "Error: Found mulitple TFNodes with same top" << std::endl;
                result = nullptr;
            }
        }
    }

    return result;
}

bool Network::formLayerNode()
{
    // Form all layers with same top to a TFNode
	for (auto iter = layerVector->begin(); iter != layerVector->end(); ++iter)
	{
		std::string myTop = (*iter)->getTop();

		// Search TFNode with the same top
		TFNode* tNode = findNodeWithTop(myTop);
		// If not found, create a new node
		if (!tNode)
		{
			tNode = new TFNode(myTop);
            layerNodes->push_back(tNode);
        }
        
        // Add current layer
        tNode->addLayer(*iter);
            
        // Now check if we need to modify or set bottom of node
        std::vector<std::string> myBottom = (*iter)->getBottom();
        // Check if this layer has the different bottom and top
        if (std::find(myBottom.begin(), myBottom.end(), myTop) == myBottom.end())
        {
            // If different, use it as the bottom of the node
            // But we still need to make sure the node doesn't have a bottom yet
            if (tNode->getBottom().size() == 0)
            {
                tNode->setBottom(myBottom);
            }
            else
            {
                // If we already a bottom of node, this means we have
                // mulitiple bottoms for one node which is not correct
                std::cout << "Error: Found mulitple bottom for one node" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool Network::linkNodes()
{
	// Link all nodes using bottom
    for (auto iter = layerNodes->begin(); iter != layerNodes->end(); ++iter)
    {
        std::vector<std::string> myBottom = (*iter)->getBottom();

        for (auto it = myBottom.begin(); it != myBottom.end(); ++it)
        {
            // Find node which top is equal to current bottom
            TFNode* bNode = findNodeWithTop(*it);
            // If found, add a link
            if (bNode)
            {
                bNode->addNext(*iter);
            }
            // Else if it's the first layer after input
            else if (*it == inputName)
            {
                inputNode->addNext(*iter);
            }
            // Otherwise, error
            else
            {
                std::cout << "Link node error: Can't find node using bottom" << std::endl;
		        return false;
            }
        }
    }
    
    return true;
}

TFNode* Network::getInputNode()
{
    return inputNode;
}

void Network::printLayers()
{
	std::cout << "---------------------Layers----------------------" << std::endl;
	std::cout << "Name\tType\t" << std::endl;

	for (auto iter = layerVector->begin(); iter != layerVector->end(); ++iter)
	{
		std::cout << (*iter)->getName() << "\t" 
			<< (*iter)->getType() << "\t" 
			<< std::endl;
	}
}

void Network::printNodes()
{
    std::cout << "---------------------Nodes----------------------" << std::endl;
	std::cout << "Name\tNext\t" << std::endl;

	for (auto iter = layerNodes->begin(); iter != layerNodes->end(); ++iter)
	{
		std::cout << (*iter)->getTop() << "\t";
        std::vector<TFNode*> next = (*iter)->getNext();

        for (auto it = next.begin(); it != next.end(); ++it)
        {
            std::cout << (*it)->getTop() << "\t";
        }

        std::cout << std::endl;
	}
}