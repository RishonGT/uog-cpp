#include "header/Read_CSV.hpp"
#include "header/sklearn.hpp"

#include <iostream>
#include <sstream>
#include <vector>


int main()
{
    // Initialise the vectors to hold the pixels ...
    std::vector<std::vector<double>> X;
    std::vector<double> Y;
    std::vector<std::vector<double>> X_2;
    std::vector<double> Y_2;

    // Input train data ...
    std::cout << "Reading training data..." << std::endl;
    Read_CSV::read_csv("./data/mnist_train.csv", X, Y, true);
    std::cout << "Data read successfully! Now data will go through preprocessing..." << std::endl;

    // Preprocess to make sure data is viable ...
    sklearn_cpp::linear_model::dataset_checker checker;
    checker.check_data(X, Y, 10);
    std::cout << "Data preprocessing completed! Now training will start..." << std::endl;

    // Construct model ...
    sklearn_cpp::linear_model::LogisticRegression model(X[0].size(), 0.001, 10, 40);

    // Perform fitting (training) ...
    std::vector<double> costs = model.fit(X, Y);
    std::cout << "Training completed." << std::endl;

    // Input test data ...
    std::cout << "Reading test data..." << std::endl;
    Read_CSV::read_csv("./data/mnist_test.csv", X_2, Y_2, true);

    // Perform predicting (testing) ...
    model.predict(X_2, Y_2);
    std::cout << "Testing completed." << std::endl;
    return 0;
    
    // Model complete 

}



