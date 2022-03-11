//
// Created by tuidan on 2021/8/24.
//
#include <Algorithm/DesignAspect/V1.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

void SESAME::V1::Initilize() {
  this->cfTree = DataStructureFactory::createCFTree();
  this->cfTree->setB(V1Param.maxInternalNodes);
  this->cfTree->setL(V1Param.maxLeafNodes);
  this->cfTree->setT(V1Param.thresholdDistance);
  this->root = DataStructureFactory::createNode();
  this->root->setIsLeaf(true);
}


void SESAME::V1::runOnlineClustering(const SESAME::PointPtr input) {
  // insert the root
  forwardInsert(input->copy());
}


void SESAME::V1::runOfflineClustering(DataSinkPtr sinkPtr) {
  std::vector<PointPtr> onlineCenters;
  for(int i = 0; i < this->leafNodes.size(); i++) {
    PointPtr centroid = DataStructureFactory::createPoint(i, 1, V1Param.dimension, 0);
    for(int j = 0; j < V1Param.dimension; j++) {
      centroid->setFeatureItem(this->leafNodes[i]->getCF()->getLS().at(j) / this->leafNodes[i]->getCF()->getN(), j);
    }
    onlineCenters.push_back(centroid->copy());
  }

  vector <PointPtr> offlineCenters;
  vector <vector<PointPtr>> groups;

  std::vector<std::vector<PointPtr>> oldGroups, newGroups;

  // here if clusterNumber is lower than online cluster center number, we choose to skip offline.
  if(onlineCenters.size() <= this->V1Param.clusterNumber){
    offlineCenters = onlineCenters;
  } else{
    this->km.runKMeans(this->V1Param.clusterNumber,
                       onlineCenters.size(),
                       offlineCenters,
                       onlineCenters,
                       oldGroups,
                       newGroups,
                       true);
  }
  // store the result input output
  this->km.produceResult(oldGroups,sinkPtr);
  timerMeter.printTime(false,false,false,false);
}

SESAME::V1::V1(param_t &cmd_params) {
  this->V1Param.pointNumber = cmd_params.pointNumber;
  this->V1Param.dimension = cmd_params.dimension;
  this->V1Param.maxInternalNodes = cmd_params.maxInternalNodes;
  this->V1Param.maxLeafNodes = cmd_params.maxLeafNodes;
  this->V1Param.thresholdDistance = cmd_params.thresholdDistance;
  this->V1Param.clusterNumber = cmd_params.clusterNumber;
}
SESAME::V1::~V1() {

}
// when a new point insert into the CF, update the CF N, LS and SS
void SESAME::V1::updateNLS(SESAME::NodePtr &node, SESAME::PointPtr &point, bool updateAll){
  SESAME::NodePtr nodeSearch = node;
  while(true) {
    SESAME::CFPtr cf = nodeSearch->getCF();
    vector<double> tmpLS = cf->getLS();
    vector<double> tmpSS = cf->getSS();
    if(tmpLS.empty()) {
      for(int i = 0; i < point->getDimension(); i++){
        tmpLS.push_back(0);
        tmpSS.push_back(0);
      }
    }
    cf->setN(cf->getN() + 1);
    for(int i = 0; i < point->getDimension(); i++){
      tmpLS[i] += point->getFeatureItem(i);
      tmpSS[i] += pow(point->getFeatureItem(i), 2);
    }
    cf->setLS(tmpLS);
    cf->setSS(tmpSS);
    if(nodeSearch->getParent() != nullptr  && updateAll) {
      nodeSearch = nodeSearch->getParent();
    } else break;
  }
}

// centroid index: -1(virtual)
// centroid feature: mean of the feature of cluster points
// centroid cluster: -1
void SESAME::V1::calculateCentroid(SESAME::CFPtr &cf, SESAME::PointPtr &centroid) {
  centroid->setIndex(-1);
  centroid->setClusteringCenter(-1);
  vector<double> ls = cf->getLS();
  for(int i = 0; i < ls.size(); i++) centroid->setFeatureItem(ls.at(i) / (double)ls.size(), i);
}

