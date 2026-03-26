/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M2 Assignment - Chapter 3, Programming Exercise 7
 * Author: Eric J. Turman
 * Date: 2026-03-23
 *
 * Description:
 * Calculates the interest on a credit card's unpaid balance using the
 * average daily balance method.
 *
 * Note:
 * This version includes modest input validation as well as accommodating
 * the fact that locale was not adding commas in as it should have.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
double get_non_negative_double(const string& prompt_message);
int get_positive_integer(const string& prompt_message);
string format_with_commas(double numeric_value);
int get_payment_days_before_cycle_end(
    int billing_cycle_days,
    const string& prompt_message
);
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
    // Constant declarations (prompts, messages)
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "You see before you John Cleese dressed in a black and red robe "
        "wearing a skullcap with ram horns."
        "\nAround his neck is a large bone necklace and he holds a long "
        "wooden staff."
        "\nHe conjures fireballs and hurtles them at nearby hills. "
        "\nYou call out to him, asking what and who he is. He turns to you "
        "and says,"
        "\n\"I am a financial enchanter capable of precisely "
        "processing average daily balances and calculating interest.\n"
        "Some call me... Tim.\"\n\n";

    const string NET_BALANCE_PROMPT =
        "Enter the net balance shown on the bill: $";

    const string PAYMENT_PROMPT =
        "Enter the payment made: $";

    const string BILLING_DAYS_PROMPT =
        "Enter the number of days in the billing cycle: ";

    const string PAYMENT_DAYS_PROMPT =
        "Enter the number of days the payment was made before the end "
        "of the billing cycle: ";

    const string INTEREST_RATE_PROMPT =
        "Enter the monthly interest rate (example: 0.0152): ";

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
    // Output formatting
    // --------------------------------------------------------------------
    cout << fixed << setprecision(2);

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRO_MESSAGE;

    // --------------------------------------------------------------------
    // Input
    // --------------------------------------------------------------------
    net_balance = get_non_negative_double(NET_BALANCE_PROMPT);

    payment_amount = get_non_negative_double(PAYMENT_PROMPT);

    billing_cycle_days = get_positive_integer(BILLING_DAYS_PROMPT);

    payment_days_before_cycle_end = get_payment_days_before_cycle_end(
        billing_cycle_days,
        PAYMENT_DAYS_PROMPT
    );

    monthly_interest_rate = get_non_negative_double(
        INTEREST_RATE_PROMPT
    );

    // --------------------------------------------------------------------
    // Processing
    // --------------------------------------------------------------------
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

    // --------------------------------------------------------------------
    // Output
    // --------------------------------------------------------------------
    cout << "\n------------------------------------------------------------\n";
    cout << "Based on a net balance of $"
         << format_with_commas(net_balance)
         << " and a payment of $"
         << format_with_commas(payment_amount)
         << " made\n"
         << payment_days_before_cycle_end
         << " days before the end of a "
         << billing_cycle_days
         << "-day billing cycle, the average daily balance\nis $"
         << format_with_commas(average_daily_balance);
    cout << ".\n\nAt a monthly interest rate of "
         << monthly_interest_rate * 100
         << "%, the interest for this billing cycle is $"
         << format_with_commas(interest_amount)
         << ".\n";

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
 * numeric_value : double
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

        // Valid input acquired; return the value.
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
 * numeric_value : int
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

        // Valid input acquired; return the value.
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
 * numeric_text : string
 *     A string formatted to two decimal places with comma separators.
 *
 * Notes
 * -----
 * Locale-based grouping (e.g., locale("") with imbue) did not behave
 * consistently in this development environment. Even explicitly specifying
 * locale("en_US.UTF-8") did not produce the expected results, so a manual
 * formatting approach was implemented.
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
 * Prompt for the number of days a payment was made before the end of
 * the billing cycle, ensuring it does not exceed the billing cycle length.
 *
 * Parameters
 * ----------
 * billing_cycle_days : int
 *     The total number of days in the billing cycle.
 * prompt_message : const string&
 *     The prompt displayed to the user before input is read.
 *
 * Returns
 * -------
 * payment_days_before_cycle_end : int
 *     A validated number of days that is positive and less than or equal
 *     to the billing cycle length.
 *
 * Notes
 * -----
 * This function encapsulates the constraint that payment timing cannot
 * exceed the length of the billing cycle, keeping main() focused on
 * high-level program flow.
 */
int get_payment_days_before_cycle_end(
    int billing_cycle_days,
    const string& prompt_message
)
{
    int payment_days_before_cycle_end = 0;

    while (true)
    {
        payment_days_before_cycle_end =
            get_positive_integer(prompt_message);

        if (payment_days_before_cycle_end <= billing_cycle_days)
        {
            // Valid input acquired; return the constrained value.
            return payment_days_before_cycle_end;
        }

        cout << "Error: Payment days before cycle end cannot exceed the "
             << "number of days in the billing cycle." << endl;
    }
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
 * average_daily_balance : double
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
    double average_daily_balance =
        (net_balance * billing_cycle_days
        - payment_amount * payment_days_before_cycle_end)
        / billing_cycle_days;

    return average_daily_balance;
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
 * interest_amount : double
 *     The interest charged on the unpaid balance.
 */
double calculate_interest(
    double average_daily_balance,
    double monthly_interest_rate
)
{
    double interest_amount =
        average_daily_balance * monthly_interest_rate;

    return interest_amount;
}
