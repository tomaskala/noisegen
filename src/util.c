#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void*
alloc(size_t size)
{
  void* p;
  if (!(p = malloc(size)))
    die("malloc:");
  return p;
}

void
die(const char* fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
    fputc(' ', stderr);
    perror(NULL);
  } else
    fputc('\n', stderr);

  exit(EXIT_FAILURE);
}

void
multifputs(const char** s, FILE* f)
{
  while (*s) {
    fputs(*s++, f);
    fputc('\n', f);
  }
}

void
write_wav_header(FILE* f, int n_samples)
{
  /* The wav file standard specifies that if subchunk2_size is odd, the
   * data portion should be padded by a zero byte. We know that the size
   * is even because the sample rate is an even number. */
  int subchunk2_size = n_samples * N_CHANNELS * BYTES_PER_SAMPLE;
  int chunk_size = 4 + (8 + SUBCHUNK1_SIZE) + (8 + subchunk2_size);
  struct wav_header header;

  header.chunk_id[0] = 'R';
  header.chunk_id[1] = 'I';
  header.chunk_id[2] = 'F';
  header.chunk_id[3] = 'F';
  header.chunk_size = chunk_size;
  header.fmt[0] = 'W';
  header.fmt[1] = 'A';
  header.fmt[2] = 'V';
  header.fmt[3] = 'E';

  header.subchunk1_id[0] = 'f';
  header.subchunk1_id[1] = 'm';
  header.subchunk1_id[2] = 't';
  header.subchunk1_id[3] = ' ';
  header.subchunk1_size = SUBCHUNK1_SIZE;
  header.audio_fmt = PCM_FMT;
  header.n_channels = N_CHANNELS;
  header.sample_rate = SAMPLE_RATE;
  header.byte_rate = N_CHANNELS * BYTES_PER_SAMPLE * SAMPLE_RATE;
  header.block_align = N_CHANNELS * BYTES_PER_SAMPLE;
  header.bits_per_sample = BITS_PER_SAMPLE;

  header.subchunk2_id[0] = 'd';
  header.subchunk2_id[1] = 'a';
  header.subchunk2_id[2] = 't';
  header.subchunk2_id[3] = 'a';
  header.subchunk2_size = subchunk2_size;

  if (fwrite(&header, sizeof(struct wav_header), 1, f) != 1)
    die("fwrite: wav header");
}
