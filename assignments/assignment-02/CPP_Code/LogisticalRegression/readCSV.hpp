#ifndef READCSV_H
#define READCSV_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class ReadCSV
{
private:
    char m_delimiter{};
    std::string m_filename{};
    int m_header{};

public:
ReadCSV(std::string filename, char delimiter,int header)
    :m_filename{ filename }
    ,m_delimiter{ delimiter}
    ,m_header{ header }
{
}

void printfilename() const
{
    std::cout << "Filename:" << m_filename << "\n";
}

DataFrame readCSV() const
{
    std::ifstream inputfile{m_filename};
    //Check if there is a file
    if (!inputfile)
    {
        std::cerr << "File not found \n";
        return {};
    }

    //Skip line(s) for header
    for (int i{0}; i < m_header; ++i)
    {
        std::cout << "Skipped line for header: " << i+1 << "\n";
        std::string dummyline;
        std::getline(inputfile, dummyline);
    }
    

    //Put data into the struct defined in the header file. 
    std::string strInput{};
    DataFrame data{};

    //Acess the row within the line
    while(std::getline(inputfile, strInput))
    {
        std::stringstream lineStream(strInput);
        std::string cell {};

        //Access the cell within the line
        std::vector<double> stringrow {};
        while(std::getline(lineStream, cell, m_delimiter))
        {
            //each cell is put into a row vector
            stringrow.push_back(std::stod(cell));
        }

        //We are assuming the label is in the last coloumn
        data.Y.push_back(std::move(stringrow.back()));
        stringrow.pop_back();
        data.X.push_back(std::move(stringrow));
    }

    inputfile.close();

    return data;
}
};

#endif