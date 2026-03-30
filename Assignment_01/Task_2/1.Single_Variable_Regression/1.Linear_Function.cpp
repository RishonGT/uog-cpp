// Function to calculate an linear function with an intercept and gradient.
// This is used to practice implementation of a header file
#include "Linear_Function.h"

double linear_function(double const &x, double const &intercept, double const &gradient)
{
    double y;

    y = gradient * x + intercept;

    return y;
}