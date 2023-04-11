//
// Created by tuidan on 2022/3/10.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V3_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V3_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Algorithm/OfflineRefinement/KMeans.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME
{

class V3Parameter : public AlgorithmParameters
{
public:
    int max_in_nodes;           // B
    int max_leaf_nodes;         // L
    double distance_threshold;  // T
    int landmark;
    double outlier_distance_threshold;
    double outlier_cap;
};

class V3 : public Algorithm
{
public:
    V3Parameter V3Param;
    std::shared_ptr<KMeans> kmeans;  // used for offline initialization
    int leafMask = 0;
    NodePtr root;
    vector<NodePtr> clusterNodes;
    vector<NodePtr> outlierNodes;
    CFTreePtr cfTree;
    TimeMeter timerMeter;
    V3(param_t &cmd_params);

    ~V3();

    void Init() override;

    void RunOnline(PointPtr input) override;

    void RunOffline(DataSinkPtr sinkPtr) override;

private:
    void forwardInsert(PointPtr point);
    void backwardEvolution(NodePtr &curNode, PointPtr &point, SESAME::NodePtr &cluster);
    void calculateCorDistance(vector<vector<double>> &distance, vector<NodePtr> &nodes);
    double calculateRadius(PointPtr &point, PointPtr &centroid);
    void selectChild(vector<NodePtr> &children, PointPtr &insertPoint, NodePtr &node);
    double clusterToClusterDist(NodePtr &nodeA, NodePtr &nodeB);
    void pointToClusterDist(PointPtr &insertPoint, NodePtr &node, double &dist);
    void calculateCentroid(CFPtr &cf, PointPtr &centroid);
    void updateNLS(NodePtr &node, PointPtr &point, bool updateAll);
    void initializeCF(CFPtr &cf, int dim);
    void setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point);
    void addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent, bool updateAll);
    void clearChildParents(vector<SESAME::NodePtr> &children);

    bool checkoutOutlier(SESAME::PointPtr &point);
    void insertPointIntoOutliers(SESAME::PointPtr &point);
    void checkOutlierTransferCluster(SESAME::NodePtr &outCluster);
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V3_HPP_
