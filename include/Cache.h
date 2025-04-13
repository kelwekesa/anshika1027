#ifndef CACHE
#define CACHE

#include <vector>
#include <cinttypes>
#include <string>
#include <mutex>

#include "Memory.h"
class Bbus;

class Cache {
  std::mutex mutex;
  std::vector<std::vector<Memory>> cache;
  Bbus *bus;
  uint32_t no_of_sets;
  uint32_t cache_lines;
  uint32_t block_size;
  uint32_t hit;
  int busIndex;
  int64_t clock;


 public:
  Cache(uint32_t, uint32_t, uint32_t, Bbus *);

  void PrRd(uint32_t, uint32_t);
  void PrWr(uint32_t, uint32_t);

  int BusRd(uint32_t, uint32_t);
  int BusRdX(uint32_t, uint32_t);
  void BusUpgr(uint32_t, uint32_t);

  void Flush(uint32_t, uint32_t);
  void FlushOpt(uint32_t, uint32_t);

  int empty_slot(uint32_t);
  int LRU(uint32_t);
  std::pair<uint32_t, uint32_t> hits_clock();
};

#endif