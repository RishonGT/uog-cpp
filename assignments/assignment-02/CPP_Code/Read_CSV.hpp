#ifndef READ_CSV_HPP
#define READ_CSV_HPP
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

/*
- A single class method (Read_CSV::read_csv) handles file parsing.
- The file is opened with std::ifstream, then processed line-by-line via std::getline.
- Each line is split on commas using std::stringstream.
- A validation pass tries std::stod on every field; rows with non-numeric data
    (such as headers) are ignored.
- For valid rows, values are converted to double, the last value is stored in Y
    (target), and preceding values are stored in X (features).
- After reading all rows, X is transposed so each row is one feature and each
    column is one sample.
*/

class Read_CSV {
    public:
    // Static method to read a CSV file and populate feature matrix X and target vector Y
        static void read_csv(const std::string& filename, std::vector<std::vector<double>>& X, std::vector<double>& Y) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error opening file: " << filename << std::endl;
                return;
            }
            

            std::string line;
            // Read the file line by line
            while (std::getline(file, line)) {
                // Skip empty lines or lines containing non-numeric values (e.g., header/string rows)
                if (line.empty()) {
                    continue;
                }

                std::stringstream check_ss(line);
                std::string check_value;
                bool has_string_data = false;

                while (std::getline(check_ss, check_value, ',')) {
                    try {
                        std::stod(check_value);
                    } catch (...) {
                        has_string_data = true;  // Found non-numeric data, likely a header or string row
                        break;
                    }
                }

                if (has_string_data) {
                    continue;
                }

                // Process the line to extract features and target
                std::stringstream ss(line);
                std::string value;
                std::vector<double> features;

                // Extract values from the line, converting them to doubles
                while (std::getline(ss, value, ',')) {
                    features.push_back(std::stod(value));
                }

                if (!features.empty()) {
                    Y.push_back(features.back()); // Last value is the target
                    features.pop_back(); // Remove target from features
                    X.push_back(features);
                }
            }


            file.close();

            // Check if any data was read
            if (X.empty() || Y.empty()) {
                std::cerr << "No data read from file: " << filename << std::endl;
            };

            // Transpose X to match the expected format (features as rows, samples as columns)
            std::vector<std::vector<double>> X_transposed(X[0].size(), std::vector<double>(X.size()));
            for (size_t i = 0; i < X.size(); i++) {
                for (size_t j = 0; j < X[i].size(); j++) {
                    X_transposed[j][i] = X[i][j];
                }
            }
            X = X_transposed;

        }
};


#endif