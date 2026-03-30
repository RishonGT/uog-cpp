#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

// Function to calculate the loss (Mean Squared Error)
double loss_function(const std::vector<double>& x1, const std::vector<double>&x2, 
    const std::vector<double>& y, double const& w1, 
    double const& w2, double const& b, int const m) {

    double total_loss = 0.0;
    for (int i = 0; i < m; i++) {
        // Squared error - Expansion of Equation 3 in assignment_01.pdf
        total_loss += (w1 * x1[i] + w2 * x2[i]+ b - y[i]) * (w1 * x1[i] + w2 * x2[i]+ b - y[i]); 
    }

    return total_loss / (2 * m); // Mean Squared Error
}

int main() {
    // Open the CSV file for reading
    std::ifstream input_file("data.csv");
    if (!input_file.is_open()) {
        std::cerr << "Unable to open file for reading." << std::endl;
        return 1;
    }

    // Vectors to store the data points
    std::vector<double> x1, x2, y;

    // Store the data from the CSV file into vectors
    std::string line;
    int index = 0;

    std::getline(input_file, line); // Skip the header line
    while (std::getline(input_file, line) && index < 1000) {
        size_t first_comma = line.find(',');
        size_t second_comma = line.find(',', first_comma + 1);

        // Check if the line contains two commas and split the line into x1, x2 and y values
        if (first_comma != std::string::npos && second_comma != std::string::npos) {
            x1.push_back(std::stod(line.substr(0, first_comma)));  // Convert x1 value to double
            x2.push_back(std::stod(line.substr(first_comma + 1, second_comma - first_comma - 1))); // Convert x2 value to double
            y.push_back(std::stod(line.substr(second_comma + 1))); // Convert y value to double
            index++;
        }
    }

    input_file.close();

    if (x1.empty() || x2.empty() || y.empty()) {
        std::cerr << "No data points found in the file or x2 is empty." << std::endl;
        return 1;
    }

    // Normalise the data to ensure convergence of the gradient descent algorithm
    // Without normalisation, the algorithm diverges such that the loss becomes NaN after a few iterations, 
    // and the parameters w and b become very large in magnitude.

    // This is due to the large range of the x and y values, 
    // which causes the gradients to become very large, 
    // leading to divergence of the algorithm.

    // Calculate mean and standard deviation for normalisation
    double mean_x1 = 0.0, mean_x2 = 0.0, mean_y = 0.0;

    for (int i = 0; i < x1.size(); i++) {
        mean_x1 += x1[i];
        mean_x2 += x2[i];
        mean_y += y[i];
    }

    mean_x1 /= x1.size();
    mean_x2 /= x2.size();
    mean_y /= y.size();

    double std_x1 = 0.0, std_x2 = 0.0, std_y = 0.0;

    for (int i = 0; i < x1.size(); i++) {
        std_x1 += (x1[i] - mean_x1) * (x1[i] - mean_x1);
        std_x2 += (x2[i] - mean_x2) *(x2[i] - mean_x2);
        std_y += (y[i] - mean_y) * (y[i] - mean_y);
    }

    std_x1 = sqrt(std_x1 / x1.size());
    std_x2 = sqrt(std_x2 / x2.size());
    std_y = sqrt(std_y / y.size());

    // Normalise the data and store it back in the same vectors
    for (int i = 0; i < x1.size(); i++) {
        x1[i] = (x1[i] - mean_x1) / std_x1;
        x2[i] = (x2[i] - mean_x2) / std_x2;
        y[i] = (y[i] - mean_y) / std_y;
    }


    // Initialize parameters for gradient descent
    double w1 = 0.0; // Slope
    double w2 = 0.0;
    double b = 0.0; // Intercept
    double learning_rate = 0.01; // Reduced learning rate for stability
    int max_iterations = 10000; // Maximum number of iterations
    int iteration = 0;

    // Calculate initial loss
    double current_loss = loss_function(x1, x2, y, w1, w2, b, x1.size());
    double prev_loss = current_loss;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        double dw1 = 0.0; // Gradient Differential for x1
        double dw2 = 0.0; // Gradient Differential for x2
        double db = 0.0; // Gradient for b


        // Calculate gradients
        for (int i = 0; i < x1.size(); i++) {
            // Use Equation 7 in assignment_01.pdf to calculate the gradients dw1, dw2 and db
            dw1 += (w1 * x1[i] + w2 * x2[i] + b - y[i]) * x1[i]; // Derivative with respect to w1
            dw2 += (w1 * x1[i] + w2 * x2[i] + b - y[i]) * x2[i];
            db += (w1 * x1[i] + w2 * x2[i] + b - y[i]); // Derivative with respect to b
        }

        dw1 /= x1.size();
        dw2 /= x2.size();
        db /= y.size();

        // Update parameters, Equation 6 in assignment_01.pdf
        w1 -= learning_rate * dw1;
        w2 -= learning_rate * dw2;
        b -= learning_rate * db;


        // Calculate current loss
        current_loss = loss_function(x1, x2, y, w1, w2, b, x1.size());
        

        if (iter % 1000 == 0) { // Print loss every 1000 iterations
            std::cout << "Iteration: " << iter << ", Loss: " << current_loss << std::endl;
            std::cout << "w1: " << w1 << ", w2: " << w2 << ", b: " << b << std::endl;
        }

        // Check for convergence
        if (current_loss < 0.0001 || std::abs(prev_loss - current_loss) < 1e-9) {
            std::cout << "\nConverged at iteration: " << iter << ", Final Loss: " << current_loss << std::endl;
            break;
        }
        
        prev_loss = current_loss;
        iteration = iter;
    }

    // Denormalize the parameters
    double w1_original = w1 * (std_y / std_x1);
    double w2_original = w2 * (std_y / std_x2);
    double b_original = mean_y - (w1_original * mean_x1) - (w2_original * mean_x2);

    // Output the calculated parameters
    std::cout << "\nCalculated parameters after Gradient Descent:" << std::endl;
    std::cout << "x1 (slope): " << w1_original << std::endl;
    std::cout << "x2 (slope): " << w2_original << std::endl;
    std::cout << "b (intercept): " << b_original << "\n" << std::endl;

    // Output Accuracy of the model
    std::cout << "Accuracy of x1: " << (1 - (std::abs(w1_original - 2.5) / 2.5)) * 100 << "%" << std::endl;
    std::cout << "Accuracy of x2: " << (1 - (std::abs(w2_original - 3.7) / 3.7)) * 100 << "%" << std::endl;
    std::cout << "Accuracy of b: " << (1 - (std::abs(b_original - 500.0) / 500.0)) * 100 << "%" << std::endl;

    return 0;
}