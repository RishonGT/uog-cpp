#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

// Function to calculate the loss (mean squared error) for given parameters w and b
double loss_function(const std::vector<double>& x, const std::vector<double>& y, double const &w, double const &b, int const &m) {
    double total_loss = 0.0;

    for (int i = 0; i < m; i++) {
        total_loss += (w * x[i] + b - y[i]) * (w * x[i] + b - y[i]); 
    }


    return total_loss / (2 * m); // Equation 3 in assignment_01.pdf
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

    std::getline(input_file, line); // Skip the header line
    while (std::getline(input_file, line) && index < 1000) {
        size_t comma_pos = line.find(',');

        // Check if the line contains a comma and split the line into x and y values
        if (comma_pos != std::string::npos) {
            x.push_back(std::stod(line.substr(0, comma_pos))); // Convert x value to double
            y.push_back(std::stod(line.substr(comma_pos + 1))); // Convert y value to double
            index++;
        }
    }

    input_file.close();

    // Check if data points were successfully read
    if (x.empty() || y.empty()) {
        std::cerr << "No data points found in the file." << std::endl;
        return 1;
    }


    // Normalise the data to ensure convergence of the gradient descent algorithm
    // Without normalisation, the algorithm diverges such that the loss becomes NaN after a few iterations, 
    // and the parameters w and b become very large in magnitude.

    // This is due to the large range of the x and y values (x ranges from 1 to 1000, and y ranges from 57 to 2557), 
    // which causes the gradients to become very large, 
    // leading to divergence of the algorithm.
    
    
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

    // Normalise the data and store it back in the same vectors
    for (int i = 0; i < x.size(); i++) {
        x[i] = (x[i] - mean_x) / std_x;
        y[i] = (y[i] - mean_y) / std_y;
    }


    // Initialize parameters for gradient descent
    double w = 0.0; // Slope
    double b = 0.0; // Intercept
    double learning_rate = 0.01;
    int max_iterations = 10000; // Maximum number of iterations
    int iteration = 0;

    // Calculate initial loss
    double current_loss = loss_function(x, y, w, b, x.size());
    std::cout << "Initial Loss: " << current_loss << std::endl;

    // Gradient descent loop
    do {
        double dw = 0.0; // Gradient for w
        double db = 0.0; // Gradient for b

        // Calculate gradients
        for (int i = 0; i < x.size(); i++) {
            // Use Equation 7 in assignment_01.pdf to calculate the gradients dw and db
            dw += (w * x[i] + b - y[i]) * x[i]; 
            db += (w * x[i] + b - y[i]); 
        }

        dw /= x.size();
        db /= x.size();


        // Update parameters, Equation 6 in assignment_01.pdf
        w -= learning_rate * dw;
        b -= learning_rate * db;

        iteration++;


        if (iteration % 1000 == 0) { // Print loss every 1000 iterations
            current_loss = loss_function(x, y, w, b, x.size());
            std::cout << "Iteration: " << iteration << ", Loss: " << current_loss << std::endl;
        }

        if (iteration >= max_iterations) {
            break;
        }

    } while (current_loss > 0.0001); // Stop if loss is sufficiently low


    // Denormalize the parameters
    double w_original = w * (std_y / std_x);
    double b_original = (mean_y - w_original * mean_x);

    // Output the calculated parameters
    std::cout << "Calculated parameters after Gradient Descent:" << std::endl;
    std::cout << "w (slope): " << w_original << std::endl;
    std::cout << "b (intercept): " << b_original << std::endl;

    // Output Accuracy of the model
    std::cout << "Accuracy of w: " << (1 - (std::abs(w_original - 2.5) / 2.5)) * 100 << "%" << std::endl;
    std::cout << "Accuracy of b: " << (1 - (std::abs(b_original - 57.0) / 57.0)) * 100 << "%" << std::endl;

    return 0;
}