// use Manhattan Distance
void SESAME::V1::pointToClusterDist(SESAME::PointPtr &insertPoint, SESAME::NodePtr &node, double & dist) {
  dist = 0;
  SESAME::PointPtr centroid = make_shared<SESAME::Point>();
  SESAME::CFPtr curCF = node->getCF();
  calculateCentroid(curCF, centroid);
  for(int i = 0; i < insertPoint->getDimension(); i++) {
    dist += abs(centroid->getFeatureItem(i) - insertPoint->getFeatureItem(i));
  }
}

// use Manhattan Distance
double SESAME::V1::clusterToClusterDist(SESAME::NodePtr &nodeA, SESAME::NodePtr &nodeB) {
  double dist = 0;
  SESAME::PointPtr centroidA = make_shared<SESAME::Point>();
  SESAME::PointPtr centroidB = make_shared<SESAME::Point>();
  SESAME::CFPtr curCFA = nodeA->getCF();
  SESAME::CFPtr curCFB = nodeB->getCF();
  calculateCentroid(curCFA, centroidA);
  calculateCentroid(curCFB, centroidB);;
  for(int i = 0; i < centroidA->getDimension(); i++) {
    dist += abs(centroidA->getFeatureItem(i) - centroidB->getFeatureItem(i));
  }
  return dist;
}

// select the closest child cluster according to Manhattan Distance
void SESAME::V1::selectChild(vector<SESAME::NodePtr> &children, SESAME::PointPtr &insertPoint, SESAME::NodePtr &node) {
  double dist = 0;
  double temp = 0;
  pointToClusterDist(insertPoint, children.at(0), dist);
  node = children.at(0);
  for(int i = 1; i < children.size(); i++) {
    pointToClusterDist(insertPoint, children.at(i), temp);
    if(temp < dist) {
      dist = temp;
      node = children.at(i);
    }
  }
}

// calculate the radius of a cluster
double SESAME::V1::calculateRadius(SESAME::PointPtr &point, SESAME::PointPtr &centroid) {
  timerMeter.dataInsertAccMeasure();
  double denominator = 0;
  double radius = 0;
  for(int i = 0; i < point->getDimension(); i++) {
    denominator += pow(centroid->getFeatureItem(i) - point->getFeatureItem(i), 2);
  }
  radius = sqrt(denominator);
  timerMeter.dataInsertEndMeasure();
  return radius;
}

void SESAME::V1::calculateCorDistance(vector<vector<double>> &distance, vector<SESAME::NodePtr> &nodes) {
  // initialization: create a metrics with nxn
  for(int i = 0; i < nodes.size(); i++) {
    vector<double> row;
    for(int j = 0; j < nodes.size(); j++) {
      row.push_back(0);
    }
    distance.push_back(row);
  }

  // calculate the correlate distance
  for(int i = 0; i < nodes.size(); i++) {
    for(int j = i; j < nodes.size(); j++) {
      double dist = clusterToClusterDist(nodes[i], nodes[j]);
      distance[i][j] = dist;
      distance[j][i] = dist;
    }
  }
}

void SESAME::V1::setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point) {
  SESAME::CFPtr curCF = curNode->getCF();
  curCF->setN(curCF->getN() + 1);
  vector<double> newLs;
  vector<double> newSs;
  for(int i = 0; i < point->getDimension(); i++) {
    newLs.push_back(point->getFeatureItem(i));
    newSs.push_back(pow(point->getFeatureItem(i), 2));
  }
  curCF->setSS(newSs);
  curCF->setLS(newLs);
}

void SESAME::V1::addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent) {
  SESAME::CFPtr childCF = child->getCF();
  SESAME::CFPtr parCF = parent->getCF();
  parCF->setN(parCF->getN() + childCF->getN());
  vector<double> newLs;
  vector<double> newSs;
  for(int i = 0; i < childCF->getLS().size(); i++) {
    newLs.push_back(childCF->getLS().at(i) + parCF->getLS().at(i));
    newSs.push_back(childCF->getSS().at(i) + parCF->getSS().at(i));
  }
  parCF->setLS(newLs);
  parCF->setSS(newSs);
}

