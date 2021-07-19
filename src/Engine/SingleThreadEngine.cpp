//
// Created by Shuhao Zhang on 19/07/2021.
//


#include <Engine/SingleThreadEngine.hpp>

Executor::Executor(){
  struct param_t init = {568592,10,54,20,10};
  this->cmd_params = init;
  this->outputPath = "/Users/tuidan/code/C++/1/Sesame/src/Benchmark/Workload/new.txt";
}

param_t Executor::getParam() {
  return this->cmd_params;
}

void Executor::parse_args(int argc, char **argv){
  if(argc != 6) cout << "need five parameter! PointNumber; ClusterNumber; Dimension; CoresetSize; Seed";
  else{
    this->cmd_params.pointNumber = atoi(argv[1]);
    this->cmd_params.clusterNumber = atoi(argv[2]);
    this->cmd_params.dimension = atoi(argv[3]);
    this->cmd_params.coreSize = atoi(argv[4]);
    this->cmd_params.seed = atoi(argv[5]);
  }
}

bool Executor::runAlgorithm() {
  return false;
}

void Executor::run() {
  // Benchmark
  Benchmark b;
  DataLoader d;
  b.initialDataLoader(this->cmd_params.pointNumber, false, this->cmd_params.dimension);
  string * data;
  data = b.getDataLoader().readFile(b.getDataLoader().getFilePath());
  //todo: input&output file path needs to become a param

  //Algorithm
  Algorithm a(b.getDataLoader().createPoints(data),
              this->cmd_params.pointNumber,
              this->cmd_params.dimension,
              this->cmd_params.coreSize,
              this->cmd_params.clusterNumber);
  Point * centresStreamingCoreset = a.Modular(StreamKMeans);

  // Store the results
  FILE *out = fopen("/Users/tuidan/code/C++/1/Sesame/src/Benchmark/Workload/1.txt","w");
  b.getDataLoader().saveCentersLiadanFormat( out ,this->cmd_params.clusterNumber, centresStreamingCoreset);
  fclose(out);

  //todo：Evaluation
}