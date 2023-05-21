#include <criterion/criterion.h>
#include "../basis/vec.h"
#include "../basis/refcount.h"
#include "math.h"

#define EPSILON 0.0001

// TEST CREATE FUNCTIONS

Test(Vectors, new_vec) {
    vec *fv = new_vec(0);
    cr_assert(!fv);
}

Test(Vectors, add_vec) {
    vec *v1 = make_vec3d(1,2,3);
    vec *v2 = make_vec3d(2,3,4);
    vec *vadd = add_vec(v1, v2);
    double *vaddc = vadd->components;

    cr_assert(vaddc[0] == 3);
    cr_assert(vaddc[1] == 5);
    cr_assert(vaddc[2] == 7);

    // should return NULL on dimension inequality
    vec *v3 = make_vec2d(1,1);
    vec *fvadd = add_vec(v1, v3);
    cr_assert(!fvadd);  

    rc_free_ref(v1);
    rc_free_ref(v2);
    rc_free_ref(v3);
    rc_free_ref(vadd);
}

Test(Vectors, normalize_vec) {
    vec *v = make_vec3d(1,1,1);
    double *vc = v->components;
    normalize_vec(v);

    cr_assert(vc[0] == 1/sqrt(3));
    cr_assert(vc[1] == 1/sqrt(3));
    cr_assert(vc[2] == 1/sqrt(3));

    rc_free_ref(v);
}

Test(Vectors, scale_vec) {
    vec *v = make_vec2d(1,2);
    scale_vec(2, v);
    double *vc = v->components;
    cr_assert(vc[0] == 2);
    cr_assert(vc[1] == 4); 
    rc_free_ref(v);
}

Test(Vectors, are_equal_vec) {
    vec *v1 = make_vec2d(1,1);
    vec *v2 = make_vec2d(1,1);
    cr_assert(are_equal_vec(v1, v2));
    vec *v3 = make_vec2d(1,2);
    cr_assert(!are_equal_vec(v1, v3));

    rc_free_ref(v1);
    rc_free_ref(v2);
    rc_free_ref(v3);
}

Test(Vectors, dot) {
    vec *v1 = make_vec2d(1,2);
    vec *v2 = make_vec2d(2,2);
    cr_assert(dot(v1,v2) == 6);

    vec *v3 = make_vec3d(1,1,1);
    cr_assert(dot(v1, v3) == INFINITY);

    rc_free_ref(v1);
    rc_free_ref(v2);
    rc_free_ref(v3);
}

Test(Vectors, are_orthogonal_vec) {
    vec *v1 = make_vec2d(1,0);
    vec *v2 = make_vec2d(0,1);
    cr_assert(are_orthogonal_vec(v1,v2));
    vec *v3 = make_vec2d(1,1);
    cr_assert(!are_orthogonal_vec(v1,v3));

    vec *v4 = make_vec3d(1,1,1);
    cr_assert(!are_orthogonal_vec(v1, v4));

    rc_free_ref(v1);
    rc_free_ref(v2);
    rc_free_ref(v3);
    rc_free_ref(v4);
}

Test(Vectors, vec_mag) {
    vec *v = make_vec2d(1,2);
    cr_assert(vec_mag(v) == sqrt(5));
    rc_free_ref(v);
}

Test(Vectors, vec_mag_squared) {
    vec *v = make_vec2d(1,3);
    cr_assert(fabs(vec_mag_squared(v) - 10) < EPSILON);
    rc_free_ref(v);
}
