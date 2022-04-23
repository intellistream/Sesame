// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include "Utils/BenchmarkUtils.hpp"
#include "Algorithm/AlgorithmFactory.hpp"
#include "Engine/SimpleEngine.hpp"
#include "Evaluation/Evaluation.hpp"
#include "Evaluation/Purity.hpp"
#include "Sinks/DataSink.hpp"
#include "Sources/DataSource.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UtilityFunctions.hpp"

#include <filesystem>

/**
 * @Description: get the super parameters from the command line and store into
 * the param_t structure
 * @Param: argc: the first parameter of main function
 * @Param: argv: the second parameter of main function
 * @Param: cmd_params: the dataset attribute received from the command line such
 * as p,c,d,s...
 * @Return: void
 */
void BenchmarkUtils::parseArgs(int argc, char **argv, param_t &cmd_params) {

  int c;
  while (1) {
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"point_number", required_argument, 0, 'p'},
        {"cluster_number", required_argument, 0, 'c'},
        {"dim", required_argument, 0, 'd'},
        {"coreset_size", required_argument, 0, 's'},
        {"seed", required_argument, 0, 'S'},
        {"num_last_arr", required_argument, 0, 'a'},
        {"time_window", required_argument, 0, 'T'},
        {"time_interval", required_argument, 0, 't'},
        {"num_online_clusters", required_argument, 0, 'C'},
        {"radius", required_argument, 0, 'r'},
        {"buf_size", required_argument, 0, 'b'},
        {"offline_time_window", required_argument, 0, 'O'},
        //            {"input_path", required_argument, 0, 'i'},
        //            {"output_path", required_argument, 0, 'o'},
    };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long(argc, argv,
                    "A:a:B:b:C:c:D:d:E:e:g:i:j:k:L:l:M:m:N:n:O:o:P:p:Q:q:R:r:s:"
                    "S:T:t:u:w:W:x:X:h",

                    long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;
    switch (c) { // TODO: change to string type in future. char is limited..
    case 0:
      /* If this option set a flag, do nothing else now. */
      if (long_options[option_index].flag != 0)
        break;
      SESAME_INFO("option " << long_options[option_index].name);
      if (optarg)
        SESAME_INFO(" with arg " << optarg);
      break;

    case 'h':
    case '?':
      /* getopt_long already printed an error message. */
      print_help(argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case 'p':
      cmd_params.num_points = atoi(optarg);
      SESAME_INFO(
          "configure cmd_params.num_points: " << cmd_params.num_points);
      break;
    case 'c':
      cmd_params.num_clusters = atoi(optarg);
      SESAME_INFO(
          "configure cmd_params.num_clusters: " << cmd_params.num_clusters);
      break;
    case 'd':
      cmd_params.dim = atoi(optarg);
      SESAME_INFO("configure cmd_params.dim: " << cmd_params.dim);
      break;
    case 's':
      cmd_params.coreset_size = atoi(optarg);
      SESAME_INFO(
          "configure cmd_params.coreset_size: " << cmd_params.coreset_size);
      break;
    case 'S':
      cmd_params.seed = atoi(optarg);
      SESAME_INFO("configure cmd_params.seed: " << cmd_params.seed);
      break;
    // EDMStream
    case 'A':
      cmd_params.alpha = atof(optarg);
      SESAME_INFO("configure cmd_params.a: " << cmd_params.alpha);
    case 'Q':
      cmd_params.opt = atof(optarg);
      SESAME_INFO("configure cmd_params.opt: " << cmd_params.opt);
    case 'E':
      cmd_params.delta = atof(optarg);
      SESAME_INFO("configure cmd_params.delta: " << cmd_params.delta);
    case 'j':
      cmd_params.num_cache = atof(optarg);
      SESAME_INFO("configure cmd_params.num_cache: " << cmd_params.num_cache);
    case 'a':
      if (atoi(optarg) == 0)
        cmd_params.algo = SESAME::StreamKMeansType;
      else if (atoi(optarg) == 1)
        cmd_params.algo = SESAME::BirchType;
      else if (atoi(optarg) == 2) {
        cmd_params.algo = SESAME::EDMStreamType;
        cmd_params.alpha = 0.998;
        cmd_params.lambda = 1;
        cmd_params.beta = 0.0021;
        cmd_params.opt = 2;
      } else if (atoi(optarg) == 3)
        cmd_params.algo = SESAME::DBStreamType;
      else if (atoi(optarg) == 4)
        cmd_params.algo = SESAME::CluStreamType;
      else if (atoi(optarg) == 5)
        cmd_params.algo = SESAME::DenStreamType;
      else if (atoi(optarg) == 6)
        cmd_params.algo = SESAME::DStreamType;
      else
        SESAME_ERROR("non selected algorithm! ");
      SESAME_INFO("configure cmd_params.algo: ");
      break;
    // USED IN BIRCH
    case 'm':
      cmd_params.max_leaf_nodes = atoi(optarg);
      SESAME_INFO(
          "configure cmd_params.max_leaf_nodes: " << cmd_params.max_leaf_nodes);
      break;
    case 'M':
      cmd_params.max_in_nodes = atoi(optarg);
      SESAME_INFO("configure cmd_params.max_in_nodes: "
                  << cmd_params.max_in_nodes);
      break;
    case 'D':
      cmd_params.distance_threshold = atof(optarg);
      SESAME_INFO("configure cmd_params.distance_threshold: "
                  << cmd_params.distance_threshold);
      break;

    // USED IN CLUSTREAM
    case 'b':
      cmd_params.buf_size = atoi(optarg);
      SESAME_INFO("configure cmd_params.buf_size: " << cmd_params.buf_size);
      break;
    case 'N':
      cmd_params.num_online_clusters = atoi(optarg);
      SESAME_INFO("configure cmd_params.num_online_clusters: "
                  << cmd_params.num_online_clusters);
      break;
    case 'r':
      cmd_params.radius = atof(optarg);
      SESAME_INFO(
          "configure cmd_params.radius: " << cmd_params.radius);
      break;
    case 'l':
      cmd_params.num_last_arr = atoi(optarg);
      SESAME_INFO("configure cmd_params.num_last_arr: "
                  << cmd_params.num_last_arr);
      break;
    case 'T':
      cmd_params.time_window = atoi(optarg);
      SESAME_INFO("configure cmd_params.time_window: " << cmd_params.time_window);
      break;
    case 't':
      cmd_params.time_interval = atoi(optarg);
      SESAME_INFO(
          "configure cmd_params.time_interval: " << cmd_params.time_interval);
      break;
    case 'W':
      cmd_params.offline_time_window = atoi(optarg);
      SESAME_INFO("configure cmd_params.offline_time_window: "
                  << cmd_params.offline_time_window);
      break;
    // USED IN DENSTREAM
    case 'n':
      cmd_params.min_points = atoi(optarg);
      SESAME_INFO("configure cmd_params.min_points: " << cmd_params.min_points);
      break;
    case 'e':
      cmd_params.epsilon = atof(optarg);
      SESAME_INFO("configure cmd_params.epsilon: " << cmd_params.epsilon);
      break;
    case 'B':
      cmd_params.base = atoi(optarg);
      SESAME_INFO("configure cmd_params.base: " << cmd_params.base);
      break;
    case 'L':
      cmd_params.lambda = atof(optarg);
      SESAME_INFO("configure cmd_params.lambda: " << cmd_params.lambda);
      break;
    case 'u':
      cmd_params.mu = atof(optarg);
      SESAME_INFO("configure cmd_params.mu: " << cmd_params.mu);
      break;
    case 'q':
      cmd_params.beta = atof(optarg);
      SESAME_INFO("configure cmd_params.beta: " << cmd_params.beta);
      break;
    case 'R':
      cmd_params.radius = atof(optarg);
      SESAME_INFO("configure cmd_params.radius: " << cmd_params.radius);
      break;
    case 'C':
      cmd_params.clean_interval = atoi(optarg);
      SESAME_INFO("configure cmd_params.clean_interval: "
                  << cmd_params.clean_interval);
      break;
    case 'w':
      cmd_params.min_weight = atof(optarg);
      SESAME_INFO("configure cmd_params.min_weight: " << cmd_params.min_weight);
      break;
    case 'P':
      cmd_params.alpha = atof(optarg);
      SESAME_INFO("configure cmd_params.alpha: " << cmd_params.alpha);
      break;
    case 'x':
      cmd_params.cm = atof(optarg);
      SESAME_INFO("configure cmd_params.cm: " << cmd_params.cm);
      break;
    case 'X':
      cmd_params.cl = atof(optarg);
      SESAME_INFO("configure cmd_params.cl: " << cmd_params.cl);
      break;
    case 'g':
      cmd_params.grid_width = atof(optarg);
      SESAME_INFO("configure cmd_params.grid_width: " << cmd_params.grid_width);
      break;
    case 'O':
      cmd_params.dataset_option = atoi(optarg);
      if (cmd_params.dataset_option == 0)
        cmd_params.input_file =
            std::filesystem::current_path().generic_string() +
            "/datasets/CoverType.txt";
      else if (cmd_params.dataset_option == 1)
        cmd_params.input_file =
            std::filesystem::current_path().generic_string() +
            "/datasets/KDD99.txt";
      else if (cmd_params.dataset_option == 2)
        cmd_params.input_file =
            std::filesystem::current_path().generic_string() +
            "/datasets/sensor.txt";
      else if (cmd_params.dataset_option == 3)
        cmd_params.input_file =
            std::filesystem::current_path().generic_string() +
            "/datasets/Diamond.txt";
      else if (cmd_params.dataset_option == 4)
        cmd_params.input_file =
            std::filesystem::current_path().generic_string() +
            "/datasets/Zelnik.txt";
      SESAME_INFO(
          "configure cmd_params.dataset_option: "
          << cmd_params
                 .input_file); // cmd_params.input_file cmd_params.dataset_option
      break;

    case 'i':
      cmd_params.input_file = optarg;
      SESAME_INFO("configure input path: " << cmd_params.input_file);
      break;
    case 'o':
      cmd_params.output_file = optarg;
      SESAME_INFO("configure output path: " << cmd_params.output_file);
      break;
    default:
      break;
    }
  }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc) {
    SESAME_ERROR("non-option arguments: ");
    while (optind < argc)
      SESAME_ERROR(" " << argv[optind++]);
  }
}

