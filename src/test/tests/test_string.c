#include <assert.h>
#include <string.h>
#include <test_string.h>
#include <stdio.h>
#include <terminal.h>

void test_strlen(const int test_num, const int test_tot) {
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

    test_pass("test_strlen()", test_num, test_tot);
}

void test_strcmp(const int test_num, const int test_tot) {
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
    test_pass("test_strcmp()", test_num, test_tot);
}

void test_strcpy(const int test_num, const int test_tot) {
    char str1[] = "hello";
    char str2[10];
    char* ret = strcpy(str2, str1);

    ASSERT(ret == str2);             // returned pointer is correct
    ASSERT(strcmp(str2, str1) == 0); // content is copied correctly

    test_pass("test_strcpy()", test_num, test_tot);
}

void test_memset(const int test_num, const int test_tot) {
    char str[10];
    char set_to = 0;
    char* ret = memset(str, set_to, sizeof(str));

    ASSERT(ret == str);

    // Check that every byte was set to 1
    for (int i = 0; i < (int)sizeof(str); i++) {
        ASSERT(str[i] == set_to);
    }

    test_pass("test_memset()", test_num, test_tot);
}

void test_memcpy(const int test_num, const int test_tot) {
    const char *str1 = "132452";
    size_t len = strlen(str1) + 1;

    char str2[10];
    memset(str2, 0xAA, sizeof(str2)); // known filler

    str2[len] = 0xCC; // canary

    char* ret = memcpy(str2, str1, len);
    ASSERT(ret == str2);

    for (size_t i = 0; i < len; i++) {
        ASSERT(str2[i] == str1[i]);
    }

    ASSERT((unsigned char)str2[7] == 0xCC); // check no overflow

    test_pass("test_memcpy()", test_num, test_tot);
}


void test_string() {
    int test_tot = 5;
    int test_num = 1;
    test_strlen(test_num++, test_tot);
    test_strcmp(test_num++, test_tot);
    test_strcpy(test_num++, test_tot);
    test_memset(test_num++, test_tot);
    test_memcpy(test_num++, test_tot);
}