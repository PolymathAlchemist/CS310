/* Author: Lane Dorscher
 * Date: 04-22-2026
 * File: M6-Discussion-LaneDorscher.cpp
 * Description: 
 * 
 * Program Prompt: Create a simple C++ program using the enumeration data type and at least two errors. 
 * The program should not execute and there should be at least two errors in the code.
*/


#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

enum Weekday {
    Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
};

void ParseDate(const string& input, int& m, int& d, int& y);
Weekday CalculateWeekday(int m, int d, int y);
string WeekdayToString(Weekday wd);


int main() {
    // user entered date
    string input;
    cout << "Enter a date (MM/DD/YYYY): ";
    getline(cin, input);

    int m, d, y; // month, day, year
    ParseDate(input, m, d, y);

    Weekday userWd = CalculateWeekday(m, d, y);
    cout << "That date is a " << WeekdayToString(userWd) << "\n\n";

    // grab today's date and calculate the weekday
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    Weekday todayWd = static_cast<Weekday>(now->tm_wday);

    cout << "\nPress Enter to exit...";
    cin.get();

    return 0;
}


// Converts the weekday enum to a string for display
string WeekdayToString(Weekday wd) {
    //No break statements are needed here because we are returning from each case, so the function will exit immediately after a match is found.
    switch (wd) {
        case Weekday::Sunday: return "Sunday";
        case Weekday::Monday: return "Monday";
        case Weekday::Tuesday: return "Tuesday";
        case Weekday::Wednesday: return "Wednesday";
        case Weekday::Thursday: return "Thursday";
        case Weekday::Friday: return "Friday";
        case Weekday::Saturday: return "Saturday";
        default: return "";
    }
}

// Calculates the weekday for a given date using the tm structure and mktime function
Weekday CalculateWeekday(int m, int d, int y) {
    tm timeStruct = {};
    timeStruct.tm_year = y - 1900; 
    timeStruct.tm_mon  = m - 1;    
    timeStruct.tm_mday = d;

    mktime(&timeStruct); //normalizes the tm structure and fills in the tm_wday field

    return static_cast<Weekday>(timeStruct.tm_wday);
}

// Parses a date string in the format MM/DD/YYYY and extracts the month, day, and year
void ParseDate(const string& input, int& m, int& d, int& y) {
    
    m = stoi(input.substr(0, 2));
    d = stoi(input.substr(3, 2));
    y = stoi(input.substr(6, 4));
}