void SESAME::V1::initializeCF(SESAME::CFPtr &cf, int dimension) {
  vector<double> ls = cf->getLS();
  vector<double> ss = cf->getSS();
  for(int i = 0; i < dimension; i++) {
    ls.push_back(0);
    ss.push_back(0);
  }
  cf->setLS(ls);
  cf->setSS(ss);
}

void SESAME::V1::clearChildParents(vector<SESAME::NodePtr> &children) {
  for(auto child : children) {
    child->clearParents();
  }
}

void SESAME::V1::forwardInsert(SESAME::PointPtr point){
  NodePtr curNode = this->root;
  if(curNode->getCF()->getN() == 0) {
    timerMeter.dataInsertAccMeasure();
    updateNLS(curNode, point, true);
    timerMeter.dataInsertEndMeasure();
  } else{
    while(1) {
      vector<NodePtr> childrenNode = curNode->getChildren();
      if(curNode->getIsLeaf()) {
        timerMeter.clusterUpdateAccMeasure();
        CFPtr curCF = curNode->getCF();
        timerMeter.dataInsertAccMeasure();
        if(curCF->getN() == 0) {
          initializeCF(curCF, point->getDimension());
        }
        PointPtr centroid = make_shared<Point>();
        calculateCentroid(curCF, centroid);
        timerMeter.dataInsertEndMeasure();
        if(calculateRadius(point,  centroid) <= this->cfTree->getT()) { // concept drift detection
          // whether the new radius is lower than threshold T
          timerMeter.dataInsertAccMeasure();
          updateNLS(curNode, point, true);
          timerMeter.dataInsertEndMeasure();

          // means this point could get included in this cluster
          //SESAME_DEBUG("No concept drift occurs(t <= T), insert tha point into the leaf node...");
          break;
          // Normally insert the data point into the tree leafNode without concept drift
        } else {
          // concept drift adaption
          // SESAME_DEBUG("Concept drift occurs(t > T), the current leaf node capacity reaches the threshold T");
          timerMeter.clusterUpdateAccMeasure();
          backwardEvolution(curNode, point);
          timerMeter.clusterUpdateEndMeasure();
          break;
        }

      } else{
        timerMeter.dataInsertAccMeasure();
        selectChild(childrenNode, point, curNode);
        timerMeter.dataInsertEndMeasure();
      }
    }
  }
}

