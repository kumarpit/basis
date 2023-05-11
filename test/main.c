#include <stdio.h>
#include "../basis/vec.h"

int main() {
    vec *v1 = new_vec(3);
    v1->components[0] = 2;
    v1->components[1] = 3;
    v1->components[2] = 4;

    vec *v2 = new_vec(3);
    v2->components[0] = 3;
    v2->components[1] = 4;
    v2->components[2] = 5;

    vec *sum = add_vec(v1, v2);

    for (int i=0; i < sum->dim; i++) {
        printf("%f \n", sum->components[i]);
    }
}