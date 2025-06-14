#define ASSERT(x) do { \
    if (!(x)) test_fail(__FILE__, __LINE__, #x); \
} while (0)

void test_fail(const char* file, int line, const char* expr);
void test_pass(const char* name);