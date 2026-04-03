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
    while (std::getline(inf, strInput))
    {
        std::stringstream lineStream(strInput);
        std::string cell {};
        std::getline(lineStream, cell, ',');
        std::cout << cell << "\n";
    }    
    return 0;
}