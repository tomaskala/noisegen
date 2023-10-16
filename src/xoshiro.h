#ifndef XOSHIRO_H
#define XOSHIRO_H

#define rotl(x, k) ((x) << (k)) | ((x) >> (64 - (k)))

void rand_seed(uint64_t);

uint64_t rand_int(void);

double rand_dbl(void);

#endif
