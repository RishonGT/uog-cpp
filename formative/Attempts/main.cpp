#include "Matrix.hpp"
namespace np = numpy;

int main() {
    // Creating a 2D matrix
    auto mat2d = np::array({
        {1.1, 2.2, 3.3},
        {4.4, 5.5, 6.6}
    });

    // Creating a 1D matrix (row vector)
    auto mat1d = np::array({10.0, 20.0, 30.0});

    std::cout << "2D Matrix:" << std::endl;
    std::cout << mat2d;

    return 0;
}