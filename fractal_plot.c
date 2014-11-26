#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#include "newtons_method.h"

double complex func(double complex z)
{
    return z*z*z - 1;
}

double complex derivative(double complex z)
{
    return 3*z*z;
}

int main()
{

    ComplexFunc f = func;
    ComplexFunc f_dash = derivative;

    double complex z = -10.0 + 10.0 * I;
    double complex x = newton_method(z, f, f_dash, 10);

    printf("The value of x is %.2f + %.2fi\n", crealf(x), cimagf(x));


    return 0;
}
