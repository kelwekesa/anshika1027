#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

class InputParser {
 public:
  InputParser(int &argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
      tokens.push_back(std::string{argv[i]});
    }
  }

  bool cmdOptionExists(const std::string &option) const {
    auto iter = std::find(tokens.begin(), tokens.end(), option);
    return iter != tokens.end();
  }

  std::string getCmdOption(const std::string &option) const {
    auto iter = std::find(tokens.begin(), tokens.end(), option);
    if (iter != tokens.end() && ++iter != tokens.end()) {
      return *iter;
    }
    return "";
  }

 private:
  std::vector<std::string> tokens;
};

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

int main(int argc, char **argv) {
  InputParser input(argc, argv);
  if (input.cmdOptionExists("-h")) {
    printHelp();
  }
  const std::string &filename = input.getCmdOption("-f");
  if (!filename.empty()) {
    // Do interesting things ...
  }
  return 0;
}
