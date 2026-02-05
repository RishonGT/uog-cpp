#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

double loss_function(double x[], double y[], double w, double b, int m) {
    double total_loss = 0.0;
    for (int i = 0; i < m; i++) {
        total_loss += (w * x[i] + b - y[i]) * (w * x[i] + b - y[i]); // Squared error
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

    double x[1000], y[1000];

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

    if (index == 0) {
        cerr << "No data points found in the file." << endl;
        return 1;
    }

    // Normalize the data
    double mean_x = 0.0, mean_y = 0.0;
    for (int i = 0; i < index; i++) {
        mean_x += x[i];
        mean_y += y[i];
    }
    mean_x /= index;
    mean_y /= index;

    double std_x = 0.0, std_y = 0.0;
    for (int i = 0; i < index; i++) {
        std_x += (x[i] - mean_x) * (x[i] - mean_x);
        std_y += (y[i] - mean_y) * (y[i] - mean_y);
    }
    std_x = sqrt(std_x / index);
    std_y = sqrt(std_y / index);

    for (int i = 0; i < index; i++) {
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
    double current_loss = loss_function(x, y, w, b, index);
    do {
        double dw = 0.0; // Gradient for w
        double db = 0.0; // Gradient for b

        // Calculate gradients
        for (int i = 0; i < index; i++) {
            dw += (w * x[i] + b - y[i]) * x[i]; // Derivative with respect to w
            db += (w * x[i] + b - y[i]); // Derivative with respect to b
        }
        dw /= index;
        db /= index;

        // Update parameters
        w -= learning_rate * dw;
        b -= learning_rate * db;

        iteration++; // Increment iteration count

        if (iteration % 500 == 0) { // Print loss every 100 iterations
            current_loss = loss_function(x, y, w, b, index);
            cout << "Iteration: " << iteration << ", Loss: " << current_loss << endl;
        }

        if (iteration >= max_iterations) {
            break;
        }

    } while (current_loss > 0.0001); // Stop when loss is sufficiently low

    // Denormalize the parameters
    double w_original = w * (std_y / std_x);
    double b_original = (mean_y - w_original * mean_x);

    // Output the calculated parameters
    cout << "Calculated parameters after Gradient Descent:" << endl;
    cout << "w (slope): " << w_original << endl;
    cout << "b (intercept): " << b_original << endl;

    return 0;
}