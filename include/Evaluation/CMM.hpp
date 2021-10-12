//
// Created by tuidan on 2021/10/11.
//

#ifndef SESAME_INCLUDE_EVALUATION_CMM_HPP_
#define SESAME_INCLUDE_EVALUATION_CMM_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <map>

namespace SESAME {

enum ClusterType {
  Cluster, GTCluster
};
enum ClusterMapType {

  vote, similarity

};

class CMMPoint;
class CMMCluster;
class CMM;

typedef std::shared_ptr<CMMPoint> CMMPointPtr;
typedef std::shared_ptr<CMMCluster> CMMClusterPtr;

class CMMPoint {

 public:
  int id;
  long startTime;
  int dim;
  std::vector<double> vec;
  double weight;
  std::string truth;
  double conCL;
  double con;

  CMMPoint(int id, long startTime,long time, std::vector<double> &vec,
           double a, double lambda, std::string &truth);

  double getDisTo(CMMPointPtr &p);

  double knnDis(int k, CMMCluster &c);

};

class CMMCluster {
 public:
  std::vector<CMMPointPtr> points;
  std::string groundTruth;
  std::vector<int> rho;
  double knhDis{};
  ClusterType type;
  CMMCluster();

  void add(CMMPointPtr &p);

  void getDistribution(std::map<std::string, int> &map);

  void getConn(int k);
};

class CMM {
 private:
 public:
  static void CMMCost(int numberOfPoints,
                      int numberOfCenters,
                      int dimention,
                      const std::vector<PointPtr> &inputs,
                      const std::vector<PointPtr> &results);
};

}
#endif //SESAME_INCLUDE_EVALUATION_CMM_HPP_
