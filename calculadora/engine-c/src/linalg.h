#ifndef LINALG_H
#define LINALG_H

#include "status.h"

#define LA_MAX 4

int la_add(const double *a, const double *b, int r, int c, double *out);
int la_sub(const double *a, const double *b, int r, int c, double *out);
int la_mul(const double *a, int r1, int c1, const double *b, int c2, double *out);
int la_transpose(const double *a, int r, int c, double *out);
int la_det(const double *a, int n, double *det);
int la_inv(const double *a, int n, double *out);

#endif
