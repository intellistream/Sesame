//
// Created by tuidan on 2021/9/1.
//

#include <Algorithm/EDMStream.hpp>

SESAME::EDMStream::EDMStream(param_t &cmd_params) {
  this->EDMParam.pointNumber = cmd_params.pointNumber;
  this->EDMParam.dimension = cmd_params.dimension;
  this->EDMParam.a = cmd_params.a;
  this->EDMParam.lamda = cmd_params.lambda;
  this->EDMParam.beta = cmd_params.beta;
  this->EDMParam.cacheNum = cmd_params.cacheNum;
  this->EDMParam.radius = cmd_params.radius;
  this->EDMParam.minDelta = cmd_params.delta;
  this->EDMParam.opt = cmd_params.opt;
}
SESAME::EDMStream::~EDMStream(){};
void SESAME::EDMStream::Initilize() {
  this->alpha = 0;
  this->cache = SESAME::DataStructureFactory::creatCache(this->EDMParam.cacheNum, this->EDMParam.a,
                                                         this->EDMParam.lamda, this->EDMParam.radius);
  this->outres = SESAME::DataStructureFactory::createOutlierReservoir(this->EDMParam.radius,
                                                                      this->EDMParam.a, this->EDMParam.lamda);
  this->dpTree = SESAME::DataStructureFactory::createDPTree(this->actCluMaxNum,
                                                            this->EDMParam.radius);
  this->dpTree->SetMinDelta(this->EDMParam.minDelta);
}

void SESAME::EDMStream::setMinDelta(double minDelta) {
  this->EDMParam.minDelta = minDelta;
  this->dpTree->SetMinDelta(minDelta);
}

void SESAME::EDMStream::InitDP(double time) {
  cache->compDeltaRho(time);
  SESAME_DEBUG("beta = " << this->EDMParam.beta);
  this->minRho = this->EDMParam.beta / (1 - pow(this->EDMParam.a, this->EDMParam.lamda));
  SESAME_DEBUG("minRho = " << this->minRho);

  this->deltaT = (log(1 - pow(this->EDMParam.a, this->EDMParam.lamda)) / log(this->EDMParam.a) -
      log(this->EDMParam.beta) / log(this->EDMParam.a))/ this->EDMParam.lamda;
  //		double deltaT = 100;
  SESAME_DEBUG("deltaT = " << this->deltaT);
  outres->setTimeGap(this->deltaT);
  cache->getDPTree(this->minRho, this->EDMParam.minDelta, dpTree, outres, clusters);
  SESAME_DEBUG("dpTree size = " << dpTree->GetSize());
  dpTree->SetLastTime(time);
}

SESAME::DPNodePtr SESAME::EDMStream::streamProcess(SESAME::PointPtr p, int opt, double time) {
  timerMeter.dataInsertAccMeasure();
  double coef = pow(this->EDMParam.a, this->EDMParam.lamda * (time - dpTree->GetLastTime()));
  dpTree->SetLastTime(time);
  auto nn = dpTree->findNN(p, coef, opt, time);
  timerMeter.dataInsertEndMeasure();
  timerMeter.outlierDetectionAccMeasure();
  if (nn == nullptr || nn->GetDis() > dpTree->GetCluR()) {
    nn = outres->insert(p, time);
    if (nn->GetRho() > this->minRho) {
      outres->remove(nn);
      dpTree->insert(nn, opt);
    }
  }
  dpTree->deleteInact(outres, this->minRho, time);
  timerMeter.outlierDetectionEndMeasure();
  return nn;
}
double SESAME::EDMStream::computeAlpha() {
  return dpTree->computeAlpha(this->EDMParam.minDelta);
}
double SESAME::EDMStream::adjustMinDelta() {
  return dpTree->adjustMinDelta(this->alpha);
}
void SESAME::EDMStream::delCluster() {
  for(auto it = this->clusters.begin(); it != this->clusters.end();) {
    auto cluster = it->get();
    if(cluster->GetCells().begin() == cluster->GetCells().end()){
      this->clusters.erase(it++);
    } else {
      it++;
    }
  }
}

SESAME::DPNodePtr SESAME::EDMStream::retrive(SESAME::PointPtr p, int opt, double time) {
  SESAME::PointPtr curP = p;
  if (!this->EDMParam.isInit) {
    timerMeter.initialMeasure();
    auto cc = cache->add(curP, time);
    if (cache->isFull()) {
      // draw decision graph
      InitDP(time);
      this->alpha = computeAlpha(); //TODO: what does it mean?
      SESAME_DEBUG("alpha = " << this->alpha);
      this->EDMParam.isInit = true;
    timerMeter.initialEndMeasure();
    }
    return cc;
  } else {
    auto nn = streamProcess(curP, opt, time);
    timerMeter.clusterUpdateAccMeasure();
    this->dpTree->adjustCluster(clusters);
    delCluster();
    timerMeter.clusterUpdateEndMeasure();
    return nn;
  }
}

void CountNode(const SESAME::DPNodePtr &node, int &num) {
  num = num + 1;
  if(!node->GetSucs().empty()) {
    for(const SESAME::DPNodePtr &el : node->GetSucs()) {
      CountNode(el, num);
    }
  }
}
void SESAME::EDMStream::runOnlineClustering(SESAME::PointPtr input) {
  double curTime = input->getTimeStamp() / 100000;
  auto c = retrive(input, this->EDMParam.opt, curTime);
  if(input->getIndex() % 100 == 0 && this->EDMParam.isInit) {
    timerMeter.clusterUpdateAccMeasure();
    setMinDelta(adjustMinDelta());
    this->dpTree->adjustCluster(this->clusters);
    this->delCluster();
    timerMeter.clusterUpdateEndMeasure();
  }
}
void SESAME::EDMStream::runOfflineClustering(SESAME::DataSinkPtr sinkPtr) {
  timerMeter.printTime(true,false,true,false);
  SESAME_DEBUG("Cluster number: "<< this->clusters.size());
  int i = 0;
  int num = 0;
  int sum = 0;
  for(auto it = this->clusters.begin(); it != this->clusters.end(); ++it) {
    i++;
    sum += num;
    num = 0;
    std::unordered_set<DPNodePtr> cells = it->get()->GetCells();
    for(auto cell = cells.begin(); cell != cells.end(); ++cell) {
      CountNode(cell->get()->copy(), num);
      PointPtr center = cell->get()->GetCenter();
      sinkPtr->put(center->copy());
    }
  }
  for(auto out = this->outres->GetOutliers().begin(); out != this->outres->GetOutliers().end(); ++ out) {
      i++;
      sum += num;
      num = 0;
      CountNode(out->get()->copy(), num);
   }
}

