#include <iostream>

class ComplexNumber{
    private:
    double re;
    double im;

    public:
    ComplexNumber(double re = 0, double im = 0) : re {re}, im {im} {}

    friend std::ostream& operator<<(std::ostream& out, const ComplexNumber& comp_num);

    ComplexNumber operator+(const ComplexNumber& other){
        double re_tot = this->re + other.re;
        double im_tot = this->im + other.im;

        return ComplexNumber(re_tot,im_tot);

    }

    ComplexNumber operator*(const ComplexNumber& other){
        double re_tot = this->re * other.re - this->im * other.im;
        double im_tot = this->re * other.im + this->im * other.re;

        return ComplexNumber(re_tot, im_tot);
    }

    ComplexNumber pow(const ComplexNumber& cn, const int& n) const{
        if (n==0){
            return ComplexNumber(1,0);
        } else if (n==1){
            return ComplexNumber(cn);
        } else if (n > 1){
            ComplexNumber result(1,0);
            for (int i = 0; i < n ; ++i){
                result = result * cn;
            }
            return result;
        }
    }

    ComplexNumber& operator=(const ComplexNumber& other){
        if (this != &other){
            re = other.re;
            im = other.im;
        }
        return *this;
    }
};

class RealNumber : public ComplexNumber {
    public:
    RealNumber(double re) : ComplexNumber(re,0){};



};

std::ostream& operator<<(std::ostream& out, const ComplexNumber& comp_num){
    if (comp_num.re != 0 && comp_num.im == 0){
        out << comp_num.re;
    } else if (comp_num.re == 0 && comp_num.im != 0){
        out << comp_num.im << 'i';
    } else if (comp_num.re != 0 && comp_num.im < 0){
        out << comp_num.re << '+' << comp_num.im << 'i';
    } else if (comp_num.re != 0 && comp_num.im > 0){
        out << comp_num.re << '-' << comp_num.im << 'i';
    } else {
        out << comp_num.re;
    }

    return out;
}

int main(){
    ComplexNumber* z_array = new ComplexNumber[5];

    for (int j = 0 ; j < 5 ; ++j){
        z_array[j] = ComplexNumber(
            static_cast<double>(j+1),
            static_cast<double>(j+2));
    }

    ComplexNumber s(0,0);

    for (int k = 0 ; k < 5; ++k) {
        s = s + z_array[k];
    }

    ComplexNumber z(1,2);

    ComplexNumber tot(0, 0);

    for (int l = 0; l < 6; ++l){
        tot = tot + ComplexNumber.pow(z, l)
    }




    delete[] z_array;
}