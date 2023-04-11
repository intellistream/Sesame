//
// Created by tuidan on 2022/3/10.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V2_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V2_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Algorithm/DesignAspect/Param.hpp>
#include <Algorithm/OfflineRefinement/DBSCAN.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>

namespace SESAME
{

class V2 : public Algorithm
{
public:
    StreamClusteringParam V2Param;
    int leafMask = 0;
    NodePtr root;
    std::shared_ptr<DBSCAN> dbscan;  // used for initialization and offline re-clustering
    vector<NodePtr> clusterNodes;
    vector<NodePtr> outlierNodes;
    CFTreePtr cfTree;
    TimeMeter timerMeter;
    V2(param_t &cmd_params);

    ~V2();

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
#endif  // SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V2_HPP_