/**
 * @Description: Set the default algorithm StreamKM++ and the default parameters
 * to run the algorithm void -p 1000 -c 10 -d 54 -s 100 -S 10
 * @Param: cmd_params: param_t &
 * @Return:
 */
void BenchmarkUtils::defaultParam(param_t &cmd_params) {
  cmd_params.num_points = 542; // number of the data points in the dataset, use
                                // the whole dataset to run benchmark
  cmd_params.seed = 1;
  cmd_params.num_clusters = 2;
  cmd_params.dim = 54;
  cmd_params.coreset_size = 100;
  cmd_params.num_last_arr = 60;
  cmd_params.time_window = 6;
  cmd_params.time_interval = 4;
  cmd_params.num_online_clusters = 10;
  cmd_params.radius = 2;
  cmd_params.buf_size = 500;
  cmd_params.offline_time_window = 0;
  cmd_params.max_leaf_nodes = 3;
  cmd_params.max_in_nodes = 3;
  cmd_params.distance_threshold = 3550;
  cmd_params.min_points = 10;
  cmd_params.epsilon = 50;
  cmd_params.base = 2;
  cmd_params.lambda = 1.8; // 1.8
  cmd_params.mu = 7;
  cmd_params.beta = 5; // 5

  // EDMStream
  cmd_params.alpha = 0.998;
  cmd_params.num_cache = 100;
  cmd_params.radius = 0.1;
  cmd_params.lambda = 1;
  cmd_params.delta = 10;
  cmd_params.beta = 0.0021;
  cmd_params.opt = 2;

  cmd_params.dataset_option = 0;
  cmd_params.input_file = std::filesystem::current_path().generic_string() +
                         "/datasets/CoverType.txt";
  SESAME_INFO("Default Input Data Directory: " + cmd_params.input_file);
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::DBStreamType;
  cmd_params.run_offline = false;
  cmd_params.detect_outlier = false;
}

