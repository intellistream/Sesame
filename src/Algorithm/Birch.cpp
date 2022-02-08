//
// Created by tuidan on 2021/8/24.
//
#include <Algorithm/Birch.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

void SESAME::Birch::Initilize() {
  this->cfTree = DataStructureFactory::createCFTree();
  this->cfTree->setB(BirchParam.maxInternalNodes);
  this->cfTree->setL(BirchParam.maxLeafNodes);
  this->cfTree->setT(BirchParam.thresholdDistance);
  this->root = DataStructureFactory::createNode();
  this->root->setIsLeaf(true);
}


void SESAME::Birch::runOnlineClustering(const SESAME::PointPtr input) {
    // insert the root
  forwardInsert(input);
}


void SESAME::Birch::runOfflineClustering(DataSinkPtr sinkPtr) {
  for(int i = 0; i < this->leafNodes.size(); i++) {
    PointPtr centroid = DataStructureFactory::createPoint(i, 1, BirchParam.dimension, 0);
    for(int j = 0; j < BirchParam.dimension; j++) {
      centroid->setFeatureItem(this->leafNodes[i]->getCF()->getLS().at(j) / this->leafNodes[i]->getCF()->getN(), j);
    }
    sinkPtr->put(centroid->copy());
  }
  timerMeter.printTime(false,false,false,false);
  //SESAME_DEBUG( "The size of the centroid is :" << sinkPtr->getResults().size());
//  std::vector<std::vector<PointPtr>> oldGroups, newGroups;
//  this->kmeans->runKMeans((int)middleCentroids.size() / 2, (int)middleCentroids.size(),
//                          middleCentroids,oldGroups,newGroups, true);
//  this->kmeans->produceResult(oldGroups, sinkPtr);
}

SESAME::Birch::Birch(param_t &cmd_params) {
  this->BirchParam.pointNumber = cmd_params.pointNumber;
  this->BirchParam.dimension = cmd_params.dimension;
  this->BirchParam.maxInternalNodes = cmd_params.maxInternalNodes;
  this->BirchParam.maxLeafNodes = cmd_params.maxLeafNodes;
  this->BirchParam.thresholdDistance = cmd_params.thresholdDistance;
}
SESAME::Birch::~Birch() {

}
// when a new point insert into the CF, update the CF N, LS and SS
void SESAME::Birch::updateNLS(SESAME::NodePtr &node, SESAME::PointPtr &point, bool updateAll){
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
    if(nodeSearch->getIsLeaf()) {
      nodeSearch->insertPoint(point);
    }
    for(int i = 0; i < point->getDimension(); i++){
      tmpLS[i] += point->getFeatureItem(i);
      tmpSS[i] += pow(point->getFeatureItem(i), 2);
    }
    cf->setLS(tmpLS);
    cf->setSS(tmpSS);
    if(!nodeSearch->getParents().empty()  && updateAll) {
      nodeSearch = nodeSearch->getParents().at(0);
    } else break;
  }
}

// centroid index: -1(virtual)
// centroid feature: mean of the feature of cluster points
// centroid cluster: -1
void SESAME::Birch::calculateCentroid(SESAME::CFPtr &cf, SESAME::PointPtr &centroid) {
  centroid->setIndex(-1);
  centroid->setClusteringCenter(-1);
  vector<double> ls = cf->getLS();
  for(int i = 0; i < ls.size(); i++) centroid->setFeatureItem(ls.at(i) / (double)ls.size(), i);
}

// use Manhattan Distance
void SESAME::Birch::pointToClusterDist(SESAME::PointPtr &insertPoint, SESAME::NodePtr &node, double & dist) {
  dist = 0;
  SESAME::PointPtr centroid = make_shared<SESAME::Point>();
  SESAME::CFPtr curCF = node->getCF();
  calculateCentroid(curCF, centroid);
  for(int i = 0; i < insertPoint->getDimension(); i++) {
    dist += abs(centroid->getFeatureItem(i) - insertPoint->getFeatureItem(i));
  }
}

