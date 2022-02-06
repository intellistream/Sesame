//
// Created by tuidan on 2021/10/11.
//

#ifndef SESAME_INCLUDE_EVALUATION_CMM_HPP_
#define SESAME_INCLUDE_EVALUATION_CMM_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <unordered_map>
#include <unordered_set>

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
  int truth;
  double conCL;
  double con;

  CMMPoint(int id, long startTime, long time, std::vector<double> &vec,
           double a, double lambda, int truth);

  double getDisTo(CMMPointPtr &p);

  double knnDis(int k, CMMCluster &c);

};

class CMMCluster {
 public:
  std::vector<CMMPointPtr> points;
  int groundTruth;
  std::vector<int> rho;
  double knhDis{};
  ClusterType type;
  CMMCluster();

  void add(CMMPointPtr &p);

  // void getDistribution(std::unordered_map<std::string, int> &map);

  void getConn();
};

class CMMDriver {
 public:
  double a;
  double lambda;
  int dim;
  int groundTruth;

  std::vector<CMMPointPtr> points;
  std::unordered_map<int, CMMClusterPtr> CL;
  std::vector<CMMClusterPtr> CLlist;
  std::unordered_map<int, CMMClusterPtr> C;
  std::unordered_map<std::string, std::string> CToCL;
  std::vector<CMMClusterPtr> Clist;
  std::vector<CMMPointPtr> faultSet;
  std::unordered_set<CMMClusterPtr> faultClu;

  CMMDriver(int dim, double a, double lambda);
  void load(const std::vector<PointPtr> &input, const std::vector<PointPtr> &center,
            int dimension, double weight);
  void voteMap();
  double computeWeight(double deltaTime);
  // int getDelta(CMMClusterPtr ci, CMMClusterPtr cljo);
  void getFaultSet();
  double compCMM();
  void compCon();

};
class CMM {
 private:
 public:
  static double CMMCost(int dimension,
                      const std::vector<PointPtr> &inputs,
                      const std::vector<PointPtr> &center);
};

}
#endif //SESAME_INCLUDE_EVALUATION_CMM_HPP_
