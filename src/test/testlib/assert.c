#include <stdio.h>
#include <assert.h>

void test_fail(const char* file, int line, const char* expr) {
    terminal_writestring("[FAIL] ");
    terminal_writestring(expr);
    terminal_writestring(" at ");
    terminal_writestring(file);
    terminal_putchar(':');
    // Print line number (optional)
    for (;;); // halt
}

void test_pass(const char* name) {
    terminal_writestring("[PASS] ");
    terminal_writestring(name);
    terminal_putchar('\n');
}
