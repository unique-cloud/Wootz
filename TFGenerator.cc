#include "TFGenerator.hh"
#include "Util.hh"
#include <iostream>
#include <algorithm>

TFGenerator::TFGenerator(std::string inFileName)
{
    myNetwork = nullptr;
    parse = nullptr;
    fileName = inFileName;
    isMultiplex = false;
    useMultiplex = true;
}

void TFGenerator::instanciateParser(Parser* newParser)
{
    parse = newParser;
    myNetwork = parse->getNetwork();
}

bool TFGenerator::genTFCode(bool genMultiPlex)
{
    isMultiplex = genMultiPlex;
    
    // read in templete code
    std::ifstream inFile;
    if (isMultiplex)
	    inFile.open("template_multiplexing.py", std::ifstream::in);
    else
        inFile.open("template.py", std::ifstream::in);

    std::string code((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // fill in attributes
    std::string number_classes = myNetwork->getNumClasses();
    std::string network_name = myNetwork->getName();
    Util::replace_all(network_name, "\"", "");
    std::string input_image_size = myNetwork->getInputSize();

    Util::replace_all(code, "NUM_PREDICTED_CLASS", number_classes);
    Util::replace_all(code, "NETWORK_NAME", network_name);
    Util::replace_all(code, "FILE_NAME", fileName);
    Util::replace_all(code, "INPUT_IMAGE_SIZE", input_image_size);

    // generate code
    std::string allCode;
    genAll(allCode);
    Util::replace_all(code, "CODE_PLACEHOLDER", allCode);

    // output to file
    std::string outFileName;
    if (isMultiplex)
        outFileName = fileName + "_multiplexing.py";
    else
        outFileName = fileName + ".py";
    std::ofstream outFile(outFileName.c_str(), std::ifstream::out);
    outFile << code;
    outFile.close();

    return true;
}

bool TFGenerator::genAll(std::string &allCode)
{
    TFNode* start = myNetwork->getInputNode();
    if(!start)
    {
        std::cout << "Invalid input node" << std::endl;
        return false;
    }
    
    auto myNodeVec = start->getNext();
    std::string cur_scope = "net";
    std::string last_scope = "inputs";

    while (myNodeVec.size() != 0)
    {
        if (myNodeVec.size() == 1)
        {
            TFNode* myNode = myNodeVec[0];

            std::string nodeCode;
            genNode(myNode, cur_scope, last_scope, nodeCode);
            allCode += nodeCode;

            myNodeVec = myNodeVec[0]->getNext();
        }
        else
        {
            // Concat
            std::string mixedCode;
            genConcat(myNodeVec, cur_scope, last_scope, mixedCode);
            allCode += mixedCode;  
        }

        allCode += "\n";
    }

    std::string returnCode;
    returnCode += "  return LAST_SCOPE, end_points\n";
    Util::replace_all(returnCode, "LAST_SCOPE", last_scope);
    allCode += returnCode;

    return true;
}

bool TFGenerator::genNode(
    TFNode* myNode, std::string cur_scope, std::string &last_scope, std::string &nodeCode)
{
    nodeCode += "      end_point = 'NODE_TOP'\n";
    std::string node_top = myNode->getTop();
    Util::replace_all(node_top, "\"", "");
    Util::replace_all(nodeCode, "NODE_TOP", node_top);

    std::string singleCode;
    if (myNode->isConv2D() 
        || myNode->isAvgPool2D()
        || myNode->isMaxPool2D()
        || myNode->isSoftmax()
        || myNode->isInnerPro())
    {
        genSingle(myNode, cur_scope, last_scope, singleCode);
        
        singleCode += "      end_points[end_point] = LAST_SCOPE\n";
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
    }
    else if (myNode->getNumOfLayers() == 1)
    {
        genLayer(myNode->getLayers()[0], cur_scope, last_scope, singleCode);

        singleCode += "      end_points[end_point] = LAST_SCOPE\n";
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
    }
    else
    {
        genMultiple(myNode, cur_scope, last_scope, singleCode);
    }
    nodeCode += singleCode;

    return true;
}

bool TFGenerator::genSingle(
    TFNode* myNode, std::string cur_scope, std::string &last_scope, std::string &singleCode)
{   
    std::string cur_scope_new = cur_scope;
    if (cur_scope != "net")
    {
        cur_scope_new = "      " + cur_scope_new;
    }

    if (myNode->isConv2D())
    {
        if (isMultiplex && useMultiplex)
            singleCode += "      CUR_SCOPE = slim.conv2d(LAST_SCOPE, selectdepth(end_point,NUM_OUTPUT), [KERNEL_SIZE, KERNEL_SIZE], stride=STRIDE, scope=MY_SCOPE)\n";
        else
            singleCode += "      CUR_SCOPE = slim.conv2d(LAST_SCOPE, NUM_OUTPUT, [KERNEL_SIZE, KERNEL_SIZE], stride=STRIDE, scope=MY_SCOPE)\n";

        std::string num_output = myNode->getLayer(Layer::LAYER_TYPE_CONV)->getOutput();
        std::string kernel_size = myNode->getLayer(Layer::LAYER_TYPE_CONV)->getKernelSize();
        std::string stride = myNode->getLayer(Layer::LAYER_TYPE_CONV)->getStride();
        if (stride == "1")
        {
            Util::replace_all(singleCode, ", stride=STRIDE", "");
        }
        std::string my_scope;
        if (cur_scope == "net")
        {
            my_scope = "end_point";
        }
        else
        {
            my_scope = myNode->getTop();
            Util::replace_all(my_scope, "\"", "");
            my_scope = Util::split(my_scope, "/").back();
            my_scope = "'" + my_scope + "'"; 
        }

        Util::replace_all(singleCode, "CUR_SCOPE", cur_scope_new);
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
        Util::replace_all(singleCode, "NUM_OUTPUT", num_output);
        Util::replace_all(singleCode, "KERNEL_SIZE", kernel_size);
        Util::replace_all(singleCode, "STRIDE", stride);
        Util::replace_all(singleCode, "MY_SCOPE", my_scope);
    }
    else if (myNode->isMaxPool2D())
    {
        singleCode += "      CUR_SCOPE = slim.max_pool2d(LAST_SCOPE, [KERNEL_SIZE, KERNEL_SIZE], stride=STRIDE, scope=MY_SCOPE)\n";

        std::string num_output = myNode->getLayer(Layer::LAYER_TYPE_POOLING)->getOutput();
        std::string kernel_size = myNode->getLayer(Layer::LAYER_TYPE_POOLING)->getKernelSize();
        std::string stride = myNode->getLayer(Layer::LAYER_TYPE_POOLING)->getStride();
        if (stride == "1")
        {
            Util::replace_all(singleCode, ", stride=STRIDE", "");
        }
        std::string my_scope;
        if (cur_scope == "net")
        {
            my_scope = "end_point";
        }
        else
        {
            my_scope = myNode->getTop();
            Util::replace_all(my_scope, "\"", "");
            my_scope = Util::split(my_scope, "/").back();
            my_scope = "'" + my_scope + "'";  
        }
 
        Util::replace_all(singleCode, "CUR_SCOPE", cur_scope_new);
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
        Util::replace_all(singleCode, "KERNEL_SIZE", kernel_size);
        Util::replace_all(singleCode, "STRIDE", stride);
        Util::replace_all(singleCode, "MY_SCOPE", my_scope);
    }
    else if (myNode->isAvgPool2D())
    {
        singleCode += "      CUR_SCOPE = slim.avg_pool2d(LAST_SCOPE, [KERNEL_SIZE, KERNEL_SIZE], stride=STRIDE, scope=MY_SCOPE)\n";

        std::string kernel_size = myNode->getLayer(Layer::LAYER_TYPE_POOLING)->getKernelSize();
        if (kernel_size == "")
        {
            // Fix kernel size here
            kernel_size = "7";
        }
        std::string stride = myNode->getLayer(Layer::LAYER_TYPE_POOLING)->getStride();
        if (stride == "")
        {
            // Fix stride here
            stride = "1";
        }
        std::string my_scope;
        if (cur_scope == "net")
        {
            my_scope = "end_point";
        }
        else
        {
            my_scope = myNode->getTop();
            Util::replace_all(my_scope, "\"", "");
            my_scope = Util::split(my_scope, "/").back();
            my_scope = "'" + my_scope + "'"; 
        }

        Util::replace_all(singleCode, "CUR_SCOPE", cur_scope_new);
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
        Util::replace_all(singleCode, "KERNEL_SIZE", kernel_size);
        Util::replace_all(singleCode, "STRIDE", stride);
        Util::replace_all(singleCode, "MY_SCOPE", my_scope);
    }
    else if (myNode->isSoftmax())
    {
        singleCode += "      CUR_SCOPE = slim.softmax(LAST_SCOPE, scope=MY_SCOPE)\n";
        std::string my_scope;
        if (cur_scope == "net")
        {
            my_scope = "end_point";
        }
        else
        {
            my_scope = myNode->getTop();
            Util::replace_all(my_scope, "\"", "");
            my_scope = Util::split(my_scope, "/").back();
            my_scope = "'" + my_scope + "'"; 
        }

        Util::replace_all(singleCode, "CUR_SCOPE", cur_scope_new);
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
        Util::replace_all(singleCode, "MY_SCOPE", my_scope);
    }
    else if (myNode->isInnerPro())
    {
        singleCode += "      CUR_SCOPE = slim.fully_connected(LAST_SCOPE, NUM_OUTPUT, scope=MY_SCOPE)\n";
        std::string num_output = myNode->getLayer(Layer::LAYER_TYPE_INNERPRO)->getOutput();
        std::string my_scope;
        if (cur_scope == "net")
        {
            my_scope = "end_point";
        }
        else
        {
            my_scope = myNode->getTop();
            Util::replace_all(my_scope, "\"", "");
            my_scope = Util::split(my_scope, "/").back();
            my_scope = "'" + my_scope + "'"; 
        }
        
        Util::replace_all(singleCode, "CUR_SCOPE", cur_scope_new);
        Util::replace_all(singleCode, "LAST_SCOPE", last_scope);
        Util::replace_all(singleCode, "NUM_OUTPUT", num_output);
        Util::replace_all(singleCode, "MY_SCOPE", my_scope);
    }
    else
    {
        std::cout << "Error: reach unknow place" << std::endl;
        return false;
    }

    last_scope = cur_scope;
    return true;
}

bool TFGenerator::genMultiple(
    TFNode* myNode, std::string cur_scope, std::string &last_scope, std::string &multiCode)
{
    multiCode += "      with tf.variable_scope(end_point):\n";
    cur_scope = myNode->getTop();
    Util::replace_all(cur_scope, "\"", "");
    
    auto layers = myNode->getLayers();
    for (auto iter = layers.begin(); iter != layers.end(); ++iter)
    {
        std::string layerCode;
        genLayer(*iter, cur_scope, last_scope, layerCode);
        multiCode += layerCode;
    }

    multiCode += "        end_points[end_point] = LAST_SCOPE\n";
    Util::replace_all(multiCode, "LAST_SCOPE", last_scope);

    return true;
}

bool TFGenerator::genConcat(
    std::vector<TFNode*> &myNodeVec, std::string cur_scope, std::string &last_scope, std::string &mixedCode)
{
    std::vector<TFNode*> concatNodeVec;
    std::vector<std::string> scopes;

    for (int i = 0; i < myNodeVec.size(); ++i)
    {
        // Here, we assume that there won't be another concat node until this concat ends
        TFNode* myNode = myNodeVec[i];
        std::string cur_scope_new = "branch_" + std::to_string(i);
        std::string last_scope_new = last_scope;
        scopes.push_back(cur_scope_new);

        mixedCode += "        with tf.variable_scope('SCOPE_NAME'):\n";
        Util::replace_all(mixedCode, "SCOPE_NAME", cur_scope_new);
        
        useMultiplex = true;

        while (!myNode->isConcat())
        {
            auto temp = myNode->getNext();
            if (temp.size() != 1)
            {
                std::cout << "Error: concat node in concat node" << std::endl;
                return false;
            }
            if (temp[0]->isConcat())
                useMultiplex = false;

            std::string nodeCode;
            if (myNode->isConv2D() 
                || myNode->isAvgPool2D()
                || myNode->isMaxPool2D()
                || myNode->isSoftmax()
                || myNode->isInnerPro())
            {
                genSingle(myNode, cur_scope_new, last_scope_new, nodeCode);
            }
            else
            {
                std::cout << "The structure is too complex in a concat layer" << std::endl;
                return false;
            }   
            mixedCode += nodeCode;

            myNode = temp[0];
        }

        // store concat node we found
        concatNodeVec.push_back(myNode);
    }

    // check if the concat nodes found from different branch are same
    std::sort(concatNodeVec.begin(), concatNodeVec.end());
	concatNodeVec.erase(std::unique(concatNodeVec.begin(), concatNodeVec.end()), concatNodeVec.end());
    if (concatNodeVec.size() != 1)
    {
        std::cout << "Found mutiple concat nodes" << std::endl;
        return false;
    }
    TFNode* concatNode = concatNodeVec[0];

    std::string node_top = concatNode->getTop();
    Util::replace_all(node_top, "\"", "");

    std::string scope_list = scopes[0];
    for (int i = 1; i < scopes.size(); ++i)
    {
        scope_list += (", " + scopes[i]);
    }

    mixedCode = "      with tf.variable_scope(end_point):\n" + mixedCode;
    mixedCode = "      end_point = 'NODE_TOP'\n" + mixedCode;
    if (isMultiplex) 
    {
        mixedCode = "      ############## end template code ##############\n" + mixedCode;
        mixedCode = "      net = selectinput(end_point, net)\n" + mixedCode;
        mixedCode = "      ############## template code added for multiplexing ##############\n" + mixedCode;
    }

    mixedCode += "        net = tf.concat(\n";
    mixedCode += "            axis=3, values=[SCOPE_LIST])\n";
    mixedCode += "      end_points[end_point] = net\n";

    Util::replace_all(mixedCode, "NODE_TOP", node_top);
    Util::replace_all(mixedCode, "SCOPE_LIST", scope_list);

    last_scope = cur_scope;
    myNodeVec = concatNode->getNext();
    return true;
}

bool TFGenerator::genLayer(
    Layer* myLayer, std::string cur_scope, std::string &last_scope, std::string &layerCode)
{
    if (myLayer->getType() == Layer::LAYER_TYPE_DROPOUT)
    {
        layerCode += "        CUR_SCOPE = slim.dropout(LAST_SCOPE, RATIO, scope='LAYER_NAME')\n";
        std::string ratio = std::to_string(1 - std::stof(myLayer->getDropoutRatio())); 
        std::string layer_name = myLayer->getName();
        layer_name = Util::split(layer_name, "/").back();

        Util::replace_all(layerCode, "CUR_SCOPE", cur_scope);
        Util::replace_all(layerCode, "LAST_SCOPE", last_scope);
        Util::replace_all(layerCode, "RATIO", ratio);
        Util::replace_all(layerCode, "LAYER_NAME", layer_name);
    }
    else if (myLayer->getType() == Layer::LAYER_TYPE_CONV)
    {
        layerCode += "        CUR_SCOPE = slim.conv2d(LAST_SCOPE, num_classes, [KERNEL_SIZE, KERNEL_SIZE], activation_fn=None,\n";
        layerCode += "                             normalizer_fn=None, scope='LAYER_NAME')\n";
        std::string kernel_size = myLayer->getKernelSize();
        std::string layer_name = myLayer->getName();
        layer_name = Util::split(layer_name, "/").back();

        Util::replace_all(layerCode, "CUR_SCOPE", cur_scope);
        Util::replace_all(layerCode, "LAST_SCOPE", last_scope);
        Util::replace_all(layerCode, "KERNEL_SIZE", kernel_size);
        Util::replace_all(layerCode, "LAYER_NAME", layer_name);
    }
    else if (myLayer->getType() == Layer::LAYER_TYPE_RESHAPE)
    {
        layerCode += "        CUR_SCOPE = tf.squeeze(LAST_SCOPE, [KERNEL_SIZE, 2], name='SpatialSqueeze')\n";
        std::string kernel_size = myLayer->getKernelSize();
        if (kernel_size == "")
        {
            // Fix kernel size here
            kernel_size = "1";
        }

        Util::replace_all(layerCode, "CUR_SCOPE", cur_scope);
        Util::replace_all(layerCode, "LAST_SCOPE", last_scope);
        Util::replace_all(layerCode, "KERNEL_SIZE", kernel_size);
    }

    last_scope = cur_scope;
    return true;
}
