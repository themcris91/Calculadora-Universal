#include <math.h>
#include <stddef.h>
#include "unity.h"
#include "numeric.h"
#include "status.h"

#define PI 3.14159265358979323846
#define E_CONST 2.71828182845904523536

void setUp(void) {}
void tearDown(void) {}

/* ---- funciones de prueba ---- */
static double f_sin(double x, void *c) { (void)c; return sin(x); }
static double f_sq(double x, void *c) { (void)c; return x * x; }
static double f_cube(double x, void *c) { (void)c; return x * x * x; }
static double f_exp(double x, void *c) { (void)c; return exp(x); }
static double f_inv(double x, void *c) { (void)c; return 1.0 / x; }
static double f_sqrt(double x, void *c) { (void)c; return sqrt(x); }
static double f_nan(double x, void *c) { (void)x; (void)c; return NAN; }
static double f_count(double x, void *c) { (*(int *)c)++; return x; }
static double f_sq_m2(double x, void *c) { (void)c; return x * x - 2.0; }
static double f_sq_m2_d(double x, void *c) { (void)c; return 2.0 * x; }
static double f_sq_m4(double x, void *c) { (void)c; return x * x - 4.0; }
static double f_cosmx(double x, void *c) { (void)c; return cos(x) - x; }
static double f_cosmx_d(double x, void *c) { (void)c; return -sin(x) - 1.0; }
static double f_cubic(double x, void *c) { (void)c; return x * x * x - x - 1.0; }
static double f_atan(double x, void *c) { (void)c; return atan(x); }
static double f_atan_d(double x, void *c) { (void)c; return 1.0 / (1.0 + x * x); }
static double f_flat(double x, void *c) { (void)c; return x * x + 1.0; }
static double f_flat_d(double x, void *c) { (void)c; return 2.0 * x; }
static double f_xm3(double x, void *c) { (void)c; return x - 3.0; }
static double f_log_dom(double x, void *c) { (void)c; return log(x); }

/* ---------------------------------------------------------------- Simpson */
static void test_simpson_sin(void)
{
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, 0.0, PI, 1000, &r));
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, 2.0, r);
}

static void test_simpson_exact_for_polynomials(void)
{
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sq, NULL, 0.0, 3.0, 2, &r));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 9.0, r);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_cube, NULL, 0.0, 2.0, 2, &r));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 4.0, r);
}

static void test_simpson_odd_n_is_forced_even(void)
{
    double r7 = 0.0, r8 = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, 0.0, PI, 7, &r7));
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, 0.0, PI, 8, &r8));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, r8, r7);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, 0.0, PI, 1, &r7));
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, 0.0, PI, 2, &r8));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, r8, r7);
}

static void test_simpson_degenerate_and_reversed(void)
{
    double r = 1.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, 1.5, 1.5, 10, &r));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 0.0, r);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_sin, NULL, PI, 0.0, 1000, &r));
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, -2.0, r);
}

static void test_simpson_passes_ctx_and_evaluates_n_plus_1(void)
{
    int calls = 0;
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_simpson(f_count, &calls, 0.0, 1.0, 10, &r));
    TEST_ASSERT_EQUAL_INT(11, calls);
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.5, r);
}

static void test_simpson_errors(void)
{
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_simpson(NULL, NULL, 0.0, 1.0, 10, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_simpson(f_sin, NULL, 0.0, 1.0, 10, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_simpson(f_sin, NULL, 0.0, 1.0, 0, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_simpson(f_sin, NULL, 0.0, 1.0, -5, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_simpson(f_sin, NULL, 0.0, 1.0, NUM_SIMPSON_MAX_N + 1, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_simpson(f_nan, NULL, 0.0, 1.0, 10, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_simpson(f_sin, NULL, NAN, 1.0, 10, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_simpson(f_sin, NULL, 0.0, INFINITY, 10, &r));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_simpson(f_sin, NULL, -1e308, 1e308, 10, &r));
    /* 1/x en [-1,1] con n par pasa por x=0: f(0)=inf */
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_simpson(f_inv, NULL, -1.0, 1.0, 10, &r));
}

/* ------------------------------------------------------------- Adaptativo */
static void test_adaptive_exp(void)
{
    double r = 0.0, err = -1.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_integrate_adaptive(f_exp, NULL, 0.0, 1.0, 1e-10, 30, &r, &err));
    TEST_ASSERT_DOUBLE_WITHIN(1e-10, E_CONST - 1.0, r);
    TEST_ASSERT(err >= 0.0 && err < 1e-9);
}

static void test_adaptive_inverse(void)
{
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_integrate_adaptive(f_inv, NULL, 1.0, E_CONST, 1e-10, 30, &r, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-10, 1.0, r);
}

static void test_adaptive_sqrt_singular_derivative(void)
{
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_integrate_adaptive(f_sqrt, NULL, 0.0, 1.0, 1e-8, 80, &r, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-8, 2.0 / 3.0, r);
}

static void test_adaptive_depth_exhausted_returns_partial(void)
{
    double r = 0.0, err = 0.0;
    int st = num_integrate_adaptive(f_sqrt, NULL, 0.0, 1.0, 1e-12, 5, &r, &err);
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, st);
    TEST_ASSERT_DOUBLE_WITHIN(1e-2, 2.0 / 3.0, r);
    TEST_ASSERT(err > 0.0);
}

static void test_adaptive_reversed_and_degenerate(void)
{
    double r = 0.0, err = 9.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_integrate_adaptive(f_exp, NULL, 1.0, 0.0, 1e-10, 30, &r, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-10, -(E_CONST - 1.0), r);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_integrate_adaptive(f_exp, NULL, 2.0, 2.0, 1e-10, 30, &r, &err));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 0.0, r);
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 0.0, err);
}

