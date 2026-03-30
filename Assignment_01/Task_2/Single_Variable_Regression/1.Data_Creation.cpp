// Include the header file for the linear function
// This is used to practice implementation of a header file
#include "linear_function.h"  

#include <iostream>
#include <cstdlib>  // For rand()
#include <fstream>
#include <vector>


// Function to add random noise to a value
double noise_function(double x, float noise)
{
    double random_noise = ((rand() % 100) / 100.0 - 0.5) * noise; // Generate random noise between -noise/2 and noise/2
    x += random_noise;
    return x;
}


int main(){
    // Equation: y = 2.5x + 57.0, noise is +/- 50
    double k = 2.5;
    double m = 57.0;
    float noise = 100.0;

    // Vectors to store the generated data points
    std::vector<double> x;
    std::vector<double> y;

    // Generate data points with noise
    for(int i = 0; i < 1000; i++){
        x.push_back(i + 1); // x values from 1 to 1000
        y.push_back(linear_function(x[i], m, k));

        // Add noise to the x and y values
        x[i] = noise_function(x[i], noise);
        y[i] = noise_function(y[i], noise);

    }

    // Write the data to a CSV file
    std::ofstream output_file("data.csv");

    // Check if the file is open before writing, and write to data.csv
    if (output_file.is_open()) {
        output_file << "x,y\n"; // Write the header

        // Write the data points to the file
        for (int i = 0; i < 1000; i++) {
            output_file << x[i] << "," << y[i] << "\n";
        }
        output_file.close();
        std::cout << "Data has been written to data.csv" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;  
    }


    return 0;
}