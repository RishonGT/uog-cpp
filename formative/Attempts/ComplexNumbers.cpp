#include <iostream>

class ComplexNumber {
    double re;
    double im;

public:
    ComplexNumber(double re = 0.0, double im = 0.0) : re(re), im(im) {}

    ComplexNumber operator+(const ComplexNumber& other){
        return ComplexNumber(re + other.re, im + other.im);
    }

    ComplexNumber operator*(const ComplexNumber& other){
        return ComplexNumber(re * other.re - im * other.im, re * other.im + im * other.re);
    }

    ComplexNumber& operator=(const ComplexNumber& other){
        if (this != &other) {
            re = other.re;
            im = other.im;
        }
        return *this;
    }

    ComplexNumber pow(const int n){
        ComplexNumber res = *this;
        for (int i = 1; i < n; i++){
            res = res * *this;
        }
        return res;
       
    }

    friend std::ostream& operator<<(std::ostream& out, const ComplexNumber& comp_num) {
        
        if (comp_num.im == 0 && comp_num.re == 0){
            out << "0";
        }
        else if (comp_num.re == 0){
            if (comp_num.im == 1) {;
                out << "i";
            }

            else {
                out << comp_num.im << " + " << "i"; 
            }
        }
    
        else if (comp_num.im == 0){
            out << comp_num.re;
        }

        else if (comp_num.im == 1){
            out << comp_num.re << " + " << "i";
        }
        
        else{
            out << comp_num.re << " + " << comp_num.im << "i";
        }
        
        return out;
        

    }

};

class RealNumber : public ComplexNumber {
public:
    RealNumber(double re) : ComplexNumber(re, 0.0) {}
};

int main() {
    // Dynamically allocate array of 5 ComplexNumber
    ComplexNumber* arr = new ComplexNumber[5];
    // Initialize: z1 = 1+2i, z2 = 2+3i, ..., z5 = 5+6i
    for (int i = 0; i < 5; ++i) {
        arr[i] = ComplexNumber(i + 1, i + 2);
    }
    // Print the complex numbers
    for (int i = 0; i < 5; ++i) {
        std::cout << "z" << (i + 1) << " = " << arr[i] << std::endl;
    }

    // Compute the sum s = z1 + z2 + ... + z5
    ComplexNumber s = ComplexNumber(0, 0);
    for (int i = 0; i < 5; ++i) {
        s = s + arr[i];
    }
    std::cout << "Sum s = ";
    std::cout << s << std::endl;
    // Clean up memory
    delete[] arr;

    ComplexNumber z = ComplexNumber(1, 2);
    ComplexNumber Lambda = ComplexNumber(1, 0); // Start with 1
    ComplexNumber z_power = z;
    for (int i = 1; i <= 5; ++i) {
        Lambda = Lambda + z_power;
        z_power = z_power * z;
    }
    std::cout << "Lambda = 1 + z + z^2 + z^3 + z^4 + z^5 = " << Lambda << std::endl;
    return 0;
}