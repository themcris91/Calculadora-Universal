#include <emscripten.h>
#include <math.h>
#include "numeric.h"
#include "finance.h"
#include "linalg.h"
#include "complexnum.h"
#include "basen.h"

EMSCRIPTEN_KEEPALIVE
int api_fin_npv(double rate, double* cashflows, int periods, double* result) {
    return fin_npv(rate, cashflows, periods, result);
}

EMSCRIPTEN_KEEPALIVE
int api_la_det(double* matrix, int n, double* det) {
    return la_det(matrix, n, det);
}

EMSCRIPTEN_KEEPALIVE
int api_cx_rect_to_polar(double re, double im, int unit, double* out_r, double* out_theta) {
    return cx_rect_to_polar(re, im, (angle_unit)unit, out_r, out_theta);
}

EMSCRIPTEN_KEEPALIVE
int api_basen_convert(const char* str, int from_base, int to_base, char* out_buf, int buf_size) {
    int32_t val;
    int status = basen_parse(str, from_base, &val);
    if (status != ENG_OK) return status;
    return basen_format(val, to_base, out_buf, buf_size);
}

static double sin_wrapper(double x, void* ctx) { return sin(x); }

EMSCRIPTEN_KEEPALIVE
int api_num_integral_sin(double a, double b, int n, double* result) {
    return num_simpson(sin_wrapper, NULL, a, b, n, result);
}
