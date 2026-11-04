#include <stddef.h>
#include <string.h>
#include "unity.h"
#include "status.h"

void setUp(void) {}
void tearDown(void) {}

static void test_ok_message(void)
{
    TEST_ASSERT_EQUAL_STRING("ok", eng_status_str(ENG_OK));
}

static void test_all_codes_have_distinct_nonempty_messages(void)
{
    const int codes[] = {ENG_OK, ENG_ERR_ARG, ENG_ERR_DOMAIN, ENG_ERR_NO_BRACKET,
                         ENG_ERR_NO_CONVERGE, ENG_ERR_SINGULAR, ENG_ERR_RANGE, ENG_ERR_BUFFER};
    const int n = (int)(sizeof codes / sizeof codes[0]);
    int i, j;
    for (i = 0; i < n; i++) {
        TEST_ASSERT_NOT_NULL(eng_status_str(codes[i]));
        TEST_ASSERT(strlen(eng_status_str(codes[i])) > 0);
        for (j = i + 1; j < n; j++) {
            TEST_ASSERT(strcmp(eng_status_str(codes[i]), eng_status_str(codes[j])) != 0);
        }
    }
}

static void test_unknown_code(void)
{
    TEST_ASSERT_EQUAL_STRING("estado desconocido", eng_status_str(-999));
    TEST_ASSERT_EQUAL_STRING("estado desconocido", eng_status_str(42));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_ok_message);
    RUN_TEST(test_all_codes_have_distinct_nonempty_messages);
    RUN_TEST(test_unknown_code);
    return UNITY_END();
}
