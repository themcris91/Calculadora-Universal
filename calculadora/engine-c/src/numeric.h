#ifndef NUMERIC_H
#define NUMERIC_H

/* Integración y búsqueda de raíces. C99 puro, sin dependencias.
 * Todas las funciones devuelven eng_status (status.h). Los parámetros de salida
 * solo se escriben en ENG_OK, salvo num_integrate_adaptive (ver abajo). */

typedef double (*num_fn)(double x, void *ctx);

#define NUM_SIMPSON_MAX_N 10000000 /* máx. subintervalos de num_simpson (par) */
#define NUM_ADAPT_MAX_DEPTH 100    /* máx. profundidad de num_integrate_adaptive */

/* Simpson 1/3 compuesto con n subintervalos. n>=1 (impar => n+1). a>b invierte el signo.
 * ENG_ERR_ARG: f/out NULL o n fuera de [1, NUM_SIMPSON_MAX_N].
 * ENG_ERR_DOMAIN: a/b no finitos o integrando/resultado no finito. */
int num_simpson(num_fn f, void *ctx, double a, double b, int n, double *out);

/* Simpson adaptativo (extrapolación de Richardson). tol>0 es la tolerancia absoluta total.
 * max_depth en [1, NUM_ADAPT_MAX_DEPTH]. err_est puede ser NULL.
 * Con ENG_ERR_NO_CONVERGE, *out y *err_est contienen igualmente el resultado parcial. */
int num_integrate_adaptive(num_fn f, void *ctx, double a, double b,
                           double tol, int max_depth, double *out, double *err_est);

/* Bisección. Termina cuando (b-a)/2 <= tol (tol absoluta). Requiere cambio de signo. iters puede ser NULL. */
int num_root_bisection(num_fn f, void *ctx, double a, double b,
                       double tol, int max_iter, double *root, int *iters);

/* Brent (zeroin). Tolerancia: 2*eps*|x| + tol/2. Requiere cambio de signo. */
int num_root_brent(num_fn f, void *ctx, double a, double b,
                   double tol, int max_iter, double *root, int *iters);

/* Newton. df==NULL => derivada numérica central. Convergencia: |dx| <= tol*(1+|x|).
 * Derivada nula/no finita o iterado no finito => ENG_ERR_NO_CONVERGE. */
int num_root_newton(num_fn f, num_fn df, void *ctx, double x0,
                    double tol, int max_iter, double *root, int *iters);

#endif /* NUMERIC_H */
