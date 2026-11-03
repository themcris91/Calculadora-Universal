#ifndef API_H
#define API_H

double sumar(double a, double b);
/* Wrapper de humo: Simpson de sin(x). Devuelve NaN si num_simpson falla. */
double integral_sin(double a, double b, int n);

#endif /* API_H */
