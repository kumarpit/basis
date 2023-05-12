#include <stdlib.h>
#include <stdio.h>
#include "vec.h"
#include <assert.h>
#include <math.h>

vec *new_vec(unsigned int dim) {
    vec *v = calloc(1, sizeof(*v));
    v->dim = dim;
    v->components = calloc(dim, sizeof(float));

    return v;
}

vec *add_vec(vec *v1, vec *v2) {
    assert(v1->dim == v2->dim);
    vec *v = new_vec(v1->dim);
    for (int i = 0; i < v->dim; i++) {
        v->components[i] = v1->components[i] + v2->components[i];
    }
    return v;
}

void normalize_vec(vec *v) {
    float mag = vec_mag(v);
    for (int i = 0; i < v->dim; i++) {
        v->components[i] = v->components[i] / mag;
    }
}

void scale_vec(float scale, vec *v) {
    for (int i = 0; i < v->dim; i++) {
        v->components[i] = scale * v->components[i];
    }
}

int are_equal_vec(vec *v1, vec *v2) {
    if (v1->dim != v2->dim) return 0;
    for (int i = 0; i < v1->dim; i++) {
        if (v1->components[i] != v2->components[i]) {
            return 0;
        }
    }
    return 1;
}

float dot(vec *v1, vec *v2) {
    assert(v1->dim == v2->dim);
    float sum = 0.0;
    for (int i = 0; i < v1->dim; i++) {
        sum += v1->components[i] * v2->components[i];
    }

    return sum;
}

int are_orthogonal_vec(vec *v1, vec *v2) {
    return dot(v1, v2) == 0;
}

vec *cross(vec *v1, vec *v2) {
    assert(v1->dim == v2->dim);
    // TODO!!!
    
}

float vec_mag(vec *v) {
    float sum = 0.0;
    for (int i = 0; i < v->dim; i++) {
        sum += pow(v->components[i], 2);
    }
    return sqrt(sum);
}

float vec_mag_squared(vec *v) {
    return pow(vec_mag(v), 2);
}

void free_vec(vec *v) {
    free(v->components);
    free(v);
}