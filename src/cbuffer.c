#include <stdlib.h>

#include "cbuffer.h"
#include "util.h"

void cb_init(struct cbuffer *cb, int cap) {
  cb->buffer = alloc(cap * sizeof(double));
  cb->cap = cap;
  cb->read_ptr = 0;
  cb->write_ptr = 0;
  cb->items_avail = 0;
}

void cb_free(struct cbuffer *cb) { free(cb->buffer); }

int cb_push(struct cbuffer *cb, double item) {
  if (cb->cap == cb->items_avail)
    return -1;

  cb->buffer[cb->write_ptr % cb->cap] = item;
  cb->write_ptr = (cb->write_ptr + 1) % cb->cap;
  cb->items_avail++;
  return 0;
}

int cb_poll(struct cbuffer *cb, double *out) {
  if (cb->items_avail == 0)
    return -1;

  *out = cb->buffer[cb->read_ptr % cb->cap];
  cb->read_ptr = (cb->read_ptr + 1) % cb->cap;
  cb->items_avail--;
  return 0;
}
