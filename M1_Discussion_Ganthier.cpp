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

/*
 * DEBUGGING FIXES
 * Line  8 : Added semicolon line terminator
 * Line 11 : Changed << insertion operator to >> extraction operator
 * Line 16 through
 * Line 19 : Added a small code block to hold the CMD window open
 */