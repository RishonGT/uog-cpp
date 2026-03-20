#include <iostream>
#include "Linear_function.h"
#include <cstdlib>
#include <fstream>
#include <vector>

// Function to add random noise to a value
double noise_function(double x, float noise)
{
    double random_noise = ((rand() % 100) / 100.0 - 0.5) * noise; // Generate random noise between -noise/2 and noise/2
    x += random_noise; // Add noise to the input value
    return x;
}

int main(){
    double gradient_1 = 2.5;
    double gradient_2 = 3.7;
    double intercept = 500.0;
    float noise = 200.0;
    std::vector<double> x1, x2;
    std::vector<double> y;

    // Generate data points with noise
    srand(42); // Set seed for reproducible results
    for(int i = 0; i < 1000; i++){
        x1.push_back(i + 1); // 1-1000
        x2.push_back(((rand() % 1000) + 1)); // Independent random values 1-1000
        y.push_back(Linear_function(x1[i], x2[i], intercept, gradient_1, gradient_2)); // Use the linear function to calculate y values

        // Add noise to the x and y values
        x1[i] = noise_function(x1[i], noise);
        x2[i] = noise_function(x2[i], noise);
        y[i] = noise_function(y[i], noise);

    }
    // Write the data to a CSV file
    std::ofstream output_file("data.csv");
    if (output_file.is_open()) {
        output_file << "x1,x2,y\n"; // Write the header
        for (int i = 0; i < 1000; i++) {
            output_file << x1[i] << "," << x2[i] << "," << y[i] << "\n";
        }
        output_file.close();
        std::cout << "Data has been written to data.csv" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }


    return 0;
}