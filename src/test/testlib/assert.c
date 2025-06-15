#include <stdio.h>

void test_fail(const char* file, int line, const char* expr) {
    printf("[FAIL] %s at '%s' : %d", expr, file, line);
    for (;;);
}

void test_pass(const char* name, const int test_num, const int test_tot) {
    printf("[PASS] %s [%d/%d]\n", name, test_num, test_tot);
}
