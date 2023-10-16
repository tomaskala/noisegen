#ifndef CBUFFER_H
#define CBUFFER_H

struct cbuffer {
  double *buffer;
  int cap;
  int read_ptr;
  int write_ptr;
  int items_avail;
};

void cb_init(struct cbuffer *, int);

void cb_free(struct cbuffer *);

int cb_push(struct cbuffer *, double);

int cb_poll(struct cbuffer *, double *);

#endif
