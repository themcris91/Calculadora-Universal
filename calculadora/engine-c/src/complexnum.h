#ifndef COMPLEXNUM_H
#define COMPLEXNUM_H

#include "status.h"

typedef enum {
    ANGLE_RAD = 0,
    ANGLE_DEG = 1,
    ANGLE_GRAD = 2
} angle_unit;

int cx_rect_to_polar(double re, double im, angle_unit u, double *r, double *theta);
int cx_polar_to_rect(double r, double theta, angle_unit u, double *re, double *im);

#endif
