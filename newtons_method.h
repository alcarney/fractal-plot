#ifndef NEWTON_METHOD_C
#define NEWTON_METHOD_C

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

// Typedef a ComplexFunction pointer, I.e a function that takes a complex value
// and returns a complex number f: C -> C
typedef double complex (*ComplexFunc)(double complex);

// Function Prototype
double complex newton_method(double complex x_0, ComplexFunc f, ComplexFunc f_dash, unsigned int n);

#endif
