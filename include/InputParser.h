#ifndef INPUT_PARSER
#define INPUT_PARSER

#include <vector>
#include <string>
#include <iostream>

void printHelp();
class InputParser {
  std::vector<std::string> tokens;

 public:
  InputParser(int &argc, char **argv);
  bool cmdOptionExists(const std::string &option) const;
  std::string getCmdOption(const std::string &option) const;
};

#endif