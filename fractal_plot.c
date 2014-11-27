#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "fractal_plot.h"

// ---------------------------- Fractal Plotter Functions --------------------------------------

/*
 * Function to apply newtons method to a given complex function
 *
 * Arguments:
 *              x_0    :    Double precision complex number representing intial guess for f
 *              f      :    Function to approximate root for
 *              f_dash :    Derivative of said function
 *              n      :    Number of iterations to perform
 *
 * Returns:
 *              The approximation after n iterations
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

/*
 * Function to check which root a particular value is closest to
 * NOTE: This function is currently hard coded for the mathematical function
 * f(z) = z^3 - 1
 *
 * Arguments:
 *              z:      The value to test
 *
 * Returns an integer corresponding to the root the value is closest to
 */
int test_root(double complex z)
{
    // Define the roots
    double complex roots[] = {1.0 + 0.0 * I, -0.5 + (sqrt(3)/2) * I, -0.5 - (sqrt(3)/2) * I};

    // Loop through them assuming the closest is at index 0
    int closest = 0;
    int i = 0;
    double min_distance;

    for (i = 0; i < 3; i++)
    {
        // Test the distance from the root
        double distance = cabs(z - roots[i]);

        // If this is the first test assume that this is the closest root
        if (i == 0)
        {
            min_distance = distance;
            continue;
        }

        // Otherwise check to see if this root is closer
        if (distance < min_distance)
        {
            min_distance = distance;
            closest = i;
        }

    }

    // Return the index for the closest root
    return closest;
}

// ---------------------------- Mathematical Functions -----------------------------------------------

double complex func(double complex z)
{
    return z*z*z - 1;
}

double complex derivative(double complex z)
{
    return 3*z*z;
}

// ----------------------------- Main Code -----------------------------------------------------------

int main()
{

    ComplexFunc f = func;
    ComplexFunc f_dash = derivative;

    double x_start = -1.0;
    double x_step = 0.1;
    double x_end = 1.0;
    double x;
    int x_i = 0;

    double y_start = 1.0;
    double y_step = -0.1;
    double y_end = -1.0;
    double y;
    int y_i = 0;


    // Loop for each row
    for (y = y_start; y > y_end; y_i++)
    {
        y = y_start + y_i*y_step;

        // Loop for each column
        for(x = x_start; x < x_end; x_i++)
        {

            x = x_start + x_i*x_step;

            // Create a complex number representing this point
            double complex z = x + y * I;

            // Perform newton's method on this point
            z = newton_method(z, f, f_dash, 5);
            int root = test_root(z);
            printf("%i", root);
        }
        x = x_start;
        x_i = 0;
        printf("\n");
    }

    return 0;
}
