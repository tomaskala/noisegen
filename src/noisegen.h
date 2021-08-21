#ifndef NOISEGEN_H
#define NOISEGEN_H

#include "cbuffer.h"

#define STR(x) XSTR(x)
#define XSTR(x) #x

#define BUF_SIZE 2048

#define DUR_DEFAULT 10
#define VOL_DEFAULT 50
#define KERNEL_DEFAULT 16
#define BN_COEF_DEFAULT 0.01
#define OUT_DEFAULT "-"

#define WN_VOL 300
#define BN_VOL 30

static const char* usage[] = {
  "Usage: noisegen [OPTIONS] NOISE",
  "",
  "Options:",
  "  -d INT    Duration of the noise in seconds (>0). Defaults to " STR(
    DUR_DEFAULT) ".",
  "  -v INT    Noise volume (0-100). Defaults to " STR(VOL_DEFAULT) ".",
  "  -k INT    Moving average kernel size, used to smoothen the noise.",
  "            Set to 1 to disable smoothing. Defaults to " STR(
    KERNEL_DEFAULT) ".",
  "  -c FLOAT  Filter coefficient for the brown noise generator (0-1).",
  "            Defaults to " STR(BN_COEF_DEFAULT) ".",
  "  -o FILE   File to store the resulting noise in or \"-\" for the",
  "            output. Defaults to " STR(OUT_DEFAULT) ".",
  "  -h        Show this message and exit.",
  "",
  "Noise: One of 'brown' or 'white'.",
  NULL,
};

struct noise_params
{
  double bn_coef;
  double prev;
  int vol;
};

typedef double (*gen_t)(struct noise_params);

static double
gen_wn(struct noise_params params);

static double
gen_bn(struct noise_params params);

static double
init_kernel(struct cbuffer* kernel, gen_t gen, struct noise_params* params);

static void
gen_noise(gen_t gen,
          int dur,
          int vol,
          size_t k_size,
          double bn_coef,
          const char* out);

#endif
