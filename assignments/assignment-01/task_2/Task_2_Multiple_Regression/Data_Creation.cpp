#include <iostream>
#include "Linear_function.h"
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

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
    double intercept = 57.0;
    float noise = 50.0;
    vector<double> x;
    vector<double> y;

    // Generate data points with noise
    for(int i = 0; i < 1000; i++){
        x.push_back(i + 1); // Start from 1
        y.push_back(Linear_function(x[i], intercept, gradient_1, gradient_2)); // Use the linear function to calculate y values

        // Add noise to the x and y values
        x[i] = noise_function(x[i], noise);
        y[i] = noise_function(y[i], noise);

    }
    // Write the data to a CSV file
    ofstream output_file("data.csv");
    if (output_file.is_open()) {
        output_file << "x,y\n"; // Write the header
        for (int i = 0; i < 1000; i++) {
            output_file << x[i] << "," << y[i] << "\n";
        }
        output_file.close();
        cout << "Data has been written to data.csv" << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }


    return 0;
}