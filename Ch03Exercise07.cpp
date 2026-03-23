/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M2 Assignment - Chapter 3, Programming Exercise 7
 * Author: Eric J. Turman
 * Date: 2026-03-23
 * Email: ejturman@my365.bellevue.edu
 *
 * Description:
 * Calculates the interest on a credit card's unpaid balance using the
 * average daily balance method.
 *
 * Note:
 * This version includes modest input validation appropriate to the level
 * of the assignment without overengineering the solution.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes)
// ========================================================================
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

// ========================================================================
// Namespace
// ========================================================================
using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
double get_non_negative_double(const string& prompt_message);
int get_positive_integer(const string& prompt_message);
string format_with_commas(double numeric_value);
double calculate_average_daily_balance(
    double net_balance,
    double payment_amount,
    int billing_cycle_days,
    int payment_days_before_cycle_end
);
double calculate_interest(
    double average_daily_balance,
    double monthly_interest_rate
);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    double net_balance = 0.0;
    double payment_amount = 0.0;
    double monthly_interest_rate = 0.0;
    double average_daily_balance = 0.0;
    double interest_amount = 0.0;

    int billing_cycle_days = 0;
    int payment_days_before_cycle_end = 0;

    // --------------------------------------------------------------------
    // Output formatting configuration
    // --------------------------------------------------------------------
    cout << fixed << setprecision(2); // two decimal places
    // --------------------------------------------------------------------

    net_balance = get_non_negative_double(
        "Enter the net balance shown on the bill: $"
    );

    payment_amount = get_non_negative_double(
        "Enter the payment made: $"
    );

    billing_cycle_days = get_positive_integer(
        "Enter the number of days in the billing cycle: "
    );

    while (true)
    {
        payment_days_before_cycle_end = get_positive_integer(
            "Enter the number of days the payment was made before the end "
            "of the billing cycle: "
        );

        if (payment_days_before_cycle_end <= billing_cycle_days)
        {
            break;
        }

        cout << "Error: Payment days before cycle end cannot exceed the "
             << "number of days in the billing cycle." << endl;
    }

    monthly_interest_rate = get_non_negative_double(
        "Enter the monthly interest rate (example: 0.0152): "
    );

    average_daily_balance = calculate_average_daily_balance(
        net_balance,
        payment_amount,
        billing_cycle_days,
        payment_days_before_cycle_end
    );

    interest_amount = calculate_interest(
        average_daily_balance,
        monthly_interest_rate
    );

    cout << "\nAverage Daily Balance: $"
         << format_with_commas(average_daily_balance) << endl;
    cout << "Interest: $"
         << format_with_commas(interest_amount) << endl;

    cout << "\nPress Enter to exit...";
    cin.get();

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Prompt for a non-negative decimal value.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The prompt displayed to the user before input is read.
 *
 * Returns
 * -------
 * double
 *     A validated non-negative decimal value.
 *
 * Notes
 * -----
 * This function clears the input stream when extraction fails so the
 * program can continue prompting cleanly after invalid input.
 */
double get_non_negative_double(const string& prompt_message)
{
    double numeric_value = 0.0;

    while (true)
    {
        cout << prompt_message;
        cin >> numeric_value;

        if (cin.fail())
        {
            cout << "Error: Please enter a valid numeric value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (numeric_value < 0.0)
        {
            cout << "Error: Value cannot be negative." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return numeric_value;
    }
}

/**
 * Prompt for a positive whole number.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The prompt displayed to the user before input is read.
 *
 * Returns
 * -------
 * int
 *     A validated integer greater than zero.
 *
 * Notes
 * -----
 * Billing cycle values should be positive whole numbers for this
 * assignment, so zero and negative values are rejected.
 */
int get_positive_integer(const string& prompt_message)
{
    int numeric_value = 0;

    while (true)
    {
        cout << prompt_message;
        cin >> numeric_value;

        if (cin.fail())
        {
            cout << "Error: Please enter a valid whole number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (numeric_value <= 0)
        {
            cout << "Error: Value must be greater than zero." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return numeric_value;
    }
}

/**
 * Format a decimal value using commas as thousands separators.
 *
 * Parameters
 * ----------
 * numeric_value : double
 *     The value to be converted into a formatted string.
 *
 * Returns
 * -------
 * string
 *     A string formatted to two decimal places with comma separators.
 *
 * Notes
 * -----
 * This helper is used because locale-based grouping did not behave
 * consistently in the local compiler environment.
 */
string format_with_commas(double numeric_value)
{
    stringstream number_stream;
    number_stream << fixed << setprecision(2) << numeric_value;

    string numeric_text = number_stream.str();
    size_t decimal_position = numeric_text.find('.');

    if (decimal_position == string::npos)
    {
        decimal_position = numeric_text.length();
    }

    for (int insert_position = static_cast<int>(decimal_position) - 3;
         insert_position > 0;
         insert_position -= 3)
    {
        numeric_text.insert(insert_position, ",");
    }

    return numeric_text;
}

/**
 * Calculate the average daily balance for the billing cycle.
 *
 * Parameters
 * ----------
 * net_balance : double
 *     The balance shown on the credit card bill.
 * payment_amount : double
 *     The payment made toward the balance.
 * billing_cycle_days : int
 *     The total number of days in the billing cycle.
 * payment_days_before_cycle_end : int
 *     The number of days the payment was made before the billing cycle
 *     ended.
 *
 * Returns
 * -------
 * double
 *     The calculated average daily balance.
 *
 * Notes
 * -----
 * This follows the assignment formula directly.
 */
double calculate_average_daily_balance(
    double net_balance,
    double payment_amount,
    int billing_cycle_days,
    int payment_days_before_cycle_end
)
{
    return (net_balance * billing_cycle_days
        - payment_amount * payment_days_before_cycle_end)
        / billing_cycle_days;
}

/**
 * Calculate the monthly interest based on the average daily balance.
 *
 * Parameters
 * ----------
 * average_daily_balance : double
 *     The computed average daily balance for the billing cycle.
 * monthly_interest_rate : double
 *     The monthly interest rate expressed as a decimal value.
 *
 * Returns
 * -------
 * double
 *     The interest charged on the unpaid balance.
 */
double calculate_interest(
    double average_daily_balance,
    double monthly_interest_rate
)
{
    return average_daily_balance * monthly_interest_rate;
}