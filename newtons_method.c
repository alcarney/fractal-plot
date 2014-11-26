#include "newtons_method.h"


/*
 * Function to apply newtons method to a given complex function
 *
 * Arguments:
 *              x_0    :    Double precision complex number representing intial guess for f
 *              f      :    Function to approximate root for
 *              f_dash :    Derivative of said function
 *              n      :    Number of iterations to perform
 */
double complex newton_method(double complex x_0, ComplexFunc f, ComplexFunc f_dash, unsigned int n)
{
    unsigned int i = 0;

    // Loop for the number of iterations required
    for(i = 0; i < n; i++)
        x_0 = x_0 - (f(x_0)/f_dash(x_0));

    // Return x_0
    return x_0;
}
