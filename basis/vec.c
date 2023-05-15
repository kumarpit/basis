#include <stdlib.h>
#include <stdio.h>
#include "vec.h"
#include <assert.h>
#include "refcount.h"
#include <math.h>

vec *new_vec(unsigned int dim) {
    vec *v = rc_malloc(sizeof(vec), free_vec);
    v->dim = dim;
    v->components = calloc(dim, sizeof(double));
    return v;
}

vec *make_vec2d(double i, double j) {
    vec *v = new_vec(2);
    double *vc = v->components;
    vc[0] = i;
    vc[1] = j;
    return v;
}

vec *make_vec3d(double i, double j, double k) {
    vec *v = new_vec(3);
    double *vc = v->components;
    vc[0] = i;
    vc[1] = j;
    vc[2] = k;
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
    double mag = vec_mag(v);
    for (int i = 0; i < v->dim; i++) {
        v->components[i] = v->components[i] / mag;
    }
}

void scale_vec(double scale, vec *v) {
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

double dot(vec *v1, vec *v2) {
    assert(v1->dim == v2->dim);
    double sum = 0;
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
    // IMPLEMENT DETERMINANTS FIRST
}

double vec_mag(vec *v) {
    double sum = 0.0;
    for (int i = 0; i < v->dim; i++) {
        sum += pow(v->components[i], 2);
    }
    return sqrt(sum);
}

double vec_mag_squared(vec *v) {
    return pow(vec_mag(v), 2);
}

void free_vec(vec *v) {
    free(v->components);
}