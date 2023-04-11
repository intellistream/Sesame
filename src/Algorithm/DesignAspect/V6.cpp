//
// Created by tuidan on 2021/8/24.
//
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <Algorithm/DesignAspect/V6.hpp>

void SESAME::V6::Init()
{
    this->cfTree = DataStructureFactory::createCFTree();
    this->cfTree->setB(V6Param.max_in_nodes);
    this->cfTree->setL(V6Param.max_leaf_nodes);
    this->cfTree->setT(V6Param.distance_threshold);
    this->root = DataStructureFactory::createNode();
    this->root->setIsLeaf(true);
}

void SESAME::V6::RunOnline(const SESAME::PointPtr input)
{
    // insert the root
    if (input->getIndex() >= this->V6Param.landmark)
    {
        forwardInsert(input->copy());
    }
}

void SESAME::V6::RunOffline(DataSinkPtr sinkPtr)
{
    for (int i = 0; i < this->clusterNodes.size(); i++)
    {
        PointPtr centroid = DataStructureFactory::createPoint(i, 1, V6Param.dim, 0);
        for (int j = 0; j < V6Param.dim; j++)
        {
            centroid->setFeatureItem(this->clusterNodes[i]->getCF()->getLS().at(j) /
                                         this->clusterNodes[i]->getCF()->getN(),
                                     j);
        }
        sinkPtr->put(centroid->copy());
    }
    timerMeter.printTime(false, false, false, false);
}

SESAME::V6::V6(param_t &cmd_params)
{
    this->V6Param.num_points         = cmd_params.num_points;
    this->V6Param.dim                = cmd_params.dim;
    this->V6Param.max_in_nodes       = cmd_params.max_in_nodes;
    this->V6Param.max_leaf_nodes     = cmd_params.max_leaf_nodes;
    this->V6Param.distance_threshold = cmd_params.distance_threshold;
    this->V6Param.landmark           = cmd_params.landmark;
}
SESAME::V6::~V6() {}
// when a new point insert into the CF, update the CF N, LS and SS
void SESAME::V6::updateNLS(SESAME::NodePtr &node, SESAME::PointPtr &point, bool updateAll)
{
    SESAME::NodePtr nodeSearch = node;
    while (true)
    {
        SESAME::CFPtr cf     = nodeSearch->getCF();
        vector<double> tmpLS = cf->getLS();
        vector<double> tmpSS = cf->getSS();
        if (tmpLS.empty())
        {
            for (int i = 0; i < point->getDimension(); i++)
            {
                tmpLS.push_back(0);
                tmpSS.push_back(0);
            }
        }
        cf->setN(cf->getN() + 1);
        for (int i = 0; i < point->getDimension(); i++)
        {
            tmpLS[i] += point->getFeatureItem(i);
            tmpSS[i] += pow(point->getFeatureItem(i), 2);
        }
        cf->setLS(tmpLS);
        cf->setSS(tmpSS);
        if (nodeSearch->getParent() != nullptr && updateAll)
        {
            nodeSearch = nodeSearch->getParent();
        }
        else
            break;
    }
}

// centroid index: -1(virtual)
// centroid feature: mean of the feature of cluster points
// centroid cluster: -1
void SESAME::V6::calculateCentroid(SESAME::CFPtr &cf, SESAME::PointPtr &centroid)
{
    centroid->setIndex(-1);
    centroid->setClusteringCenter(-1);
    vector<double> ls = cf->getLS();
    for (int i = 0; i < ls.size(); i++) centroid->setFeatureItem(ls.at(i) / cf->getN(), i);
}

// use Manhattan Distance
void SESAME::V6::pointToClusterDist(SESAME::PointPtr &insertPoint, SESAME::NodePtr &node,
                                    double &dist)
{
    dist                      = 0;
    SESAME::PointPtr centroid = make_shared<SESAME::Point>(V6Param.dim);
    SESAME::CFPtr curCF       = node->getCF();
    calculateCentroid(curCF, centroid);
    for (int i = 0; i < insertPoint->getDimension(); i++)
    {
        dist += pow(centroid->getFeatureItem(i) - insertPoint->getFeatureItem(i), 2);
    }
}

