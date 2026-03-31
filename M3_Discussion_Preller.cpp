/*
 *Student Name: Daniel Preller
 *File Name: Week2.cpp
 *Date: 3/30/2026
 */

#include <conio.h>
#include <iostream>

using namespace std;

int main() {
    int miles;

    //Gets user input
    cout << "Enter the number of miles to be converted to feet: ";
    cin >> miles;

    int feet = miles * 5280; //Converts miles to feet (the number is correct and not a trick logic error)

    //Properly pluralizes "mile" in output
    //Compatability with negative numbers is excluded for demonstration purposes
    string milesPlural;
    if (miles > 1) {
        milesPlural = "miles";
    } else if (miles == 0) {
        milesPlural = "miles";
    } else {
        milesPlural = "mile";
    }

    cout << miles << " " << milesPlural << " is equal to " << feet << " feet" << endl;

    getch();//Halts execution until user presses enter
    return 0;
}