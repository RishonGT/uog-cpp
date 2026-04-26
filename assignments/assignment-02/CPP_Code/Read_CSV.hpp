#ifndef READ_CSV_HPP
#define READ_CSV_HPP
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class Read_CSV {
    public:
        static void read_csv(const std::string& filename, std::vector<std::vector<double>>& X, std::vector<double>& Y) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error opening file: " << filename << std::endl;
                return;
            }
            

            std::string line;
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
                        has_string_data = true;
                        break;
                    }
                }

                if (has_string_data) {
                    continue;
                }

                std::stringstream ss(line);
                std::string value;
                std::vector<double> features;

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