// use Manhattan Distance
double SESAME::V6::clusterToClusterDist(SESAME::NodePtr &nodeA, SESAME::NodePtr &nodeB)
{
    double dist                = 0;
    SESAME::PointPtr centroidA = make_shared<SESAME::Point>(V6Param.dim);
    SESAME::PointPtr centroidB = make_shared<SESAME::Point>(V6Param.dim);
    SESAME::CFPtr curCFA       = nodeA->getCF();
    SESAME::CFPtr curCFB       = nodeB->getCF();
    calculateCentroid(curCFA, centroidA);
    calculateCentroid(curCFB, centroidB);
    ;
    for (int i = 0; i < centroidA->getDimension(); i++)
    {
        dist += pow(centroidA->getFeatureItem(i) - centroidB->getFeatureItem(i), 2);
    }
    return dist;
}

// select the closest child cluster according to Manhattan Distance
void SESAME::V6::selectChild(vector<SESAME::NodePtr> &children, SESAME::PointPtr &insertPoint,
                             SESAME::NodePtr &node)
{
    double dist = 0;
    double temp = 0;
    pointToClusterDist(insertPoint, children.at(0), dist);
    node = children.at(0);
    for (int i = 1; i < children.size(); i++)
    {
        pointToClusterDist(insertPoint, children.at(i), temp);
        if (temp < dist)
        {
            dist = temp;
            node = children.at(i);
        }
    }
}

// calculate the radius of a cluster
double SESAME::V6::calculateRadius(SESAME::PointPtr &point, SESAME::PointPtr &centroid)
{
    timerMeter.dataInsertAccMeasure();
    double denominator = 0;
    double radius      = 0;
    for (int i = 0; i < point->getDimension(); i++)
    {
        denominator += pow(centroid->getFeatureItem(i) - point->getFeatureItem(i), 2);
    }
    radius = sqrt(denominator);
    timerMeter.dataInsertEndMeasure();
    return radius;
}

void SESAME::V6::calculateCorDistance(vector<vector<double>> &distance,
                                      vector<SESAME::NodePtr> &nodes)
{
    // initialization: create a metrics with nxn
    for (int i = 0; i < nodes.size(); i++)
    {
        vector<double> row;
        for (int j = 0; j < nodes.size(); j++)
        {
            row.push_back(0);
        }
        distance.push_back(row);
    }

    // calculate the correlate distance
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = i; j < nodes.size(); j++)
        {
            double dist    = clusterToClusterDist(nodes[i], nodes[j]);
            distance[i][j] = dist;
            distance[j][i] = dist;
        }
    }
}

void SESAME::V6::setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point)
{
    SESAME::CFPtr curCF = curNode->getCF();
    curCF->setN(curCF->getN() + 1);
    vector<double> newLs;
    vector<double> newSs;
    for (int i = 0; i < point->getDimension(); i++)
    {
        newLs.push_back(point->getFeatureItem(i));
        newSs.push_back(pow(point->getFeatureItem(i), 2));
    }
    curCF->setSS(newSs);
    curCF->setLS(newLs);
}

void SESAME::V6::addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent)
{
    SESAME::CFPtr childCF = child->getCF();
    SESAME::CFPtr parCF   = parent->getCF();
    parCF->setN(parCF->getN() + childCF->getN());
    vector<double> newLs;
    vector<double> newSs;
    for (int i = 0; i < childCF->getLS().size(); i++)
    {
        newLs.push_back(childCF->getLS().at(i) + parCF->getLS().at(i));
        newSs.push_back(childCF->getSS().at(i) + parCF->getSS().at(i));
    }
    parCF->setLS(newLs);
    parCF->setSS(newSs);
}

void SESAME::V6::initializeCF(SESAME::CFPtr &cf, int dim)
{
    vector<double> ls, ss;
    for (int i = 0; i < dim; i++)
    {
        ls.push_back(0);
        ss.push_back(0);
    }
    cf->setN(0);
    cf->setLS(ls);
    cf->setSS(ss);
}

