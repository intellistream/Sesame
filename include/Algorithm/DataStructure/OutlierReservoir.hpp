//
// Created by tuidan on 2021/9/9.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_OUTLIERRESERVOIR_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_OUTLIERRESERVOIR_HPP_
#include <Algorithm/DataStructure/DPNode.hpp>
#include <vector>
#include <iostream>
#include <memory>

namespace SESAME {

class OutlierReservoir;
typedef std::shared_ptr<OutlierReservoir> OutPtr;
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
  OutlierReservoir();
  ~OutlierReservoir();
  OutlierReservoir(double r, double a, double lamd);

  void setTimeGap(double timeGap);

  void insert(SESAME::DPNodePtr c);

  SESAME::DPNodePtr insert(SESAME::PointPtr p, double time);

  void remove(SESAME::DPNodePtr nn);
};

}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_OUTLIERRESERVOIR_HPP_
