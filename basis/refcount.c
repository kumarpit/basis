#include <stdlib.h>
#include "refcount.h"

// A lightweight reference counting implementation

typedef struct _rc_header {
  int refcount;
  rc_finalizer_t finalizer;
} rc_header;

void *rc_malloc(int nbytes, rc_finalizer_t finalizer) {
  rc_header *rc = malloc(nbytes + sizeof(rc_header));
  NP_CHECK(rc);
  rc->refcount = 1;
  rc->finalizer = finalizer;
  return (void *)(rc + 1);
}

void rc_keep_ref(void *p) {
  NP_CHECK(p);
  rc_header *rc = ((rc_header *)p) - 1;
  rc->refcount++;
}

void rc_free_ref(void *p) {
  NP_CHECK(p);
  rc_header *rc = ((rc_header *)p) - 1;
  rc->refcount--;
  if (rc->refcount == 0) {
    if (rc->finalizer)
      rc->finalizer(p);
    free(rc);
  }
}
