#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbuffer.h"
#include "noisegen.h"
#include "util.h"
#include "xoshiro.h"

static double
gen_wn(struct noise_params params)
{
  double wn = rand_dbl();
  return (2.0 * wn - 1.0) / WN_VOL * params.vol;
}

static double
gen_bn(struct noise_params params)
{
  double wn = rand_dbl();
  wn = (2.0 * wn - 1.0) / BN_VOL * params.vol;
  return params.prev - params.bn_coef * (params.prev - wn);
}

static double
init_kernel(struct cbuffer* kernel, gen_t gen, struct noise_params* params)
{
  size_t i;
  double sample, sample_sum = 0.0;
  for (i = 0; i < kernel->cap; ++i) {
    sample = gen(*params);
    params->prev = sample;
    sample_sum += sample;
    if (cb_push(kernel, sample))
      die("fill cb_push");
  }
  return sample_sum / kernel->cap;
}

static void
gen_noise(gen_t gen,
          int dur,
          int vol,
          size_t k_size,
          double bn_coef,
          const char* out)
{
  size_t i, b, c, items_written, n_samples;
  double sample_old, sample_new, sample_avg[N_CHANNELS];
  int16_t buffer[BUF_SIZE];
  struct noise_params params[N_CHANNELS];
  struct cbuffer* kernel[N_CHANNELS];
  FILE* f = stdout;

  if ((unsigned long)dur > SIZE_MAX / SAMPLE_RATE)
    die("duration too large, overflow");

  n_samples = dur * SAMPLE_RATE;

  for (c = 0; c < N_CHANNELS; ++c) {
    kernel[c] = cb_init(k_size);
    params[c].bn_coef = bn_coef;
    params[c].prev = 0.0;
    params[c].vol = vol;
    sample_avg[c] = init_kernel(kernel[c], gen, &params[c]);
  }

  if (strcmp(out, "-") && !(f = fopen(out, "wb")))
    die("fopen %s: %s", out, strerror(errno));

  write_wav_header(f, n_samples);

  for (i = 0, items_written = 0; i < n_samples;) {
    for (b = 0; i < n_samples && b < BUF_SIZE; ++i) {
      for (c = 0; b < BUF_SIZE && c < N_CHANNELS; ++b, ++c) {
        sample_new = gen(params[c]);
        params[c].prev = sample_new;

        if (cb_poll(kernel[c], &sample_old) == -1)
          die("cb_poll");
        if (cb_push(kernel[c], sample_new) == -1)
          die("cb_push");

        sample_avg[c] += (sample_new - sample_old) / k_size;
        buffer[b] = (int16_t)(sample_avg[c] * INT16_MAX);
      }
    }

    if (fwrite(&buffer, sizeof(int16_t), b, f) != b)
      die("fwrite: wav data");

    items_written += b;
  }

  if (strcmp(out, "-") && fclose(f))
    die("fclose %s: %s", out, strerror(errno));

  for (c = 0; c < N_CHANNELS; ++c) {
    cb_free(kernel[c]);
    kernel[c] = NULL;
  }

  assert(items_written == n_samples * N_CHANNELS);
}

int
main(int argc, char** argv)
{
  int option, dur = DUR_DEFAULT, vol = VOL_DEFAULT;
  double bn_coef = BN_COEF_DEFAULT;
  size_t k_size = KERNEL_DEFAULT;
  char* p;
  const char *out = OUT_DEFAULT, *noise;
  gen_t gen = NULL;

  while ((option = getopt(argc, argv, "hd:v:k:c:o:")) != -1) {
    switch (option) {
      case 'h':
        multifputs(usage, stdout);
        return EXIT_SUCCESS;
      case 'd':
        errno = 0;
        dur = strtol(optarg, &p, 10);
        if (errno || *p)
          die("invalid argument: %s", optarg);
        assert(dur > 0);
        break;
      case 'v':
        errno = 0;
        vol = strtol(optarg, &p, 10);
        if (errno || *p)
          die("invalid argument: %s", optarg);
        assert(0 <= vol && vol <= 100);
        break;
      case 'k':
        errno = 0;
        k_size = strtol(optarg, &p, 10);
        if (errno || *p)
          die("invalid argument: %s", optarg);
        assert(k_size > 0);
        break;
      case 'c':
        errno = 0;
        bn_coef = strtod(optarg, &p);
        if (errno || *p)
          die("invalid argument: %s", optarg);
        assert(0.0 < bn_coef && bn_coef < 1.0);
        break;
      case 'o':
        out = optarg;
        break;
      default:
        multifputs(usage, stderr);
        return EXIT_FAILURE;
    }
  }

  if (optind == argc)
    die("missing noise type");

  noise = argv[optind];

  if (strcmp(noise, "white") == 0)
    gen = &gen_wn;
  else if (strcmp(noise, "brown") == 0)
    gen = &gen_bn;
  else
    die("unrecognized noise type: %s", noise);

  rand_seed(12345);
  gen_noise(gen, dur, vol, k_size, bn_coef, out);
  return EXIT_SUCCESS;
}
