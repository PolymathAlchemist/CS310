/*
*Student Name: Daniel Preller
*File Name: Week1.cpp
*Date: 3/25/2026
*/

#include <iostream>
#include <conio.h>

using namespace std;

int main() {
    //Gets first user input
    int value1;
    cout << "\n\n  Please enter an integer: ";
    cin >> value1;

    //Gets second user input
    int value2;
    cout << "\n\n  Please enter another integer: ";
    cin >> value2;

    cout << "\n\n  " << value1 << " times " << value2 << " equals " << value1 * value2 << endl;//Prints results
    getch();//Stops execution until user presses enter
}