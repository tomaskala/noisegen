#include <stdlib.h>

#include "cbuffer.h"
#include "util.h"

struct cbuffer*
cb_init(size_t cap)
{
  struct cbuffer* cb = alloc(sizeof(struct cbuffer));
  cb->buffer = alloc(cap * sizeof(double));
  cb->cap = cap;
  cb->read_ptr = 0;
  cb->write_ptr = 0;
  cb->items_avail = 0;
  return cb;
}

void
cb_free(struct cbuffer* cb)
{
  free(cb->buffer);
  free(cb);
}

int
cb_push(struct cbuffer* cb, double item)
{
  if (cb->cap == cb->items_avail) {
    return -1;
  }

  cb->buffer[cb->write_ptr % cb->cap] = item;
  cb->write_ptr = (cb->write_ptr + 1) % cb->cap;
  cb->items_avail++;
  return 0;
}

int
cb_poll(struct cbuffer* cb, double* out)
{
  if (cb->items_avail == 0) {
    return -1;
  }

  *out = cb->buffer[cb->read_ptr % cb->cap];
  cb->read_ptr = (cb->read_ptr + 1) % cb->cap;
  cb->items_avail--;
  return 0;
}
