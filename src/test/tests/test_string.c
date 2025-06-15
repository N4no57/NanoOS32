#include <assert.h>
#include <string.h>
#include <test_string.h>

void test_strlen() {
    ASSERT(strlen("") == 0);
    ASSERT(strlen("a") == 1);
    ASSERT(strlen("abc") == 3);
    ASSERT(strlen("a b c") == 5);
    ASSERT(strlen("@#$%^") == 5);
    ASSERT(strlen("a\0bcd") == 1);
    ASSERT(strlen("abcdefghijklmnopqrstuvwxyz") == 26);

    
    char buf[256];
    for (int i = 0; i < 255; i++) buf[i] = 'x';
    buf[255] = '\0';
    ASSERT(strlen(buf) == 255);

    char all_nulls[4] = {0, 0, 0, 0};
    ASSERT(strlen(all_nulls) == 0);

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

void test_strcpy() {
    char str1[] = "hello";
    char str2[10];
    char* ret = strcpy(str2, str1);

    ASSERT(ret == str2);             // returned pointer is correct
    ASSERT(strcmp(str2, str1) == 0); // content is copied correctly

    test_pass("test_strcpy()");
}

void test_string() {
    test_strlen();
    test_strcmp();
    test_strcpy();
}