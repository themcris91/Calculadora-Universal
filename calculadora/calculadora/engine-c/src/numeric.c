#include <float.h>
#include <math.h>
#include <stddef.h>
#include "numeric.h"
#include "status.h"

static int is_finite(double x)
{
    return isfinite(x) != 0;
}

/* ---------------------------------------------------------------- Simpson */

int num_simpson(num_fn f, void *ctx, double a, double b, int n, double *out)
{
    double h, acc;
    int i;

    if (f == NULL || out == NULL || n < 1 || n > NUM_SIMPSON_MAX_N) {
        return ENG_ERR_ARG;
    }
    if (!is_finite(a) || !is_finite(b)) {
        return ENG_ERR_DOMAIN;
    }
    if (a == b) {
        *out = 0.0;
        return ENG_OK;
    }
    if (n % 2 != 0) {
        n += 1;
    }

    h = (b - a) / (double)n;
    if (!is_finite(h)) {
        return ENG_ERR_DOMAIN;
    }

    acc = f(a, ctx) + f(b, ctx);
    for (i = 1; i < n; i++) {
        acc += f(a + (double)i * h, ctx) * ((i % 2 == 0) ? 2.0 : 4.0);
    }
    acc = acc * h / 3.0;

    if (!is_finite(acc)) {
        return ENG_ERR_DOMAIN;
    }
    *out = acc;
    return ENG_OK;
}

/* --------------------------------------------------------- Simpson adaptativo */

typedef struct {
    num_fn f;
    void *ctx;
    int max_depth;
    int truncated; /* se alcanzó max_depth o el intervalo colapsó */
    int bad;       /* integrando no finito */
    double err;    /* estimación acumulada del error */
} adapt_t;

static double adapt_eval(adapt_t *s, double x)
{
    const double y = s->f(x, s->ctx);
    if (!is_finite(y)) {
        s->bad = 1;
    }
    return y;
}

static double adapt_rec(adapt_t *s, double a, double fa, double m, double fm,
                        double b, double fb, double whole, double tol, int depth)
{
    const double lm = 0.5 * (a + m);
    const double rm = 0.5 * (m + b);
    const double flm = adapt_eval(s, lm);
    const double frm = adapt_eval(s, rm);
    const double left = (m - a) / 6.0 * (fa + 4.0 * flm + fm);
    const double right = (b - m) / 6.0 * (fm + 4.0 * frm + fb);
    const double delta = left + right - whole;

    if (s->bad) {
        return left + right;
    }
    if (fabs(delta) <= 15.0 * tol) {
        s->err += fabs(delta) / 15.0;
        return left + right + delta / 15.0;
    }
    if (depth >= s->max_depth || !(a < lm && lm < m && m < rm && rm < b)) {
        s->truncated = 1;
        s->err += fabs(delta) / 15.0;
        return left + right + delta / 15.0;
    }
    return adapt_rec(s, a, fa, lm, flm, m, fm, left, tol * 0.5, depth + 1) +
           adapt_rec(s, m, fm, rm, frm, b, fb, right, tol * 0.5, depth + 1);
}

int num_integrate_adaptive(num_fn f, void *ctx, double a, double b,
                           double tol, int max_depth, double *out, double *err_est)
{
    adapt_t s;
    double sign = 1.0, tmp, fa, fb, fm, m, whole, res;

    if (f == NULL || out == NULL || !(tol > 0.0) || !is_finite(tol) ||
        max_depth < 1 || max_depth > NUM_ADAPT_MAX_DEPTH) {
        return ENG_ERR_ARG;
    }
    if (!is_finite(a) || !is_finite(b)) {
        return ENG_ERR_DOMAIN;
    }
    if (a == b) {
        *out = 0.0;
        if (err_est != NULL) {
            *err_est = 0.0;
        }
        return ENG_OK;
    }
    if (a > b) {
        tmp = a;
        a = b;
        b = tmp;
        sign = -1.0;
    }

    s.f = f;
    s.ctx = ctx;
    s.max_depth = max_depth;
    s.truncated = 0;
    s.bad = 0;
    s.err = 0.0;

    m = 0.5 * (a + b);
    fa = adapt_eval(&s, a);
    fb = adapt_eval(&s, b);
    fm = adapt_eval(&s, m);
    if (s.bad || !is_finite(m)) {
        return ENG_ERR_DOMAIN;
    }
    whole = (b - a) / 6.0 * (fa + 4.0 * fm + fb);
    res = adapt_rec(&s, a, fa, m, fm, b, fb, whole, tol, 0);

    if (s.bad || !is_finite(res)) {
        return ENG_ERR_DOMAIN;
    }
    *out = sign * res;
    if (err_est != NULL) {
        *err_est = s.err;
    }
    return s.truncated ? ENG_ERR_NO_CONVERGE : ENG_OK;
}

/* ---------------------------------------------------------------- Bisección */

