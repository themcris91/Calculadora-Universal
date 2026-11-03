#include <math.h>
#include <stddef.h>
#include "unity.h"
#include "api.h"

#define PI 3.14159265358979323846

void setUp(void) {}
void tearDown(void) {}

static void test_sumar(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 5.0, sumar(2.0, 3.0));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 0.0, sumar(-1.5, 1.5));
    TEST_ASSERT_DOUBLE_WITHIN(1e-15, 0.3, sumar(0.1, 0.2));
}

static void test_integral_sin(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, 2.0, integral_sin(0.0, PI, 1000));
    TEST_ASSERT_DOUBLE_WITHIN(1e-2, 2.0, integral_sin(0.0, PI, 7)); /* n impar se ajusta */
}

static void test_integral_sin_invalid_n_returns_nan(void)
{
    TEST_ASSERT(isnan(integral_sin(0.0, PI, 0)));
    TEST_ASSERT(isnan(integral_sin(0.0, PI, -3)));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sumar);
    RUN_TEST(test_integral_sin);
    RUN_TEST(test_integral_sin_invalid_n_returns_nan);
    return UNITY_END();
}
