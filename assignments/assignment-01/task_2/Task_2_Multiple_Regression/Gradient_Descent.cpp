#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

double loss_function(const vector<double>& x1, const vector<double>&x2, const vector<double>& y, double w1, double w2, double b, int m) {
    double total_loss = 0.0;
    for (int i = 0; i < m; i++) {
        total_loss += (w1 * x1[i] + w2 * x2[i]+ b - y[i]) * (w1 * x1[i] + w2 * x2[i]+ b - y[i]); // Squared error
    }
    return total_loss / (2 * m); // Mean Squared Error
}

int main() {
    // Open the CSV file for reading
    ifstream input_file("data.csv");
    if (!input_file.is_open()) {
        cerr << "Unable to open file for reading." << endl;
        return 1;
    }

    vector<double> x1, x2, y;

    // Store the data from the CSV file into vectors
    string line;
    int index = 0;
    getline(input_file, line); // Skip the header line
    while (getline(input_file, line) && index < 1000) {
        size_t first_comma = line.find(',');
        size_t second_comma = line.find(',', first_comma + 1);
        if (first_comma != string::npos && second_comma != string::npos) {
            x1.push_back(stod(line.substr(0, first_comma)));
            x2.push_back(stod(line.substr(first_comma + 1, second_comma - first_comma - 1)));
            y.push_back(stod(line.substr(second_comma + 1)));
            index++;
        }
    }

    input_file.close();

    if (x1.empty() || x2.empty() || y.empty()) {
        cerr << "No data points found in the file or x2 is empty." << endl;
        return 1;
    }

    // Normalize the data
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

    // Gradient Descent Algorithm
    double current_loss = loss_function(x1, x2, y, w1, w2, b, x1.size());
    double prev_loss = current_loss;
    
    for (int iter = 0; iter < max_iterations; iter++) {
        double dw1 = 0.0; // Gradient Differential for x1
        double dw2 = 0.0; // Gradient Differential for x2
        double db = 0.0; // Gradient for b

        // Calculate gradients
        for (int i = 0; i < x1.size(); i++) {
            dw1 += (w1 * x1[i] + w2 * x2[i] + b - y[i]) * x1[i]; // Derivative with respect to w1
            dw2 += (w1 * x1[i] + w2 * x2[i] + b - y[i]) * x2[i];
            db += (w1 * x1[i] + w2 * x2[i] + b - y[i]); // Derivative with respect to b
        }
        dw1 /= x1.size();
        dw2 /= x2.size();
        db /= y.size();

        // Update parameters
        w1 -= learning_rate * dw1;
        w2 -= learning_rate * dw2;
        b -= learning_rate * db;

        // Calculate current loss
        current_loss = loss_function(x1, x2, y, w1, w2, b, x1.size());
        
        if (iter % 500 == 0) { // Print loss every 500 iterations
            cout << "Iteration: " << iter << ", Loss: " << current_loss << endl;
            cout << "w1: " << w1 << ", w2: " << w2 << ", b: " << b << endl;
        }

        // Check for convergence
        if (current_loss < 0.0001 || abs(prev_loss - current_loss) < 1e-9) {
            cout << "Converged at iteration: " << iter << ", Final Loss: " << current_loss << endl;
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
    cout << "Calculated parameters after Gradient Descent:" << endl;
    cout << "x1 (slope): " << w1_original << endl;
    cout << "x2 (slope): " << w2_original << endl;
    cout << "b (intercept): " << b_original << endl;

    return 0;
}