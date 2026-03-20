//#include "Matrix.hpp"
#include <initializer_list>
#include <iostream>
#include <vector>

namespace numpy{
class ndarray
{
    public:

        std::vector<std::vector<double> > data;

        ndarray(std::initializer_list<std::initializer_list<double> > array)
        { 
            for (const auto& row : array) {
                data.emplace_back(row);
            }
            std::cout << "constructed with a " << data.size() << "-element list\n";
        };



};
}

int main(){
    numpy::ndarray s = {{1, 2.0},{3,4}}; // copy list-initialization
    return 0;
};