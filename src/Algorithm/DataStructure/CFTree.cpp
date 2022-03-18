//
// Created by tuidan on 2021/8/24.
//

#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"

#include <limits>
#include <vector>

namespace SESAME {

CFTree::CFTree(const StreamClusteringParam &param)
    : maxInternalNodes(param.maxInternalNodes),
      maxLeafNodes(param.maxLeafNodes),
      thresholdDistance(param.thresholdDistance) {}

CFTree::CFTree(int b = 0, int l = 0, double t = 0.0)
    : maxInternalNodes(b), maxLeafNodes(l), thresholdDistance(t) {}

CFTree::~CFTree() {}
int CFTree::getB() const { return this->maxInternalNodes; }
int CFTree::getL() const { return this->maxLeafNodes; }
double CFTree::getT() const { return this->thresholdDistance; }
void CFTree::setB(int b) { this->maxInternalNodes = b; }
void CFTree::setT(double t) { this->thresholdDistance = t; }
void CFTree::setL(int l) { this->maxLeafNodes = l; }

CFPtr CFNode::getCF() { return this->curCF; }
NodePtr CFNode::getParent() { return this->parent; }
int CFNode::getIndex() const { return this->index; }
std::vector<NodePtr> CFNode::getChildren() { return this->children; }

void CFNode::setNode(CFPtr &Node) { this->curCF = Node; }
void CFNode::setParent(NodePtr &Parent) { this->parent = Parent->copy(); }
void CFNode::setIndex(int Index) { this->index = Index; }
void CFNode::setChild(NodePtr &child) { this->children.push_back(child); }
void CFNode::setChildren(std::vector<NodePtr> children) {
  this->children = children;
}
CFNode::CFNode() {
  this->curCF = std::make_shared<CF>();
  this->isLeaf = true;
}
CFNode::~CFNode() {}
bool CFNode::getIsLeaf() { return this->isLeaf; }
void CFNode::setIsLeaf(bool leaf) { this->isLeaf = leaf; }
NodePtr CFNode::copy() { return std::make_shared<CFNode>(*this); }
void CFNode::setCF(CFPtr &cf) {
  this->curCF->setN(cf->getN());
  std::vector<double> ls = cf->getLS();
  std::vector<double> ss = cf->getSS();
  this->curCF->setLS(ls);
  this->curCF->setSS(ss);
}
// when need to clear the parent node, directly set the index = -1
void CFNode::clearParents() { this->parent->setIndex(-1); }
void CFNode::removeChild(NodePtr &child) {
  for (int i = 0; i < this->children.size(); i++) {
    if (this->children[i]->getIndex() == child->getIndex()) {
      this->children.erase(this->children.begin() + i);
    }
  }
}
bool SESAME::CFNode::getIsOutlier() { return this->isOutlier; }
void SESAME::CFNode::setIsOutlier(bool flag) { this->isOutlier = flag; }

ClusteringFeaturesTree::~ClusteringFeaturesTree() {}

ClusteringFeaturesTree::ClusteringFeaturesTree(
    const StreamClusteringParam &param)
    : dim(param.dimension), maxInternalNodes(param.maxInternalNodes),
      maxLeafNodes(param.maxLeafNodes),
      thresholdDistance(param.thresholdDistance),
      root(GenericFactory::create<Node>(param.dimension)) {}

void ClusteringFeaturesTree::insert(
    PointPtr point, std::vector<ClusteringFeaturesTree::NodePtr> &outliers) {
  auto curNode = root;
  if (curNode->cf.numPoints == 0) {
    // timerMeter.dataInsertAccMeasure();
    curNode->update(point, true);
    // timerMeter.dataInsertEndMeasure();
  } else {
    while (1) {
      auto childrenNode = curNode->children;
      if (curNode->isLeaf()) {
        // timerMeter.clusterUpdateAccMeasure();
        // timerMeter.dataInsertAccMeasure();
        auto centroid = curNode->centroid();
        // timerMeter.dataInsertEndMeasure();
        if (point->radius(centroid) <=
            thresholdDistance) { // concept drift detection
          // whether the new radius is lower than threshold T
          // timerMeter.dataInsertAccMeasure();
          curNode->update(point, true);
          // TODO: Outlier

          // timerMeter.dataInsertEndMeasure();

          // means this point could get included in this cluster
          // SESAME_DEBUG("No concept drift occurs(t <= T), insert tha point
          // into the leaf node...");
          break;
          // Normally insert the data point into the tree leafNode without
          // concept drift
        } else {
          // concept drift adaption
          // SESAME_DEBUG("Concept drift occurs(t > T), the current leaf node
          // capacity reaches the threshold T");
          // timerMeter.clusterUpdateAccMeasure();
          backwardEvolution(curNode, point);
          // timerMeter.clusterUpdateEndMeasure();
          break;
        }

      } else {
        // timerMeter.dataInsertAccMeasure();
        curNode = closestNode(childrenNode, point);
        // timerMeter.dataInsertEndMeasure();
      }
    }
  }
}

void ClusteringFeaturesTree::backwardEvolution(NodePtr node, PointPtr point) {
  if (node->parent == nullptr) { // means current node is root node
    // l <= L, create a new leaf node and insert the point into it(root
    // change)
    auto newRoot = GenericFactory::create<Node>(dim);
    newRoot->addChild(node);

    auto newNode = GenericFactory::create<Node>(dim, newRoot);
    newRoot->setCF(node->cf);
    newRoot->index = leafMask++;
    // here we need to remove the old root and add the new one into the
    // leafnodes set update the parent node
    newRoot->addChild(newNode);
    newNode->update(point, true);
    root = newRoot;
  } else {
    auto parent = node->parent;
    auto newNode = GenericFactory::create<Node>(dim, parent);
    newNode->update(point, false);
    if (parent->children.size() < maxLeafNodes) {
      // whether the number of CFs(clusters) in the current leaf node is
      // lower thant threshold L l <= L, create a new leaf node and insert
      // the point into it update the parent node and all nodes on the path
      // to root node
      parent->update(point, true);
    } else {
      // l > L, parent node of the current leaf node capacity reaches the
      // threshold L, split a new parent node from the old one
      bool nodeIsClus = true;
      while (true) {
        NodePtr parParent;
        if (parent->parent == nullptr) {
          // if the parent node is the root, we need to create a new root as
          // a parParent
          parParent = GenericFactory::create<Node>(dim);
          root = parParent;
          // since the parent node's nls has not been updated by the point,
          // so we directly copy the nls in parent node to the parParent one
          parParent->setCF(parent->cf);
        } else {
          // if the parent node is not the root, we can get the parParent
          // one directly
          parParent = parent->parent;
        }
        // we need to create a new parent node since the old one has to
        // split
        auto newParentA = GenericFactory::create<Node>(dim);
        // insert the new parent into the allNode list
        // we also need to insert the new parent node into the clusterNode
        // list if its children is a leaf node.
        if (parent->children[0]->isLeaf()) {
          newParentA->index = leafMask++;
        }
        // we only create a new parent rather and keep the old parent node
        // as the split two sub-nodes so we need to refresh the old parent
        // node as a blank one and treat it as a new parent B

        parent->parent = parParent; // link the parparent node and the new
                                    // created new parent A
        parParent->addChild(newParentA);
        // clean cf of the old parent node and initialize the cf of new
        // parent A (ls and ss all have d number of 0)
        // split the child nodes of the old parent nodes
        auto broNodes = parent->children;
        parent->children.clear();
        auto adjMatrix =
            calcAdjacencyMatrix(broNodes); //  calculate the distance between
                                           //  each two brother nodes
        // choose two farthest CFs as seedA and seedB
        int seedA = 0, seedB = 0;
        double maxDis = 0;
        for (int i = 0; i < broNodes.size(); i++) {
          for (int j = i; j < broNodes.size(); j++) {
            if (maxDis < adjMatrix[i][j]) {
              seedA = i;
              seedB = j;
              maxDis = adjMatrix[i][j];
            }
          }
        }
        // insert the child node into the nearest seed(A / B)
        for (auto node : broNodes) {
          node->clearParents();
        }
        // insert seedA node into new parent A and link them
        newParentA->addChild(broNodes[seedA]);
        newParentA->merge(broNodes[seedA]);
        // insert seed B node into new parent B and link them
        parent->addChild(broNodes[seedB]);
        parent->merge(broNodes[seedB]);
        // if other one brother node is near seed A then split it into new
        // parent A, otherwise new parent B.
        for (int i = 0; i < broNodes.size(); i++) {
          if (i != seedA && i != seedB) {
            if (adjMatrix[i][seedA] < adjMatrix[i][seedB]) {
              newParentA->addChild(broNodes[i]);
              newParentA->merge(
                  broNodes[i]); // since the brother nodes list contains the
                                // one we insert our point, so after this
                                // function, the parent node's nls are also
                                // updated.
              broNodes[i]->clearParents();
            } else {
              parent->addChild(broNodes[i]);
              parent->merge(broNodes[i]);
              broNodes[i]->clearParents();
            }
          }
        }
        // if the current node(parent) is a cluster nodes, then we need to
        // update the nls of its parent using new point. we only update the
        // parparent in the first loop.
        if (nodeIsClus) {
          parParent->update(point, true);
        }

        if (parParent->children.size() <= maxInternalNodes) {
          // b < B, remove the old node and insert the new nodeA and nodeB
          // into the parent node.
          break;
        } else {
          // b >= B, parent node of the current interior node capacity
          // reaches the threshold B.
          node = node->parent;
          parent = parParent;
          nodeIsClus = false;
        }
      }
    }
  }
}

std::vector<std::vector<double>> ClusteringFeaturesTree::calcAdjacencyMatrix(
    const std::vector<ClusteringFeaturesTree::NodePtr> &nodes) {
  int n = nodes.size();
  std::vector<std::vector<double>> adjacencyMatrix(n,
                                                   std::vector<double>(n, 0.0));
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      auto centroid1 = nodes[i]->centroid();
      auto centroid2 = nodes[j]->centroid();
      auto distance = centroid1->distance(centroid2);
      adjacencyMatrix[i][j] = distance;
      adjacencyMatrix[j][i] = distance;
    }
  }
  return adjacencyMatrix;
}

