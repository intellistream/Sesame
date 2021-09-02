// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by 1124a on 2021/8/23.
//
#include <Algorithm/DenStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
SESAME::DenStream::DenStream(param_t &cmd_params) {
  this->denStreamParams.pointNumber = cmd_params.pointNumber;
  this->denStreamParams.dimension = cmd_params.dimension;
  this->denStreamParams.minPoints = cmd_params.minPoints;
  this->denStreamParams.epsilon = cmd_params.epsilon;
  this->denStreamParams.base = cmd_params.base;
  this->denStreamParams.lambda = cmd_params.lambda;
  this->denStreamParams.mu = cmd_params.mu;
  this->denStreamParams.beta = cmd_params.beta;
  this->denStreamParams.initBufferSize = cmd_params.initBuffer;

}
SESAME::DenStream::~DenStream() {

}
void SESAME::DenStream::init(vector<PointPtr> &initData) {
  SESAME_INFO("tp is " << this->Tp);
  this->pMicroClusterIndex = -1;
  this->oMicroClusterIndex = -1;
  for (int i = 0; i < denStreamParams.initBufferSize; i++) {
    if (initData.at(i)->getClusteringCenter() == noVisited) {
      std::vector<int> pointIndex;
      pMicroClusterIndex++;
      MicroClusterPtr newMicroCluster =
          SESAME::DataStructureFactory::createMicroCluster(denStreamParams.dimension, pMicroClusterIndex);
      newMicroCluster->init(initData.at(i), 0);
      pointsNearCorePoint(initData, pointIndex, newMicroCluster);
      if (newMicroCluster->weight <= this->minWeight)//TODO need to change minweight
      {
        pMicroClusterIndex--;
        for (vector<int>::size_type a = 0; a < pointIndex.size(); a++) {
          int index = pointIndex.at(a);
          initData.at(index)->setClusteringCenter(noVisited);
        }
      } else
        pMicroClusters.push_back(newMicroCluster);
    }
  }
  SESAME_INFO("NOW PMC number is: " << this->pMicroClusterIndex << " , Init succeed!");
}
void SESAME::DenStream::pointsNearCorePoint(vector<PointPtr> &initData,
                                            std::vector<int> pointIndex,
                                            MicroClusterPtr microCluster) {
  int size = denStreamParams.initBufferSize;
  for (int i = 0; i < size; i++) {
    if (initData.at(i)->getClusteringCenter() == noVisited) {
      double dist = microCluster->calCentroidDistance(initData.at(i));
      if (dist < denStreamParams.epsilon) {
        initData[i]->setClusteringCenter(microCluster->id.front());
        microCluster->insert(initData.at(i), 0);
        pointIndex.push_back(i);
      }
    }
  }
}
void SESAME::DenStream::Initilize() {
  this->dampedWindow = WindowFactory::createDampedWindow(denStreamParams.base, denStreamParams.lambda);
  this->dbscan =
      std::make_shared<DBSCAN>(denStreamParams.minPoints, denStreamParams.epsilon, denStreamParams.initBufferSize);
  this->startTime = clock();
  SESAME_INFO("Start time: " << this->startTime);
  this->pointArrivingTime = this->startTime;
  this->minWeight = denStreamParams.beta * denStreamParams.mu;
  this->Tp = (long) (1 / denStreamParams.lambda) * (log(minWeight / (minWeight - 1)) / log(denStreamParams.base));
}
void SESAME::DenStream::runOnlineClustering(PointPtr input) {

  if (!this->isInitial) {
    Initilize();
    input->setClusteringCenter(noVisited);
    this->initialBuffer.push_back(input);
    if (this->initialBuffer.size() == this->denStreamParams.initBufferSize) {
      init(this->initialBuffer);
      this->isInitial = true;
    }
  } else {

    this->pointArrivingTime = clock();
    SESAME_INFO("pointArrivingTime time: " << this->pointArrivingTime);
    merge(input);

    long elapsedTime = (long) (this->pointArrivingTime - this->startTime) / CLOCKS_PER_SEC;
    SESAME_INFO("Merge! " << elapsedTime << "," << elapsedTime % this->Tp);

    if (elapsedTime % this->Tp == 0) {//SESAME_INFO("Check");
      for (int iter = 0; iter < pMicroClusters.size(); iter++) {
        if (pMicroClusters.at(iter)->weight < minWeight) {
          pMicroClusters.erase(pMicroClusters.begin() + iter);
          SESAME_INFO("NOW PMC number is: " << this->pMicroClusterIndex);
        }
      }
      if (oMicroClusters.size() != 0) {
        for (int iter = 0; iter < oMicroClusters.size(); iter++) {
          double a = (-(denStreamParams.lambda) * floor(((pointArrivingTime - oMicroClusters.at(iter)->createTime)
              / CLOCKS_PER_SEC + this->Tp)));
          double b = (-denStreamParams.lambda * this->Tp);
          double Xi = (pow(denStreamParams.base, a) - 1) / (pow(denStreamParams.base, b) - 1);
          // SESAME_INFO("NOW Xi  "<<Xi);
          if (oMicroClusters.at(iter)->weight < Xi) {
            oMicroClusters.erase(oMicroClusters.begin() + iter);
            SESAME_INFO("NOW oMicroClusterIndex number is: " << oMicroClusters.size());
          }
        }
      }
    }
    // SESAME_INFO("Insert Succeed "<<iterpoint);
  }
  iterpoint++;
}

