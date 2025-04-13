#include "../include/utility.h"

bool is_power_of_2(uint32_t x) { return (x != 0) && ((x & (x - 1)) == 0); }

uint32_t nearest_power_of_2(uint32_t x) {
  if (is_power_of_2(x)) {
    return x;
  }
  uint32_t power = 1;
  while (power < x) {
    power *= 2;
  }
  return power;
}

uint32_t create_mask(int start, int stop) {
  uint32_t mask = 0;
  for (int i = start; i < stop; ++i) {
    uint32_t bit = 1;
    bit = bit << i;
    mask |= bit;
  }
  return mask;
}