//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_FEATUREVECTOR_H_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_FEATUREVECTOR_H_
#include <memory>
#include <vector>
#include <iostream>

namespace SESAME {
class CF;
typedef std::shared_ptr<CF> CFPtr;
class CF {
 private:
  // N是子类中节点的数目，LS是N个节点的线性和，SS是N个节点的平方和
  int NumberOfNodes;
  int index;
 public:
  std::vector<double> LS;
  std::vector<double> SS;
  CF();
  ~CF();
  int getN() const;
  void setN(int n);
  std::vector<double> getLS() const;
  std::vector<double> getSS() const;
  double getLSItem(int index) const;
  double getSSItem(int index) const;
  void setLS(std::vector<double> &newLs);
  void setSS(std::vector<double> &newSs);
  SESAME::CFPtr copy();
  int getIndex();
  void setIndex(int id);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_FEATUREVECTOR_H_
