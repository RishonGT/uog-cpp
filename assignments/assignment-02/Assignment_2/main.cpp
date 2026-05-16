#include "header/sklearn.hpp" // Allows access to LinearRegression and LogisticRegression classes
#include "header/Read_CSV.hpp" // Allows access to Read_CSV class for reading datasets from CSV files

#include <algorithm> // For std::shuffle
#include <random> // For std::mt19937
#include <string> 

// Helper function to print a banner message
void print_banner(const std::string& message) {
    const int width = 50;
    std::cout << "\n\n" << std::string(width, '=') << std::endl;
    std::cout << message << std::endl;
    std::cout << std::string(width, '=') << std::endl;
}

// Function to split dataset into training and testing sets
void split_train_test(const std::vector<std::vector<double>>& features, const std::vector<double>& targets,
                      std::vector<std::vector<double>>& x_train, std::vector<double>& y_train,
                      std::vector<std::vector<double>>& x_test, std::vector<double>& y_test,
                      double train_ratio = 0.8) {

    const size_t n_samples = features[0].size();
    const size_t n_features = features.size();
    const size_t train_size = static_cast<size_t>(train_ratio * n_samples);

    // Create indices and shuffle them for random splitting
    std::vector<size_t> indices(n_samples);
    for (size_t i = 0; i < n_samples; ++i) {
        indices[i] = i;
    }
    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::shuffle(indices.begin(), indices.end(), rng);

    // Prepare training and testing datasets
    x_train.resize(n_features, std::vector<double>(train_size));
    y_train.resize(train_size);
    x_test.resize(n_features, std::vector<double>(n_samples - train_size));
    y_test.resize(n_samples - train_size);

    // Distribute samples into training and testing sets based on shuffled indices
    for (size_t feature = 0; feature < n_features; ++feature) {
        for (size_t sample = 0; sample < train_size; ++sample) {
            x_train[feature][sample] = features[feature][indices[sample]];
        }
        for (size_t sample = train_size; sample < n_samples; ++sample) {
            x_test[feature][sample - train_size] = features[feature][indices[sample]];
        }
    }

    for (size_t sample = 0; sample < train_size; ++sample) {
        y_train[sample] = targets[indices[sample]];
    }
    for (size_t sample = train_size; sample < n_samples; ++sample) {
        y_test[sample - train_size] = targets[indices[sample]];
    }

}

/*The main function performs the following tasks:
  Cement Strength Prediction using Linear Regression 
  ECG Dataset Analysis with (Binary) Logistic Regression
  Digit Recognition with (Multi-class) Logistic Regression
  
  For each task, the following steps are performed:
  - Load the dataset
  - Preprocess the data
  - Split the data into training and testing sets
  - Train the model
  - Evaluate the model*/

int main() {
    //=== Cement Strength Prediction using Linear Regression ===//
    print_banner("Cement Strength Prediction using Linear Regression");

    // Read the concrete dataset from CSV file into feature matrix and target vector
    std::vector<std::vector<double>> concrete_features;
    std::vector<double> concrete_targets;

    Read_CSV::read_csv("data/concrete.csv", concrete_features, concrete_targets);

    if (concrete_features.empty() || concrete_targets.empty() || concrete_features[0].size() != concrete_targets.size()) {
        std::cerr << "Failed to read dataset. Exiting." << std::endl;
        return 1;
    }
    
    const size_t n_features = concrete_features.size();  // Number of features
    const size_t n_samples = concrete_features[0].size();  // Number of samples 

    // Print size of the dataset
    std::cout << "Dataset size: " << n_samples << " samples, " << n_features << " features" << std::endl;


    print_banner("Splitting dataset into training and testing sets");

    std::vector<std::vector<double>> x_train, x_test;
    std::vector<double> y_train, y_test;

    split_train_test(concrete_features, concrete_targets, x_train, y_train, x_test, y_test);

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
    sklearn_cpp::preprocessing::PolynomialFeatures poly_transformer(2, false); // Degree 2 polynomial features without bias term (intercept))

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



    // === Binary Logistic Regression on ECG Dataset ===
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
    std::vector<std::vector<double>> ecg_x_train, ecg_x_test;
    std::vector<double> ecg_y_train, ecg_y_test;

    split_train_test(ecg_x, ecg_y, ecg_x_train, ecg_y_train, ecg_x_test, ecg_y_test);

    // Train logistic regression model on ECG dataset
    sklearn_cpp::linear_model::LogisticRegression log_reg;
    log_reg.fit(ecg_x_train, ecg_y_train);

    // Evaluate logistic regression model on ECG test set
    size_t correct_predictions = 0;
    for (size_t i = 0; i < ecg_y_test.size(); ++i) {
        double prediction = log_reg.predict(ecg_x_test, i);
        if ((prediction > 0.5 && ecg_y_test[i] == 1) || (prediction <= 0.5 && ecg_y_test[i] == 0)) {
            ++correct_predictions;
        }
    }

    double accuracy = static_cast<double>(correct_predictions) / ecg_y_test.size() * 100.0;
    std::cout << "Logistic Regression Accuracy on ECG test set: " << accuracy << "% (" << correct_predictions << "/" << ecg_y_test.size() << " correct predictions)" << std::endl;
    print_banner("End of Binary Logistic Regression on ECG Dataset");


    // === Multiclass Logistic Regression on MNIST Dataset ===
    print_banner("Multiclass Logistic Regression on MNIST Dataset");
    std::vector<std::vector<double>> mnist_x_train;
    std::vector<double> mnist_y_train;
    std::vector<std::vector<double>> mnist_x_test;
    std::vector<double> mnist_y_test;

    Read_CSV::read_csv("data/mnist_train.csv", mnist_x_train, mnist_y_train);  

    if (mnist_x_train.empty() || mnist_y_train.empty() || mnist_x_train[0].size() != mnist_y_train.size()) {
        std::cerr << "Failed to read MNIST training dataset. Exiting." << std::endl;
        return 1;
    }

    Read_CSV::read_csv("data/mnist_test.csv", mnist_x_test, mnist_y_test);  
    
    if (mnist_x_test.empty() || mnist_y_test.empty() || mnist_x_test[0].size() != mnist_y_test.size()) {
        std::cerr << "Failed to read MNIST test dataset. Exiting." << std::endl;
        return 1;
    }

    // Ensure Data is viable
    sklearn_cpp::preprocessing::dataset_checker checker;
    checker.check_data(mnist_x_train, mnist_y_train, 10);  // Assuming 10 classes for MNIST (digits 0-9)
    checker.check_data(mnist_x_test, mnist_y_test, 10);

    // Train logistic regression model on MNIST dataset
    sklearn_cpp::linear_model::LogisticRegression multiclass_log_reg;
    multiclass_log_reg.fit(mnist_x_train, mnist_y_train);

    // Evaluate logistic regression model on MNIST test set
    multiclass_log_reg.predict(mnist_x_test, mnist_y_test);

    print_banner("End of Multiclass Logistic Regression on MNIST Dataset");

    return 0;
}