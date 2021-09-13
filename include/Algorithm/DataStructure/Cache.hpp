//
// Created by tuidan on 2021/9/9.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CACHE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CACHE_HPP_
#include <Algorithm/DataStructure/DPNode.hpp>
#include <Algorithm/DataStructure/OutlierReservoir.hpp>
#include <Algorithm/DataStructure/DPTree.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_set>

namespace SESAME {

// define the share point of the class object
class Cache;
typedef std::shared_ptr<Cache> CachePtr;

class Cache {
 private:
  int num;
  int size;
  double a;
  double lamd;
  double r;
  std::vector<DPNodePtr> buffer;
  std::vector<DPNodePtr> clus;
  int pnum{};
 public:
  Cache();
  ~Cache();
  int GetNum() const;
  void SetNum(int num);
  int GetSize() const;
  void SetSize(int size);
  double GetA() const;
  void SetA(double a);
  double GetLamd() const;
  void SetLamd(double lamd);
  double GetR() const;
  void SetR(double r);
  const std::vector<DPNodePtr> &GetBuffer() const;
  void SetBuffer(const std::vector<DPNodePtr> &buffer);
  const std::vector<DPNodePtr> &GetClus() const;
  void SetClus(const std::vector<DPNodePtr> &clus);
  int GetPnum() const;
  void SetPnum(int pnum);
  Cache(int num, double a, double lamd, double r);
  DPNodePtr add(PointPtr &p, double startTime);
  bool isFull();
  void compDeltaRho(double time);
  void getDPTree(double minRho, double minDelta, DPTreePtr &dpTree,
                 OutPtr &outs, std::unordered_set<ClusterPtr> &clusters);
  //void outputBuffer(String outpath, double minRho)
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CACHE_HPP_
