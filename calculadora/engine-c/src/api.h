#ifndef API_H
#define API_H

double sumar(double a, double b);
double integral_simpson(double (*f)(double), double a, double b, int n);
double integral_sin(double a, double b, int n);

#endif /* API_H */
