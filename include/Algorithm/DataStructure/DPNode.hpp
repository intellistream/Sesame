//
// Created by tuidan on 2021/9/9.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPNODE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPNODE_HPP_
#include <vector>
#include <iostream>
#include <memory>
#include <Algorithm/DataStructure/Point.hpp>
static int id = 0;
namespace SESAME {
class DPNode;
class Cluster;
typedef std::shared_ptr<DPNode> DPNodePtr;
typedef std::shared_ptr<Cluster> ClusterPtr;

class Cluster {
 private:
  int label;
  std::vector<DPNodePtr> cells;

 public:
  explicit Cluster(int label);
  void add(DPNodePtr &node);
  void remove(DPNodePtr &node);
  int GetLabel() const;
  void SetLabel(int label);
  const std::vector<DPNodePtr> &GetCells() const;
  void SetCells(const std::vector<DPNodePtr> &cells);
};
class DPNode {
 private:
  int cid;
  int Cid;
  int num;
  double rho;
  double delta;
  SESAME::DPNodePtr dep; //TODO: father
  SESAME::PointPtr center;
  double lastTime;
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
  DPNode(SESAME::PointPtr p, double time);
  [[nodiscard]] int GetId() const;
  void SetId(int id);
  [[nodiscard]] int GetCId() const;
  void SetCId(int Cid);
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
  [[nodiscard]] double GetLastTime() const;
  void SetLastTime(double last_time);
  [[nodiscard]] bool IsActive() const;
  void SetActive(bool active);
  [[nodiscard]] const std::vector<SESAME::DPNodePtr> &GetSucs() const;
  void SetSucs(const std::vector<SESAME::DPNodePtr> &sucs);
  [[nodiscard]] const ClusterPtr &GetCluster() const;
  void SetCluster(const SESAME::ClusterPtr &cluster);
  [[nodiscard]] double GetInactiveTime() const;
  void SetInactiveTime(double inactive_time);
  [[nodiscard]] double GetDis() const;
  void SetDis(double dis);
  SESAME::DPNodePtr copy();


  void insert(double startTime);
  void add(double coef, double startTime);
  double getDisTo(SESAME::DPNodePtr &node);
  void removeSuccessor(SESAME::DPNodePtr &node);
  bool hasSuccessor();

  void addSuccessor(DPNodePtr &node);
};

}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPNODE_HPP_
