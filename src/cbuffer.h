#ifndef CBUFFER_H
#define CBUFFER_H

struct cbuffer
{
  double* buffer;
  size_t cap;
  size_t read_ptr;
  size_t write_ptr;
  size_t items_avail;
};

struct cbuffer*
cb_init(size_t cap);

void
cb_free(struct cbuffer* cb);

int
cb_push(struct cbuffer* cb, double item);

int
cb_poll(struct cbuffer* cb, double* out);

#endif
