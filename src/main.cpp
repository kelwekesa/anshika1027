#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "Bbus.h"
#include "Cache.h"
#include "InputParser.h"
#include "utility.h"

std::string appName;
uint32_t S;
uint32_t B;
uint32_t E;
std::string logFile;
std::string tracesFolder = "../assignment3_traces/";

int main(int argc, char** argv) {
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

  std::cout << std::setw(37) << std::right << "Simulation Parameters:\n"
            << std::endl;
  std::cout << std::setw(27) << std::right << "Trace Prefix: " << appName
            << std::endl;
  std::cout << std::setw(27) << std::right << "Set index bits: " << S
            << std::endl;
  std::cout << std::setw(27) << std::right << "Associativity: " << E
            << std::endl;
  std::cout << std::setw(27) << std::right << "Block bits: " << B << std::endl;
  std::cout << std::setw(27) << std::right << "Block Size (Bytes): " << (1 << B)
            << std::endl;
  std::cout << std::setw(27) << std::right << "Number of Sets: " << (1 << S)
            << std::endl;
  std::cout << std::setw(27) << std::right
            << "Cache Size (KB per core): " << (1 << S) * (1 << B) * E / 1024
            << std::endl;
  std::cout << std::setw(27) << std::right << "MESI Protocol: " << "Enabled"
            << std::endl;
  std::cout << std::setw(27) << std::right
            << "Write Policy: " << "Write-back, Write-allocate" << std::endl;
  std::cout << std::setw(27) << std::right << "Replacement Policy: " << "LRU"
            << std::endl;
  std::cout << std::setw(27) << std::right << "Bus: " << "Central snooping bus"
            << std::endl;
  /*
  std::cout << std::setw(29) << std::left << "  logFile: " << logFile
            << std::endl;
  std::cout << std::setw(29) << std::left << "  tracesFolder: " << tracesFolder
            << std::endl;
  */

  std::cout << std::endl;
  // open traces
  std::vector<std::ifstream> traces;
  for (int i = 0; i < 4; ++i) {
    std::string filename =
        tracesFolder + appName + "_proc" + std::to_string(i) + ".trace";
    std::ifstream trace{filename};
    if (trace) {
      traces.push_back(std::move(trace));
      // std::cout << "  " << filename << std::endl;
    } else {
      std::cout << "failed to open tracefile: " << filename << std::endl;
      return EXIT_FAILURE;
    }
  }

  uint32_t no_of_sets = 1 << S;
  uint32_t blocksize = 1 << B;

  Bbus bus{};

  Cache cache0{E, no_of_sets, blocksize, &bus};
  Cache cache1{E, no_of_sets, blocksize, &bus};
  Cache cache2{E, no_of_sets, blocksize, &bus};
  Cache cache3{E, no_of_sets, blocksize, &bus};

  bus.add_cache(&cache0);
  bus.add_cache(&cache1);
  bus.add_cache(&cache2);
  bus.add_cache(&cache3);

  auto core = [](auto trace, auto cache) {
    // process a file
    uint32_t addr;
    std::string rw;

    uint32_t set_mask = create_mask(B, B + S);
    uint32_t tag_mask = create_mask(B + S, 31);
    while (trace >> rw >> std::hex >> addr) {
      uint32_t set_number = (addr & set_mask) >> B;
      uint32_t tag = (addr & tag_mask) >> (B + S);

      if (rw == "R") {
        cache->PrRd(tag, set_number);
      } else {
        cache->PrWr(tag, set_number);
      }
    }
  };

  std::vector<std::thread> processor;

  for (size_t i = 0; i < traces.size(); ++i) {
    // std::cout << "  thread " << i << std::endl;
    processor.push_back(
        std::thread{core, std::move(traces[i]), bus.caches()[i]});
  }

  std::cout << std::endl;

  for (auto&& core : processor) {
    core.join();
  }

  for (size_t i = 0; i < bus.caches().size(); ++i) {
    auto core = bus.caches()[i];
    std::cout << std::setw(21) << std::right << "Core " << i
              << " Statistics:" << std::endl;
    std::cout << std::setw(27) << std::right
              << "Total Instructions: " << core->total_instrs << std::endl;
    std::cout << std::setw(27) << std::right << "Total Reads: " << core->reads
              << std::endl;
    std::cout << std::setw(27) << std::right << "Total Writes: " << core->writes
              << std::endl;
    std::cout << std::setw(27) << std::right
              << "Total Execution Cycles: " << core->cycles << std::endl;
    //
    std::cout << std::setw(27) << std::right << "Idle Cycles: " << core->reads
              << std::endl;
    int64_t misses = (core->total_instrs - core->hit);
    std::cout << std::setw(27) << std::right << "Cache Misses: " << misses
              << std::endl;
    double miss_rate = (double)misses * 100 / core->total_instrs;
    std::cout << std::setw(27) << std::right
              << "Cache Miss Rate: " << std::fixed << std::setprecision(2)
              << miss_rate << "%" << std::endl;
    std::cout << std::setw(27) << std::right
              << "Cache Evictions: " << core->evictions << std::endl;
    std::cout << std::setw(27) << std::right
              << "Writebacks: " << core->writebacks << std::endl;
    std::cout << std::setw(27) << std::right
              << "Bus Invalidations: " << core->invalidations << std::endl;
    std::cout << std::setw(27) << std::right
              << "Data Traffic (Bytes): " << core->total_instrs * blocksize
              << std::endl;
    std::cout << std::endl;
  }

  std::cout << std::setw(27) << std::right
            << "Overall Bus Summary:" << std::endl;
  std::cout << std::setw(27) << std::right << "Total Bus Transactions:" << ""
            << std::endl;
  std::cout << std::setw(27) << std::right << "Total Bus Traffic (Bytes):" << ""
            << std::endl;

  return EXIT_SUCCESS;
}
