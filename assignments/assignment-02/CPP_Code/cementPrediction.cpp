#include "sklearn.hpp"  // For LinearRegression and PolynomialFeatures
#include "Read_CSV.hpp"  // For reading the dataset
#include <algorithm>
#include <random>

int main() {
    std::vector<std::vector<double>> x;
    std::vector<double> y;

    Read_CSV::read_csv("../data/concrete.csv", x, y);

    if (x.empty() || y.empty() || x[0].size() != y.size()) {
        std::cerr << "Failed to read dataset. Exiting." << std::endl;
        return 1;
    }
    
    // Print the first 5 rows of the dataset to verify it was read correctly
    std::cout << "First 5 samples of the dataset:" << std::endl;
    const size_t n_features = x.size();  // Number of features
    const size_t n_samples = x[0].size();  // Number of samples (assuming all feature columns have the same number of samples)

    for (size_t sample = 0; sample < std::min(n_samples, static_cast<size_t>(5)); ++sample) {

        for (size_t feature = 0; feature < n_features; ++feature) {
            std::cout << x[feature][sample] << " ";
        }

        std::cout << "| " << y[sample] << std::endl;

    }
    std::cout << "\n\n" << std::endl;

    // Print size of the dataset
    std::cout << "Dataset size: " << x[0].size() << " samples, " << x.size() << " features" << std::endl;
    std::cout << "Target size: " << y.size() << "\n\n" << std::endl;

    std::cout << "Splitting dataset into training and testing sets..." << "\n\n" << std::endl;
    const size_t train_size = static_cast<size_t>(0.8 * n_samples);  // 80% for training, 20% for testing

    // Prepare training and testing datasets
    std::vector<std::vector<double>> x_train(x.size(), std::vector<double>(train_size));
    std::vector<double> y_train(train_size);

    std::vector<std::vector<double>> x_test(x.size(), std::vector<double>(n_samples - train_size));
    std::vector<double> y_test(n_samples - train_size);


    // Index Shuffling to ensure random distribution of samples in train and test sets
    std::vector<size_t> indices(n_samples);
    for (size_t i = 0; i < n_samples; ++i) {
        indices[i] = i;
    }

    // Use a fixed seed for reproducibility
    std::mt19937 rng(42);
    std::shuffle(indices.begin(), indices.end(), rng);

    // Distribute samples into training and testing sets based on shuffled indices
    for (size_t feature = 0; feature < n_features; ++feature) {
        for (size_t sample = 0; sample < train_size; ++sample) {
            x_train[feature][sample] = x[feature][indices[sample]];
        }
        for (size_t sample = train_size; sample < n_samples; ++sample) {
            x_test[feature][sample - train_size] = x[feature][indices[sample]];
        }
    }

    // Distribute target values into training and testing sets based on shuffled indices
    for (size_t sample = 0; sample < train_size; ++sample) {
        y_train[sample] = y[indices[sample]];
    }
    
    for (size_t sample = train_size; sample < n_samples; ++sample) {
        y_test[sample - train_size] = y[indices[sample]];
    }

    sklearn_cpp::linear_model::LinearRegression model;

    // Fit the linear regression model on the training data
    if (!model.fit(x_train, y_train)) {
        std::cerr << "Model fitting failed. Exiting." << std::endl;
        return 1;
    }

    // Evaluate the model using R-squared metric on both training and testing sets
    double r2_train = model.score(x_train, y_train);
    std::cout << "R-squared on training set: " << r2_train << std::endl;
    double r2_test = model.score(x_test, y_test);
    std::cout << "R-squared on test set: " << r2_test << std::endl;
 
    // Polynomial Features Test
    sklearn_cpp::preprocessing::PolynomialFeatures poly(2);

    // Transform the original features into polynomial features
    std::vector<std::vector<double>> x_train_poly = poly.transform(x_train);
    std::vector<std::vector<double>> x_test_poly = poly.transform(x_test);

    sklearn_cpp::linear_model::LinearRegression poly_model;

    std::cout << "\nOriginal Features: " << x_train.size() << "\nPolynomial Features: " << x_train_poly.size() << "\n\n" << std::endl;

    if (!poly_model.fit(x_train_poly, y_train)) {
        std::cerr << "Polynomial model fitting failed. Exiting." << std::endl;
        return 1;
    }
    
    // Evaluate the polynomial model using R-squared metric on both training and testing sets
    double r2_poly_train = poly_model.score(x_train_poly, y_train);
    std::cout << "R-squared on polynomial training set: " << r2_poly_train << std::endl;
    double r2_poly_test = poly_model.score(x_test_poly, y_test);
    std::cout << "R-squared on polynomial test set: " << r2_poly_test << std::endl;


    // Final comparison of results
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "\n\nFinal Comparison:" << std::endl;
    std::cout << "Linear Regression R-squared (train): " << r2_train << std::endl;
    std::cout << "Linear Regression R-squared (test): " << r2_test << std::endl;
    std::cout << "Polynomial Regression R-squared (train): " << r2_poly_train << std::endl;
    std::cout << "Polynomial Regression R-squared (test): " << r2_poly_test << std::endl;
    std::cout << "----------------------------------------------" << std::endl;


    

    return 0;
}