void SESAME::V6::clearChildParents(vector<SESAME::NodePtr> &children)
{
    for (auto child : children)
    {
        child->clearParents();
    }
}

void SESAME::V6::forwardInsert(SESAME::PointPtr point)
{
    NodePtr curNode = this->root;
    if (curNode->getCF()->getN() == 0)
    {
        updateNLS(curNode, point, true);
        clusterNodes.push_back(curNode);
    }
    else
    {
        while (1)
        {
            vector<NodePtr> childrenNode = curNode->getChildren();
            if (curNode->getIsLeaf())
            {
                CFPtr curCF = curNode->getCF();
                if (curCF->getN() == 0)
                {
                    initializeCF(curCF, point->getDimension());
                }
                PointPtr centroid = make_shared<Point>(V6Param.dim);
                calculateCentroid(curCF, centroid);
                if (calculateRadius(point, centroid) <= this->cfTree->getT())
                {  // concept drift detection
                    // whether the new radius is lower than threshold T
                    updateNLS(curNode, point, true);
                    // means this point could get included in this cluster
                    // SESAME_DEBUG("No concept drift occurs(t <= T), insert tha point into the leaf
                    // node...");
                    break;
                    // Normally insert the data point into the tree leafNode without concept drift
                }
                else
                {
                    // concept drift adaption
                    // SESAME_DEBUG("Concept drift occurs(t > T), the current leaf node capacity
                    // reaches the threshold T");
                    backwardEvolution(curNode, point);
                    break;
                }
            }
            else
            {
                selectChild(childrenNode, point, curNode);
            }
        }
    }
}

