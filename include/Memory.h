#ifndef MEMORY
#define MEMORY

#include <cinttypes>
#include <ctime>

enum class MESI { Modified, Exclusive, Shared, Invalid };

class Memory{
  uint32_t tag;
  std::clock_t time;
  bool dirty;
  MESI state;

public:
  Memory(uint32_t tag, std::clock_t time);
  void set_dirty();
  void set_time(std::clock_t);
  uint32_t get_tag();
  void set_tag(uint32_t);
  std::clock_t get_time();
  bool is_dirty();
  MESI get_state();
};

#endif