ClusteringFeaturesTree::NodePtr ClusteringFeaturesTree::closestNode(
    const std::vector<ClusteringFeaturesTree::NodePtr> &nodes, PointPtr point) {
  double minDistance = std::numeric_limits<double>::max();
  NodePtr closestNode = nullptr;
  for (auto child : nodes) {
    auto centroid = child->centroid();
    auto distance = centroid->distance(point);
    if (distance < minDistance) {
      minDistance = distance;
      closestNode = child;
    }
  }
  return closestNode;
}

ClusteringFeaturesList::ClusteringFeaturesList(
    const StreamClusteringParam &param)
    : dim(param.dimension), maxInternalNodes(param.maxInternalNodes),
      maxLeafNodes(param.maxLeafNodes),
      thresholdDistance(param.thresholdDistance) {}

ClusteringFeaturesList::~ClusteringFeaturesList() {}

void ClusteringFeaturesList::insert(
    PointPtr point, std::vector<ClusteringFeaturesList::NodePtr> &outliers) {
  if (nodes.empty()) {
    auto node = GenericFactory::create<Node>(dim);
    nodes.push_back(node);
    node->update(point);
    // TODO: outlier
  } else {
    auto node = closestNode(nodes, point);
    node->update(point);
    // TODO: outlier
  }
}

ClusteringFeaturesList::NodePtr ClusteringFeaturesList::closestNode(
    const std::vector<ClusteringFeaturesList::NodePtr> &nodes, PointPtr point) {
  double minDistance = std::numeric_limits<double>::max();
  NodePtr closestNode = nullptr;
  for (auto child : nodes) {
    auto centroid = child->centroid();
    auto distance = centroid->distance(point);
    if (distance < minDistance) {
      minDistance = distance;
      closestNode = child;
    }
  }
  return closestNode;
}

} // namespace SESAME
