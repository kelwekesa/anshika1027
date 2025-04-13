#ifndef CACHE
#define CACHE

#include <vector>
#include <cinttypes>
#include <string>
#include <mutex>

#include "Memory.h"
#include "Bus.h"

class Cache {
  std::mutex mutex;
  std::vector<std::vector<Memory>> cache;
  uint32_t no_of_sets;
  uint32_t cache_lines;
  uint32_t hit;
  Bus bus;


 public:
  Cache(uint32_t, uint32_t);
  bool exists(uint32_t, uint32_t);
  bool get(uint32_t, uint32_t);

  void PrRd(uint32_t, uint32_t);
  void PrWr(uint32_t, uint32_t);

  void BusRd(uint32_t, uint32_t);
  void BusRdX(uint32_t, uint32_t);
  void BusUpgr(uint32_t, uint32_t);

  void Flush(uint32_t, uint32_t);
  void FlushOpt(uint32_t, uint32_t);

  void access(uint32_t, uint32_t, const std::string&);
  uint32_t hits();

};

#endif