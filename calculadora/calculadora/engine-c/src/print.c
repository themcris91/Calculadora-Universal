#include <stdio.h>
#include "print.h"

void print_result(const char *label, double value)
{
    printf("%s = %.12g\n", label, value);
}
