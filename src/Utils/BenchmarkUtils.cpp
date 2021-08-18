#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSource.hpp>
#include <Sinks/DataSink.hpp>
#include <Engine/SimpleEngine.hpp>
#include <Evaluation/Evaluation.hpp>
#include <Algorithm/AlgorithmFactory.hpp>
#include <filesystem>

using namespace std;

/**
 * @Description: get the super parameters from the command line and store into the param_t structure
 * @Param: argc: the first parameter of main function
 * @Param: argv: the second parameter of main function
 * @Param: cmd_params: the dataset attribute received from the command line such as p,c,d,s...
 * @Return: void
 */
void BenchmarkUtils::parseArgs(int argc, char **argv, param_t &cmd_params) {

  int c;
  while (1) {
    static struct option long_options[] =
        {
            {"help", no_argument, 0, 'h'},
            {"point_number", required_argument, 0, 'p'},
            {"cluster_number", required_argument, 0, 'c'},
            {"dimension", required_argument, 0, 'd'},
            {"coreset_size", required_argument, 0, 's'},
            {"seed", required_argument, 0, 'S'},
            {"lastArrivingNum", required_argument, 0, 'a'},
            {"timeWindow", required_argument, 0, 'T'},
            {"timeInterval", required_argument, 0, 't'},
            {"onlineClusterNumber", required_argument, 0, 'C'},
            {"radiusFactor", required_argument, 0, 'r'},
            {"initBuffer", required_argument, 0, 'b'},
            {"offlineTimeWindow", required_argument, 0, 'O'},
//            {"input_path", required_argument, 0, 'i'},
//            {"output_path", required_argument, 0, 'o'},
        };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long(argc, argv, "p:c:d:s:S:a:T:t:C:r:b:O:h",
                    long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;
    switch (c) {//TODO: change to string type in future. char is limited..
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        SESAME_INFO("option " << long_options[option_index].name);
        if (optarg) SESAME_INFO(" with arg " << optarg);
        break;

      case 'h':
      case '?':
        /* getopt_long already printed an error message. */
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'p':cmd_params.pointNumber = atoi(optarg);
        SESAME_INFO("configure cmd_params.pointNumber: " << cmd_params.pointNumber);
        break;
      case 'c':cmd_params.clusterNumber = atoi(optarg);
        SESAME_INFO("configure cmd_params.clusterNumber: " << cmd_params.clusterNumber);
        break;
      case 'd':cmd_params.dimension = atoi(optarg);
        SESAME_INFO("configure cmd_params.dimension: " << cmd_params.dimension);
        break;
      case 's':cmd_params.coresetSize = atoi(optarg);
        SESAME_INFO("configure cmd_params.coresetSize: " << cmd_params.coresetSize);
        break;
      case 'S':cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.seed: " << cmd_params.seed);
        break;
      case 'a': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.lastArrivingNum: " << cmd_params.lastArrivingNum);
        break;
      case 'T': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.timeWindow: " << cmd_params.timeWindow);
        break;
      case 't': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.timeInterval: " << cmd_params.timeInterval);
        break;
      case 'C': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.onlineClusterNumber: " << cmd_params.onlineClusterNumber);
        break;
      case 'r': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.radiusFactor: " << cmd_params.radiusFactor);
        break;
      case 'b': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.initBuffer: " << cmd_params.initBuffer);
        break;
      case 'O': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.offlineTimeWindow: " << cmd_params.offlineTimeWindow);
        break;

      case 'i':cmd_params.inputPath = optarg;
        SESAME_INFO("configure input path: " << cmd_params.inputPath);
        break;
      case 'o':cmd_params.outputPath = optarg;
        SESAME_INFO("configure output path: " << cmd_params.outputPath);
        break;
      default:break;
    }
  }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc) {
    SESAME_ERROR("non-option arguments: ");
    while (optind < argc) SESAME_ERROR(" " << argv[optind++]);
  }
}

/**
 * @Description: Set the default algorithm StreamKM++ and the default parameters to run the algorithm
 * void -p 1000 -c 10 -d 54 -s 100 -S 10
 * @Param: cmd_params: param_t &
 * @Return:
 */
void BenchmarkUtils::defaultParam(param_t &cmd_params) {
  cmd_params.pointNumber = 1000;
  cmd_params.seed = 10;
  cmd_params.clusterNumber = 10;
  cmd_params.dimension = 54;
  cmd_params.coresetSize = 100;
  cmd_params.lastArrivingNum = 60;
  cmd_params.timeWindow = 6;
  cmd_params.timeInterval = 4;
  cmd_params.onlineClusterNumber = 15;
  cmd_params.radiusFactor = 70;
  cmd_params.initBuffer = 500;
  cmd_params.offlineTimeWindow = 2;
  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/new.txt";
  SESAME_INFO("Default Input Data Directory: " + cmd_params.inputPath);
  cmd_params.outputPath = "results.txt";
  cmd_params.algoName = "CluStream";//StreamKMeans  CluStream
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
 * @Description: load data from the given dataset and convert the data format into the given point data structure
 * @Param: cmd_params: the dataset attribute received from the command line such as p,c,d,s...
 * @Param: input: the vector to store the point list from the dataset
 * @Return: void
 */
void BenchmarkUtils::loadData(param_t &cmd_params, SESAME::DataSourcePtr dataSourcePtr) {
  // Pass input file as a string to DataSource.
  std::vector<std::string> data;
  ifstream infile;
  infile.open(cmd_params.inputPath);
  SESAME_INFO("Read from the file...");

  // insert the data once per line into the string vector, every string element represents a data line
  for (int i = 0; i < cmd_params.pointNumber; i++) {
    data.emplace_back();
    getline(infile, data[i]);
  }
  SESAME_INFO("Complete reading from the file...");
  infile.close();

  // convert the string format into point vector, every string represents a point
  dataSourcePtr->load(cmd_params.pointNumber, cmd_params.dimension, data);
  SESAME_INFO("Finished loading input data");
}
void BenchmarkUtils::runBenchmark(param_t &cmd_params,
                                  SESAME::DataSourcePtr sourcePtr,
                                  SESAME::DataSinkPtr sinkPtr,
                                  SESAME::AlgorithmPtr algoPtr) {

  SESAME::SimpleEngine engine(sourcePtr, sinkPtr, algoPtr);//TODO: create multithread engine in future.

  engine.run();

  engine.barrierPtr->arrive_and_wait();//ask algorithm to start execution
  engine.barrierPtr->arrive_and_wait();//wait algorithm to finish execution

  engine.stop();
  //Store results.
  algoPtr->store(cmd_params.outputPath, cmd_params.clusterNumber, cmd_params.dimension, sinkPtr->getResults());
  SESAME_INFO("Finished store results.");
}


