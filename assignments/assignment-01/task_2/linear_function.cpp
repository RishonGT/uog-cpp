//Function to calculate an linear function with an intercept and gradient.
#include "linear_function.h"

double linear_function(double x, double intercept, double gradient)
{
    double y;

    y = gradient * x + intercept;

    return y;
}