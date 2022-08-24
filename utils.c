#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

double rand_interval(double min, double max) {
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    return (min + d * (max - min));
}