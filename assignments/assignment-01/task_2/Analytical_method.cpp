#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // Open the CSV file for reading
    ifstream input_file("data.csv");
    if (!input_file.is_open()) {
        cerr << "Unable to open file for reading." << endl;
        return 1;
    }

    double x[1000],y[1000];

    // Store the data from the CSV file into arrays
    string line;
    int index = 0;
    getline(input_file, line); // Skip the header line
    while (getline(input_file, line) && index < 1000) {
        size_t comma_pos = line.find(',');
        if (comma_pos != string::npos) {
            x[index] = stod(line.substr(0, comma_pos)); // Convert x value to double
            y[index] = stod(line.substr(comma_pos + 1)); // Convert y value to double
            index++;
        }
    }

    input_file.close();

    // Calculate the mean of x and y
    double mean_x = 0.0, mean_y = 0.0;
    for (int i = 0; i < index; i++) {
        mean_x += x[i];
        mean_y += y[i];
    }

    mean_x /= index;
    mean_y /= index;

    // Calculate w and b using Normal Equation
    double w = 0.0, b = 0.0;
    double numerator_w = 0.0, denominator_w = 0.0;
    for (int i = 0; i < index; i++) {
        numerator_w += (x[i] - mean_x) * (y[i] - mean_y);
        denominator_w += (x[i] - mean_x) * (x[i] - mean_x);
    }

    w = numerator_w / denominator_w;
    b = mean_y - w * mean_x;

    // Output the calculated parameters
    cout << "Calculated parameters after Normal Equation:" << endl;
    cout << "w (slope): " << w << endl;
    cout << "b (intercept): " << b << endl;
}
