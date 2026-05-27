#include <iostream>
#include <stdexcept>

using namespace std;

// ========================================================================
// Class Template Definition
// ========================================================================
template <typename T>
class DataHandler {
private:
    T data;

public:
    // Constructor
    DataHandler(T val) : data(val) {}

    // Overloaded Operator: Allows us to add two DataHandler objects together
    DataHandler operator+(const DataHandler& other) {
        return DataHandler(this->data + other.data);
    }

    // Member Function with Exception Handling
    void divideData(T divisor) {
        try {
            if (divisor == 0) {
                // Throw a standard runtime error if division by zero is attempted
                throw runtime_error("Attempted to divide by zero!");
            }
            data = data / divisor;
            cout << "Division successful. New data: " << data << endl;
            
        }
        catch (const runtime_error& e) {
            // ERROR 2: The error is located in this catch block
            cout << "Exception Handled: " << e.what() << endl;
        }
    }

    // Getter
    T getData() const {
        return data;
    }
};

// ========================================================================
// Main Execution
// ========================================================================
int main() {
    // Instantiate template classes with integers
    DataHandler<int> obj1(50);
    DataHandler<int> obj2(10);

    // Utilize the overloaded + operator
    DataHandler<int> result = obj1 + obj2;
    cout << "Result of addition: " << result.getData() << endl;

    // Attempt to trigger the exception handling inside the member function
    result.divideData(0);

    return 0;
}

