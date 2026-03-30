//Function to calculate an linear function with an intercept and gradient.
#include "Linear_function.h"

double Linear_function(double x1, double x2, double intercept, double gradient_1, double gradient_2)
{
    double y;

    y = (gradient_1 * x1) + (gradient_2 * x2) + intercept;

    return y;
}