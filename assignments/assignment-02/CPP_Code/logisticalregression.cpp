#include "sklearn_logistical.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


int main()
{
    std::ifstream inf{"../data/ecg.csv"};
    if (!inf)
    {
        std::cerr << "Bad Files";
        return 1;
    }

    std::string strInput{};
    std::vector<std::vector<double> > data{};
    
    while (std::getline(inf, strInput))
    {
        std::stringstream lineStream(strInput);
        std::string cell {};
        
        std::vector<double> stringrow {};
        while(std::getline(lineStream, cell, ','))
            stringrow.push_back(std::stod(cell));
        
        data.push_back(stringrow);
    }    



    std::cout << data[0].back();
    return 0;
}