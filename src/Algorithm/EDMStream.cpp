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
  // cache.drawDecision(bufferPath, decisionPath);
  // scan = new Scanner(System.in);
  //SESAME_DEBUG("beta=" << this->EDMParam.beta);
  this->minRho = this->EDMParam.beta / (1 - pow(this->EDMParam.a, this->EDMParam.lamda));
  //SESAME_DEBUG("minRho=" << this->minRho);

  this->deltaT = (log(1 - pow(this->EDMParam.a, this->EDMParam.lamda)) / log(this->EDMParam.a) -
      log(this->EDMParam.beta) / log(this->EDMParam.a))/ this->EDMParam.lamda;
  //		double deltaT = 100;
  //SESAME_DEBUG("deltaT=" << this->deltaT);
  outres->setTimeGap(this->deltaT);
  cache->getDPTree(this->minRho, this->EDMParam.minDelta, dpTree, outres, clusters);
  //SESAME_DEBUG("dpTree size = " << dpTree->GetSize());
  dpTree->SetLastTime(time);
}

SESAME::DPNodePtr SESAME::EDMStream::streamProcess(SESAME::PointPtr p, int opt, double time) {
  double coef = pow(this->EDMParam.a, this->EDMParam.lamda * (time - dpTree->GetLastTime()));
  dpTree->SetLastTime(time);
  auto nn = dpTree->findNN(p, coef, opt, time);
  if (nn == nullptr || nn->GetDis() > dpTree->GetCluR()) {

    nn = outres->insert(p, time);
    if (nn->GetRho() > this->minRho) {
//				System.out.println("change");
      outres->remove(nn);
//				System.out.println("merged by o_micro:" + nn.dis);
      dpTree->insert(nn, opt);
    }
  }
//		else{
//			System.out.println("merged by p_micro:" + nn.dis);
//		}

  dpTree->deleteInact(outres, this->minRho, time);
//		dpTree.adjustCluster(clusters);
//		delCluster();
  return nn;
}
double SESAME::EDMStream::computeAlpha() {
  return dpTree->computeAlpha(this->EDMParam.minDelta);
}
double SESAME::EDMStream::adjustMinDelta() {
  return dpTree->adjustMinDelta(this->alpha);
}
void SESAME::EDMStream::delCluster() {
  for(int i = 0; i < this->clusters.size(); i++) {
    if(clusters[i]->GetCells().empty()) {
      this->clusters.erase(this->clusters.begin(), this->clusters.end());
    }
  }
}

SESAME::DPNodePtr SESAME::EDMStream::retrive(SESAME::PointPtr p, int opt, double time) {
  SESAME::PointPtr curP = p;
  if (!this->EDMParam.isInit) {
    auto cc = cache->add(curP, time);
    if (cache->isFull()) {
      // draw decision graph
      InitDP(time);
      this->alpha = computeAlpha(); //TODO: what does it mean?
      //SESAME_DEBUG("alpha=" << this->alpha);
      this->EDMParam.isInit = true;
    }
    return cc;
  } else {
    auto nn = streamProcess(curP, opt, time);

    this->dpTree->adjustCluster(clusters);
    delCluster();
    return nn;
  }
}

void SESAME::EDMStream::runOnlineClustering(SESAME::PointPtr input) {
  double curTime = (input->getIndex() + 1) / 10;
  auto c = retrive(input, this->EDMParam.opt, curTime);
  if(input->getIndex() % 100 == 0 && this->EDMParam.isInit) {
    setMinDelta(adjustMinDelta());
    this->dpTree->adjustCluster(this->clusters);
    this->delCluster();
  }
  if(input->getIndex() % (this->EDMParam.pointNumber -1) == 0 && input->getIndex() != 0) {
    SESAME_DEBUG(input->getDimension());
  }
}
void SESAME::EDMStream::runOfflineClustering(SESAME::DataSinkPtr sinkPtr) {
  for(auto & cluster : this->clusters) {
    std::vector<DPNodePtr> cells = cluster->GetCells();
    for(auto & cell : cells) {
      PointPtr center = cell->GetCenter();
      sinkPtr->put(center->copy());
    }
  }
  //SESAME_DEBUG( "The size of the centroid is :" << sinkPtr->getResults().size());
}

