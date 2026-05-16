#include "header/sklearn.hpp" // Allows access to LinearRegression and LogisticRegression classes
#include "header/Read_CSV.hpp" // Allows access to Read_CSV class for reading datasets from CSV files

#include <algorithm> // For std::shuffle
#include <random> // For std::mt19937

// Helper function to print a banner message
void print_banner(const std::string& message) {
    const int width = 50;
    std::cout << "\n\n" << std::string(width, '=') << std::endl;
    std::cout << message << std::endl;
    std::cout << std::string(width, '=') << std::endl;
}


int main() {
    print_banner("Cement Strength Prediction using Linear Regression");

    std::vector<std::vector<double>> concrete_features;
    std::vector<double> concrete_targets;

    Read_CSV::read_csv("data/concrete.csv", concrete_features, concrete_targets);

    if (concrete_features.empty() || concrete_targets.empty() || concrete_features[0].size() != concrete_targets.size()) {
        std::cerr << "Failed to read dataset. Exiting." << std::endl;
        return 1;
    }
    
    const size_t n_features = concrete_features.size();  // Number of features
    const size_t n_samples = concrete_features[0].size();  // Number of samples (assuming all feature columns have the same number of samples)

    // Print size of the dataset
    std::cout << "Dataset size: " << n_samples << " samples, " << n_features << " features" << std::endl;

    print_banner("Splitting dataset into training and testing sets");

    const size_t train_size = static_cast<size_t>(0.8 * n_samples);  // 80% for training, 20% for testing

    // Prepare training and testing datasets
    std::vector<std::vector<double>> x_train(n_features, std::vector<double>(train_size));
    std::vector<double> y_train(train_size);

    // Prepare testing dataset
    std::vector<std::vector<double>> x_test(n_features, std::vector<double>(n_samples - train_size));
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
            x_train[feature][sample] = concrete_features[feature][indices[sample]];
        }

        for (size_t sample = train_size; sample < n_samples; ++sample) {
            x_test[feature][sample - train_size] = concrete_features[feature][indices[sample]];
        }
    }

    // Distribute target values into training and testing sets based on shuffled indices
    for (size_t sample = 0; sample < train_size; ++sample) {
        y_train[sample] = concrete_targets[indices[sample]];
    }

    for (size_t sample = train_size; sample < n_samples; ++sample) {
        y_test[sample - train_size] = concrete_targets[indices[sample]];
    }

    print_banner("Training Linear Regression Model");
    sklearn_cpp::linear_model::LinearRegression model;

    if (!model.fit(x_train, y_train)) {
        std::cerr << "Model fitting failed. Exiting." << std::endl;
        return 1;
    }

    print_banner("Evaluating Model on Test Set");
    double r2 = model.score(x_test, y_test);
    std::cout << "R² score on test set: " << r2 << std::endl;

    // Polynomial feature transformation and fitting a new model
    print_banner("Training Polynomial Regression Model (degree 2)");
    sklearn_cpp::preprocessing::PolynomialFeatures poly_transformer(2, false);

    std::vector<std::vector<double>> x_train_poly = poly_transformer.transform(x_train);
    std::vector<std::vector<double>> x_test_poly = poly_transformer.transform(x_test);

    sklearn_cpp::linear_model::LinearRegression poly_model;

    if (!poly_model.fit(x_train_poly, y_train)) {
        std::cerr << "Polynomial model fitting failed. Exiting." << std::endl;
        return 1;
    }

    print_banner("Evaluating Polynomial Model on Test Set");
    double r2_poly = poly_model.score(x_test_poly, y_test);
    std::cout << "R² score on test set with polynomial features: " << r2_poly << std::endl;

    // Final summary of results
    print_banner("Summary of Results");
    std::cout << "Linear Regression R² score: " << r2 << std::endl;
    std::cout << "Polynomial Regression R² score: " << r2_poly << std::endl;
    print_banner("End of Cement Strength Prediction");

    // Binary Logistic Regression on ECG dataset
    print_banner("Binary Logistic Regression on ECG Dataset");
    std::vector<std::vector<double>> ecg_x;
    std::vector<double> ecg_y;

    Read_CSV::read_csv("data/ecg.csv", ecg_x, ecg_y);
    if (ecg_x.empty() || ecg_y.empty() || ecg_x[0].size() != ecg_y.size()) {
        std::cerr << "Failed to read ECG dataset. Exiting." << std::endl;
        return 1;
    }

    const size_t ecg_features = ecg_x.size();
    const size_t ecg_samples = ecg_x[0].size();
    std::cout << "ECG Dataset size: " << ecg_samples << " samples, " << ecg_features << " features" << std::endl;

    // Split ECG dataset into training and testing sets
    const size_t ecg_train_size = static_cast<size_t>(0.8 * ecg_samples);

    std::vector<size_t> ecg_indices(ecg_samples);
    std::iota(ecg_indices.begin(), ecg_indices.end(), 0);
    std::shuffle(ecg_indices.begin(), ecg_indices.end(), std::mt19937{42});

    // Prepare training and testing datasets for ECG
    std::vector<std::vector<double>> ecg_x_train(ecg_features, std::vector<double>(ecg_train_size));
    std::vector<double> ecg_y_train(ecg_train_size);  

    std::vector<std::vector<double>> ecg_x_test(ecg_features, std::vector<double>(ecg_samples - ecg_train_size));
    std::vector<double> ecg_y_test(ecg_samples - ecg_train_size);

    // Distribute ECG samples into training and testing sets based on shuffled indices
    for (size_t feature = 0; feature < ecg_features; ++feature) {
        for (size_t sample = 0; sample < ecg_train_size; ++sample) {
            ecg_x_train[feature][sample] = ecg_x[feature][ecg_indices[sample]];
        }
        for (size_t sample = ecg_train_size; sample < ecg_samples; ++sample) {
            ecg_x_test[feature][sample - ecg_train_size] = ecg_x[feature][ecg_indices[sample]];
        }
    }

    for (size_t sample = 0; sample < ecg_train_size; ++sample) {
        ecg_y_train[sample] = ecg_y[ecg_indices[sample]];
    }
    for (size_t sample = ecg_train_size; sample < ecg_samples; ++sample) {
        ecg_y_test[sample - ecg_train_size] = ecg_y[ecg_indices[sample]];
    }

    



}