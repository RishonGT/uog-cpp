#include <iostream>
#include "linear_function"


int main(){
    double k = 3.0;
    double m = 2.0;

    double x[1000];
    double y[1000];
    double data[1000][1000];
    for(int i = 1; i <= 1000; i++){
        x[i] = i;
        y[i] = linear_function(x[i], m, k);

        std::cout << y[i] << '\n';
    } 
    return 0;
}