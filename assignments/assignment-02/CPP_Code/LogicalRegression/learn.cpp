#include <iostream>
#include <sstream>
#include <vector>


int main()
{
std::vector<double> x {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

for (auto i{0}; i < x.size(); ++i)
{
    std::cout << x[i] << "\n";
}
}