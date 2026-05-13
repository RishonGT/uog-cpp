#include "sklearn_logistical.hpp"
#include "../Read_CSV.hpp"

#include <iostream>
#include <sstream>
#include <vector>



int main()
{
    DataFrame ecg_data;
    Read_CSV::read_csv("../../data/ecg.csv",ecg_data.feature,ecg_data.label);
    std::cout << "Datasets: "<< ecg_data.row() <<" "<< "Features: "<< ecg_data.col() << '\n';
    
    for(size_t i{0}; i <= 2; i++)
    {
        for(size_t j{0}; j <= 4; j++)
        {
            std::cout << ecg_data.feature[j][i] << ' ';
        }
        std::cout << "...\n";
    }

    /*Testing*/
    DataFrame test_data;
    LogisticalRegression test_class;
    test_data.feature = {{1.0,2.0,3.0},{1.0,2.0,3.0},{1.0,2.0,3.0}};
    test_data.weight = {0.2,0.2,0.2};
    test_data.label = {1, 1, 1};
    test_data.bias = 0.5;

    size_t row_index {1};
    double z {test_class.predict(test_data.feature,test_data.weight,test_data.bias,row_index)};
    std::cout << "Z: " << z << '\n';
    test_class.printState(test_data.feature,test_data.label,test_data.weight,test_data.bias);
    
    return 0;
}