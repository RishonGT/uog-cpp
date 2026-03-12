#include <iostream>

class Fraction{
    private:
        int nom;
        int denom;
    public:
        Fraction(int nom = 0, int denom = 1) : nom(nom), denom(denom){
            if (denom ==0){
                std::cout << "Denominator must not be zero, setting to 1 by default."<< std::endl;
                denom = 1;
            };
        };

        Fraction add(const Fraction& other){
            int total_nom = nom * other.denom + other.nom * denom;
            int total_denom = denom * other.denom;
            return Fraction(total_nom, total_denom);
        };

        Fraction multiply(const Fraction& other){
            int total_nom = nom*other.nom;
            int total_denom = denom*other.denom;
            return Fraction(total_nom, total_denom);
        };

        friend std::ostream& operator<<(std::ostream& out, const Fraction& frac){
            if (frac.nom == 0){
                out << "0";
            }
            else if (frac.denom == 1){
                out << frac.nom;
            }
            else if (frac.nom/frac.denom > 0){
                out << frac.nom << "/" << frac.denom;
            }
            }
        }
    };

