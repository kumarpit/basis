#ifndef UTILS
#define UTILS

typedef unsigned int uint;

double rand_interval(double min, double max);
void map(void (*f) (void *, void *, void **), int n, void **a0, void **a1, void **d);

#endif