// use Manhattan Distance
double SESAME::Birch::clusterToClusterDist(SESAME::NodePtr &nodeA, SESAME::NodePtr &nodeB) {
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
void SESAME::Birch::selectChild(vector<SESAME::NodePtr> &children, SESAME::PointPtr &insertPoint, SESAME::NodePtr &node) {
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
double SESAME::Birch::calculateRadius(SESAME::PointPtr &point, SESAME::PointPtr &centroid) {
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

void SESAME::Birch::calculateCorDistance(vector<vector<double>> &distance, vector<SESAME::NodePtr> &nodes) {
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

void SESAME::Birch::setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point) {
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

void SESAME::Birch::addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent) {
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

void SESAME::Birch::initializeCF(SESAME::CFPtr &cf, int dimension) {
  vector<double> ls = cf->getLS();
  vector<double> ss = cf->getSS();
  for(int i = 0; i < dimension; i++) {
    ls.push_back(0);
    ss.push_back(0);
  }
  cf->setLS(ls);
  cf->setSS(ss);
}

void SESAME::Birch::clearChildParents(vector<SESAME::NodePtr> &children) {
  for(auto child : children) {
    child->clearParents();
  }
}

void SESAME::Birch::forwardInsert(SESAME::PointPtr point){
  NodePtr curNode = this->root;
  if(curNode->getCF()->getN() == 0) {
    timerMeter.dataInsertAccMeasure();
    updateNLS(curNode, point, true);
    timerMeter.dataInsertEndMeasure();
  } else{
    while(1) {
      vector<NodePtr> childrenNode = curNode->getChildren();
      if(curNode->getIsLeaf()) {
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
void SESAME::Birch::backwardEvolution(SESAME::NodePtr &curNode, SESAME::PointPtr &point) {
  if(curNode->getParents().empty()) { // means current node is root node
    //SESAME_DEBUG("l <= L, create a new leaf node and insert the point into it(root change)");
    NodePtr newRoot = make_shared<CFNode>();
    newRoot->setIsLeaf(false);
    newRoot->setChild(curNode);
    curNode->setParent(newRoot);
    NodePtr newNode = make_shared<CFNode>();
    newNode->setIsLeaf(true);
    newNode->setParent(curNode->getParents().at(0));
    vector<double> curLS = curNode->getCF()->getLS();
    vector<double> curSS = curNode->getCF()->getSS();
    int curN = curNode->getCF()->getN();
    newRoot->getCF()->setLS(curLS);
    newRoot->getCF()->setSS(curSS);
    newRoot->getCF()->setN(curN);
    newRoot->setIndex(this->leafMask++);
    this->leafNodes.push_back(newRoot);


    // update the parent node
    newRoot->setChild(newNode);
    updateNLS(newNode, point, true);
    this->root = newRoot;

  } else{
    NodePtr parent = curNode->getParents().at(0);
    NodePtr newNode = make_shared<CFNode>();
    newNode->setIsLeaf(true);
    updateNLS(newNode, point, false);
    newNode->setParent(parent);
    parent->setChild(newNode);
    if(parent->getChildren().size() <= this->cfTree->getL()){
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
        if(parent->getParents().empty()) {
          parParent = make_shared<CFNode>();
          parParent->setIsLeaf(false);
          this->root = parParent;
          CFPtr parCF = parent->getCF();
          parParent->setCF(parCF);
        } else{
          parParent = parent->getParents().at(0);
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
        parParent->setChild(newParentB);
        parParent->setChild(newParentA);
        newParentA->setParent(parParent);
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
        newParentA->setChild(broNodes[seedA]);
        addNodeNLSToNode(broNodes[seedA], newParentA);
        broNodes[seedA]->setParent(newParentA);
        newParentB->setChild(broNodes[seedB]);
        broNodes[seedB]->setParent(newParentB);
        addNodeNLSToNode(broNodes[seedB], newParentB);
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
          curNode = curNode->getParents()[0];
          parent = parParent;
          CurNodeIsLeaf = false;
        }
      }
    }
  }
}

