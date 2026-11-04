#include "unity.h"
#include "linalg.h"

void test_det_2x2(void) {
    double m[] = {1.0, 2.0, 3.0, 4.0}; // det = 1*4 - 2*3 = -2
    double det;
    la_det(m, 2, &det);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -2.0, det);
}

void test_inv_2x2(void) {
    double m[] = {4.0, 7.0, 2.0, 6.0}; // det = 10. inv = {0.6, -0.7, -0.2, 0.4}
    double inv[4];
    la_inv(m, 2, inv);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.6, inv[0]);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -0.7, inv[1]);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, -0.2, inv[2]);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.4, inv[3]);
}

void test_mul_2x2(void) {
    double a[] = {1.0, 2.0, 3.0, 4.0};
    double b[] = {2.0, 0.0, 1.0, 2.0};
    double out[4];
    la_mul(a, 2, 2, b, 2, out);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 4.0, out[0]);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 4.0, out[1]);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 10.0, out[2]);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 8.0, out[3]);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_det_2x2);
    RUN_TEST(test_inv_2x2);
    RUN_TEST(test_mul_2x2);
    return UNITY_END();
}
