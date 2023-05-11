#ifndef VEC_H
#define VEC_H

typedef struct {
    unsigned int dim;
    float *components;
} vec;

vec *new_vec(unsigned int dim);
vec *add_vec(vec *v1, vec *v2);
void normalize_vec(vec *v);
void scale_vec(float scale, vec *v);
int are_equal_vec(vec *v1, vec *v2);
float dot(vec *v1, vec *v2);
vec *cross(vec *v1, vec *v2);
int are_orthogonal_vec(vec *v1, vec *v2);
float vec_mag(vec *v);
float vec_mag_squared(vec *v);
void free_vec(vec *v);
void print_vec(vec *v);

#endif