#ifndef MEMORY
#define MEMORY

#include <cinttypes>
#include <ctime>

enum class MESI { Modified, Exclusive, Shared, Invalid };

class Memory{
  int64_t tag;
  std::clock_t time;
  MESI state;

public:
  Memory(int64_t tag, std::clock_t time);
  void set_time(std::clock_t);
  std::clock_t get_time();

  int64_t get_tag();
  void set_tag(int64_t);
  
  MESI get_state();
  void set_state(MESI);
};

#endif