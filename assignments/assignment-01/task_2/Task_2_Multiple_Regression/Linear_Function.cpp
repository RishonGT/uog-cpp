//Function to calculate an linear function with an intercept and gradient.
#include "Linear_function.h"

double Linear_function(double x, double intercept, double gradient_1, double gradient_2)
{
    double y;

    y = (gradient_1 * x) + (gradient_2 * x) + intercept;

    return y;
}