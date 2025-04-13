#ifndef BBUS
#define BBUS

#include <cinttypes>
#include <vector>

#include "Cache.h"

class Bbus{
  std::vector<Cache*> bus;
public:  
  Bbus();
  uint32_t add_cache(Cache*);
  std::vector<int> BusRd(uint32_t, uint32_t, uint32_t);
  std::vector<int> BusRdX(uint32_t, uint32_t, uint32_t);
  void BusUpgr(uint32_t, uint32_t);
  void Flush(uint32_t, uint32_t);
  std::vector<Cache*> caches();
};
#endif