void SESAME::DenStream::merge(PointPtr dataPoint) {
  int index = -1;
  if (!this->pMicroClusters.empty()) {
    index = mergeToMicroCluster(dataPoint, this->pMicroClusters);
    //SESAME_INFO("Merge into PMC! "<<iterpoint<<","<< index<<",");
  }

  if (index < 0 && !this->oMicroClusters.empty()) {
    index = mergeToMicroCluster(dataPoint, this->oMicroClusters);
    // SESAME_INFO("Merge into OMC! "<<iterpoint<<","<< index<<",");
    if (index >= 0) {
      double decayFactor =
          this->dampedWindow->decayFunction(this->oMicroClusters.at(index)->lastUpdateTime, pointArrivingTime);
      //SESAME_INFO("Merge INTO OMC! "<<iterpoint<<","<<index);
      if ((this->oMicroClusters.at(index)->weight) * decayFactor > minWeight) {
        // SESAME_INFO("erase OMC and turn into PMC! ");
        pMicroClusterIndex++;
        oMicroClusters.at(index)->resetID(pMicroClusterIndex);
        pMicroClusters.push_back(oMicroClusters.at(index)->copy());
        oMicroClusters.erase(oMicroClusters.begin() + index);

      }
    }

  }
  if (index < 0) {
    oMicroClusterIndex++;
    MicroClusterPtr
        newOMicroCluster = DataStructureFactory::createMicroCluster(denStreamParams.dimension, oMicroClusterIndex);
    newOMicroCluster->init(dataPoint, 0);
    oMicroClusters.push_back(newOMicroCluster->copy());
    // SESAME_INFO("Create new OMC! "<<iterpoint<<","<<oMicroClusterIndex);
  }
}
int SESAME::DenStream::mergeToMicroCluster(PointPtr dataPoint, std::vector<MicroClusterPtr> microClusters) {
  int index = nearestNeighbor(dataPoint, microClusters);
  if (index != -1) {
    double decayFactor = this->dampedWindow->decayFunction(pointArrivingTime, clock());
    if (!microClusters.at(index)->insert(dataPoint, decayFactor, denStreamParams.epsilon))
      index = -1;
  }
  return index;
}
int SESAME::DenStream::nearestNeighbor(PointPtr dataPoint, std::vector<MicroClusterPtr> microClusters) {
  int index = -1;
  double dist = 0, minDist = std::numeric_limits<double>::max();
  for (vector<MicroClusterPtr>::size_type i = 0; i < microClusters.size(); i++) {
    dist = microClusters.at(i)->calCentroidDistance(dataPoint);
    if (dist < minDist) {
      minDist = dist;
      index = i;
    }
  }

  return index;
}

void SESAME::DenStream::runOfflineClustering(DataSinkPtr sinkPtr) {
  SESAME_INFO("Finish online");
  vector<PointPtr> transformedPoints;
  std::vector<std::vector<PointPtr>> oldGroups;
  microClusterToPoint(pMicroClusters, transformedPoints);
  this->dbscan->run(transformedPoints);

  this->dbscan->produceResult(transformedPoints, sinkPtr);

}
void SESAME::DenStream::microClusterToPoint(std::vector<MicroClusterPtr> &microClusters,
                                            vector<PointPtr> &points) {
  for (std::vector<MicroClusterPtr>::size_type i = 0; i < microClusters.size(); i++) {
    PointPtr
        point =
        DataStructureFactory::createPoint(i, microClusters.at(i)->weight, microClusters.at(i)->centroid.size(), 0);
    for (int j = 0; j < microClusters.at(i)->centroid.size(); j++)
      point->setFeatureItem(microClusters[i]->centroid[j], j);
    points.push_back(point);
  }
}
