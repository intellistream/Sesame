//
// Created by tuidan on 2022/3/11.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V4_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V4_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Algorithm/OfflineRefinement/KMeans.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME
{

class V4Parameter : public AlgorithmParameters
{
public:
    int max_in_nodes;           // B
    int max_leaf_nodes;         // L
    double distance_threshold;  // T
    int slidingCount;
    double outlier_distance_threshold;
    double outlier_cap;
};

class V4 : public Algorithm
{
public:
    V4Parameter V4Param;
    std::shared_ptr<KMeans> kmeans;  // used for offline initialization
    int leafMask = 0;
    SESAME::NodePtr root;
    vector<SESAME::NodePtr> clusterNodes;
    vector<NodePtr> outlierNodes;
    CFTreePtr cfTree;
    std::vector<SESAME::PointPtr> slidingWindowPoints;  // points stored in the sliding window
    std::vector<SESAME::NodePtr> SlidingWindowNodes;    // every point in the sliding window will
                                                        // insert into a corresponding node
    TimeMeter timerMeter;
    V4(param_t &cmd_params);

    ~V4();

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

    // additionally add a function to incrementally delete the first come point in the sliding
    // window rather than rebuild the whole tree
    void deletePointFromTree(SESAME::NodePtr &node, SESAME::PointPtr &point);

    bool checkoutOutlier(SESAME::PointPtr &point);
    void insertPointIntoOutliers(SESAME::PointPtr &point);
    void checkOutlierTransferCluster(SESAME::NodePtr &outCluster);
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V4_HPP_
