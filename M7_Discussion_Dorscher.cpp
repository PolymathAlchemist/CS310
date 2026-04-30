/*
 * Author: Lane Dorscher
 * Date: 04/29/2026
 * Program Assignment:	Create a simple C++ program using two-dimensional arrays and at least two errors.
						The program should not execute and there should be at least two errors in the code.

 * Program Description: This program is a calendar generator that allows the user to input the number of days in a month and
						the starting day of the week. The program then generates a calendar for that month and allows the user to
						generate calendars for subsequent months.
*/

#include <iostream>
#include <string>

using namespace std;

int PopulateCalendar(const int rowCount, const int colCount, int startDay, int numOfDays, std::string calender[6][7]);
void PrintCalendar(const int rowCount, const int colCount, std::string  calender[6][7]);
void PrintWelcomeMessage();
void GetNumberofDaysInMonth(int& numOfDays);
void GetStartingWeekday(int& startDay);
bool CheckForContinue();
void Generate12MonthCalendars(const int rowCount, const int colCount, int startDay, int numOfDays, std::string calender[6][7]);


enum Weekday {
	Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
};



int main() {

	string calender[6][7];

	// Calculate number of rows
	const int rowCount = sizeof(calender) / sizeof(calender[0]);
	const int colCount = sizeof(calender[0]) / sizeof(calender[0][0]);

	int numOfDays;
	int startDay;

	PrintWelcomeMessage();

	do {

		GetNumberofDaysInMonth(numOfDays);

		GetStartingWeekday(startDay);

		//commented out as enters days for 12 months is a bit much for testing purposes, but can be uncommented to test the full functionality of the program
		//Generate12MonthCalendars(rowCount, colCount, startDay, numOfDays, calender);

		PopulateCalendar(rowCount, colCount, startDay, numOfDays, calender);
		PrintCalendar(rowCount, colCount, calender);

	} while (CheckForContinue()); // Loop indefinitely to allow multiple calendar generations
}

bool CheckForContinue()
{
	while (true) {
		char choice;

		cout << "Do you want to generate another calendar? (y/n): ";
		cin >> choice;

		if (choice == 'y' || choice == 'Y') {
			return true;
		}
		else if (choice == 'n' || choice == 'N') {
			cout << "Exiting the program. Goodbye!" << endl;
			return false;
		}
		else {
			cout << "Invalid input. Please enter 'y' or 'n'." << endl;
		}
	}
}

void GetStartingWeekday(int& startDay)
{
	while (true) {
		cout << "Menu Options: " << endl;
		cout << "   0: Sunday\n   1: Monday\n   2: Tuesday\n   3: Wednesday\n   4: Thursday\n   5: Friday\n   6: Saturday" << endl;
		cout << "Enter the starting day of the month (0-6): ";
		cin >> startDay;
		if (startDay >= 0 && startDay <= 6) {
			break; // Valid input, exit the loop
		}
		cout << "Invalid input. Please enter a number between 0 and 6." << endl;
	}
}

void GetNumberofDaysInMonth(int& numOfDays)
{
	while (true)
	{
		cout << "Enter the number of days in the month (28-31): ";
		cin >> numOfDays;
		if (numOfDays >= 28 && numOfDays <= 31) {
			break; // Valid input, exit the loop
		}
		cout << "Invalid input. Please enter a number between 28 and 31." << endl;
	}
}

void PrintWelcomeMessage() {
	cout << "Welcome to the Calendar Generator!" << endl;
	cout << "This program will help you create a calendar for any month." << endl;
	cout << "You will be prompted to enter the number of days in the month and the starting day." << endl;
	cout << "Let's get started!" << endl << endl;
}


// This function populates the calendar for a month based on the starting day and the number of days in the month. 
// It returns the last day of the week that was filled in the calendar.
int PopulateCalendar(const int rowCount, const int colCount, int startDay, int numOfDays, std::string  calender[6][7])
{
	// Fill the calendar with day numbers populating only 2 characters per day, so we can use "--" for empty days and " 1", " 2", ..., "30" for the actual days.
	int dayTracker = 1; // Start tracking days from 1
	int lastDayWeek = startDay; // Variable to keep track of the last day filled in the calendar
	for (int week = 0; week < rowCount; week++) {

		for (int day = 0; day < colCount; day++) {
			if (week == 0 && day < startDay || dayTracker > numOfDays) {
				calender[week][day] = "--";
				continue; // Stop filling if we exceed the number of days in the month
			}
			string dayStr = to_string(dayTracker);
			calender[week][day] = ((dayTracker < 10) ? " " : "") + dayStr; // Format single-digit days with a leading space
			dayTracker++;
			lastDayWeek = day; // Update the last day of the week
		}
	}

	return lastDayWeek;
}

// This function populates and prints the calendar for all 12 months, updating the starting day for each subsequent month based on the last day of the previous month.
void Generate12MonthCalendars(const int rowCount, const int colCount, int startDay, int numOfDays, std::string  calender[6][7]) {

	for (int month = 1; month <= 12; month++) {
		startDay = PopulateCalendar(rowCount, colCount, startDay, numOfDays, calender) + 1;
		if (startDay > 6) {
			startDay = startDay % 7; // Wrap around to the next week if we exceed Saturday
		}
		PrintCalendar(rowCount, colCount, calender);

		// get number of days for the next month
		GetNumberofDaysInMonth(numOfDays);
	}
}

void PrintCalendar(const int rowCount, const int colCount, std::string  calender[6][7])
{
	// Print the calendar
	cout << "-------------------------------------------" << endl;
	cout << "| Sun | Mon | Tue | Wed | Thu | Fri | Sat |" << endl;
	cout << "===========================================" << endl;


	for (int r = 0; r < rowCount; r++) {
		for (int c = 0; c < colCount; c++) {
			cout << "|  " << calender[r][c] << " ";
		}
		cout << "|" << endl; // New line after each week
		cout << "-------------------------------------------" << endl;
	}
	cout << endl; // Add an extra line for better readability between calendars
}






