#include "sklearn_cpp.hpp"
#include <fstream>
#include <sstream>

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
                // skip first line (header)
                if (line.find("cement") != std::string::npos) {
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


int main() {
    std::vector<std::vector<double>> x;
    std::vector<double> y;

    Read_CSV::read_csv("../data/concrete.csv", x, y);

    // Print the first 5 rows of the dataset to verify it was read correctly
    std::cout << "First 5 rows of the dataset:" << std::endl;
    for (size_t i = 0; i < std::min(x.size(), static_cast<size_t>(5)); i++) {
        for (size_t j = 0; j < x[i].size(); j++) {
            std::cout << x[i][j] << " ";
        }
        std::cout << "| " << y[i] << std::endl;
    }

    // Print size of the dataset
    std::cout << "Dataset size: " << x.size() << " samples, " << x[0].size() << " features" << std::endl;
    std::cout << "Target size: " << y.size() << std::endl;

    sklearn_cpp::linear_model::LinearRegression model;
    model.fit(x, y);

    //std::vector<double> input = {4.0, 5.0};

    ///double prediction = model.predict(input);
    //std::cout << "Prediction: " << prediction << std::endl;
    return 0;
}