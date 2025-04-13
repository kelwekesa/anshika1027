#ifndef BUS
#define BUS

#include <cinttypes>
#include <vector>
#include "Cache.h"

class Bus{
  std::vector<Cache> bus;
public:  
  Bus();
  uint32_t add_cache(Cache&);
  std::vector<int> BusRd(uint32_t, uint32_t);
  void BusRdX(uint32_t, uint32_t);
  void BusUpgr(uint32_t, uint32_t);
};
#endif