// concept drift adaption
void SESAME::V6::backwardEvolution(SESAME::NodePtr &curNode, SESAME::PointPtr &point)
{
    if (curNode->getParent() == nullptr)
    {  // means current node is root node
        // l <= L, create a new leaf node and insert the point into it(root change)
        NodePtr newRoot = make_shared<CFNode>();
        newRoot->setIsLeaf(false);
        newRoot->setChild(curNode);
        curNode->setParent(newRoot);

        NodePtr newNode = make_shared<CFNode>();
        newNode->setIsLeaf(true);
        newNode->setParent(newRoot);
        vector<double> curLS = curNode->getCF()->getLS();
        vector<double> curSS = curNode->getCF()->getSS();
        int curN             = curNode->getCF()->getN();
        newRoot->getCF()->setLS(curLS);
        newRoot->getCF()->setSS(curSS);
        newRoot->getCF()->setN(curN);
        newRoot->setIndex(this->leafMask++);
        this->clusterNodes.push_back(newRoot);
        // here we need to remove the old root and add the new one into the leafnodes set
        // update the parent node
        newRoot->setChild(newNode);
        updateNLS(newNode, point, true);
        this->root = newRoot;
    }
    else
    {
        NodePtr parent  = curNode->getParent();
        NodePtr newNode = make_shared<CFNode>();
        newNode->setIsLeaf(true);
        newNode->setParent(parent);
        parent->setChild(newNode);
        updateNLS(newNode, point, false);
        if (parent->getChildren().size() < this->cfTree->getL())
        {
            // whether the number of CFs(clusters) in the current leaf node is lower thant threshold
            // L l <= L, create a new leaf node and insert the point into it update the parent node
            // and all nodes on the path to root node
            updateNLS(parent, point, true);
        }
        else
        {
            // l > L, parent node of the current leaf node capacity reaches the threshold L, split a
            // new parent node from the old one
            bool CurNodeIsClus = true;
            while (true)
            {
                NodePtr parParent;
                if (parent->getParent() == nullptr)
                {
                    // if the parent node is the root, we need to create a new root as a parParent
                    parParent = make_shared<CFNode>();
                    parParent->setIsLeaf(false);
                    this->root = parParent;
                    // since the parent node's nls has not been updated by the point, so we directly
                    // copy the nls in parent node to the parParent one
                    CFPtr parCF = parent->getCF();
                    parParent->setCF(parCF);
                    parParent->setChild(parent);
                }
                else
                {
                    // if the parent node is not the root, we can get the parParent one directly
                    parParent = parent->getParent();
                }
                // we need to create a new parent node since the old one has to split
                NodePtr newParentA = make_shared<CFNode>();
                // insert the new parent into the allNode list
                // we also need to insert the new parent node into the clusterNode list if its
                // children is a leaf node.
                if (parent->getChildren().at(0)->getIsLeaf())
                {
                    newParentA->setIndex(++this->leafMask);
                    this->clusterNodes.push_back(newParentA);
                }
                // we only create a new parent rather and keep the old parent node as the split two
                // sub-nodes so we need to refresh the old parent node as a blank one and treat it
                // as a new parent B
                parent->setIsLeaf(false);
                newParentA->setIsLeaf(false);
                parent->setParent(
                    parParent);  // link the parparent node and the new created new parent A
                newParentA->setParent(parParent);
                parParent->setChild(newParentA);
                // clean cf of the old parent node and initialize the cf of new parent A (ls and ss
                // all have d number of 0)
                CFPtr cfA = newParentA->getCF();
                CFPtr cfB = parent->getCF();
                initializeCF(cfA, point->getDimension());
                initializeCF(cfB, point->getDimension());
                // split the child nodes of the old parent nodes
                vector<NodePtr> broNodes = parent->getChildren();
                std::vector<NodePtr> blankChildren;
                parent->setChildren(blankChildren);
                vector<vector<double>> corCFDistance;
                calculateCorDistance(
                    corCFDistance,
                    broNodes);  //  calculate the distance between each two brother nodes
                // choose two farthest CFs as seedA and seedB
                int seedA  = 0;
                int seedB  = 0;
                double max = 0;
                for (int i = 0; i < broNodes.size(); i++)
                {
                    for (int j = i; j < broNodes.size(); j++)
                    {
                        if (max < corCFDistance[i][j])
                        {
                            seedA = i;
                            seedB = j;
                            max   = corCFDistance[i][j];
                        }
                    }
                }
                // insert the child node into the nearest seed(A / B)
                clearChildParents(broNodes);
                // insert seedA node into new parent A and link them
                newParentA->setChild(broNodes[seedA]);
                broNodes[seedA]->setParent(newParentA);
                addNodeNLSToNode(broNodes[seedA], newParentA);
                // insert seed B node into new parent B and link them
                parent->setChild(broNodes[seedB]);
                broNodes[seedB]->setParent(parent);
                addNodeNLSToNode(broNodes[seedB], parent);
                // if other one brother node is near seed A then split it into new parent A,
                // otherwise new parent B.
                for (int i = 0; i < broNodes.size(); i++)
                {
                    if (i != seedA and i != seedB)
                    {
                        if (corCFDistance[i][seedA] < corCFDistance[i][seedB])
                        {
                            newParentA->setChild(broNodes[i]);
                            addNodeNLSToNode(
                                broNodes[i],
                                newParentA);  // since the brother nodes list contains the one we
                                              // insert our point, so after this function, the
                                              // parent node's nls are also updated.
                            broNodes[i]->clearParents();
                            broNodes[i]->setParent(newParentA);
                        }
                        else
                        {
                            parent->setChild(broNodes[i]);
                            addNodeNLSToNode(broNodes[i], parent);
                            broNodes[i]->clearParents();
                            broNodes[i]->setParent(parent);
                        }
                    }
                }
                // if the current node(parent) is a cluster nodes, then we need to update the nls of
                // its parent using new point. we only update the parparent in the first loop.
                if (CurNodeIsClus)
                {
                    updateNLS(parParent, point, true);
                }

                if (parParent->getChildren().size() <= this->cfTree->getB())
                {
                    // b < B, remove the old node and insert the new nodeA and nodeB into the parent
                    // node.
                    break;
                }
                else
                {
                    // b >= B, parent node of the current interior node capacity reaches the
                    // threshold B.
                    curNode       = curNode->getParent();
                    parent        = parParent;
                    CurNodeIsClus = false;
                }
            }
        }
    }
}
