#include <assert.h>
#include <string.h>
#include <test_string.h>

void test_strlen() {
    ASSERT(strlen("") == 0);
    ASSERT(strlen("abc") == 3);
    test_pass("test_strlen()");
}

void test_strcmp() {
    ASSERT(strcmp("", "") == 0);
    ASSERT(strcmp("a", "a") == 0);
    ASSERT(strcmp("hello", "hello") == 0);
    ASSERT(strcmp("a", "ab") < 0);
    ASSERT(strcmp("ab", "a") > 0);
    ASSERT(strcmp("abc", "abcd") < 0);
    ASSERT(strcmp("abcd", "abc") > 0);
    ASSERT(strcmp("a", "b") < 0);
    ASSERT(strcmp("abc", "abd") < 0);
    ASSERT(strcmp("b", "a") > 0);
    ASSERT(strcmp("abd", "abc") > 0);
    ASSERT(strcmp("a", "a\0b") == 0);  // should stop at \0
    ASSERT(strcmp("a\0c", "a") == 0);  // should compare only first 'a'
    ASSERT(strcmp("", "a") < 0);
    ASSERT(strcmp("a", "") > 0);
    test_pass("test_strcmp()");
}

void test_string() {
    test_strlen();
    test_strcmp();
}