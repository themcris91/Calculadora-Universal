#include <emscripten.h>
#include "numeric.h"
#include "finance.h"
#include "linalg.h"

// Puente para Finanzas (VAN)
EMSCRIPTEN_KEEPALIVE
int api_fin_npv(double rate, double* cashflows, int periods, double* result) {
    return fin_npv(rate, cashflows, periods, result);
}

// Puente para Álgebra Lineal (Determinante)
EMSCRIPTEN_KEEPALIVE
int api_la_det(double* matrix, int n, double* det) {
    return la_det(matrix, n, det);
}

// Puente de prueba (Fase 0)
EMSCRIPTEN_KEEPALIVE
double sumar(double a, double b) {
    return a + b;
}
