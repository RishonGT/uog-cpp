//Function to calculate an linear function with an intercept and gradient.

double linear_function( double x, double intercept, double gradient)
{
    double y;

    y = gradient * x + intercept;

    return y;
}