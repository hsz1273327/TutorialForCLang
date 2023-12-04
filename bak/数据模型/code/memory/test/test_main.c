#include <stdbool.h>
#include <stdio.h>
#include "minunit.h"
#include "memory.h"

typedef struct T{
    int x;
    int y;
} *T_p;

MU_TEST(test_memcmp_not) {
    char * a1 = "asdfg";
    char * a2 = "qwert";
    mu_check(Mem_memcmp(a1,a2,5)); 
}

MU_TEST(test_memcmp_yes) {
    char * a1 = "asdfg";
    char * a2 = "asdfg123";
    mu_check(Mem_memcmp(a1,a2,5)); 
}

MU_TEST(test_memmove) {
    char * a1 = "asdfg";
    char * p;
    mu_check((char *)Mem_memmove(p,a1,5) == "asdfg"); 
}

MU_TEST(test_alloc) {
    T_p test_struct;
    NEW(test_struct);
    FREE(test_struct)
}

MU_TEST(test_calloc) {
    mu_check(_test_exception(0) == 2);
}


MU_TEST(test_resize) {
    mu_check(_test_finally(-1) == 3); 
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_check_exception_user_exception);
    MU_RUN_TEST(test_check_exception_excption_else);
    MU_RUN_TEST(test_asert);
    MU_RUN_TEST(test_asert_false);
    MU_RUN_TEST(test_check_finally_exception);
    MU_RUN_TEST(test_check_finally_else);
    MU_RUN_TEST(test_check_finally_no_exception);

}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}