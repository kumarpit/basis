#include "utils.h"

#ifndef VEC_H
#define VEC_H

typedef struct {
    unsigned int dim;
    double *components;
} vec;

vec *new_vec(uint dim);
vec *make_vec2d(double i, double j);
vec *make_vec3d(double i, double j, double k);
vec *add_vec(vec *v1, vec *v2);
void normalize_vec(vec *v);
void scale_vec(double scale, vec *v);
int are_equal_vec(vec *v1, vec *v2);
double dot(vec *v1, vec *v2);
vec *cross(vec *v1, vec *v2);
int are_orthogonal_vec(vec *v1, vec *v2);
double vec_mag(vec *v);
double vec_mag_squared(vec *v);
void free_vec(void *v);
void print_vec(vec *v);

#endif