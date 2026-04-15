/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M5 Assignment (Ch. 6, Exercise 14)
 * Author: Eric J. Turman
 * Date: 2026-04-13
 *
 * Description:
 * This program calculates the billing amount for consulting services
 * provided by J&J Accounting. Charges depend on income level and
 * consulting duration, with different free thresholds and billing
 * rates applied.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
double calculate_billing_amount(
    double hourly_rate,
    double consulting_minutes,
    bool is_low_income
);

double get_valid_double(const string& prompt_message);

bool get_yes_or_no(const string& prompt_message);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "This program calculates consulting charges for J&J Accounting.";

    const string REPEAT_PROMPT =
        "\nWould you like to process another client? (Y/N): ";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    double hourly_rate = 0.0;
    double consulting_minutes = 0.0;
    bool is_low_income = false;
    double billing_amount = 0.0;

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRO_MESSAGE << endl;

    // --------------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------------
    do
    {
        // ---------------------------------------------------------------
        // Input
        // ---------------------------------------------------------------
        hourly_rate = get_valid_double(
            "\nEnter hourly rate: "
        );

        consulting_minutes = get_valid_double(
            "Enter consulting time (minutes): "
        );

        is_low_income = get_yes_or_no(
            "Is the client low income (<= 25,000)? (Y/N): "
        );

        // ---------------------------------------------------------------
        // Processing
        // ---------------------------------------------------------------
        billing_amount = calculate_billing_amount(
            hourly_rate,
            consulting_minutes,
            is_low_income
        );

        // ---------------------------------------------------------------
        // Output
        // ---------------------------------------------------------------
        cout << fixed << setprecision(2);

        cout << "\nBilling Summary" << endl;
        cout << "----------------" << endl;
        cout << "Hourly rate: $" << hourly_rate << endl;
        cout << "Consulting time: " << consulting_minutes << " minutes" << endl;
        cout << "Low income: " << (is_low_income ? "Yes" : "No") << endl;
        cout << "Amount due: $" << billing_amount << endl;

    } while (get_yes_or_no(REPEAT_PROMPT));

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Calculate the billing amount based on income level and consulting time.
 *
 * Parameters
 * ----------
 * hourly_rate : double
 *     The standard hourly consulting rate.
 *
 * consulting_minutes : double
 *     The total time spent consulting, in minutes.
 *
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * billing_amount : double
 *     The calculated amount owed for the consulting session.
 *
 * Notes
 * -----
 * Low income clients receive 30 minutes free and are charged 40% of the
 * hourly rate for additional time. All other clients receive 20 minutes
 * free and are charged 70% of the hourly rate beyond that threshold.
 */
double calculate_billing_amount(
    double hourly_rate,
    double consulting_minutes,
    bool is_low_income
)
{
    double free_minutes = 0.0;
    double billing_rate = 0.0;

    if (is_low_income)
    {
        free_minutes = 30.0;
        billing_rate = 0.40;
    }
    else
    {
        free_minutes = 20.0;
        billing_rate = 0.70;
    }

    if (consulting_minutes <= free_minutes)
    {
        return 0.0;
    }

    double billable_minutes = consulting_minutes - free_minutes;

    double billing_amount =
        hourly_rate * billing_rate * (billable_minutes / 60.0);

    return billing_amount;
}

/**
 * Prompt the user for a valid numeric input.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed to the user.
 *
 * Returns
 * -------
 * numeric_value : double
 *     A validated numeric value.
 *
 * Notes
 * -----
 * Uses stringstream to ensure clean parsing and rejection of invalid input.
 */
double get_valid_double(const string& prompt_message)
{
    string input;
    double value = 0.0;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input);

        stringstream stream(input);
        stream >> value;

        if (!stream.fail() && stream.eof())
        {
            return value;
        }

        cout << "Error: Please enter a valid number." << endl;
    }
}

/**
 * Prompt the user for a yes/no decision.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed to the user.
 *
 * Returns
 * -------
 * user_choice : bool
 *     True for 'Y' or 'y', false for 'N' or 'n'.
 *
 * Notes
 * -----
 * Ensures only valid single-character responses are accepted.
 */
bool get_yes_or_no(const string& prompt_message)
{
    string input;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input);

        if (input.length() == 1)
        {
            if (input[0] == 'Y' || input[0] == 'y')
            {
                return true;
            }

            if (input[0] == 'N' || input[0] == 'n')
            {
                return false;
            }
        }

        cout << "Error: Please enter Y or N." << endl;
    }
}