//
// Created by tuidan on 2021/9/1.
//

#include <Algorithm/DesignAspect//V10.hpp>

SESAME::V10::V10(param_t &cmd_params) {
  this->param = cmd_params;
  this->V10Param.landmark = cmd_params.landmark;
  this->V10Param.num_points = cmd_params.num_points;
  this->V10Param.dim = cmd_params.dim;
  this->V10Param.alpha = cmd_params.alpha;
  this->V10Param.lamda = cmd_params.lambda;
  this->V10Param.beta = cmd_params.beta;
  this->V10Param.num_cache = cmd_params.num_cache;
  this->V10Param.radius = cmd_params.radius;
  this->V10Param.minDelta = cmd_params.delta;
  this->V10Param.opt = cmd_params.opt;
}

SESAME::V10::~V10(){};

void SESAME::V10::Init() {
  this->alpha = 0;
  this->cache = SESAME::DataStructureFactory::creatCache(this->V10Param.num_cache, this->V10Param.alpha,
                                                         this->V10Param.lamda, this->V10Param.radius);
  this->outres = SESAME::DataStructureFactory::createOutlierReservoir(this->V10Param.radius,
                                                                      this->V10Param.alpha, this->V10Param.lamda);
  this->dpTree = SESAME::DataStructureFactory::createDPTree(this->actCluMaxNum,
                                                            this->V10Param.radius);
  this->dpTree->SetMinDelta(this->V10Param.minDelta);
  sum_timer.Tick();
}

void SESAME::V10::setMinDelta(double minDelta) {
  this->V10Param.minDelta = minDelta;
  this->dpTree->SetMinDelta(minDelta);
}

void SESAME::V10::InitDP(double time) {
  cache->compDeltaRho(time);
  SESAME_DEBUG("beta = " << this->V10Param.beta);
  this->minRho = this->V10Param.beta / (1 - pow(this->V10Param.alpha, this->V10Param.lamda));
  SESAME_DEBUG("minRho = " << this->minRho);

  this->deltaT = (log(1 - pow(this->V10Param.alpha, this->V10Param.lamda)) / log(this->V10Param.alpha) -
      log(this->V10Param.beta) / log(this->V10Param.alpha))/ this->V10Param.lamda;
  //		double deltaT = 100;
  SESAME_DEBUG("deltaT = " << this->deltaT);
  outres->setTimeGap(this->deltaT);
  cache->getDPTree(this->minRho, this->V10Param.minDelta, dpTree, outres, clusters);
  SESAME_DEBUG("dpTree size = " << dpTree->GetSize());
  dpTree->SetLastTime(time);
}

SESAME::DPNodePtr SESAME::V10::streamProcess(SESAME::PointPtr p, int opt, double time) {
  ds_timer.Tick();
  double coef = pow(this->V10Param.alpha, this->V10Param.lamda * (time - dpTree->GetLastTime()));
  dpTree->SetLastTime(time);
  auto nn = dpTree->findNN(p, coef, opt, time);
  ds_timer.Tock();
  out_timer.Tick();
  if (nn == nullptr || nn->GetDis() > dpTree->GetCluR()) {
    nn = outres->insert(p, time);
    if (nn->GetRho() > this->minRho) {
      outres->remove(nn);
      dpTree->insert(nn, opt);
    }
  }
  dpTree->deleteInact(outres, this->minRho, time);
  out_timer.Tock();
  return nn;
}
double SESAME::V10::computeAlpha() {
  return dpTree->computeAlpha(this->V10Param.minDelta);
}
double SESAME::V10::adjustMinDelta() {
  return dpTree->adjustMinDelta(this->alpha);
}
void SESAME::V10::delCluster() {
  for(auto it = this->clusters.begin(); it != this->clusters.end();) {
    auto cluster = it->get();
    if(cluster->GetCells().begin() == cluster->GetCells().end()){
      this->clusters.erase(it++);
    } else {
      it++;
    }
  }
}

SESAME::DPNodePtr SESAME::V10::retrive(SESAME::PointPtr p, int opt, double time) {
  SESAME::PointPtr curP = p;
  if (!this->V10Param.isInit) {
    auto cc = cache->add(curP, time);
    if (cache->isFull()) {
      // draw decision graph
      InitDP(time);
      this->alpha = computeAlpha(); //TODO: what does it mean?
      SESAME_DEBUG("alpha = " << this->alpha);
      this->V10Param.isInit = true;
    }
    return cc;
  } else {
    ds_timer.Tick();
    auto nn = streamProcess(curP, opt, time);
    this->dpTree->adjustCluster(clusters);
    ds_timer.Tock();

    out_timer.Tick();
    delCluster();
    out_timer.Tock();
    return nn;
  }
}

void SESAME::V10::CountNode(const SESAME::DPNodePtr &node, int &num) {
  num = num + 1;
  if(!node->GetSucs().empty()) {
    for(const SESAME::DPNodePtr &el : node->GetSucs()) {
      CountNode(el, num);
    }
  }
}
void SESAME::V10::RunOnline(SESAME::PointPtr input) {
  if(input->getIndex() >= this->V10Param.landmark){
    double curTime = input->getTimeStamp() / 100000;
    auto c = retrive(input, this->V10Param.opt, curTime);
    if(input->getIndex() % 100 == 0 && this->V10Param.isInit) {
      ds_timer.Tick();
      setMinDelta(adjustMinDelta());
      this->dpTree->adjustCluster(this->clusters);
      ds_timer.Tock();

      out_timer.Tick();
      this->delCluster();
      out_timer.Tock();
    }
  }
  lat_timer.Add(input->toa);
}
void SESAME::V10::RunOffline(SESAME::DataSinkPtr sinkPtr) {
  ref_timer.Tick();
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
      center->setOutlier(false);
      sinkPtr->put(center->copy());
    }
  }
  for(auto out = this->outres->getOutliers().begin(); out != this->outres->getOutliers().end(); ++ out) {
    i++;
    sum += num;
    num = 0;
    CountNode(out->get()->copy(), num);
    PointPtr center = out->get()->GetCenter();
    center->setOutlier(true);
    sinkPtr->put(center->copy());
  }
  ref_timer.Tock();
  sum_timer.Tock();
}

