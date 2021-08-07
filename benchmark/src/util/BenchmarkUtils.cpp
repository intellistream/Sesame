//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <util/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSource.hpp>
#include <Sinks/DataSink.hpp>
#include <Engine/SingleThreadEngine.hpp>
#include <Evaluation/Evaluation.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

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
//            {"input_path", required_argument, 0, 'i'},
//            {"output_path", required_argument, 0, 'o'},
        };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long(argc, argv, "p:c:d:s:S:h",
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
      case 'p': cmd_params.pointNumber = atoi(optarg);
        SESAME_INFO("configure cmd_params.pointNumber: " << cmd_params.pointNumber);
        break;
      case 'c': cmd_params.clusterNumber = atoi(optarg);
        SESAME_INFO("configure cmd_params.clusterNumber: " << cmd_params.clusterNumber);
        break;
      case 'd': cmd_params.dimension = atoi(optarg);
        SESAME_INFO("configure cmd_params.dimension: " << cmd_params.dimension);
        break;
      case 's': cmd_params.coresetSize = atoi(optarg);
        SESAME_INFO("configure cmd_params.coresetSize: " << cmd_params.coresetSize);
        break;
      case 'S': cmd_params.seed = atoi(optarg);
        SESAME_INFO("configure cmd_params.seed: " << cmd_params.seed);
        break;
      case 'i': cmd_params.inputPath = optarg;
        SESAME_INFO("configure input path: " << cmd_params.inputPath);
        break;
      case 'o': cmd_params.outputPath = optarg;
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
 * @Param: cmd_params: param_t &
 * @Return: void
 */
void BenchmarkUtils::defaultParam(param_t &cmd_params) {
  cmd_params.pointNumber = 568592;
  cmd_params.seed = 10;
  cmd_params.clusterNumber = 10;
  cmd_params.dimension = 54;
  cmd_params.coresetSize = 20;
  string home = getenv("HOME");
  cmd_params.inputPath = "/home/ubuntu/database/Sesame/benchmark/datasets/new.txt";
  SESAME_INFO("Default Input Data Directory: " + cmd_params.inputPath);
  cmd_params.outputPath = "results.txt";
  cmd_params.algoName = "StreamKMeans";
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
void BenchmarkUtils::loadData(param_t &cmd_params, vector<SESAME::PointPtr> &input) {
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
  SESAME::DataSource::create(cmd_params.pointNumber, cmd_params.dimension, data, input);
  SESAME_INFO("Finished loading input data");
}

/**
 * TODO: data type should not be exposed to users, change the logic here to hide it.
 * @Description: assemble the modules and run the benchmark according to different algorithm.
 * @param: cmd_params
 * @param: input: the input point vector(clustering center attribute is GT)
 * @param: output: the output point vector(set computed clustering result in clustering center attribute)
 */
void BenchmarkUtils::runBenchmark(param_t &cmd_params,
                                  const vector<SESAME::PointPtr> &input,
                                  vector<SESAME::PointPtr> &output) {
  SESAME::AlgorithmPtr algo =
      SESAME::AlgorithmFactory::create(cmd_params.algoName,
                                       cmd_params.pointNumber,
                                       cmd_params.clusterNumber,
                                       cmd_params.dimension,
                                       cmd_params.coresetSize,
                                       cmd_params.seed);

  SESAME::SingleThreadEngine().runAlgorithm(input, output,
                                            algo);
}

/**
 * @Description: evaluate the output point result using some metrics
 * @Param: cmd_params
 * @Param: output
 * @Return: void
 */
void BenchmarkUtils::evaluate(param_t &cmd_params,
                              const vector<SESAME::PointPtr> &input,
                              const vector<SESAME::PointPtr> &output) {
  SESAME::Evaluation::euclideanCost(cmd_params.pointNumber,
                                    cmd_params.clusterNumber,
                                    cmd_params.dimension,
                                    input,
                                    output);
}


/**
 * @Description: store the final clustering result, also point format but the clustering center of each point has been set
 * @Param: cmd_params; output
 * @Return: void
 */
void BenchmarkUtils::store(param_t &cmd_params, const vector<SESAME::PointPtr> &output) {
  SESAME::DataSink::store(cmd_params.outputPath, cmd_params.clusterNumber, cmd_params.dimension, output);
}



