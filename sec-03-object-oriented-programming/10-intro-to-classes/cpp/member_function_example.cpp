// Member function version
#include <iostream>

struct Date
{
    int year {};        // we don't use m_ here for struct.
    int month {};
    int day {};

    void print() // defines a member function named print
    {
        std::cout << year << '/' << month << '/' << day;
    }
};

int main()
{
    Date today { 2026, 02, 26 }; // aggregate initialize our struct
    today.day = 16; // member variables accessed using member selection operator (.)
    today.print();  // member functions also accessed using member selection operator (.)
    std::cout << '\n';
    return 0;
}