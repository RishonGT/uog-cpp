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
    double k = 3.0;
    double m = 2.0;
    float noise = 70.0;
    double x[1000];
    double y[1000];
    double data[1000][2];

    // Generate data points with noise
    for(int i = 0; i < 1000; i++){
        x[i] = i + 1; // Start from 1
        y[i] = linear_function(x[i], m, k);

        // Add noise to the x and y values
        x[i] = noise_function(x[i], noise);
        y[i] = noise_function(y[i], noise);
        
        // Store the noisy data in a 2D array
        data[i][0] = x[i];
        data[i][1] = y[i];

    }
    // Write the data to a CSV file
    ofstream output_file("data.csv");
    if (output_file.is_open()) {
        output_file << "x,y\n"; // Write the header
        for (int i = 0; i < 1000; i++) {
            output_file << data[i][0] << "," << data[i][1] << "\n";
        }
        output_file.close();
        cout << "Data has been written to data.csv" << endl;
    } else {
        cerr << "Unable to open file for writing." << endl;
    }


    return 0;
}