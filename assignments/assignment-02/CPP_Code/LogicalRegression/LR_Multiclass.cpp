#include "readCSV.hpp"
#include "sklearn_logistical.hpp"

#include <iostream>
#include <sstream>
#include <vector>


int main()
{
    
    // Load data for the current class
    ReadCSV numbers("/Users/quantumpip-boy/Documents/Code/ML_Multiclass/assignments/assignment-02/CPP_Code/LogicalRegression/Data/mnist_train.csv", ',', 1);
    auto dataframe {numbers.readCSV()};
    
    //std::cout << dataframe.Y[3000] << std::endl;
    
    linear_model::LogisticalRegressionMulticlass model(dataframe.X[0].size(), 0.1, 10);
    std::vector<double> costs = model.train(dataframe.X, dataframe.Y, 20);

    std::cout << "Training completed." << std::endl;

    ReadCSV numbers2("/Users/quantumpip-boy/Documents/Code/ML_Multiclass/assignments/assignment-02/CPP_Code/LogicalRegression/Data/mnist_test.csv", ',', 1);
    auto dataframe2 {numbers2.readCSV()};
    std::cout << dataframe2.X.size() << std::endl; 
    model.predict_picture(dataframe2.X, dataframe2.Y);
    std::cout << "Testing completed." << std::endl;
    return 0;

}



