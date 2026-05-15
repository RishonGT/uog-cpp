#include "sklearn_logistical.hpp"
#include "../Read_CSV.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

int main()
{
    DataFrame ecg_data;
    Read_CSV::read_csv("../../data/ecg.csv",ecg_data.feature,ecg_data.label);
    std::cout << "Data from CSV: \n";
    ecg_data.print();

    //Print out first few datasets and features
    for(size_t i{0}; i <= 2; i++)
    {
        for(size_t j{0}; j <= 4; j++)
        {
            std::cout << ecg_data.feature[j][i] << ' ';
        }
        std::cout << "...\n";
    }

    //Calculate index to split data
    const size_t train_size = static_cast<size_t>(0.8 * ecg_data.row());
    //Shuffle Data
    std::vector<size_t> indices(ecg_data.label.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{42});
    //Create DataFrames to hold the train and test data

    DataFrame ecg_test;
    DataFrame ecg_train;

    ecg_train.feature.resize(ecg_data.col());
    ecg_train.resize(train_size);
    ecg_test.feature.resize(ecg_data.col());
    ecg_test.resize(ecg_data.row()-train_size);
    
    for (size_t j{0}; j < ecg_data.col(); ++j)
    {
        for(size_t i=0; i<train_size; ++i)
        {
            ecg_train.feature[j][i] = ecg_data.feature[j][indices[i]];
        }
        for(size_t i{train_size}; i<ecg_data.row();++i)
        {
            ecg_test.feature[j][i-train_size] = ecg_data.feature[j][indices[i]];
        }
    }

    for (size_t i{0}; i<train_size;++i)
    {
        ecg_train.label[i] = ecg_data.label[indices[i]];
    }
    for (size_t i{train_size};i<ecg_data.row();++i)
    {
        ecg_test.label[i-train_size] = ecg_data.label[indices[i]];
    }
    std::cout << "Split Data: \n";
    ecg_train.print();
    ecg_test.print();


    LogisticalRegression log_reg;
    log_reg.train(ecg_train.feature,ecg_train.label);

   
    return 0;
}