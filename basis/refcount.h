#include <stdio.h>

#define NP_CHECK(ptr) \
        if (!(ptr)) { \
            fprintf(stderr, "%s:%d NULL POINTER: %s n", \
                __FILE__, __LINE__, (#ptr)); \
            exit(-1); \
        } \

typedef void (*rc_finalizer_t)(void *);
void* rc_malloc   (int nbytes, rc_finalizer_t finalizer);
void  rc_keep_ref (void* p);
void  rc_free_ref (void* p);