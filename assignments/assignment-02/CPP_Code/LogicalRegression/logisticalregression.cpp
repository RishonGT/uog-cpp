#include "sklearn_logistical.hpp"
#include "readCSV.hpp"

#include <iostream>
#include <sstream>
#include <vector>


int main()
{
    ReadCSV ecgdata("../../data/ecg.csv", ',', 0);
    //ecgdata.printfilename();
    auto dataframe {ecgdata.readCSV()};

    size_t x_rows {dataframe.X.size()};
    size_t x_cols {dataframe.X[0].size()};
    std::cout << "X Rows: "<< x_rows <<" "<< "Cols: "<< x_cols << "\n";

    size_t y_rows {dataframe.Y.size()};

    std::cout << "Y Rows: " << y_rows << "\n";

    //set weights and learning rate
    size_t features {x_cols-1};
    linear_model::LogisticalRegression logistical(features, 0.01);
    


    return 0;
}