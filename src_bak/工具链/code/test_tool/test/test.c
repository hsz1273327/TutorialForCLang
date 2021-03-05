#include "minunit.h"
#include "binary_vector.h"

struct BINARY_VECTOR A = {1.0,2.0},B = {3.0,4.0};

MU_TEST(test_check_modA) {
    mu_check(VEC_mod(&A) == 2.236068);
}
MU_TEST(test_check_modB) {
    mu_check(VEC_mod(&B) == 5.000000);
}
MU_TEST(test_check_mulA_B) {
    mu_check(VEC_mul(&A,&B) == 11.000000);
}
MU_TEST(test_check_addA_B) {
    BINARY_VECTOR_P result = VEC_add(&A,&B);
    mu_check(result->x == 4.000000);
    mu_check(result->y == 6.000000);
}
MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_check_modA);
    MU_RUN_TEST(test_check_modB);
    MU_RUN_TEST(test_check_mulA_B);
    MU_RUN_TEST(test_check_addA_B);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}