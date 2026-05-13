#include <iostream>

int findmax(const int &a, const int &b) {
    if (a > b){
        return a;
    }

    //if b > a or b = a
    else{
        return b;
    }
}

template <typename T, typename X>
X findmax(const T &a, const X &b){
    return (a > b) ? a : b;
}

int main(){
    std::cout << findmax(3, 4.5) << std::endl;
}
