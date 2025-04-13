#include "Memory.h"

Memory::Memory(uint32_t tagg, std::clock_t timee){
  tag = tagg;
  time = timee;
  dirty = false;
  state = MESI::Ivalid;
}

void Memory::set_dirty(){
  dirty = true;
}

bool Memory::is_dirty(){
  return dirty;
}

void Memory::set_time(std::clock_t timee){
  time = timee;
}

std::clock_t Memory::get_time(){
  return time;
}

uint32_t Memory::get_tag(){
  return tag;
}

void Memory::set_tag(uint32_t tagg){
  tag = tagg;
}