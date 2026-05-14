#include "Read_CSV.hpp"
#include "sklearn_logistical.hpp"

#include <iostream>
#include <sstream>
#include <vector>


int main()
{
    std::cout << "Reading training data..." << std::endl;
    std::vector<std::vector<double>> X;
    std::vector<double> Y;
    Read_CSV::read_csv("/Users/quantumpip-boy/Documents/Code/ML_Multiclass/assignments/assignment-02/CPP_Code/LogicalRegression/Data/mnist_train.csv", X, Y, true);
    std::cout << "Data read successfully! Now data will go through preprocessing..." << std::endl;
    linear_model::DatasetChecker checker;
    checker.Check_data(X, Y, 10);
    std::cout << "Data preprocessing completed! Now training will start..." << std::endl;
    linear_model::LogisticalRegressionMulticlass model(X[0].size(), 0.001, 10);
    std::vector<double> costs = model.fit(X, Y, 10);

    std::cout << "Training completed." << std::endl;

    std::cout << "Reading test data..." << std::endl;
    Read_CSV::read_csv("/Users/quantumpip-boy/Documents/Code/ML_Multiclass/assignments/assignment-02/CPP_Code/LogicalRegression/Data/mnist_test.csv", X, Y, true);
    
    model.predict_picture(X, Y);
    std::cout << "Testing completed." << std::endl;
    return 0;

}



