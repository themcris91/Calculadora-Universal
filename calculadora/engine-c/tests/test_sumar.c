#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../src/api.h"
#include "../src/print.h"

#define PI 3.14159265358979323846

int main(void)
{
    assert(sumar(2.0, 3.0) == 5.0);
    assert(sumar(-1.5, 1.5) == 0.0);
    assert(fabs(sumar(0.1, 0.2) - 0.3) < 1e-15);

    const double r = integral_sin(0.0, PI, 1000);
    print_result("integral_sin(0, pi, 1000)", r);
    assert(fabs(r - 2.0) < 1e-6);

    /* n impar o inválido no debe romper */
    assert(fabs(integral_sin(0.0, PI, 7) - 2.0) < 1e-2);
    assert(isfinite(integral_sin(0.0, PI, 0)));

    puts("OK: test_sumar");
    return 0;
}
