#include <stdlib.h>
#include "refcount.h"

// A LIGHTWEIGHT REFERENCE COUNTING IMPLEMENTATION FOR BASIS

typedef struct _rc_header {
  int refcount;
  rc_finalizer_t finalizer;
} rc_header;

void *rc_malloc(int nbytes, rc_finalizer_t finalizer) {
  rc_header *rc = malloc(nbytes + sizeof(rc_header));
  
  rc->refcount = 1;
  rc->finalizer = finalizer;
  
  // return offset pointer (+1 on a structure type points just past the end)
  return (void *)(rc + 1);
}

void rc_keep_ref(void *p) {
  rc_header *rc = ((rc_header *)p) - 1;
  rc->refcount++;
}

void rc_free_ref(void *p) {
  rc_header *rc = ((rc_header *)p) - 1;
  rc->refcount--;
  if (rc->refcount == 0) {
    if (rc->finalizer)
      rc->finalizer(p);
    free(rc);
  }
}
