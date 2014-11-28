#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <png.h>

#include <pnghelper/png_writer.h>
#include <pnghelper/pixel_ops.h>


#include "fractal_plot.h"

// Data Structures
typedef struct fractal_params
{
    double start;
    double end;
    double step;

    ComplexFunc f;
    ComplexFunc f_dash;
} FractalParams;

// Function Prototypes

double complex derivative(double complex z);
double complex func(double complex z);
double complex newton_method(double complex x_0, ComplexFunc f, ComplexFunc f_dash, unsigned int n);
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

    double complex z = (p->start + p->step*x) + (p->start + p->step*y) * I;
    z = newton_method(z, p->f, p->f_dash, 5);
    int root = test_root(z);

    switch(root)
    {
        case 0:
            pixel[0] = 255;
            pixel[1] = 0;
            pixel[2] = 0;
            break;

        case 1:
            pixel[0] = 0;
            pixel[1] = 255;
            pixel[2] = 0;
            break;

        case 2:
            pixel[0] = 0;
            pixel[1] = 0;
            pixel[2] = 255;
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
    // Create a parameters struct for the fractal
    FractalParams frac_params = {
            .start  =  -1.0,
            .end    =   1.0,
            .f      =   func,
            .f_dash =  derivative
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
