#include <criterion/criterion.h>
#include "../basis/matrix.h"
#include "../basis/refcount.h"

Test(Matrix, read_matrix) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix1.txt");
    matrix *m = new_matrix(3,3);
    
    set_matrix_val(m, 1, 0, 0);
    set_matrix_val(m, 2, 0, 1);
    set_matrix_val(m, 3, 0, 2);

    set_matrix_val(m, 2, 1, 0);
    set_matrix_val(m, 3, 1, 1);
    set_matrix_val(m, 4, 1, 2);

    set_matrix_val(m, 4, 2, 0);
    set_matrix_val(m, 5, 2, 1);
    set_matrix_val(m, 6, 2, 2);

    cr_assert(are_equal_matrices(read->matrices[0], m)); 

    rc_free_ref(read);
    rc_free_ref(m);
}

Test(Matrix, scalar_mult) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix1.txt");
    scalar_mult(read->matrices[0], 2);
    matrix *m = new_matrix(3,3);
    
    set_matrix_val(m, 2, 0, 0);
    set_matrix_val(m, 4, 0, 1);
    set_matrix_val(m, 6, 0, 2);

    set_matrix_val(m, 4, 1, 0);
    set_matrix_val(m, 6, 1, 1);
    set_matrix_val(m, 8, 1, 2);

    set_matrix_val(m, 8, 2, 0);
    set_matrix_val(m, 10, 2, 1);
    set_matrix_val(m, 12, 2, 2);

    cr_assert(are_equal_matrices(read->matrices[0], m)); 

    rc_free_ref(read);
    rc_free_ref(m);
}

Test(Matrix, matrix_mult) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix2.txt");
    matrix *res = matrix_mult(read->matrices[0], read->matrices[1]);
    cr_assert(are_equal_matrices(res, read->matrices[2]));
    rc_free_ref(read);
    rc_free_ref(res);
}