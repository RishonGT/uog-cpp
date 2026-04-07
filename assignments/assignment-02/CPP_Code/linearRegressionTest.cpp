#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>
#include "sklearn.hpp"



int main() {
    auto safe_predict = [](const std::string& label, sklearn_cpp::linear_model::LinearRegression& model, const std::vector<double>& input) {
        try {
            std::cout << label << model.predict(input) << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Prediction skipped: " << e.what() << std::endl;
        }
    };

    // Test 1: Simple linear relationship y = 2*x1 + 3
    {
        std::cout << "\nTest 1: y = 2*x1 + 3" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2, 3, 4, 5} };
        std::vector<double> Y = {5, 7, 9, 11, 13};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 6: ", model, {6});
    }

    // Test 2: Two features y = 2*x1 + 3*x2 + 1
    {
        std::cout << "\nTest 2: y = 2*x1 + 3*x2 + 1" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2, 4, 7, 6}, {2, 1, 5, 7, 3} };
        std::vector<double> Y = {9, 9, 23, 36, 22};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = [12, 6]: ", model, {12, 6});
    }

    // Test 3: All features constant
    {
        std::cout << "\nTest 3: All features constant" << std::endl;
        std::vector<std::vector<double>> X = { {5, 5, 5, 5, 5} };
        std::vector<double> Y = {1, 2, 3, 4, 5};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 5: ", model, {5});
    }

    // Test 4: All targets constant
    {
        std::cout << "\nTest 4: All targets constant" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2, 3, 4, 5} };
        std::vector<double> Y = {7, 7, 7, 7, 7};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 6: ", model, {6});
    }

    // Test 5: Features with zero values
    {
        std::cout << "\nTest 5: Features with zero values" << std::endl;
        std::vector<std::vector<double>> X = { {0, 0, 0, 0, 0}, {1, 2, 3, 4, 5} };
        std::vector<double> Y = {2, 4, 6, 8, 10};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = [0, 6]: ", model, {0, 6});
    }

    // Test 6: Features with negative values
    {
        std::cout << "\nTest 6: Features with negative values" << std::endl;
        std::vector<std::vector<double>> X = { {-2, -1, 0, 1, 2} };
        std::vector<double> Y = {-1, 1, 3, 5, 7};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 3: ", model, {3});
    }

    // Test 7: Single data point
    {
        std::cout << "\nTest 7: Single data point" << std::endl;
        std::vector<std::vector<double>> X = { {1} };
        std::vector<double> Y = {2};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 1: ", model, {1});
    }

    // Test 8: Large values
    {
        std::cout << "\nTest 8: Large values" << std::endl;
        std::vector<std::vector<double>> X = { {1000, 2000, 3000, 4000, 5000} };
        std::vector<double> Y = {2001, 4001, 6001, 8001, 10001};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 6000: ", model, {6000});
    }

    // Test 9: Different scales
    {
        std::cout << "\nTest 9: Features with different scales" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2, 3, 4, 5}, {1000, 2000, 3000, 4000, 5000} };
        std::vector<double> Y = {2002, 4004, 6006, 8008, 10010};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = [6, 6000]: ", model, {6, 6000});
    }

    // Test 10: Noisy data
    {
        std::cout << "\nTest 10: Noisy data" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2, 3, 4, 5} };
        std::vector<double> Y = {5.1, 6.9, 9.2, 10.8, 13.05};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = 6: ", model, {6});
    }

    // Test 11: More features than samples
    {
        std::cout << "\nTest 11: More features than samples" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2}, {3, 4}, {5, 6} };
        std::vector<double> Y = {10, 20};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = [7, 8, 9]: ", model, {7, 8, 9});
    }

    // Test 12: Predict with mismatched feature size
    {
        std::cout << "\nTest 12: Predict with mismatched feature size" << std::endl;
        std::vector<std::vector<double>> X = { {1, 2, 3}, {4, 5, 6} };
        std::vector<double> Y = {10, 20, 30};
        sklearn_cpp::linear_model::LinearRegression model;
        model.fit(X, Y);
        safe_predict("Prediction for x = [7]: ", model, {7});
    }

    return 0;
}