//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
#define SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Algorithm/OfflineRefinement/KMeans.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>
#include "Algorithm/DesignAspect/Generic.hpp"
namespace SESAME
{
class Benne : public Algorithm
{
public:
    std::shared_ptr<KMeans> kmeans;  // used for offline initialization
    int leafMask = 0;
    NodePtr root;
    vector<NodePtr> leafNodes;
    CFTreePtr cfTree;
    Benne(param_t &cmd_params);

    ~Benne();

    void Init() override;

    void RunOnline(PointPtr input) override;

    void RunOffline(DataSinkPtr sinkPtr) override;

private:
    void autoDetection(PointPtr point);
    void autoSelection(NodePtr &curNode, PointPtr &point);
    void reconfiguration(NodePtr &curNode, PointPtr &point);
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
