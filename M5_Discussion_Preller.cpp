/*
 * Student Name: Daniel Preller
 * File Name: Week5.cpp
 * Date: 4/13/2026
*/

#include <conio.h>
#include <iostream>

using namespace std;

//Function prototypes
int yardsToFeet(int yards);
int milesToYards(int miles);
string checkPlural(int value, string singular, string plural);

int main () {
    int miles;

    //Gets user input
    cout << "\n\n  Please enter the number of miles to be converted to yards and feet: ";
    cin >> miles;

    //Converts units
    int yards = milesToYards(miles);
    int feet = yardsToFeet(yards);

    //Prints output and holds
    cout << "  " << miles << " " << checkPlural(miles, "mile", "miles") << " is equal to " << feet << " feet, or "
    << yards << " yards." << endl;
    getch();
}

int yardsToFeet(int yards) {//Converts yards to feet
    return yards * 3;
}

int milesToYards(int miles) {//Converts miles to yards
    return miles * 1760;
}

//Takes two strings and a value
//Returns the first string if the value is one (singular), or the second otherwise (plural)
string checkPlural(int value, string singular, string plural) {
    if (value == 1)
        return singular;

    return plural;
}