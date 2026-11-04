#include "unity.h"
#include "basen.h"
#include <string.h>

void test_basen_parse_hex(void) {
    int32_t result;
    TEST_ASSERT_EQUAL_INT(ENG_OK, basen_parse("FF", 16, &result));
    TEST_ASSERT_EQUAL_INT(255, result);
}

void test_basen_parse_bin(void) {
    int32_t result;
    TEST_ASSERT_EQUAL_INT(ENG_OK, basen_parse("1010", 2, &result));
    TEST_ASSERT_EQUAL_INT(10, result);
}

void test_basen_format_dec(void) {
    char buf[16];
    basen_format(123, 10, buf, 16);
    TEST_ASSERT_EQUAL_STRING("123", buf);
}

void test_basen_logic_ops(void) {
    TEST_ASSERT_EQUAL_INT(0, basen_and(0x0F, 0x00));
    TEST_ASSERT_EQUAL_INT(0x0F, basen_or(0x0F, 0x00));
    TEST_ASSERT_EQUAL_INT(0x0F, basen_xor(0x0F, 0x00));
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_basen_parse_hex);
    RUN_TEST(test_basen_parse_bin);
    RUN_TEST(test_basen_format_dec);
    RUN_TEST(test_basen_logic_ops);
    return UNITY_END();
}
