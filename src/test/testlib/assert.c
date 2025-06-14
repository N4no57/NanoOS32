#include <stdio.h>

void test_fail(const char* file, int line, const char* expr) {
    printf("[FAIL] %s at %s : %d", expr, file, line);
    for (;;);
}

void test_pass(const char* name) {
    printf("[PASS] %s\n", name);
}
