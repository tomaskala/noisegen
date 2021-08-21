#include <stdint.h>

#include "xoshiro.h"

static uint64_t x;
static uint64_t s[4];

void
rand_seed(uint64_t seed)
{
  int i;
  uint64_t z;
  x = seed;

  for (i = 0; i < 4; ++i) {
    z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    s[i] = z ^ (z >> 31);
  }
}

uint64_t
rand_int()
{
  const uint64_t result = rotl(s[1] * 5, 7) * 9;
  const uint64_t t = s[1] << 17;
  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];
  s[2] ^= t;
  s[3] = rotl(s[3], 45);
  return result;
}

double
rand_dbl()
{
  uint64_t x = rand_int();
  return (x >> 11) * (0.5 / ((uint64_t)1 << 52));
}
