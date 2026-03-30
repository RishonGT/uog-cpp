#include <iostream>
#include <cmath>



// Fixed-point and Newton-Raphson methods for solving nonlinear equation


/////////////////////////////////////////////////////////
///////////////////////Fixed-Point///////////////////////
/////////////////////////////////////////////////////////


// This function does the maths step. Called by g(), only return to maximise line efficiency.
double g(double y) 
{
    return 0.5 * (y + 5.0 / y);
}

/* 
The initial guess is called here,
this loop checks if the new value is within the tolerance,
a low tolerance is used to get a more accurate result.
*/
double iter(double x0)
{
    /* Defining values and data types at the start for clarity*/
     double tol = 1e-6;
     int n = 100;
     int nc = 0;
     double xold = x0;
     for (int k = 0; k < n; k++) 
     {    
        // Function call to calculate the new value and checks if the new value is within the tolerance.
        double xnew = g(xold);
        if (std::fabs(xnew - xold) < tol) { // Fabs is used to get the absolute value instead of abs, as its a double.
            std::cout << "done at " << nc + 1 << " iterations. Root: " << xnew;
            break;
        }
        // If loop condition not matched, code repeats.
        xold = xnew;
        // Resetting value
        nc++;
     }
    return 0;
}


/////////////////////////////////////////////////////////
////////////////////Newton-Raphson///////////////////////
/////////////////////////////////////////////////////////


// This function does the maths step. Called by f(), only return to maximise line efficiency.
double f(double l) 
{
    return exp(-0.1 * l) * sin(2 * l);   
}
// This function does the maths differential step. Called by df(), only return to maximise line efficiency.
double df(double l) 
{
    return exp(-0.1 * l) * (2 * cos(2*l) - (0.1 * sin(2*l)));
}
/*This function uses the Newton-Raphson method,
to find the root of a nonlinear equation.
The input is the initial guess.*/
double newt(double x0) 
{   
    /* Defining values and data types at the start for clarity*/
    double tol = 1e-6;
    int n = 10000;
    int nc = 0;
    double xold = x0;
    // This loops over the function, checks for conditions and breaks if met.
    for (int k = 0; k < n; k++) 
    {   
        // Initalises the math steps.
        double fval = f(xold);
        double dfval = df(xold);
        // Checks if the derivative is zero.
        if (std::fabs(dfval) < 1e-15) 
        {
            // If condition met, it breaks and spits out the divergence message.
            std::cout << "Divergence: Derivative is zero at" << xold << "at iteration" << nc;
            break;
        }
        // Main Newton-Raphson step, calculates the new value.
        double xnew = xold - fval/dfval; 
        /*Checks if the new value is within the tolerance.
        If met, breaks and spits out the result.*/
        if (std::fabs(xnew - xold) < tol) 
        {
            std::cout << "done at " << nc + 1 << " iterations. Root: " << xnew;
            break;
        }
        xold = xnew;
        // Resetting value
        nc++;
        // Increment
    }
    return 0;
}



// Just initalises the function.
int main() 
{
    double I = 10;
    std::cout << "Set the value at " << I << " and " << iter(I) << " ,sqrt 5 is " << std::sqrt(5) << std::endl;
    /*This loop calls the Newton-Raphson method for different initial guesses, ranging from 0-10
    to display how it runs over different initial values. */
    for (int k = 0; k < 11; k++) 
    {
         std::cout << "Set the value at " << k << " and " << newt(k) << std::endl;
    }
    return 0;
}