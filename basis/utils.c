#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

double rand_interval(double min, double max) {
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    return (min + d * (max - min));
}

void map(void (*f)(void *, void *, void **), int n, void **a0, void **a1, void **d) {
    for (int i = 0; i < n; i++) {
        f(a0[i], a1[i], d);
    }
} 