//
// Created by tuidan on 2021/10/11.
//

#include <Evaluation/CMM.hpp>
#include <cmath>

/**
 * @Description: CMMPoint
 */
SESAME::CMMPoint::CMMPoint(int id, long startTime, long time,
                           std::vector<double> &vec, double a,
                           double lambda, std::string &truth) {
  this->id = id;
  this->startTime = startTime;
  this->dim = (int)vec.size();
  this->vec = vec;
  this->weight = pow(a, lambda*(time - startTime));
  this->truth = truth;
}

double SESAME::CMMPoint::getDisTo(CMMPointPtr &p) {
  double dis = 0;
  double temp = 0;
  for (int i = 0; i < dim; i++) {
    temp = p->vec[i] - vec[i];
    dis += temp * temp;
  }
  return sqrt(dis);
}

double SESAME::CMMPoint::knnDis(int k, SESAME::CMMCluster &c) {
  std::vector<CMMPointPtr> list = c.points;
  int size = c.points.size();
  std::vector<double> diss(size);
  for(int i = 0; i < size; i++){
    CMMPointPtr p = list.at(i);
    if(list.at(i)->id != this->id){
      diss[i] = getDisTo(list.at(i));
    }
  }
  sort(diss.begin(), diss.end());
  double sum = 0;
  int num = 0;
  for(int i = 0; i < k && i < size; i++){
    sum += diss[i];
    num++;
  }
  return sum/num;
}


/**
 * @Description: CMMCluster
 */
SESAME::CMMCluster::CMMCluster() {}

void SESAME::CMMCluster::add(SESAME::CMMPointPtr &p) {
  this->points.push_back(p);
}

void SESAME::CMMCluster::getDistribution(std::map<std::string, int> &map) {
  std::vector<int> blank(map.size());
  rho = blank;
  int size = (int)points.size();
  for (int i = 0; i < size; i++) {
    rho[map[points.at(i)->truth]]++;
  }
}

void SESAME::CMMCluster::getConn(int k) {
  int size = (int)points.size();
//		System.out.println(size);
  std::vector<double> knhPDis(size);
  double sum = 0;
  for (int i = 0; i < size; i++) {
//			System.out.println(i);
    CMMPointPtr p = points.at(i);
    knhPDis[i] = p->knnDis(k, *this);
    sum += knhPDis[i];
  }
  knhDis = sum/size;
  for(int i = 0; i < size; i++){
//			System.out.println(i);
    if(type == SESAME::GTCluster){
      if(knhPDis[i] < knhDis){
        points.at(i)->conCL = 1;
      }else{
        if(points.size() == 1){
          points.at(i)->conCL = 1;
        }else{
          points.at(i)->conCL = knhDis/knhPDis[i];
        }
      }
    }else{
      if(knhPDis[i] < knhDis){
        points.at(i)->con = 1;
      }else{
        points.at(i)->con = knhDis/knhPDis[i];
      }
    }
  }
}

