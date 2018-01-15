#include "minunit.h"
#include "stack.h"
#include <stdbool.h>


MU_TEST(test_check_stack) {

    Stack_T names = Stack_new();
    Stack_push(names,"a")
    Stack_push(names,"b")
    Stack_push(names,"c")
    mu_check(Stack_empty(names) == 0);
    mu_check((char*) Stack_pop(names) == "c");
    mu_check((char*) Stack_pop(names) == "b");
    mu_check((char*) Stack_pop(names) == "a");
    mu_check(Stack_empty(names) == 1);
    Stack_free(names);
}


MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_check_stack);

}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}