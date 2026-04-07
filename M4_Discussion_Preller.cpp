/*
 *Student Name: Daniel Preller
 *File Name: Week4.cpp
 *Date: 4/6/2026
 */

#include <conio.h>
#include <iostream>
#include <ctime>
#include <limits>

using namespace std;

int main() {
    srand(time(0));
    /*srand() function seeded with the time is used for simplicity, but it should be avoided in real code because it is
    highly deterministic, very insecure, and not evenly distributed*/

    // Generates two random-ish numbers from 1 to 100 and multiplies them together to find the solution
    int number1 = rand() % 100 + 1;
    int number2 = rand() % 100 + 1;
    int solution = number1 * number2;

    int answer = 0;//Initializes answer to an invalid number (possible solutions to generate range from 1 to 10,000)

    while (answer != solution) {
        cout << "What is " << number1 << " * " << number2 << "? ";
        cin >> answer;

        if (cin.fail())
        {
            // Letter characters will cause an infinite loop unless cleared
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input. Please enter a whole number.\n";
            continue;
        }

        if (answer == solution) {
            cout << "Correct! Press 'enter' to exit." << endl;
        } else {
            cout << "Incorrect. ";// endl intentionally omitted to keep the question on the same line
        }
    }
    getch();//Halts and waits for user input
}