// concept drift adaption
void SESAME::V1::backwardEvolution(SESAME::NodePtr &curNode, SESAME::PointPtr &point) {
  if(curNode->getParent() == nullptr) { // means current node is root node
    //SESAME_DEBUG("l <= L, create a new leaf node and insert the point into it(root change)");
    NodePtr newRoot = make_shared<CFNode>();
    newRoot->setIsLeaf(false);
    newRoot->setChild(curNode);
    curNode->setParent(newRoot);

    NodePtr newNode = make_shared<CFNode>();
    newNode->setIsLeaf(true);
    newNode->setParent(newRoot);
    vector<double> curLS = curNode->getCF()->getLS();
    vector<double> curSS = curNode->getCF()->getSS();
    int curN = curNode->getCF()->getN();
    newRoot->getCF()->setLS(curLS);
    newRoot->getCF()->setSS(curSS);
    newRoot->getCF()->setN(curN);
    newRoot->setIndex(this->leafMask++);
    // here we need to remove the old root and add the new one into the leafnodes set
    this->leafNodes.push_back(newRoot);


    // update the parent node
    newRoot->setChild(newNode);
    updateNLS(newNode, point, true);
    this->root = newRoot;

  } else{
    NodePtr parent = curNode->getParent();
    NodePtr newNode = make_shared<CFNode>();
    newNode->setIsLeaf(true);
    newNode->setParent(parent);
    parent->setChild(newNode);
    updateNLS(newNode, point, false);
    if(parent->getChildren().size() < this->cfTree->getL()){
      // whether the number of CFs(clusters) in the current leaf node is lower thant threshold L
      //SESAME_DEBUG("l <= L, create a new leaf node and insert the point into it");

      // update the parent node
      updateNLS(parent, point, true);
    } else{
      // SESAME_DEBUG("l > L, parent node of the current leaf node capacity reaches the threshold L");
      //SESAME_DEBUG("split a new parent node from the old one ");
      bool CurNodeIsLeaf = true;
      while(true) {
        NodePtr parParent;
        if(parent->getParent() == nullptr) {
          parParent = make_shared<CFNode>();
          parParent->setIsLeaf(false);
          this->root = parParent;
          CFPtr parCF = parent->getCF();
          parParent->setCF(parCF);
        } else{
          parParent = parent->getParent();
          parParent->removeChild(parent);
        }
        NodePtr newParentA = make_shared<CFNode>();
        NodePtr newParentB = make_shared<CFNode>();
        if(parent->getChildren().at(0)->getIsLeaf()) {
          for(int i = 0; i < this->leafNodes.size(); i++) {
            if(this->leafNodes.at(i)->getIndex() == parent->getIndex()) {
              this->leafNodes.erase(this->leafNodes.begin() + i);
            }
          }
          newParentA->setIndex(++this->leafMask);
          newParentB->setIndex(++this->leafMask);
          this->leafNodes.push_back(newParentA);
          this->leafNodes.push_back(newParentB);
        }


        newParentB->setIsLeaf(false);
        newParentA->setIsLeaf(false);
        newParentB->setParent(parParent);
        newParentA->setParent(parParent);
        parParent->setChild(newParentB);
        parParent->setChild(newParentA);
        CFPtr cfA = newParentA->getCF();
        CFPtr cfB = newParentB->getCF();
        initializeCF(cfA, point->getDimension());
        initializeCF(cfB, point->getDimension());

        vector<NodePtr> broNodes = parent->getChildren();
        vector<vector<double>> corCFDistance;
        calculateCorDistance(corCFDistance, broNodes);

        // choose two farthest CFs as seedA and seedB
        int seedA = 0;
        int seedB = 0;
        double max = 0;
        for(int i = 0; i < broNodes.size(); i++) {
          for(int j = i; j < broNodes.size(); j++) {
            if(max < corCFDistance[i][j]) {
              seedA = i;
              seedB = j;
              max = corCFDistance[i][j];
            }
          }
        }

        // insert the child node into the nearest seed(A / B)
        clearChildParents(broNodes);
        // insert seedA node into parent A
        newParentA->setChild(broNodes[seedA]);
        broNodes[seedA]->setParent(newParentA);
        addNodeNLSToNode(broNodes[seedA], newParentA);
        // insert seedB node into parent B
        newParentB->setChild(broNodes[seedB]);
        broNodes[seedB]->setParent(newParentB);
        addNodeNLSToNode(broNodes[seedB], newParentB);
        // split other nodes into A and B
        for(int i = 0; i < broNodes.size(); i++) {
          if(i != seedA and i != seedB){
            if(corCFDistance[i][seedA] < corCFDistance[i][seedB]) {
              newParentA->setChild(broNodes[i]);
              addNodeNLSToNode(broNodes[i], newParentA);
              broNodes[i]->clearParents();
              broNodes[i]->setParent(newParentA);
            }else {
              newParentB->setChild(broNodes[i]);
              addNodeNLSToNode(broNodes[i], newParentB);
              broNodes[i]->clearParents();
              broNodes[i]->setParent(newParentB);
            }
          }
        }
        if(CurNodeIsLeaf){
          updateNLS(parParent, point, true);
        }

        if(parParent->getChildren().size() <= this->cfTree->getB()) {
          //SESAME_DEBUG("b < B, remove the old node and insert the new nodeA and nodeB into the parent node");
          break;
        }else {
          //SESAME_DEBUG("b >= B, parent node of the current interior node capacity reaches the threshold B");
          curNode = curNode->getParent();
          parent = parParent;
          CurNodeIsLeaf = false;
        }
      }
    }
  }
}

