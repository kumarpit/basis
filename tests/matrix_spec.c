#include <criterion/criterion.h>
#include "../basis/matrix.h"
#include "../basis/refcount.h"

// TODO: UPDATE TESTS TO TEST FOR ERROR CASES

int pivot_idx(matrix *m, uint row, uint col);

Test(Matrix, read_matrix) {
    fmatrices *read = read_matrix("tests/test_matrices/read.data");
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
    fmatrices *read = read_matrix("tests/test_matrices/read.data");
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
    fmatrices *read = read_matrix("tests/test_matrices/mat_mult.data");
    matrix *res = matrix_mult(read->matrices[0], read->matrices[1]);
    cr_assert(are_equal_matrices(res, read->matrices[2]));
    rc_free_ref(read);
    rc_free_ref(res);
}

Test(Matrix, get_matrix_col) {
    fmatrices *read = read_matrix("tests/test_matrices/get_col.data");
    matrix *col = get_matrix_col(read->matrices[0], 1);
    cr_assert(are_equal_matrices(read->matrices[1], col));
    rc_free_ref(read);
    rc_free_ref(col);
}

Test(Matrix, get_matrix_row) {
    fmatrices *read = read_matrix("tests/test_matrices/get_row.data");
    matrix *row = get_matrix_row(read->matrices[0], 0);
    cr_assert(are_equal_matrices(read->matrices[1], row));
    rc_free_ref(read);
    rc_free_ref(row);
}

Test(Matrix, add_row) {
    fmatrices *read = read_matrix("tests/test_matrices/row_ops.data");
    matrix *initial = read->matrices[0];
    add_row(initial, 0, 2, 2);
    cr_assert(are_equal_matrices(read->matrices[1], initial));
    rc_free_ref(read);
}

Test(Matrix, swap_row) {
    fmatrices *read = read_matrix("tests/test_matrices/row_ops.data");
    matrix *initial = read->matrices[0];
    swap_row(initial, 0, 1);
    cr_assert(are_equal_matrices(read->matrices[2], initial));
    rc_free_ref(read);
}

Test(Matrix, normalize_row) {
    fmatrices *read = read_matrix("tests/test_matrices/row_ops.data");
    matrix *initial = read->matrices[0];
    normalize_row(initial, 1);
    cr_assert(are_equal_matrices(read->matrices[3], initial));
    rc_free_ref(read);
}

Test(Matrix, mult_row) {
    fmatrices *read = read_matrix("tests/test_matrices/row_ops.data");
    matrix *initial = read->matrices[0];
    mult_row(initial, 2, 0);
    cr_assert(are_equal_matrices(read->matrices[4], initial));
    rc_free_ref(read);
}

Test(Matrix, pivot_idx) {
    fmatrices *read = read_matrix("tests/test_matrices/pivot_idx.data");
    matrix *m = read->matrices[0];
    cr_assert(pivot_idx(m, 0, 0) == 2);
}

Test(Matrix, ref) {
    fmatrices *read = read_matrix("tests/test_matrices/ref.data");
    matrix *initial = read->matrices[0];
    matrix *after_ref = read->matrices[1];
    ref(initial);
    print_matrix(initial);
    cr_assert(are_equal_matrices(after_ref, initial));
}