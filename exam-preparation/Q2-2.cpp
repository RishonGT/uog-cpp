#include <iostream>

class ComplexNumber {
    private:
        double re;
        double im;

    public:
        ComplexNumber(double r = 0, double i = 0) : re(r), im(i){}

        friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& cn){
            if (cn.re != 0 && cn.im > 0){
                os << cn.re << "+" << cn.im << "i";
            } else if (cn.re != 0 && cn.im < 0){
                os << cn.re << "-" << std::abs(cn.im) << "i";
            } else if (cn.re != 0 && cn.im == 0){
                os << cn.re;
            } else {
                os << cn.im << "i";
            }

            return os;
        }

        ComplexNumber operator+(const ComplexNumber& other) const{
            return ComplexNumber(this->re + other.re, this->im +other.im);

        }

        ComplexNumber operator*(const ComplexNumber& other) const{
            return ComplexNumber((this->re * other.re)-(this->im * other.im), (this->re * other.im)+(this->im * other.re));
        }

        ComplexNumber pow(const int &n) const{
            ComplexNumber result(1,0);
            for (int i=0; i < n; ++i){
                ComplexNumber result = result * (*this); 
            }
            return result;
            }

        ComplexNumber& operator=(const ComplexNumber& other){
            if (this != &other){
                re = other.re;
                im = other.im;
            }
            return (*this);
        }
    };

class RealNumber : public ComplexNumber {
    public:
    RealNumber(double r) : ComplexNumber (r,0){}
};



int main(){
    ComplexNumber* z_array = new ComplexNumber[5];

    for (int j = 0; j < 5; ++j){
        z_array[j] = ComplexNumber(j+1, j+2);
    }

    ComplexNumber S(0,0);

    for (int k = 0; k < 5; ++k){
        S = S + z_array[k];
    }

    std::cout << S << std::endl;

    ComplexNumber z(1,2);

    




    delete[] z_array;
};