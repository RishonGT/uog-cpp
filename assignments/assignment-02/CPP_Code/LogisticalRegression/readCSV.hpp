#ifndef READCSV_H
#define READCSV_H

#include <sstream>
#include <vector>

/* Encapsulates features and label so it can be read through a single function*/
struct DataFrame
{
    std::vector<std::vector<double>> X; //Features
    std::vector<double> Y; //Label
};

class ReadCSV
{
private:
    char m_delimiter{};
    std::string m_filename{};
    int m_header{};

public:
    ReadCSV(std::string m_filename, char m_delimiter,int m_header);

    void printfilename() const;

    DataFrame readCSV() const;
};

#endif