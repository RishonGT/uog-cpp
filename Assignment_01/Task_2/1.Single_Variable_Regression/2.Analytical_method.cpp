#include <iostream>
#include <fstream>
#include <vector>


int main() {
    // Open the CSV file for reading
    std::ifstream input_file("data.csv");
    if (!input_file.is_open()) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return 1;
    }

    std::vector<double> x, y;

    // Store the data from the CSV file into vectors
    std::string line;
    int index = 0;

    getline(input_file, line); // Skip the header line
    while (getline(input_file, line) && index < 1000) {
        size_t comma_pos = line.find(',');

        // Check if the comma is found and parse the x and y values
        if (comma_pos != std::string::npos) {
            x.push_back(stod(line.substr(0, comma_pos))); // Convert x value to double
            y.push_back(stod(line.substr(comma_pos + 1))); // Convert y value to double
            index++;
        }
    }

    input_file.close();

    // Check if data points were successfully read
    if (x.empty() || y.empty()) {
        std::cerr << "No data points found in the file." << std::endl;
        return 1;
    }

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

    // Use the Normal Equation to calculate w and b (Equation 4 in assignment_01.pdf)
    for (int i = 0; i < index; i++) {
        numerator_w += (x[i] - mean_x) * (y[i] - mean_y);
        denominator_w += (x[i] - mean_x) * (x[i] - mean_x);
    }

    w = numerator_w / denominator_w;
    b = mean_y - w * mean_x;

    
    // Output the calculated parameters
    std::cout << "Calculated parameters after Normal Equation:" << std::endl;
    std::cout << "w (slope): " << w << std::endl;
    std::cout << "b (intercept): " << b << std::endl;

    // Output Accuracy of the model
    std::cout << "Accuracy of w: " << (1 - (std::abs(w - 2.5) / 2.5)) * 100 << "%" << std::endl;
    std::cout << "Accuracy of b: " << (1 - (std::abs(b - 57.0) / 57.0)) * 100 << "%" << std::endl;

    return 0;
}
