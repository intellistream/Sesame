//
// Created by tuidan on 2021/9/1.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
#include <iostream>
#include <memory>
#include <vector>
#include <Algorithm/DataStructure/FeatureVector.hpp>
#include <Algorithm/DataStructure/Point.hpp>
namespace SESAME {

// define the share point of the class object
class DPNode;
class Cluster;
class DPTree;
class OutlierReservoir;
typedef std::shared_ptr<DPNode> DPNodePtr;
typedef std::shared_ptr<DPTree> DPTreePtr;
typedef std::shared_ptr<Cluster> ClusterPtr;
typedef std::shared_ptr<OutlierReservoir> ORPtr;

class DPNode {
 private:
  int id;
  int num;
  double rho;
  double delta;
  SESAME::DPNodePtr dep; //TODO: father
  SESAME::PointPtr center;
  long lastTime;
  bool active;
  std::vector<SESAME::DPNodePtr> sucs; //TODO: children
  SESAME::ClusterPtr cluster;
  // public double sumDelta;
  // public int sucNum;
  long inactiveTime;

  /**
   * we will use dis to quickly update the delta of CluCell
   */
  double dis;
 public:
  DPNode();
  ~DPNode();
  DPNode(SESAME::PointPtr p, int index, long time);
  [[nodiscard]] int GetId() const;
  void SetId(int id);
  [[nodiscard]] int GetNum() const;
  void SetNum(int num);
  [[nodiscard]] double GetRho() const;
  void SetRho(double rho);
  [[nodiscard]] double GetDelta() const;
  void SetDelta(double delta);
  [[nodiscard]] const DPNodePtr &GetDep() const;
  void SetDep(const DPNodePtr &dep);
  [[nodiscard]] const PointPtr &GetCenter() const;
  void SetCenter(const PointPtr &center);
  [[nodiscard]] long GetLastTime() const;
  void SetLastTime(long last_time);
  [[nodiscard]] bool IsActive() const;
  void SetActive(bool active);
  [[nodiscard]] const std::vector<SESAME::DPNodePtr> &GetSucs() const;
  void SetSucs(const std::vector<SESAME::DPNodePtr> &sucs);
  [[nodiscard]] const ClusterPtr &GetCluster() const;
  void SetCluster(const SESAME::ClusterPtr &cluster);
  [[nodiscard]] long GetInactiveTime() const;
  void SetInactiveTime(long inactive_time);
  [[nodiscard]] double GetDis() const;
  void SetDis(double dis);


  void insert(long startTime);
  void add(double coef, long startTime);
  double getDisTo(SESAME::DPNodePtr &node);
  void removeSuccessor(SESAME::DPNodePtr &node);
  bool hasSuccessor();

  void addSuccessor(DPNodePtr &node);
};

class Cluster {
 private:
  int label;
  std::vector<DPNodePtr> cells;
 public:
  explicit Cluster(int label);
  void add(DPNodePtr &node);
  void remove(DPNodePtr &node);
};

class OutlierReservoir {
 private:
  double r;
  double timeGap;
  long lastDelTime;

  double a;
  double lamd;

  std::vector<DPNodePtr> outliers;

 public:
  [[nodiscard]] double GetR() const;
  void SetR(double r);
  [[nodiscard]] double GetTimeGap() const;
  void SetTimeGap(double time_gap);
  [[nodiscard]] long GetLastDelTime() const;
  void SetLastDelTime(long last_del_time);
  [[nodiscard]] double GetA() const;
  void SetA(double a);
  [[nodiscard]] double GetLamd() const;
  void SetLamd(double lamd);
  [[nodiscard]] const std::vector<SESAME::DPNodePtr> &GetOutliers() const;
  void SetOutliers(const std::vector<SESAME::DPNodePtr> &outliers);

  OutlierReservoir(double r, double a, double lamd);

  void setTimeGap(double timeGap);

  void insert(SESAME::DPNodePtr c);

  SESAME::DPNodePtr insert(SESAME::PointPtr p, int curNodeNum, double time);

  void remove(SESAME::DPNodePtr nn);
};

class DPTree {
 private:
  long lastTime;
  int size;
  int num;
  std::vector<SESAME::DPNodePtr> Clus;

  double a;
  double lamd;
  double CluR;

  int cluLabel;
  double minDelta;
 public:
  DPTree();
  ~DPTree();
  DPTree(int num, double CluR);
  void insert(SESAME::DPNodePtr cc, double minRho, Point p, int opt);
  void init(std::vector<SESAME::DPNodePtr> clus, int size, double minRho, double minDelta,
            OutlierReservoir outs, std::vector<Cluster> clusters);
  SESAME::DPNodePtr  findNN(Point p, double coef,
                       std::vector<Cluster> clusters, double minRho, int opt);
  void adjustNoDelta(int index, double minRho, Point p);
  void adjustNoOpt(int index, double minRho, Point p);
  void computeDeltaNoOpt(int index);
  void adjustOpt1(int index, double minRho, Point p);
  void computeDeltaF1(int index);
  void adjust(int index, double minRho, Point p);
  void computeHeadDelta();
  void computeDelta(int index);
  void deleteInact(OutlierReservoir outres, double coef,
                         double minRho, long time);
  double computeAlpha(double minDelta);
  double adjustMinDelta(double alpha);
  double djustMinDelta(double alpha, double minDelta);
  double fun(double alpha, double upavg, double downavg, double avg);
  bool check(OutlierReservoir outres);
  void check(std::vector<Cluster> clusters);
  void adjustCluster(std::vector<Cluster> clusters, bool minChed);
  void writeInfo(std::string string);


};




}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
