#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include <png.h>

#include <pnghelper/png_writer.h>
#include <pnghelper/pixel_ops.h>


#include "fractal_plot.h"

// Data Structures

typedef struct newton_params
{
    ComplexFunc f;
    ComplexFunc f_dash;

    double complex* solns;
    unsigned int num_solns;
    double tolerance;

    bool stop;
    unsigned int stop_threshold;
} NewtonParams;

typedef struct fractal_params
{
    double start;
    double end;
    double step;

    NewtonParams newton_params;

} FractalParams;

// Function Prototypes

double complex derivative(double complex z);
double complex func(double complex z);
double complex newton_method(double complex x_0, ComplexFunc f, ComplexFunc f_dash, unsigned int n);
double complex newton_method_exact(double complex x_0, unsigned int* n, unsigned int *root, NewtonParams* params);
int test_root(double complex z);
void generate_fractal(Pixel pixel, unsigned int x, unsigned int y, void* params);

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
 * Improved and more flexible version of the function above, its behavoir can be customised using
 * the NewtonParams struct described below. It also performs the root test so the test_root function
 * is not required with this function
 *
 * The NewtonParams struct:
 *
 * This handles a lot of the configuration for this function, so that we don't have to
 * pass an argument list three miles long, it provides the following options:
 *
 *              f              : A ComplexFunc that represents the function to solve
 *              f_dash         : A ComplexFunc that represents the derivative of the above function
 *              solns          : A pointer to an array of double complex numbers that represents the above function
 *              num_solns      : An unsigned int representing how many solutions are in the solns array
 *              tolerance      : A double representing how "exact" you want the solutions to be
 *              stop           : A boolean flag indicating whether or not you want this function to keep running until it finds an exact solution
 *              stop_threshold : If the above flag is false, this is an unsigned integer that represents the maximum number of iterations the function will do.
 *
 * ComplexFunc: A function pointer to a function of type double complex (func) (double complex)
 *
 * Arguments:
 *              x_0    : The initial guess to the solution
 *              n      : A pointer to an unsigned int, the number of iterations will be stored here
 *              root   : A pointer to an unsigned int, the root corresponding to the given x_0 will be stored here
 *              params : A pointer to a NewtonParams struct, used to pass the function and solutions to this function
 */
double complex newton_method_exact(double complex x_0, unsigned int* n, unsigned int *root, NewtonParams* params)
{
    unsigned int count = 0;
    unsigned int i = 0;
    double min_distance, distance;
    int closest;

    // Loop for the number of iterations required
    while (true)
    {
        // Increment the counter
        count++;

        // Calculate the next value in the iteration
        x_0 = x_0 - (params->f(x_0)/params->f_dash(x_0));


        // We we have stop = true then if count is high enough return what we have
        if(params->stop && count == params->stop_threshold)
        {

            // Since we might not be within "tolerance" of a solution simply return the nearest
            for(i = 0; i < params->num_solns; i++)
            {
                // Assume that when i = 0, the current solution is the closest
                if(i == 0)
                {
                    min_distance = cabs(x_0 - params->solns[i]);
                    closest = i;
                    continue;
                }

                // Otherwise check to see if we have a new nearest
                if((distance = cabs(x_0 - params->solns[i])) < min_distance)
                {
                    min_distance = distance;
                    closest = i;
                }
            }

            // Now set the root we found
           *root = closest;
           *n = count;

           // Finally return x_0
           return x_0;
        }

        // Check if we match a solution
        for (i = 0; i < params->num_solns; i++)
        {
            // It might take ages to get the exact answer so let's see how it looks if we get it within a certain
            // distance from the solution
            if (cabs(x_0 - params->solns[i]) <= params->tolerance)
            {
                // Set the number of iterations this took
                *n = count;

                // Set the root this corresponds to
                *root = i;

                // Return the value
                return x_0;
            }
        }
    }
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

/*
 * This function will get passed to the pixel iterator and will compute the
 * color of the pixel at that point.
 */
void generate_fractal(Pixel pixel, unsigned int x, unsigned int y, void* params)
{
    FractalParams* p = (FractalParams*) params;
    unsigned int num_iterations = 1;
    unsigned int root = 0;

    double complex z = (p->start + p->step*x) + (p->start + p->step*y) * I;
    z = newton_method_exact(z, &num_iterations, &root, &(p->newton_params));

    switch(root)
    {
        case 0:
            pixel[0] = 255/num_iterations;
            pixel[1] = 0;
            pixel[2] = 0;
            break;

        case 1:
            pixel[0] = 0;
            pixel[1] = 255/num_iterations;
            pixel[2] = 0;
            break;

        case 2:
            pixel[0] = 0;
            pixel[1] = 0;
            pixel[2] = 255/num_iterations;
            break;
    }
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
    double complex solns[] = {1.0 + 0.0 * I, -0.5 + (sqrt(3)/2) * I, -0.5 - (sqrt(3)/2) * I};
    // Cretae parameters struct for newtons method
    NewtonParams newtonparams = {
            .f = func,
            .f_dash = derivative,
            .num_solns = 3,
            .solns = solns,
            .stop = true,
            .stop_threshold = 50,
            .tolerance = 0.5f,
    };

    // Create a parameters struct for the fractal
    FractalParams frac_params = {
            .start  =  -1.0,
            .end    =   1.0,
            .newton_params = newtonparams
    };

    // Create and initialise a new png image
    PNGImage img;
    IMGParams img_params;

    img_params.width = 4096;
    img_params.height = 4096;
    img_params.color_type = PNG_COLOR_TYPE_RGB;

    if(!(new_png_image(&img, &img_params)))
    {
        fprintf(stderr, "Unable to create image for the fractal\n");
        return 1;
    }

    // Now we have our image calculate the step for each pixel
    frac_params.step = (frac_params.end - frac_params.start) / img_params.width;

    // Set our pixel iterator function
    PixelIterator iter = generate_fractal;

    // Run the pixel iterator
    png_pixel_iterate(&img, iter, (void*)&frac_params);

    // Save the image file
    if(!(write_png_to_file(&img, "fractal.png")))
    {
        fprintf(stderr, "Unable to save png image\n");
        destroy_png_image(&img);
        return 1;
    }

    destroy_png_image(&img);

    return 0;
}
