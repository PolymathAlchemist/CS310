/*
* Student Name: Jesse Bunnell
* File Name: Week11Errors.cpp
* Date: 05/21/2026
*
*   PSEUDOCODE
*   1. Define a template class to store two values.
*   2. Overload the + operator to combine the stored values.
*   3. In a member function, throw an exception if division by zero is attempted.
*   4. In main, create objects and try to use the class.
*   5. Catch the exception and display a message.
*   6. Include intentional errors so the program does not execute.
*/

#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class PairBox
{
private:
    T firstValue;
    T secondValue;

public:
    PairBox(T first, T second) : firstValue(first), secondValue(second) {}

    // Overloaded + operator
    PairBox operator+(const PairBox& other) const
    {
        return PairBox(firstValue + other.firstValue,
                       secondValue + other.secondValue);
    }

    T divideValues()
    {
        if (secondValue == 0)
        {
            throw runtime_error("Cannot divide by zero.");
        }
        return firstValue / secondValue;
    }

    void display() const
    {
        cout << "First: " << firstValue << ", Second: " << secondValue << endl;
    }
};

int main()
{
    // PSEUDOCODE FOR MAIN
    // Create two PairBox objects
    // Add them with the overloaded + operator
    // Display the result
    // Try dividing values and catch any exception

    PairBox<int> box1(10, 2);
    PairBox<int> box2(5, 0);
    PairBox<int> total = box1 + box2;

    total.display();

    try
    {
        cout << "Division result: " << box2.divideValues() << endl;
    }
    catch (const runtime_error& ex)
    {
        cout << "Caught exception: " << ex.what() << endl;
    }

    return 0;
}
