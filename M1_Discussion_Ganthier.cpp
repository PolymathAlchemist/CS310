#include <iostream>
using namespace std;

int main() {
    int userAge;
    
    // Prompt the user for their age
    cout << "Please enter your age: ";
    
    // Take the user's input
    cin >> userAge;
    
    // Output the result
    cout << "Wow, you are " << userAge << " years old!" << endl;

    // Keep window open until Enter is pressed
    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
    
    return 0;
}
