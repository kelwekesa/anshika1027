#include "Memory.h"

Memory::Memory(int64_t tagg, std::clock_t timee){
  tag = tagg;
  time = timee;
  state = MESI::Invalid;
}


void Memory::set_time(std::clock_t timee){
  time = timee;
}

std::clock_t Memory::get_time(){
  return time;
}

int64_t Memory::get_tag(){
  return tag;
}

void Memory::set_tag(int64_t tagg){
  tag = tagg;
}

MESI Memory::get_state(){
  return state;
}

void Memory::set_state(MESI st){
  state = st;
}