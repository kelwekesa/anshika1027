#include "Cache.h"
#include <algorithm>

Cache::Cache(uint32_t assosiativity, uint32_t setnum/*, uint32_t blocksize*/) {
  no_of_sets = setnum;
  cache_lines = assosiativity;
  hit = 0;
  //
  cache.resize(no_of_sets, std::vector<Memory>(
                               cache_lines, {0, std::clock()}));
}

bool Cache::exists(uint32_t tag, uint32_t set_number){
  std::vector<Memory> &bucket = cache[set_number];
  // search bucket for slot
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });
  //
  return iter != bucket.end();
}

bool Cache::get(uint32_t tag, uint32_t set_number) {
  std::vector<Memory> &bucket = cache[set_number];
  // search bucket for slot
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });
  if (iter != bucket.end()) {
    // update time
    iter->set_time(std::clock());
    // hit
    hit ++;
    return true;
  }
  // miss !
  return false;
}

void Cache::access(uint32_t tag, uint32_t set_number, const std::string& rw) {
  std::vector<Memory> &bucket = cache[set_number];
  // check for empty slot
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });
  if (iter != bucket.end()) {
    // hit
    hit ++;
    iter->set_time(std::clock());
    if (rw == "W"){
      iter->set_dirty();
    }
  } else {
    // miss
    /* we have to evict */
    // sort the bucket according to time
    std::sort(bucket.begin(), bucket.end(), [](auto addr1, auto addr2) {
      return addr1.get_time() < addr2.get_time();
    });
    // evict the last item in bucket
    Memory evicted = bucket[bucket.size()-1];
    if (evicted.is_dirty()){
      /* 100 clock cycles */
    }
    bucket.pop_back();
    // give this one
    Memory mem{tag, std::clock()};
    if (rw == "W"){
      mem.set_dirty();
    }
    bucket.push_back(mem);
  }
}

void Cache::PrRd(uint32_t tag, uint32_t set_number){
  std::lock_guard<std::mutex> guard(mutex);
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  }
  switch (state) {
    case MESI::Invalid: {
      bus.BusRd(busIndex);
      break;
    }
    case MESI::Modified: {
      break;
    }
    case MESI::Exclusive: {
      break;
    }
    case MESI::Shared: {
      break;
    }
  }
}

void Cache::PrWr(uint32_t tag , uint32_t set_number){
  std::lock_guard<std::mutex> guard(mutex);
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  }
  switch (state) {
    case MESI::Invalid: {
      break;
    }
    case MESI::Modified: {
      break;
    }
    case MESI::Exclusive: {
      break;
    }
    case MESI::Shared: {
      break;
    }
  }
}

void Cache::BusRd(uint32_t tag, uint32_t set_number){
  std::lock_guard<std::mutex> guard(mutex);
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  }
  switch (state) {
    case MESI::Invalid: {
      break;
    }
    case MESI::Modified: {
      break;
    }
    case MESI::Exclusive: {
      break;
    }
    case MESI::Shared: {
      break;
    }
  }
}
void Cache::BusRdX(uint32_t tag, uint32_t set_number){
std::lock_guard<std::mutex> guard(mutex);
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  }
  switch (state) {
    case MESI::Invalid: {
      break;
    }
    case MESI::Modified: {
      break;
    }
    case MESI::Exclusive: {
      break;
    }
    case MESI::Shared: {
      break;
    }
  }
}

void Cache::BusUpgr(uint32_t tag, uint32_t set_number){
std::lock_guard<std::mutex> guard(mutex);
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  }
  switch (state) {
    case MESI::Invalid: {
      break;
    }
    case MESI::Modified: {
      break;
    }
    case MESI::Exclusive: {
      break;
    }
    case MESI::Shared: {
      break;
    }
  }
}


void Cache::Flush(uint32_t, uint32_t){

}

void Cache::FlushOpt(uint32_t, uint32_t){

}

uint32_t Cache::hits(){
  return hit;
}
