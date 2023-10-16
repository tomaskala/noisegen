#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>

#define SAMPLE_RATE 44100
#define SUBCHUNK1_SIZE 16
#define PCM_FMT 1
#define N_CHANNELS 2
#define BYTES_PER_SAMPLE 2
#define BITS_PER_SAMPLE (BYTES_PER_SAMPLE * 8)

struct wav_header {
  char chunk_id[4];
  int32_t chunk_size;
  char fmt[4];

  char subchunk1_id[4];
  int32_t subchunk1_size;
  int16_t audio_fmt;
  int16_t n_channels;
  int32_t sample_rate;
  int32_t byte_rate;
  int16_t block_align;
  int16_t bits_per_sample;

  char subchunk2_id[4];
  int32_t subchunk2_size;
};

void *alloc(size_t);

void die(const char *, ...);

void multifputs(const char **, FILE *);

void write_wav_header(FILE *, int);

#endif
