#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdint>

class InputParser {
  std::vector<std::string> tokens;
 public:
  InputParser(int &argc, char **argv);
  bool cmdOptionExists(const std::string &option) const;
  std::string getCmdOption(const std::string &option) const;
};

void printHelp();

std::string appName;
size_t S;
size_t B;
size_t E;
std::string logFile;
std::string tracesFolder = "../assignment3_traces/";

int main(int argc, char **argv) {
  InputParser input(argc, argv);
  if (input.cmdOptionExists("-h")) {
    printHelp();
    return EXIT_SUCCESS;
  }

  appName = input.getCmdOption("-t");
  if (appName.empty()) {
    std::cout << "tracefile was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  }

  std::string ss = input.getCmdOption("-s");
  if (ss.empty()){
    std::cout << "number of set index bits was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  }else{
    S = std::stoi(ss);
  }

  std::string ee = input.getCmdOption("-E");
  if (ee.empty()){
    std::cout << "associativity was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  }else{
    E = std::stoi(ee);
  }

  std::string bb = input.getCmdOption("-b");
  if (bb.empty()){
    std::cout << "blocksize was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  }else{
    B = std::stoi(bb);
  }

  logFile = input.getCmdOption("-o");
  if (logFile.empty()) {
    std::cout << "logFile was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  }

  std::cout << std::setw(27) << std::right << "L1 CACHE SIMULATION\n" << std::endl;
  std::cout << std::setw(23) << std::left << "appName: " << appName
            << std::endl;
  std::cout << std::setw(23) << std::left << "set index bits: " << S
            << std::endl;
  std::cout << std::setw(23) << std::left << "number of block bits: " << B
            << std::endl;
  std::cout << std::setw(23) << std::left << "associativity: " << E
            << std::endl;
  std::cout << std::setw(23) << std::left << "logFile: " << logFile
            << std::endl;
  std::cout << std::setw(23) << std::left << "tracesFolder: " << tracesFolder
            << std::endl;

  // open traces
  std::vector<std::ifstream> traces;
  for (int i = 0; i < 4; ++i) {
    std::string filename =
        tracesFolder + appName + "_proc" + std::to_string(i) + ".trace";
    std::ifstream trace{filename};
    if (trace){
      traces.push_back(std::move(trace));
      std::cout << "" << filename << std::endl;
    }else{
      std::cout << "failed to open tracefile: " << filename << std::endl;
      return EXIT_FAILURE;
    }
  }
  // process a file
  uint64_t addr;
  std::string rw;
  while (traces[0] >> rw >> std::hex >> addr) {
    std::cout << std::setw(5) << std::right << rw << std::setw(15) << std::right
              << std::hex << addr << std::endl;
    break;
  }

  return EXIT_SUCCESS;
}

void printHelp() {
  std::cout << "-t <tracefile>: name of parallel application (e.g. app1) whose "
               "4 traces are to be used in simulation "
            << std::endl;

  std::cout << "-s <s>: number of set index bits (number of sets in the cache "
               "= S = 2^s)"
            << std::endl;

  std::cout << "-E <E>: associativity (number of cache lines per set)"
            << std::endl;

  std::cout << "-b <b>: number of block bits (block size = B = 2^b)"
            << std::endl;

  std::cout << "-o: <outfilename> logs output in file for plotting etc."
            << std::endl;

  std::cout << "-h: prints this help" << std::endl;
}

InputParser::InputParser(int &argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    tokens.push_back(std::string{argv[i]});
  }
}

bool InputParser::cmdOptionExists(const std::string &option) const {
  auto iter = std::find(tokens.begin(), tokens.end(), option);
  return iter != tokens.end();
}

std::string InputParser::getCmdOption(const std::string &option) const {
  auto iter = std::find(tokens.begin(), tokens.end(), option);
  if (iter != tokens.end() && ++iter != tokens.end()) {
    return *iter;
  }
  return "";
}
