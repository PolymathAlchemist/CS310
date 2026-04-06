/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M4 - Repetition Control Statements
 * Author: Eric J. Turman
 * Date: 2026-04-05
 *
 * Description:
 * Prompts the user to enter numbers and calculates the sum.
 * The program repeats until the user presses Enter on a blank line.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
bool get_number_or_exit(
    const string& prompt_message,
    int& numeric_value
);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations (fixed values, prompts, messages)
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "This program adds numbers entered by the user.\n";

    const string INSTRUCTION_MESSAGE =
        "To finish entering numbers, press Enter on a blank line.\n";

    const string INPUT_PROMPT =
        "Enter a whole number: ";

    const string EXIT_MESSAGE =
        "\nPress Enter to exit...";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    int user_number = 0;
    int user_numbers_sum = 0;
    int user_numbers_added = 0;

    double user_numbers_average = 0.0;

    bool continue_loop = true;

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRO_MESSAGE;
    cout << INSTRUCTION_MESSAGE << endl;

    // --------------------------------------------------------------------
    // Main Loop (Repetition Control - do/while)
    // --------------------------------------------------------------------
    do
    {
        // --------------------------------------------------------------------
        // Input
        // --------------------------------------------------------------------
        continue_loop = get_number_or_exit(INPUT_PROMPT, user_number);

        // --------------------------------------------------------------------
        // Processing
        // --------------------------------------------------------------------
        if (continue_loop)
        {
            user_numbers_sum += user_number;
            user_numbers_added++;
        }

        // --------------------------------------------------------------------
        // Output
        // --------------------------------------------------------------------
        if (continue_loop)
        {
            cout << "Current sum: " << user_numbers_sum << endl;
            cout << "Numbers entered: " << user_numbers_added << "\n\n";
        }

        while (continue_loop);
    }

    // --------------------------------------------------------------------
    // Final Processing
    // --------------------------------------------------------------------
    if (user_numbers_added > 0)
    {
        double user_numbers_sum_as_double = static_cast<double>(user_numbers_sum);

        user_numbers_average = user_numbers_sum_as_double / user_numbers_added;
    }

    // --------------------------------------------------------------------
    // Final Output
    // --------------------------------------------------------------------
    cout << "\nFinal sum: " << user_numbers_sum << endl;
    cout << "Total numbers entered: " << user_numbers_added << endl;

    if (user_numbers_added > 0)
    {
        cout << "Average: " << user_numbers_average << endl;
    }
    else
    {
        cout << "No numbers were entered. Average is undefined." << endl;
        break;
    }

    // --------------------------------------------------------------------
    // Pause before exit
    //
    // Note
    // ----
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // is not required because input is handled consistently with getline,
    // so there is no residual newline in the input buffer.
    // --------------------------------------------------------------------
    cout << EXIT_MESSAGE;
    cin.get();

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Prompt the user for a number or allow exit via blank input.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed to the user before input is read.
 * numeric_value : int&
 *     Reference used to store the validated integer value.
 *
 * Returns
 * -------
 * bool
 *     True if a valid number is entered.
 *     False if the user presses Enter (exit condition).
 *
 * Notes
 * -----
 * This function uses getline to allow detection of empty input.
 */
bool get_number_or_exit(
    const string& prompt_message,
    int& numeric_value
)
{
    string user_input;

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        // Exit condition
        if (user_input.empty())
        {
            return false;
        }

        stringstream input_stream(user_input);
        input_stream >> numeric_value;

        if (input_stream.fail())
        {
            cout << "Error: Please enter a valid whole number.\n";
            continue;
        }

        input_stream >> ws;

        if (!input_stream.eof())
        {
            cout << "Error: Invalid trailing characters detected.\n";
            continue;
        }

        return true;
    }
}