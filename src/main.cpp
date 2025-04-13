#include <algorithm>
#include <vector>
#include <fstream>
#include <iomanip>

#include "InputParser.h"
#include "Cache.h"
#include "utility.h"

std::string appName;
uint32_t S;
uint32_t B;
uint32_t E;
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
  if (ss.empty()) {
    std::cout << "number of set index bits was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  } else {
    S = std::stoi(ss);
  }

  std::string ee = input.getCmdOption("-E");
  if (ee.empty()) {
    std::cout << "associativity was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  } else {
    E = std::stoi(ee);
  }

  std::string bb = input.getCmdOption("-b");
  if (bb.empty()) {
    std::cout << "blocksize was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  } else {
    B = std::stoi(bb);
  }

  logFile = input.getCmdOption("-o");
  if (logFile.empty()) {
    std::cout << "logFile was not provided" << std::endl;
    printHelp();
    return EXIT_FAILURE;
  }

  std::cout << std::setw(27) << std::right << "L1 CACHE SIMULATION\n"
            << std::endl;
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
    if (trace) {
      traces.push_back(std::move(trace));
      std::cout << "" << filename << std::endl;
    } else {
      std::cout << "failed to open tracefile: " << filename << std::endl;
      return EXIT_FAILURE;
    }
  }
  // process a file
  uint32_t addr;
  std::string rw;
  uint32_t no_of_sets = 1 << S;
  Cache cache{E, no_of_sets};
  while (traces[0] >> rw >> std::hex >> addr) {

    uint32_t set_mask = create_mask(B, B+S);
    uint32_t set_number = (addr & set_mask) >> B;

    uint32_t tag_mask = create_mask(B+S, 31);
    uint32_t tag = (addr & tag_mask) >> (B + S);

    cache.access(tag, set_number, rw);
    // break;
  }
  
  std::cout << "hits: " << cache.hits() << std::endl;
  return EXIT_SUCCESS;
}
