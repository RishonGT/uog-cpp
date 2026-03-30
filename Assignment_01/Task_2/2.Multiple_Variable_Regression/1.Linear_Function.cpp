// Function to calculate an linear function with an intercept and gradient.
// This is used to practice implementation of a header file
#include "Linear_Function.h"

double Linear_function(double const &x1, double const &x2, 
    double const &intercept, double const &gradient_1, 
    double const &gradient_2)
{
    double y;

    y = (gradient_1 * x1) + (gradient_2 * x2) + intercept;

    return y;
}