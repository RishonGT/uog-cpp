#include <iostream>

class ComplexNumber{
    private:
        float re;
        float im;
    public:
        ComplexNumber(float re, float im) : re{re}, im{im}{}

        ComplexNumber add(const ComplexNumber& other){
            return ComplexNumber(this->re+other.re,this->im +other.im);
        }

        ComplexNumber multiply_by_scalar(const float& scalar){
            return ComplexNumber(this->re * scalar, this->im * scalar);

        }

        ComplexNumber multiply(const ComplexNumber& other){
            return ComplexNumber(this->re*other.re - this->im * other.im, this->re * other.im + this->im * other.re);
        }

        friend std::ostream& operator<<(std::ostream& out, const ComplexNumber& comp_num);
};

std::ostream& operator<<(std::ostream& out, const ComplexNumber& comp_num){
    if (comp_num.re != 0 & comp_num.im != 0){
        out << comp_num.re << " + " << comp_num.im << "*i";
    }

    else if (comp_num.re == 0 & comp_num.im == 0){
        out << 0;
    }

    else if (comp_num.im == 0){
        out << comp_num.re;
    }

    else if (comp_num.re == 0){
        out << comp_num.im << "*i";
    }


    return out;

}

int main(){
    ComplexNumber num1{0.0, 0.0};
    std::cout << num1 << std::endl;

}