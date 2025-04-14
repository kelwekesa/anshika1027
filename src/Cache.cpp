#include "Cache.h"

#include <algorithm>
#include <iostream>

#include "Bbus.h"

Cache::Cache(uint32_t assosiativity, uint32_t setnum, uint32_t blocksize,
             Bbus *b) {
  bus = b;
  no_of_sets = setnum;
  cache_lines = assosiativity;
  block_size = blocksize / 4;
  busIndex = -1;

  cycles = 0;
  idle_cycles = 0;
  hit = 0;
  total_instrs = 0;
  reads = 0;
  writes = 0;
  evictions = 0;
  writebacks = 0;
  invalidations = 0;
  data_traffic = 0;

  
  cache.resize(no_of_sets,
               std::vector<Memory>(cache_lines, {-1, std::clock()}));
  
}

std::pair<uint32_t, uint32_t> Cache::hits_clock() {
  return std::make_pair(hit, cycles);
}

int Cache::empty_slot(uint32_t set_number) {
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(), [&](auto &addr) {
    return addr.get_tag() == -1 || addr.get_state() == MESI::Invalid;
  });
  if (iter != bucket.end()) {
    return std::distance(bucket.begin(), iter);
  }
  return -1;
}

int Cache::LRU(uint32_t set_number) {
  evictions ++;
  std::vector<Memory> &bucket = cache[set_number];
  // item with the least clock value
  auto iter = std::min_element(
      bucket.begin(), bucket.end(),
      [](auto&& a, auto&& b) { return a.get_time() < b.get_time(); });
  if (iter->get_state() == MESI::Modified) {
    // if it has modified state
    // we need to write it back
    cycles += 100;
    bus->Flush(iter->get_tag(), set_number);
    writebacks ++;
  }
  return std::distance(bucket.begin(), iter);
}

void Cache::PrRd(uint32_t tag, uint32_t set_number) {
  //std::cout << std::hex << tag << std::endl;
  reads ++;
  total_instrs ++;

  //std::lock_guard<std::mutex> guard{mutex};
  mutex.lock();

  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  } 
  switch (state) {
    case MESI::Invalid: {
      // we require a fresh slot anyway
      int index = empty_slot(set_number);
      if (index == -1) {
        // we have to evict
        // memory with smallest clock cycle
        index = LRU(set_number);
      }
      // check if other cores have this line
      mutex.unlock();
      std::vector<int> res = bus->BusRd(tag, set_number, busIndex);
      mutex.lock();
      if (std::any_of(res.begin(), res.end(), [](auto r) { return r != -1; })) {
        // one of the cores has a copy
        bucket[index].set_tag(tag);
        bucket[index].set_time(std::clock());
        bucket[index].set_state(MESI::Shared);
        cycles += (2 * block_size);
        // hit ++;
      } else {
        // none has a copy
        // pull from main memory
        bucket[index].set_tag(tag);
        bucket[index].set_time(std::clock());
        bucket[index].set_state(MESI::Exclusive);
        // since we pulled from memory
        cycles += 100;
      }
      break;
    }
    case MESI::Modified:
    case MESI::Exclusive:
    case MESI::Shared: {
      hit++;
    }
  }
  mutex.unlock();
}

void Cache::PrWr(uint32_t tag, uint32_t set_number) {
  //std::cout << std::hex << tag << std::endl;
  writes ++;
  total_instrs ++;
  //std::lock_guard<std::mutex> guard{mutex};
  mutex.lock();

  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  } 
  switch (state) {
    case MESI::Invalid: {
      // we require a fresh slot anyway
      int index = empty_slot(set_number);
      if (index == -1) {
        // we have to evict
        // memory with smallest clock cycle
        index = LRU(set_number);
      }
      // check if other cores have this line
      mutex.unlock();
      std::vector<int> res = bus->BusRdX(tag, set_number, busIndex);
      mutex.lock();
      if (std::any_of(res.begin(), res.end(), [](auto r) { return r != -1; })) {
        // one of the cores has a copy
        bucket[index].set_tag(tag);
        bucket[index].set_time(std::clock());
        bucket[index].set_state(MESI::Modified);
        cycles += (2 * block_size);
        // hit ++;
      } else {
        // pull copy from main memory
        bucket[index].set_tag(tag);
        bucket[index].set_time(std::clock());
        bucket[index].set_state(MESI::Modified);
        cycles += 100;
      }
      break;
    }
    case MESI::Modified:
    case MESI::Exclusive: {
      hit++;
      iter->set_state(MESI::Modified);
      break;
    }
    case MESI::Shared: {
      // check if other cores have this line
      // ask them to mark it as invalid
      mutex.unlock();
      bus->BusUpgr(tag, set_number);
      mutex.lock();
      // change state to modified
      iter->set_state(MESI::Modified);
      break;
    }
  }
  mutex.unlock();
}

int Cache::BusRd(uint32_t tag, uint32_t set_number) {
  // std::lock_guard<std::mutex> guard(mutex);
  mutex.lock();
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  } 
  switch (state) {
    case MESI::Invalid: {
      mutex.unlock();
      return -1;
      break;
    }
    case MESI::Modified: {
      iter->set_state(MESI::Shared);
      mutex.unlock();
      return 1;
      break;
    }
    case MESI::Exclusive: {
      iter->set_state(MESI::Shared);
      mutex.unlock();
      return 1;
      break;
    }
    case MESI::Shared: {
      mutex.unlock();
      return 1;
      break;
    }
  }
  mutex.unlock();
  return -1;
}

int Cache::BusRdX(uint32_t tag, uint32_t set_number) {
  // std::lock_guard<std::mutex> guard(mutex);
  mutex.lock();
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  } 
  switch (state) {
    case MESI::Invalid: {
      mutex.unlock();
      return -1;
      break;
    }
    case MESI::Modified: {
      iter->set_state(MESI::Invalid);
      mutex.unlock();
      return 1;
      break;
    }
    case MESI::Exclusive: {
      iter->set_state(MESI::Invalid);
      mutex.unlock();
      return 1;
      break;
    }
    case MESI::Shared: {
      iter->set_state(MESI::Invalid);
      mutex.unlock();
      return 1;
      break;
    }
  }
  mutex.unlock();
  return -1;
}

void Cache::BusUpgr(uint32_t tag, uint32_t set_number) {
  // std::lock_guard<std::mutex> guard(mutex);
  mutex.lock();
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });

  MESI state = MESI::Invalid;
  if (iter != bucket.end()) {
    state = iter->get_state();
  } 
  switch (state) {
    case MESI::Invalid:
    case MESI::Modified:
    case MESI::Exclusive:
    case MESI::Shared: {
      if (iter != bucket.end()){
        iter->set_state(MESI::Invalid);
      }
      break;
    }
  }
  mutex.unlock();
}

void Cache::Flush(uint32_t tag, uint32_t set_number) {
  // std::lock_guard<std::mutex> guard(mutex);
  //mutex.lock();
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });
  // has been written back to main memory
  // therefore invalidate it
  if (iter != bucket.end()) {
    iter->set_state(MESI::Invalid);
    iter->set_tag(-1);
  }
  //mutex.unlock();
}

void Cache::FlushOpt(uint32_t, uint32_t) {
  /*
  std::lock_guard<std::mutex> guard(mutex);
  std::vector<Memory> &bucket = cache[set_number];
  auto iter = std::find_if(bucket.begin(), bucket.end(),
                           [&](auto &addr) { return addr.get_tag() == tag; });
  */
  // has been written back to main memory
  // therefore invalidate it
  // iter->set_state(MESI::Shared);
}
