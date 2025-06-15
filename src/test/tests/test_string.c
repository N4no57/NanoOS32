#include <assert.h>
#include <string.h>
#include <test_string.h>

void test_strlen() {
    ASSERT(strlen("") == 0);
    ASSERT(strlen("abc") == 3);
    test_pass("test_strlen()\n");
}