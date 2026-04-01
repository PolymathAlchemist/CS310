#include <iostream>
#include <limits>
using namespace std;

int main() {
    int score;

    cout << "Enter your test score (0-100): ";
    cin >> score;

    // Declare variables
    string letterGrade;

    // Determine the letter grade using selection statements
    if (score >= 90){
        letterGrade = 'A';
    }
    else if (score >= 80) {
        letterGrade = 'B';
    }
    else if (score >= 70) {
        letterGrade = 'C';
    }
    else {
        letterGrade = 'F';
    }

    // Output the final result
    cout << "Your letter grade is: " << letterGrade << endl;

    cout << "\nPress Enter to exit...";

    // Newline from last cout is read as enter and needs to be ignored.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
