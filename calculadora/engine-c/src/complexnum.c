#include "complexnum.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int cx_rect_to_polar(double re, double im, angle_unit u, double *r, double *theta) {
    if (!r || !theta) return ENG_ERR_ARG;
    *r = hypot(re, im);
    double rad = atan2(im, re);
    
    if (u == ANGLE_DEG) *theta = rad * 180.0 / M_PI;
    else if (u == ANGLE_GRAD) *theta = rad * 200.0 / M_PI;
    else *theta = rad;
    
    return ENG_OK;
}

int cx_polar_to_rect(double r, double theta, angle_unit u, double *re, double *im) {
    if (!re || !im) return ENG_ERR_ARG;
    double rad = theta;
    if (u == ANGLE_DEG) rad = theta * M_PI / 180.0;
    else if (u == ANGLE_GRAD) rad = theta * M_PI / 200.0;
    
    *re = r * cos(rad);
    *im = r * sin(rad);
    return ENG_OK;
}