static void test_adaptive_errors(void)
{
    double r = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_integrate_adaptive(NULL, NULL, 0.0, 1.0, 1e-8, 10, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_integrate_adaptive(f_sin, NULL, 0.0, 1.0, 1e-8, 10, NULL, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_integrate_adaptive(f_sin, NULL, 0.0, 1.0, 0.0, 10, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_integrate_adaptive(f_sin, NULL, 0.0, 1.0, NAN, 10, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_integrate_adaptive(f_sin, NULL, 0.0, 1.0, 1e-8, 0, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_integrate_adaptive(f_sin, NULL, 0.0, 1.0, 1e-8, NUM_ADAPT_MAX_DEPTH + 1, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_integrate_adaptive(f_sin, NULL, NAN, 1.0, 1e-8, 10, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_integrate_adaptive(f_nan, NULL, 0.0, 1.0, 1e-8, 10, &r, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_integrate_adaptive(f_inv, NULL, 0.0, 1.0, 1e-8, 10, &r, NULL));
}

/* --------------------------------------------------------------- Bisección */
static void test_bisection_sqrt2(void)
{
    double root = 0.0;
    int it = 0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_bisection(f_sq_m2, NULL, 0.0, 2.0, 1e-13, 100, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, sqrt(2.0), root);
    TEST_ASSERT(it > 0 && it <= 60);
}

static void test_bisection_reversed_bracket(void)
{
    double root = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_bisection(f_sq_m2, NULL, 2.0, 0.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, sqrt(2.0), root);
}

static void test_bisection_exact_root_at_endpoint(void)
{
    double root = 0.0;
    int it = -1;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_bisection(f_sq_m4, NULL, 2.0, 3.0, 1e-12, 100, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 2.0, root);
    TEST_ASSERT_EQUAL_INT(0, it);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_bisection(f_sq_m4, NULL, 0.0, 2.0, 1e-12, 100, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 2.0, root);
    TEST_ASSERT_EQUAL_INT(0, it);
}

static void test_bisection_errors(void)
{
    double root = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_BRACKET, num_root_bisection(f_sq_m2, NULL, 2.0, 3.0, 1e-9, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_bisection(f_sq_m2, NULL, 0.0, 2.0, 1e-13, 3, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_bisection(NULL, NULL, 0.0, 2.0, 1e-9, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_bisection(f_sq_m2, NULL, 0.0, 2.0, 1e-9, 100, NULL, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_bisection(f_sq_m2, NULL, 0.0, 2.0, 0.0, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_bisection(f_sq_m2, NULL, 0.0, 2.0, 1e-9, 0, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_bisection(f_sq_m2, NULL, NAN, 2.0, 1e-9, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_bisection(f_nan, NULL, 0.0, 2.0, 1e-9, 100, &root, NULL));
    /* log(x) en [-1, 2]: f(-1)=NaN */
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_bisection(f_log_dom, NULL, -1.0, 2.0, 1e-9, 100, &root, NULL));
}

/* ------------------------------------------------------------------- Brent */
static void test_brent_roots(void)
{
    double root = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_sq_m2, NULL, 0.0, 2.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, sqrt(2.0), root);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_cubic, NULL, 1.0, 2.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 1.3247179572447460, root);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_cosmx, NULL, 0.0, 1.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.7390851332151607, root);
}

static void test_brent_needs_fewer_iterations_than_bisection(void)
{
    double r1 = 0.0, r2 = 0.0;
    int ib = 0, ibr = 0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_bisection(f_cubic, NULL, 1.0, 2.0, 1e-12, 200, &r1, &ib));
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_cubic, NULL, 1.0, 2.0, 1e-12, 200, &r2, &ibr));
    TEST_ASSERT(ibr < ib);
    TEST_ASSERT_DOUBLE_WITHIN(1e-11, r1, r2);
}

static void test_brent_endpoints_and_orientation(void)
{
    double root = 0.0;
    int it = -1;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_sq_m4, NULL, 2.0, 3.0, 1e-12, 100, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 2.0, root);
    TEST_ASSERT_EQUAL_INT(0, it);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_sq_m4, NULL, 0.0, 2.0, 1e-12, 100, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 2.0, root);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_brent(f_sq_m2, NULL, 2.0, 0.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, sqrt(2.0), root);
}

static void test_brent_errors(void)
{
    double root = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_BRACKET, num_root_brent(f_sq_m2, NULL, 2.0, 3.0, 1e-9, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_brent(f_cubic, NULL, 1.0, 2.0, 1e-13, 2, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_brent(NULL, NULL, 0.0, 2.0, 1e-9, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_brent(f_sq_m2, NULL, 0.0, 2.0, -1.0, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_brent(f_sq_m2, NULL, 0.0, INFINITY, 1e-9, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_brent(f_nan, NULL, 0.0, 2.0, 1e-9, 100, &root, NULL));
}

/* ------------------------------------------------------------------ Newton */
static void test_newton_cos_minus_x(void)
{
    double root = 0.0;
    int it = 0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_newton(f_cosmx, f_cosmx_d, NULL, 1.0, 1e-13, 50, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.7390851332151607, root);
    TEST_ASSERT(it > 0 && it < 10);
}

static void test_newton_numeric_derivative(void)
{
    double root = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_newton(f_cosmx, NULL, NULL, 1.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.7390851332151607, root);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_newton(f_sq_m2, NULL, NULL, 1.0, 1e-13, 100, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, sqrt(2.0), root);
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_newton(f_sq_m2, f_sq_m2_d, NULL, 1.0, 1e-14, 50, &root, NULL));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, sqrt(2.0), root);
}

static void test_newton_exact_start(void)
{
    double root = 0.0;
    int it = -1;
    TEST_ASSERT_EQUAL_INT(ENG_OK, num_root_newton(f_xm3, NULL, NULL, 3.0, 1e-12, 50, &root, &it));
    TEST_ASSERT_DOUBLE_WITHIN(0.0, 3.0, root);
    TEST_ASSERT_EQUAL_INT(0, it);
}

static void test_newton_divergence_and_flat_derivative(void)
{
    double root = 0.0;
    /* atan con x0=2 diverge */
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_newton(f_atan, f_atan_d, NULL, 2.0, 1e-12, 100, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_newton(f_atan, NULL, NULL, 2.0, 1e-12, 100, &root, NULL));
    /* f'(0)=0 con f=x^2+1 */
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_newton(f_flat, f_flat_d, NULL, 0.0, 1e-12, 50, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_newton(f_flat, NULL, NULL, 0.0, 1e-12, 50, &root, NULL));
    /* sin iteraciones suficientes */
    TEST_ASSERT_EQUAL_INT(ENG_ERR_NO_CONVERGE, num_root_newton(f_cosmx, f_cosmx_d, NULL, 1.0, 1e-13, 1, &root, NULL));
}

static void test_newton_errors(void)
{
    double root = 0.0;
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_newton(NULL, NULL, NULL, 1.0, 1e-9, 50, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_newton(f_cosmx, NULL, NULL, 1.0, 1e-9, 50, NULL, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_newton(f_cosmx, NULL, NULL, 1.0, 0.0, 50, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_ARG, num_root_newton(f_cosmx, NULL, NULL, 1.0, 1e-9, 0, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_newton(f_cosmx, NULL, NULL, NAN, 1e-9, 50, &root, NULL));
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_newton(f_nan, NULL, NULL, 1.0, 1e-9, 50, &root, NULL));
    /* log(x) en x0=-1 -> NaN */
    TEST_ASSERT_EQUAL_INT(ENG_ERR_DOMAIN, num_root_newton(f_log_dom, NULL, NULL, -1.0, 1e-9, 50, &root, NULL));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_simpson_sin);
    RUN_TEST(test_simpson_exact_for_polynomials);
    RUN_TEST(test_simpson_odd_n_is_forced_even);
    RUN_TEST(test_simpson_degenerate_and_reversed);
    RUN_TEST(test_simpson_passes_ctx_and_evaluates_n_plus_1);
    RUN_TEST(test_simpson_errors);
    RUN_TEST(test_adaptive_exp);
    RUN_TEST(test_adaptive_inverse);
    RUN_TEST(test_adaptive_sqrt_singular_derivative);
    RUN_TEST(test_adaptive_depth_exhausted_returns_partial);
    RUN_TEST(test_adaptive_reversed_and_degenerate);
    RUN_TEST(test_adaptive_errors);
    RUN_TEST(test_bisection_sqrt2);
    RUN_TEST(test_bisection_reversed_bracket);
    RUN_TEST(test_bisection_exact_root_at_endpoint);
    RUN_TEST(test_bisection_errors);
    RUN_TEST(test_brent_roots);
    RUN_TEST(test_brent_needs_fewer_iterations_than_bisection);
    RUN_TEST(test_brent_endpoints_and_orientation);
    RUN_TEST(test_brent_errors);
    RUN_TEST(test_newton_cos_minus_x);
    RUN_TEST(test_newton_numeric_derivative);
    RUN_TEST(test_newton_exact_start);
    RUN_TEST(test_newton_divergence_and_flat_derivative);
    RUN_TEST(test_newton_errors);
    return UNITY_END();
}