int num_root_bisection(num_fn f, void *ctx, double a, double b,
                       double tol, int max_iter, double *root, int *iters)
{
    double fa, fb, fm, m, tmp;
    int i;

    if (f == NULL || root == NULL || !(tol > 0.0) || !is_finite(tol) || max_iter < 1) {
        return ENG_ERR_ARG;
    }
    if (!is_finite(a) || !is_finite(b)) {
        return ENG_ERR_DOMAIN;
    }
    if (a > b) {
        tmp = a;
        a = b;
        b = tmp;
    }
    fa = f(a, ctx);
    fb = f(b, ctx);
    if (!is_finite(fa) || !is_finite(fb)) {
        return ENG_ERR_DOMAIN;
    }
    if (fa == 0.0) {
        *root = a;
        if (iters != NULL) {
            *iters = 0;
        }
        return ENG_OK;
    }
    if (fb == 0.0) {
        *root = b;
        if (iters != NULL) {
            *iters = 0;
        }
        return ENG_OK;
    }
    if ((fa > 0.0) == (fb > 0.0)) {
        return ENG_ERR_NO_BRACKET;
    }

    for (i = 1; i <= max_iter; i++) {
        m = a + (b - a) * 0.5;
        fm = f(m, ctx);
        if (!is_finite(fm)) {
            return ENG_ERR_DOMAIN;
        }
        /* Raíz exacta, tolerancia alcanzada o intervalo no divisible (límite de precisión). */
        if (fm == 0.0 || (b - a) * 0.5 <= tol || m == a || m == b) {
            *root = m;
            if (iters != NULL) {
                *iters = i;
            }
            return ENG_OK;
        }
        if ((fm > 0.0) == (fa > 0.0)) {
            a = m;
            fa = fm;
        } else {
            b = m;
        }
    }
    return ENG_ERR_NO_CONVERGE;
}

/* ------------------------------------------------------------------- Brent */

int num_root_brent(num_fn f, void *ctx, double a, double b,
                   double tol, int max_iter, double *root, int *iters)
{
    double fa, fb, c, fc, d, e;
    int i;

    if (f == NULL || root == NULL || !(tol > 0.0) || !is_finite(tol) || max_iter < 1) {
        return ENG_ERR_ARG;
    }
    if (!is_finite(a) || !is_finite(b)) {
        return ENG_ERR_DOMAIN;
    }
    fa = f(a, ctx);
    fb = f(b, ctx);
    if (!is_finite(fa) || !is_finite(fb)) {
        return ENG_ERR_DOMAIN;
    }
    if (fa == 0.0) {
        *root = a;
        if (iters != NULL) {
            *iters = 0;
        }
        return ENG_OK;
    }
    if (fb == 0.0) {
        *root = b;
        if (iters != NULL) {
            *iters = 0;
        }
        return ENG_OK;
    }
    if ((fa > 0.0) == (fb > 0.0)) {
        return ENG_ERR_NO_BRACKET;
    }

    c = b;
    fc = fb;
    d = e = b - a;
    for (i = 1; i <= max_iter; i++) {
        double tol1, xm, p, q, r, s, min1, min2;

        if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
            c = a;
            fc = fa;
            d = e = b - a;
        }
        if (fabs(fc) < fabs(fb)) {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }
        tol1 = 2.0 * DBL_EPSILON * fabs(b) + 0.5 * tol;
        xm = 0.5 * (c - b);
        if (fabs(xm) <= tol1 || fb == 0.0) {
            *root = b;
            if (iters != NULL) {
                *iters = i - 1;
            }
            return ENG_OK;
        }
        if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
            s = fb / fa;
            if (a == c) {
                p = 2.0 * xm * s;
                q = 1.0 - s;
            } else {
                q = fa / fc;
                r = fb / fc;
                p = s * (2.0 * xm * q * (q - r) - (b - a) * (r - 1.0));
                q = (q - 1.0) * (r - 1.0) * (s - 1.0);
            }
            if (p > 0.0) {
                q = -q;
            }
            p = fabs(p);
            min1 = 3.0 * xm * q - fabs(tol1 * q);
            min2 = fabs(e * q);
            if (2.0 * p < (min1 < min2 ? min1 : min2)) {
                e = d;
                d = p / q;
            } else {
                d = xm;
                e = d;
            }
        } else {
            d = xm;
            e = d;
        }
        a = b;
        fa = fb;
        if (fabs(d) > tol1) {
            b += d;
        } else {
            b += (xm >= 0.0) ? tol1 : -tol1;
        }
        fb = f(b, ctx);
        if (!is_finite(fb)) {
            return ENG_ERR_DOMAIN;
        }
    }
    return ENG_ERR_NO_CONVERGE;
}

/* ------------------------------------------------------------------ Newton */

int num_root_newton(num_fn f, num_fn df, void *ctx, double x0,
                    double tol, int max_iter, double *root, int *iters)
{
    double x = x0, fx, dfx, dx, h, f1, f2;
    int i;

    if (f == NULL || root == NULL || !(tol > 0.0) || !is_finite(tol) || max_iter < 1) {
        return ENG_ERR_ARG;
    }
    if (!is_finite(x0)) {
        return ENG_ERR_DOMAIN;
    }

    for (i = 0; i < max_iter; i++) {
        fx = f(x, ctx);
        if (!is_finite(fx)) {
            return ENG_ERR_DOMAIN;
        }
        if (fx == 0.0) {
            *root = x;
            if (iters != NULL) {
                *iters = i;
            }
            return ENG_OK;
        }
        if (df != NULL) {
            dfx = df(x, ctx);
        } else {
            h = pow(DBL_EPSILON, 1.0 / 3.0) * fmax(1.0, fabs(x));
            f1 = f(x + h, ctx);
            f2 = f(x - h, ctx);
            if (!is_finite(f1) || !is_finite(f2)) {
                return ENG_ERR_DOMAIN;
            }
            dfx = (f1 - f2) / (2.0 * h);
        }
        if (!is_finite(dfx) || dfx == 0.0) {
            return ENG_ERR_NO_CONVERGE;
        }
        dx = fx / dfx;
        x -= dx;
        if (!is_finite(x)) {
            return ENG_ERR_NO_CONVERGE;
        }
        if (fabs(dx) <= tol * (1.0 + fabs(x))) {
            *root = x;
            if (iters != NULL) {
                *iters = i + 1;
            }
            return ENG_OK;
        }
    }
    return ENG_ERR_NO_CONVERGE;
}
