#include <stdlib.h>
#include <stdio.h>
#include "vec.h"
#include "refcount.h"
#include <math.h>
#include "utils.h"

#define CANNOT_MAKE_VEC \
    "Vector initialization requires a dimension of atleast 1 \n" \

#define CANNOT_ADD_VEC \
    "Vector addition requires two vectors of the same dimension \n" \

#define CANNOT_DOT_VEC \
    "Vector dot product requires two vectors of the same dimension \n" \

#define CANNOT_CROSS_VEC \
    "Vector cross product requires two vectors of the same dimension \n" \ 


// generates new zero-valued vector
vec *new_vec(uint dim) {
    if (dim == 0) {
        BASIS_ERROR(CANNOT_MAKE_VEC);
        return NULL;
    }
    vec *v = rc_malloc(sizeof(vec), free_vec);
    v->dim = dim;
    v->components = rc_malloc(dim * sizeof(double), NULL);
    return v;
}

// generate new 2d vector <i,j>
vec *make_vec2d(double i, double j) {
    vec *v = new_vec(2);
    double *vc = v->components;
    vc[0] = i;
    vc[1] = j;
    return v;
}

// generate new 3d vector <i,j,k>
vec *make_vec3d(double i, double j, double k) {
    vec *v = new_vec(3);
    double *vc = v->components;
    vc[0] = i;
    vc[1] = j;
    vc[2] = k;
    return v;
}

// vector addition
vec *add_vec(vec *v1, vec *v2) {
    if (v1->dim != v2->dim) {
        BASIS_ERROR(CANNOT_ADD_VEC);
        return NULL;
    }
    vec *v = new_vec(v1->dim);
    for (uint i = 0; i < v->dim; i++) {
        v->components[i] = v1->components[i] + v2->components[i];
    }
    return v;
}

// vector normalization
void normalize_vec(vec *v) {
    double mag = vec_mag(v);
    for (uint i = 0; i < v->dim; i++) {
        v->components[i] = v->components[i] / mag;
    }
}

// scale a vector by a double value
void scale_vec(double scale, vec *v) {
    for (uint i = 0; i < v->dim; i++) {
        v->components[i] = scale * v->components[i];
    }
}

// check vector equality (by value, not by reference)
int are_equal_vec(vec *v1, vec *v2) {
    if (v1->dim != v2->dim) return 0;
    for (uint i = 0; i < v1->dim; i++) {
        if (v1->components[i] != v2->components[i]) {
            return 0;
        }
    }
    return 1;
}

// vector dot product
double dot(vec *v1, vec *v2) {
    if (v1->dim != v2->dim) {
        BASIS_ERROR(CANNOT_DOT_VEC);
        return INFINITY;
    }
    double sum = 0;
    for (uint i = 0; i < v1->dim; i++) {
        sum += v1->components[i] * v2->components[i];
    }

    return sum;
}

// check if two vectors are othogonal
int are_orthogonal_vec(vec *v1, vec *v2) {
    if (v1->dim != v2->dim) return 0;
    return dot(v1, v2) == 0;
}

// vector cross-product
// TODO!!!
vec *cross(vec *v1, vec *v2) {
    if (v1->dim != v2->dim) {
        BASIS_ERROR(CANNOT_DOT_VEC);
        return NULL;
    }
    // IMPLEMENT DETERMINANTS FIRST
    return NULL;
}

// calculate the magnitude of a vector
double vec_mag(vec *v) {
    double sum = 0.0;
    for (uint i = 0; i < v->dim; i++) {
        sum += pow(v->components[i], 2);
    }
    return sqrt(sum);
}

// calculate the squared magnitude of a vector - often a useful quantity
double vec_mag_squared(vec *v) {
    return pow(vec_mag(v), 2);
}

// destroy a vector
void free_vec(void *_v) {
    vec *v = (vec *)_v;
    rc_free_ref(v->components);
}