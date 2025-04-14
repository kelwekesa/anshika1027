#include "Memory.h"

Memory::Memory(int64_t tagg, std::clock_t timee){
  tag = tagg;
  time = timee;
  state = MESI::Invalid;
}



void Memory::set_time(std::clock_t timee){
  //std::lock_guard<std::mutex> guard(mutex);
  time = timee;
}

std::clock_t Memory::get_time(){
  //std::lock_guard<std::mutex> guard(mutex);
  return time;
}

int64_t Memory::get_tag(){
  //std::lock_guard<std::mutex> guard(mutex);
  return tag;
}

void Memory::set_tag(int64_t tagg){
  //std::lock_guard<std::mutex> guard(mutex);
  tag = tagg;
}

MESI Memory::get_state(){
  //std::lock_guard<std::mutex> guard(mutex);
  return state;
}

void Memory::set_state(MESI st){
  //std::lock_guard<std::mutex> guard(mutex);
  state = st;
}