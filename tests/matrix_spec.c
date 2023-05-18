#include <criterion/criterion.h>
#include "../basis/matrix.h"
#include "../basis/refcount.h"

Test(Matrix, read_matrix) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix1.txt");
    matrix *m1 = new_matrix(3,3);
    matrix *m2 = new_matrix(2,1);
    
    set_matrix_val(m1, 1, 0, 0);
    set_matrix_val(m1, 2, 0, 1);
    set_matrix_val(m1, 3, 0, 2);

    set_matrix_val(m1, 2, 1, 0);
    set_matrix_val(m1, 3, 1, 1);
    set_matrix_val(m1, 4, 1, 2);

    set_matrix_val(m1, 4, 2, 0);
    set_matrix_val(m1, 5, 2, 1);
    set_matrix_val(m1, 6, 2, 2);

    set_matrix_val(m2, 1, 0, 0);
    set_matrix_val(m2, 0, 1, 0);

    cr_assert(read->count == 2);
    cr_assert(are_equal_matrices(read->matrices[0], m1));
    cr_assert(are_equal_matrices(read->matrices[1], m2));

    rc_free_ref(read);
    rc_free_ref(m1);
    rc_free_ref(m2);
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

Test(Matrix, get_matrix_col) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix3.txt");
    matrix *col = get_matrix_col(read->matrices[0], 1);
    cr_assert(are_equal_matrices(read->matrices[1], col));
    rc_free_ref(read);
    rc_free_ref(col);
}

Test(Matrix, get_matrix_row) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix4.txt");
    matrix *row = get_matrix_row(read->matrices[0], 0);
    cr_assert(are_equal_matrices(read->matrices[1], row));
    rc_free_ref(read);
    rc_free_ref(row);
}