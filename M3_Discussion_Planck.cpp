#include <iostream>
#include <limits>

using namespace std;

int main() {
    int number;

    cout << "Enter a number: ";
    cin >> number;

    if (number > 0) {
        cout << "The number is positive." << endl;
    }
    else {
        cout << "The number is not positive." << endl;
    }
    cout << "\nPress Enter to exit...";
    // Newline from last cout is read as enter and needs to be ignored.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}
