#include "readCSV.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//constructor for reading CSV files
ReadCSV::ReadCSV(std::string filename, char delimiter, int header)
    :m_filename{ filename }
    ,m_delimiter{ delimiter}
    ,m_header{ header }
{
}
//sanity check
void ReadCSV::printfilename() const
{
    std::cout << "Filename:" << m_filename << "\n";
}