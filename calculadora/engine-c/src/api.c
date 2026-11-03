#include <math.h>
#include <stddef.h>
#include "api.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE /* build nativo (tests unitarios) */
#endif

EMSCRIPTEN_KEEPALIVE
double sumar(double a, double b)
{
    return a + b;
}

/* Regla compuesta de Simpson 1/3. n se fuerza a par y >= 2. */
EMSCRIPTEN_KEEPALIVE
double integral_simpson(double (*f)(double), double a, double b, int n)
{
    if (f == NULL) {
        return NAN;
    }
    if (n < 2) {
        n = 2;
    }
    if (n % 2 != 0) {
        n += 1;
    }

    const double h = (b - a) / (double)n;
    double acc = f(a) + f(b);

    for (int i = 1; i < n; i++) {
        acc += f(a + (double)i * h) * ((i % 2 == 0) ? 2.0 : 4.0);
    }
    return acc * h / 3.0;
}

/* Wrapper concreto: JS no puede pasar punteros a función C directamente. */
EMSCRIPTEN_KEEPALIVE
double integral_sin(double a, double b, int n)
{
    return integral_simpson(sin, a, b, n);
}