/* command line handling functions */
/**
 * TODO: Make it more useful @WangXin.
 * @param string
 */
void BenchmarkUtils::print_help(char *string) {
  SESAME_ERROR("Usage: " << string << " [options]");
  SESAME_ERROR(" Available options: ");
}

/**
 * @Description: load data from the given dataset and convert the data format
 * into the given point data structure
 * @Param: cmd_params: the dataset attribute received from the command line such
 * as p,c,d,s...
 * @Param: input: the vector to store the point list from the dataset
 * @Return: void
 */
void BenchmarkUtils::loadData(param_t &cmd_params,
                              SESAME::DataSourcePtr dataSourcePtr) {
  // Pass input file as a string to DataSource.
  std::vector<std::string> data;
  ifstream infile;
  infile.open(cmd_params.input_file);
  SESAME_INFO("Read from the file...");

  // insert the data once per line into the string vector, every string element
  // represents a data line
  for (int i = 0; i < cmd_params.num_points; i++) {
    data.emplace_back();
    getline(infile, data[i]);
  }
  SESAME_INFO("Complete reading from the file...");
  infile.close();

  // convert the string format into point vector, every string represents a
  // point
  dataSourcePtr->load(cmd_params.num_points, cmd_params.dim, data);
  SESAME_INFO("Finished loading input data");
}
BenchmarkResultPtr BenchmarkUtils::runBenchmark(param_t &cmd_params,
                                                SESAME::DataSourcePtr sourcePtr,
                                                SESAME::DataSinkPtr sinkPtr,
                                                SESAME::AlgorithmPtr algoPtr) {
  std::cerr << "data number: " << cmd_params.num_points << std::endl;

  switch (cmd_params.algo) {
  case SESAME::CluStreamType:
    std::cerr << "Algorithm: CluStream "
              << "num_last_arr: " << cmd_params.num_last_arr
              << "   time_window: " << cmd_params.time_window
              << "   num_offline_clusters: " << cmd_params.num_clusters
              << "   ClusterNumber: " << cmd_params.num_online_clusters
              << "   radius: " << cmd_params.radius
              << "   buf_size: " << cmd_params.buf_size << "\n";
    break;
  case SESAME::DenStreamType:
    std::cerr << "Algorithm: DenStream "
              << "buf_size: " << cmd_params.buf_size
              << "   min_points: " << cmd_params.min_points
              << "   epsilon: " << cmd_params.epsilon
              << "   lambda: " << cmd_params.lambda
              << "   mu: " << cmd_params.mu << "   beta: " << cmd_params.beta
              << "\n";
    break;
  case SESAME::DBStreamType:
    std::cerr << "Algorithm: DBStream "
              << "lambda: " << cmd_params.lambda
              << "   radius: " << cmd_params.radius
              << "   clean_interval: " << cmd_params.clean_interval
              << "   min_weight: " << cmd_params.min_weight
              << "   alpha: " << cmd_params.alpha << "\n";
    break;
  case SESAME::DStreamType:
    std::cerr << "Algorithm: DStream "
              << "lambda: " << cmd_params.lambda
              << "   beta: " << cmd_params.beta << "   cm: " << cmd_params.cm
              << "   cl: " << cmd_params.cl << "\n";
    break;
  case SESAME::StreamKMeansType:
    std::cerr << "Algorithm: StreamKMeans "
              << "Seed: " << cmd_params.seed
              << "   ClusterNumber: " << cmd_params.num_clusters
              << "   CoresetSize: " << cmd_params.coreset_size << "\n";
    break;
  case SESAME::BirchType:
    std::cerr << "Algorithm: Birch "
              << "maxLeafNode: " << cmd_params.max_leaf_nodes
              << "   maxInnerNodes: " << cmd_params.max_in_nodes
              << "   distance_threshold: " << cmd_params.distance_threshold
              << "\n";
    break;
  case SESAME::V1Stream:
    std::cerr << "Algorithm: BirchV1 "
              << "maxLeafNode: " << cmd_params.max_leaf_nodes
              << "   maxInnerNodes: " << cmd_params.max_in_nodes
              << "   distance_threshold: " << cmd_params.distance_threshold
              << "   ClusterNumber: " << cmd_params.num_clusters << "\n";
    break;
  case SESAME::V2Stream:
    std::cerr << "Algorithm: BirchV2 "
              << "maxLeafNode: " << cmd_params.max_leaf_nodes
              << "   maxInnerNodes: " << cmd_params.max_in_nodes
              << "   distance_threshold: " << cmd_params.distance_threshold
              << "\n";
    break;
  case SESAME::V3Stream:
    std::cerr << "Algorithm: BirchV3 "
              << "maxLeafNode: " << cmd_params.max_leaf_nodes
              << "   maxInnerNodes: " << cmd_params.max_in_nodes
              << "   distance_threshold: " << cmd_params.distance_threshold
              << "\n";
    break;
  case SESAME::EDMStreamType:
    std::cerr << "Algorithm: EDMStream "
              << "CacheNum: " << cmd_params.num_cache
              << "   Radius: " << cmd_params.radius
              << "   MinDelta: " << cmd_params.delta << "\n";
  case SESAME::Generic:
    std::cerr << "Algorithm: Generic "
              << "\n";
  default:
    break;
  }
  SESAME::SimpleEngine engine(
      sourcePtr, sinkPtr,
      algoPtr); // TODO: create multithread engine in future.

  engine.run();

  while (!sinkPtr->isFinished())
    usleep(100);
  // wait for sink to stop.

  std::vector<SESAME::PointPtr> inputs = sourcePtr->getInputs();
  std::vector<SESAME::PointPtr> results = sinkPtr->getResults();
  std::vector<SESAME::PointPtr> predicts;
  if (results.empty()) {
    std::cerr << "ERROR! No output!" << std::endl;
  }
  // TODO: be sure the output clusterID start from 0!
  if (cmd_params.run_offline) {
    SESAME::UtilityFunctions::groupByCentersWithOffline(
        inputs, results, predicts, cmd_params.dim);
    // 使用offline的算法不管是否detect
    // outlier输出都是一样，如果detect则clusteringIndex = 0代表outlier
    // clustering center
  } else {
    // the output is the clustering center so we need to help every input data
    // find its nearest center
    SESAME::UtilityFunctions::groupByCenters(inputs, results, predicts,
                                             cmd_params.dim);
  }

  // Store results.
  algoPtr->Store(cmd_params.output_file, cmd_params.dim,
                 sinkPtr->getResults());
  SESAME_INFO("Finished store results: " << sinkPtr->getResults().size());

  auto res = SESAME::Evaluation::runEvaluation(
      cmd_params.dim, cmd_params.num_clusters, cmd_params.time_decay,
      inputs, predicts);

  engine.stop();

  return res;
}
