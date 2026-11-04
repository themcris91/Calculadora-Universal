#include <math.h>
#include <stddef.h>
#include "api.h"
#include "numeric.h"
#include "status.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE /* build nativo (tests unitarios) */
#endif

static double sin_cb(double x, void *ctx)
{
    (void)ctx;
    return sin(x);
}

EMSCRIPTEN_KEEPALIVE
double sumar(double a, double b)
{
    return a + b;
}

EMSCRIPTEN_KEEPALIVE
double integral_sin(double a, double b, int n)
{
    double out = 0.0;
    if (num_simpson(sin_cb, NULL, a, b, n, &out) != ENG_OK) {
        return NAN;
    }
    return out;
}
