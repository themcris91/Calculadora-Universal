#include "unity.h"
#include "complexnum.h"
#include <math.h>

void test_rect_to_polar_rad(void) {
    double r, theta;
    cx_rect_to_polar(3.0, 4.0, ANGLE_RAD, &r, &theta);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 5.0, r);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.927295, theta);
}

void test_polar_to_rect_deg(void) {
    double re, im;
    cx_polar_to_rect(2.0, 90.0, ANGLE_DEG, &re, &im);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 0.0, re);
    TEST_ASSERT_DOUBLE_WITHIN(0.001, 2.0, im);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_rect_to_polar_rad);
    RUN_TEST(test_polar_to_rect_deg);
    return UNITY_END();
}
