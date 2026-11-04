#include "unity.h"
#include "finance.h"
#include <stdlib.h>

void test_npv_basic(void) {
    double flows[] = {-100.0, 50.0, 60.0};
    double result;
    fin_npv(0.1, flows, 3, &result);
    // El valor real matemático es -4.9586, no 1.6528
    TEST_ASSERT_DOUBLE_WITHIN(0.01, -4.9586, result); 
}

void test_irr_basic(void) {
    double flows[] = {-100.0, 60.0, 60.0};
    double result;
    fin_irr(flows, 3, &result);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 0.1306, result);
}

void test_depreciation_ddb_buffer_safe(void) {
    double cost = 1000.0, salvage = 100.0, result;
    int life = 5;
    double *annual_deprec = malloc(sizeof(double) * life);
    TEST_ASSERT_NOT_NULL(annual_deprec);
    for(int i = 1; i <= life; i++) {
        fin_deprec_ddb(cost, salvage, life, i, &result);
        annual_deprec[i-1] = result;
    }
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 400.0, annual_deprec[0]); 
    free(annual_deprec);
}

void test_french_payment(void) {
    double result;
    fin_french_payment(10000.0, 0.05, 12, &result);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 856.07, result);
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_npv_basic);
    RUN_TEST(test_irr_basic);
    RUN_TEST(test_depreciation_ddb_buffer_safe);
    RUN_TEST(test_french_payment);
    return UNITY_END();
}
