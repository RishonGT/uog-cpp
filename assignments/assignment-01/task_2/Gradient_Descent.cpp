#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>


double loss_function(const std::vector<double>& x, const std::vector<double>& y, double w, double b, int m) {
    double total_loss = 0.0;
    for (int i = 0; i < m; i++) {
        total_loss += (w * x[i] + b - y[i]) * (w * x[i] + b - y[i]); 
    }
    return total_loss / (2 * m); 
}

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
        if (comma_pos != std::string::npos) {
            x.push_back(stod(line.substr(0, comma_pos))); // Convert x value to double
            y.push_back(stod(line.substr(comma_pos + 1))); // Convert y value to double
            index++;
        }
    }

    input_file.close();

    if (x.empty() || y.empty()) {
        std::cerr << "No data points found in the file." << std::endl;
        return 1;
    }

    // Normalise the data to ensure convergence of the gradient descent algorithm
    // Calculate mean and standard deviation for normalisation
    double mean_x = 0.0, mean_y = 0.0;
    for (int i = 0; i < x.size(); i++) {
        mean_x += x[i];
        mean_y += y[i];
    }
    mean_x /= x.size();
    mean_y /= x.size();

    double std_x = 0.0, std_y = 0.0;
    for (int i = 0; i < x.size(); i++) {
        std_x += (x[i] - mean_x) * (x[i] - mean_x);
        std_y += (y[i] - mean_y) * (y[i] - mean_y);
    }
    std_x = sqrt(std_x / x.size());
    std_y = sqrt(std_y / x.size());

    // Normalisation of x and y
    for (int i = 0; i < x.size(); i++) {
        x[i] = (x[i] - mean_x) / std_x;
        y[i] = (y[i] - mean_y) / std_y;
    }

    // Initialize parameters for gradient descent
    double w = 0.0; // Slope
    double b = 0.0; // Intercept
    double learning_rate = 0.1;
    int max_iterations = 10000; // Maximum number of iterations
    int iteration = 0;

    // Gradient Descent Algorithm
    double current_loss = loss_function(x, y, w, b, x.size());
    do {
        double dw = 0.0; // Gradient for w
        double db = 0.0; // Gradient for b

        // Calculate gradients
        for (int i = 0; i < x.size(); i++) {
            dw += (w * x[i] + b - y[i]) * x[i]; // Derivative with respect to w
            db += (w * x[i] + b - y[i]); // Derivative with respect to b
        }
        dw /= x.size();
        db /= x.size();

        // Update parameters
        w -= learning_rate * dw;
        b -= learning_rate * db;

        iteration++; // Increment iteration count

        if (iteration % 500 == 0) { // Print loss every 500 iterations
            current_loss = loss_function(x, y, w, b, x.size());
            std::cout << "Iteration: " << iteration << ", Loss: " << current_loss << std::endl;
        }

        if (iteration >= max_iterations) {
            break;
        }

    } while (current_loss > 0.0001); // Stop when loss is sufficiently low

    // Denormalize the parameters
    double w_original = w * (std_y / std_x);
    double b_original = (mean_y - w_original * mean_x);

    // Output the calculated parameters
    std::cout << "Calculated parameters after Gradient Descent:" << std::endl;
    std::cout << "w (slope): " << w_original << std::endl;
    std::cout << "b (intercept): " << b_original << std::endl;

    return 0;
}