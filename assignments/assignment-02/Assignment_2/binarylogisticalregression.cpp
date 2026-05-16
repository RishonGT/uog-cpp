#include "./header/sklearn.hpp"
#include "./header/Read_CSV.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

int main()
{
    //Create and read the data into a primary data vector of vectors
    std::vector<std::vector<double>> ecg_x;
    std::vector<double> ecg_y;

    Read_CSV::read_csv("./data/ecg.csv",ecg_x,ecg_y);
    if (ecg_x.empty()||ecg_x[0].size() != ecg_y.size()){
        std::cerr << "Failed to read dataset. Exiting." << std::endl;
        return 1;
    }

    const size_t ecg_features {ecg_x.size()};
    const size_t ecg_samples {ecg_x[0].size()};
    std::cout << "Dataset size: " << ecg_samples << " samples, " << ecg_features << " features" << std::endl;

    //Split the data vector into training and testing
    //Calculate the index which to split the data at
    const size_t train_size = static_cast<size_t>(0.8 * ecg_samples); //80% will be used to train

    //A vector to hold shuffled indices
    std::vector<size_t> indices(ecg_samples);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{42});
    
    //Create and size vectors to match expected sizes from the data vector
    std::vector<std::vector<double>> ecg_x_train(ecg_x.size(), std::vector<double>(train_size));
    std::vector<double> ecg_y_train(train_size);

    std::vector<std::vector<double>> ecg_x_test(ecg_x.size(), std::vector<double>(ecg_samples - train_size));
    std::vector<double> ecg_y_test(ecg_samples - train_size);

    //Split dataset between training and testing
    //Data is aligned by features, where datasets sample are within each feature
    for (size_t i{0}; i<ecg_features; ++i){
        for (size_t j{0}; j<train_size; ++j){
            ecg_x_train[i][j] = std::move(ecg_x[i][indices[j]]);
        }
        for (size_t j{train_size}; j < ecg_samples; ++j){
            ecg_x_test[i][j-train_size] = std::move(ecg_x[i][indices[j]]);
        }
    }

    //Split labels between training and testing
    for (size_t i{0};i<train_size;++i){
        ecg_y_train[i] = std::move(ecg_y[indices[i]]);
    }
    for (size_t i{train_size}; i<ecg_samples; ++i){
        ecg_y_test[i-train_size] = std::move(ecg_y[indices[i]]);
    }

    sklearn_cpp::linear_model::LogisticRegression log_reg;
    log_reg.fit(ecg_x_train,ecg_y_train);

    //Test the model by prediciting all the x_test cases with our model and assigning true or false
    std::vector<double> y_predict (ecg_x_test[0].size());
    for(size_t i{0}; i<ecg_y_test.size();i++)
    {
        if (log_reg.predict(ecg_x_test,i) > 0.5)
            {y_predict[i] = 1;}
        else 
            {y_predict[i] = 0;}
    }

    int correct_predictions{};
    for(size_t i{0}; i<ecg_y_test.size();i++)
    {
        if(y_predict[i]==ecg_y_test[i]){correct_predictions++;}
    }
    double accuracy {static_cast<double>(correct_predictions)/ecg_y_test.size()};
    std::cout << "Model accuracy: " << accuracy << '\n';

   
    return 0;
}