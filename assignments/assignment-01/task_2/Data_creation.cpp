#include <iostream>
#include "linear_function"
#include <cstdlib>
#include <fstream>

using namespace std;

// Function to add random noise to a value
double noise_function(double x, float noise)
{
    double random_noise = ((rand() % 100) / 100.0 - 0.5) * noise; // Generate random noise between -noise/2 and noise/2
    x += random_noise; // Add noise to the input value
    return x;
}

int main(){
    double k = 2.5;
    double m = 57.0;
    float noise = 50.0;
    double x[1000];
    double y[1000];

    // Generate data points with noise
    for(int i = 0; i < 1000; i++){
        x[i] = i + 1; // Start from 1
        y[i] = linear_function(x[i], m, k);

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