/**
* ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: Module 3 Discussion - Debugging Exercise
 * Author: Eric J. Turman
 * Date: 2026-03-28
 *
 * Description:
 * Demonstrates basic selection control statements using a simple age check.
 *
 * Note:
 * This version intentionally contains errors and will not compile.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
string display_age_category(const string& name, int age);
bool is_adult(int age);
bool is_exactly_twenty_one(int age);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    string name;
    int age = 0;


    // --------------------------------------------------------------------
    // Input
    // --------------------------------------------------------------------
    cout << "Enter your name: ";
    cin >> name;

    cout << "Enter your age: ";
    cin >> age;


    // --------------------------------------------------------------------
    // Output based on selection
    // --------------------------------------------------------------------
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << endl

    display_age_category(name, age);

    cout << "\nPress Enter to exit...";

    // Newline from last cout is read as enter and needs to be ignored.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Display age-related output for the user.
 *
 * Parameters
 * ----------
 * name : const string&
 *     The user's name.
 * age : int
 *     The user's age.
 *
 * Notes
 * -----
 * This function centralizes the selection logic so main() can remain
 * focused on overall program flow.
 */
string display_age_category(const string& name, int age)
{
    if (is_adult(age))
    {
        cout << name << " is an adult";
    }
    else
    {
        cout << name << " is a minor";
    }

    if (is_exactly_twenty_one(age))
    {
        cout << ", and is exactly 21!" << endl;
    }
    else
    {
        cout << "." << endl;
    }
}

/**
 * Determine whether the user is an adult.
 *
 * Parameters
 * ----------
 * age : int
 *     The user's age.
 *
 * Returns
 * -------
 * is_adult : bool
 *     True if age is 18 or older; otherwise false.
 */
bool is_adult(int age)
{
    if (age >= 18)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Determine whether the user is exactly 21 years old.
 *
 * Parameters
 * ----------
 * age : int
 *     The user's age.
 *
 * Returns
 * -------
 * is_exactly_twenty_one : bool
 *     True if age is exactly 21; otherwise false.
 */
bool is_exactly_twenty_one(int age)
{
    if (age = 21)
    {
        return true;
    }
    else
    {
        return false;
    }
}