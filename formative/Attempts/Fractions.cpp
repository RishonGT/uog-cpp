#include <iostream>
#include <bits/stdc++.h>
class Fraction {
    private:
        int nom;
        int denom;
    
    public:
        Fraction(int nom = 0, int denom = 1):nom(nom), denom(denom) {

        if (denom == 0){
            std::cout << "Denominator Cannot Be 0, Setting To 1" << std::endl;
            this->denom = 1;

            };
        };

        Fraction(const Fraction& frac) : nom{frac.nom}, denom {frac.denom} 
        {
        }

        Fraction add(const Fraction& other){
            int newNom = (nom*other.denom)+(other.nom*denom);
            int newDenom = (denom*other.denom);
            return Fraction( newNom, newDenom);
        }

        Fraction multiply(const Fraction& other){
            int newNom = nom*other.nom;
            int newDenom = denom*other.denom;
            return Fraction( newNom, newDenom);
        }

        Fraction operator+(const Fraction& other){
            int newNom = (nom*other.denom)+(other.nom*denom);
            int newDenom = (denom*other.denom);
            return Fraction( newNom, newDenom);
        };

        Fraction operator*(const Fraction& other){
            int newNom = nom*other.nom;
            int newDenom = denom*other.denom;
            return Fraction( newNom, newDenom);
        };

        Fraction& operator=(const Fraction& other) {
            this->nom = other.nom;
            this->denom = other.denom;
            return *this;
        }

        Fraction reduce(){
            int gcd = std::__gcd(this->nom,this->denom);
            int newNom = this->nom/gcd;
            int newDenom = this->denom/gcd;
            return Fraction(newNom,newDenom);
        }

        friend std::ostream& operator<<(std::ostream& out, const Fraction& frac);


};

std::ostream& operator<<(std::ostream& out, const Fraction& frac){
    if (frac.nom == 0){
        out << "0";

    } else if (frac.denom == 1){
        out << frac.nom;

    } else if (static_cast<double>(frac.nom)/static_cast<double>(frac.denom) > 0){
        if (frac.nom < 0){
            out << frac.nom * -1 << "/" << frac.denom*-1;
        } else{
            out << frac.nom << "/" << frac.denom;
        }

    } else{
        if (frac.nom < 0){
            out << "-" << frac.nom*-1 << "/" << frac.denom;
        } else{
            out << "-" << frac.nom << "/" << frac.denom*-1;
        }
    
    }

    return out;
};


int main () {
    std::vector<Fraction> vector;
    for (int i=2; i<7; i++){
        Fraction F{1,i};
        vector.push_back(F);
    }

    Fraction result(1,1);
    for (int i=0;i<5;i++){
        result = result * vector[i];
    }

    std::cout << result << std::endl;
    return 0;

    
};