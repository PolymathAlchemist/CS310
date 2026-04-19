/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M5 Assignment (Chapter 6, Exercise 14)
 * Author: Eric J. Turman
 * Date: 2026-04-13
 *
 * Description:
 * This program calculates consulting charges for J&J Accounting during
 * tax season. It provides two clearly separated execution paths:
 *
 * 1. Assignment billing mode
 *    A direct implementation of the required assignment workflow.
 *
 * 2. Friday simulation mode
 *    An optional enhancement that reuses the same billing function while
 *    simulating a full workday of random clients and tracking revenue,
 *    labor cost, overtime, overhead, and net profit.
 *
 * Notes:
 * The required billing logic remains central and easy to inspect. The
 * simulation features are intentionally separated so the assignment path
 * stays obvious and easy to grade.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <cctype>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

using namespace std;

// ========================================================================
// Core Assignment Constants
// ========================================================================
const double LOW_INCOME_FREE_MINUTES = 30.0;
const double STANDARD_FREE_MINUTES = 20.0;
const double LOW_INCOME_BILLING_RATE = 0.40;
const double STANDARD_BILLING_RATE = 0.70;

// ========================================================================
// Simulation Constants
// ========================================================================
const double REGULAR_WORKDAY_MINUTES = 480.0;
const double OVERTIME_MULTIPLIER = 1.50;
const int MIN_CLIENT_MINUTES = 20;
const int MAX_CLIENT_MINUTES = 120;

// ========================================================================
// Core Assignment Function Prototypes (Declarations)
// ========================================================================
double calculate_billing_amount(
    double hourly_rate,
    double consulting_minutes,
    bool is_low_income
);

double calculate_free_minutes(bool is_low_income);

double calculate_billing_rate(bool is_low_income);

double calculate_billable_minutes(
    double consulting_minutes,
    bool is_low_income
);

string get_income_classification(bool is_low_income);

string get_billing_category(double billing_amount, bool is_low_income);

double get_bounded_double(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
);

bool get_boolean_choice(
    const string& prompt_message,
    const string& true_association,
    const string& false_association
);

void print_client_billing_summary(
    double hourly_rate,
    double consulting_minutes,
    bool is_low_income,
    double billing_amount
);

string build_assignment_mode_intro();

string build_simulation_mode_intro();

void run_assignment_mode();

// ========================================================================
// Simulation Function Prototypes (Declarations)
// ========================================================================
int generate_random_consultation_minutes(mt19937& random_engine);

bool generate_random_low_income_status(mt19937& random_engine);

double calculate_regular_minutes_worked(
    double regular_minutes_remaining,
    double consulting_minutes
);

double calculate_overtime_minutes_worked(
    double regular_minutes_remaining,
    double consulting_minutes
);

double calculate_employee_labor_cost(
    double employee_hourly_rate,
    double regular_minutes_worked,
    double overtime_minutes_worked,
    double overtime_multiplier
);

double calculate_net_profit(
    double total_revenue,
    double total_labor_cost,
    double overhead_cost
);

string format_currency(double amount);

void print_simulation_table_header();

void print_simulation_client_row(
    int client_number,
    bool is_low_income,
    double consulting_minutes,
    double billing_amount,
    double regular_minutes_worked,
    double overtime_minutes_worked,
    double employee_labor_cost,
    double consultation_profit,
    double total_minutes_worked
);

void print_simulation_day_summary(
    int clients_processed,
    int free_consultations,
    int low_income_clients,
    double total_minutes_worked,
    double regular_minutes_total,
    double overtime_minutes_total,
    double total_revenue,
    double total_labor_cost,
    double overhead_cost,
    double net_profit
);

void run_simulation_mode();

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations (prompts, messages)
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
    "This program calculates consulting charges for J&J Accounting.";

    const string MODE_PROMPT =
        "\nSelect program mode:\n"
        "A - Assignment billing calculator\n"
        "S - Friday simulation\n"
        "Enter choice: ";

    const string AGAIN_PROMPT =
        "\nChoose another mode? (Y) or press Enter to exit:";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    bool run_assignment_billing_mode = false;

    // --------------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------------
    do
    {
        cout << INTRO_MESSAGE << endl;

        run_assignment_billing_mode = get_boolean_choice(
            MODE_PROMPT,
            "A",
            "S"
        );

        if (run_assignment_billing_mode)
        {
            run_assignment_mode();
        }
        else
        {
            run_simulation_mode();
        }

    } while (get_boolean_choice(AGAIN_PROMPT, "Y", ""));

    return 0;
}

// ========================================================================
// Core Assignment Function Definitions
// ========================================================================

/**
 * Format a decimal value using commas as thousands separators.
 *
 * Parameters
 * ----------
 * numeric_value : double
 *     The value to be converted into a formatted string.
 *
 * decimal_precision : int
 *     Used to set the decimal precision.
 *
 * Returns
 * -------
 * numeric_text : string
 *     A string formatted with comma separators and the requested precision.
 *
 * Notes
 * -----
 * Locale-based grouping (e.g., locale("") with imbue) did not behave
 * consistently in this development environment. Even explicitly specifying
 * locale("en_US.UTF-8") did not produce the expected results, so a manual
 * formatting approach was implemented. This extended version handles negative
 * numbers without adding a comma for only three digit negative numbers.
 */
string format_with_commas(double numeric_value, int decimal_precision)
{
    stringstream number_stream;
    number_stream << fixed << setprecision(decimal_precision) << numeric_value;

    string numeric_text = number_stream.str();
    size_t decimal_position = numeric_text.find('.');

    if (decimal_position == string::npos)
    {
        decimal_position = numeric_text.length();
    }

    int start_position = (numeric_text[0] == '-') ? 1 : 0;

    for (int insert_position = static_cast<int>(decimal_position) - 3;
         insert_position > start_position;
         insert_position -= 3)
    {
        numeric_text.insert(insert_position, ",");
    }

    return numeric_text;
}

/**
 * Build the assignment-mode setup message.
 *
 * Returns
 * -------
 * intro_message : string
 *     A short scenario description that reflects the current assignment
 *     billing rules without hard-coding duplicate values inline.
 */
string build_assignment_mode_intro()
{
    ostringstream output_stream;

    output_stream << "\nAssignment billing scenario" << endl;
    output_stream << "---------------------------" << endl;
    output_stream << "During the tax season, every Friday, the J&J accounting"
                  << " firm provides assistance\n"
                  << "to people who prepare their own tax returns.\n"
                  << "Standard clients receive the first "
                  << format_with_commas(STANDARD_FREE_MINUTES, 0)
                  << " minutes free and are billed at "
                  << format_with_commas(STANDARD_BILLING_RATE * 100.0, 0)
                  << "% of the hourly rate after that.\n"
                  << "Low-income clients receive the first "
                  << format_with_commas(LOW_INCOME_FREE_MINUTES, 0)
                  << " minutes free and are billed at "
                  << format_with_commas(LOW_INCOME_BILLING_RATE * 100.0, 0)
                  << "% of the hourly rate after that.\n";

    return output_stream.str();
}

/**
 * Calculate the billing amount for a client consultation.
 *
 * Parameters
 * ----------
 * hourly_rate : double
 *     The standard hourly consulting rate charged to the client.
 *
 * consulting_minutes : double
 *     The total consulting time in minutes.
 *
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * billing_amount : double
 *     The amount owed by the client according to the assignment rules.
 */
double calculate_billing_amount(
    double hourly_rate,
    double consulting_minutes,
    bool is_low_income
)
{
    const double billable_minutes = calculate_billable_minutes(
        consulting_minutes,
        is_low_income
    );

    const double billing_rate = calculate_billing_rate(is_low_income);

    return hourly_rate * billing_rate * (billable_minutes / 60.0);
}

/**
 * Return the number of free consultation minutes allowed.
 *
 * Parameters
 * ----------
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * free_minutes : double
 *     The number of free minutes granted before billing begins.
 */
double calculate_free_minutes(bool is_low_income)
{
    if (is_low_income)
    {
        return LOW_INCOME_FREE_MINUTES;
    }

    return STANDARD_FREE_MINUTES;
}

/**
 * Return the billing-rate multiplier for the client's classification.
 *
 * Parameters
 * ----------
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * billing_rate : double
 *     The reduced billing rate expressed as a decimal multiplier.
 */
double calculate_billing_rate(bool is_low_income)
{
    if (is_low_income)
    {
        return LOW_INCOME_BILLING_RATE;
    }

    return STANDARD_BILLING_RATE;
}

/**
 * Calculate the number of billable consultation minutes.
 *
 * Parameters
 * ----------
 * consulting_minutes : double
 *     The total consultation time in minutes.
 *
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * billable_minutes : double
 *     The consultation minutes that remain after the free threshold
 *     has been applied.
 */
double calculate_billable_minutes(
    double consulting_minutes,
    bool is_low_income
)
{
    const double free_minutes = calculate_free_minutes(is_low_income);

    if (consulting_minutes <= free_minutes)
    {
        return 0.0;
    }

    return consulting_minutes - free_minutes;
}

/**
 * Return a text label describing the client's income classification.
 *
 * Parameters
 * ----------
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * income_classification : string
 *     A user-facing description of the client's classification.
 */
string get_income_classification(bool is_low_income)
{
    if (is_low_income)
    {
        return "Low Income";
    }

    return "Standard";
}

/**
 * Return a text label describing the billing result.
 *
 * Parameters
 * ----------
 * billing_amount : double
 *     The amount charged to the client.
 *
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * Returns
 * -------
 * billing_category : string
 *     A descriptive billing category label.
 */
string get_billing_category(double billing_amount, bool is_low_income)
{
    if (billing_amount <= 0.0)
    {
        return "Free Consultation";
    }

    if (is_low_income)
    {
        return "Reduced Rate (Low Income)";
    }

    return "Reduced Rate (Standard Client)";
}

/**
 * Prompt for a numeric value within an allowed range.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before input is read.
 *
 * minimum_value : double
 *     The smallest acceptable value, inclusive.
 *
 * maximum_value : double
 *     The largest acceptable value, inclusive.
 *
 * Returns
 * -------
 * numeric_value : double
 *     A validated numeric value within the requested bounds.
 *
 * Notes
 * -----
 * Reading the full line first makes it easier to reject malformed input
 * cleanly instead of partially accepting it.
 */
double get_bounded_double(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
)
{
    string user_input;
    double numeric_value = 0.0;

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        if (user_input.empty())
        {
            cout << "Error: Please enter a numeric value." << endl;
            continue;
        }

        stringstream input_stream(user_input);
        input_stream >> numeric_value;

        if (input_stream.fail())
        {
            cout << "Error: Please enter a valid numeric value." << endl;
            continue;
        }

        input_stream >> ws;

        if (!input_stream.eof())
        {
            cout << "Error: Invalid trailing characters detected." << endl;
            continue;
        }

        if (numeric_value < minimum_value || numeric_value > maximum_value)
        {
            cout << "Error: Value must be between "
                 << fixed << setprecision(2)
                 << minimum_value
                 << " and "
                 << maximum_value
                 << "."
                 << endl;
            continue;
        }

        return numeric_value;
    }
}

/**
 * Prompt the user for one of two text associations and return a Boolean.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before input is read.
 *
 * true_association : const string&
 *     The input token that maps to true.
 *
 * false_association : const string&
 *     The input token that maps to false.
 *
 * Returns
 * -------
 * user_choice : bool
 *     True when the user enters the true association and false when the
 *     user enters the false association.
 *
 * Notes
 * -----
 * This keeps binary choices consistent across the program while allowing
 * the calling code to define the meaning of true and false. Normalizes all
 * parameter and input characters to uppercase for easier comparison.
 */
bool get_boolean_choice(
    const string& prompt_message,
    const string& true_association,
    const string& false_association
)
{
    string user_input;
    string normalized_input;
    string normalized_true_association;
    string normalized_false_association;

    normalized_true_association = true_association;
    normalized_false_association = false_association;

    for (char& character : normalized_true_association)
    {
        character = static_cast<char>(toupper(static_cast<unsigned char>(character)));
    }

    for (char& character : normalized_false_association)
    {
        character = static_cast<char>(toupper(static_cast<unsigned char>(character)));
    }

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        normalized_input = user_input;

        for (char& character : normalized_input)
        {
            character = static_cast<char>(toupper(static_cast<unsigned char>(character)));
        }

        if (normalized_input == normalized_true_association)
        {
            return true;
        }

        if (normalized_input == normalized_false_association)
        {
            return false;
        }

        cout << "Error: Please enter "
             << true_association
             << " or "
             << false_association
             << "."
             << endl;
    }
}

/**
 * Print a shared billing summary for a single client.
 *
 * Parameters
 * ----------
 * hourly_rate : double
 *     The client's hourly consulting rate.
 *
 * consulting_minutes : double
 *     The total consultation time in minutes.
 *
 * is_low_income : bool
 *     Indicates whether the client qualifies as low income.
 *
 * billing_amount : double
 *     The final amount charged to the client.
 */
void print_client_billing_summary(
    double hourly_rate,
    double consulting_minutes,
    bool is_low_income,
    double billing_amount
)
{
    const double free_minutes = calculate_free_minutes(is_low_income);
    const double billing_rate = calculate_billing_rate(is_low_income);
    const double billable_minutes = calculate_billable_minutes(
        consulting_minutes,
        is_low_income
    );

    cout << fixed << setprecision(2);

    cout << "\nClient Billing Summary" << endl;
    cout << "----------------------" << endl;
    cout << "Income classification: "
         << get_income_classification(is_low_income)
         << endl;
    cout << "Hourly rate: $"
         << format_with_commas(hourly_rate, 2)
         << endl;
    cout << "Applied billing rate: "
         << format_with_commas((billing_rate * 100.0), 0)
         << "%" << endl;
    cout << "Consulting time: "
         << format_with_commas(consulting_minutes, 0)
         << " minutes" << endl;
    cout << "Free consultation threshold: " << free_minutes << " minutes" << endl;
    cout << "Billable time: "
         << format_with_commas(billable_minutes, 0)
         << " minutes" << endl;
    cout << "Billing category: "
         << get_billing_category(billing_amount, is_low_income)
         << endl;
    cout << "Amount due: $"
         << format_with_commas(billing_amount, 2)
         << endl;
}

/**
 * Run the straightforward assignment-compliant billing workflow.
 *
 * Notes
 * -----
 * This path stays literal so the required behavior remains easy to locate
 * and easy to grade.
 */
void run_assignment_mode()
{
    // ----------------------------------------------------------------
    // Constant declarations
    // ----------------------------------------------------------------
    const string REPEAT_PROMPT =
        "\nWould you like to process another client? (Y/N): ";

    cout << build_assignment_mode_intro() << endl;

    // ----------------------------------------------------------------
    // Variable declarations
    // ----------------------------------------------------------------
    double hourly_rate = 0.0;
    double consulting_minutes = 0.0;
    bool is_low_income = false;
    double billing_amount = 0.0;

    // ----------------------------------------------------------------
    // Main loop
    // ----------------------------------------------------------------
    do
    {
        // ------------------------------------------------------------
        // Input
        // ------------------------------------------------------------
        hourly_rate = get_bounded_double(
            "\nEnter client hourly rate: $",
            0.01,
            1000.0
        );

        consulting_minutes = get_bounded_double(
            "Enter total consulting time (minutes): ",
            0.01,
            1440.0
        );

        is_low_income = get_boolean_choice(
            "Does the client qualify as low income? (Y/N): ",
            "Y",
            "N"
        );

        // ------------------------------------------------------------
        // Processing
        // ------------------------------------------------------------
        billing_amount = calculate_billing_amount(
            hourly_rate,
            consulting_minutes,
            is_low_income
        );

        // ------------------------------------------------------------
        // Output
        // ------------------------------------------------------------
        print_client_billing_summary(
            hourly_rate,
            consulting_minutes,
            is_low_income,
            billing_amount
        );

    } while (get_boolean_choice(REPEAT_PROMPT, "Y", "N"));
}

// ========================================================================
// Simulation Function Definitions
// ========================================================================

/**
 * Build the simulation-mode setup message.
 *
 * Returns
 * -------
 * intro_message : string
 *     A short scenario description that reflects the current simulation
 *     rules and constants without duplicating those values in prose.
 */
string build_simulation_mode_intro()
{
    ostringstream output_stream;

    output_stream << "\nFriday simulation scenario" << endl;
    output_stream << "--------------------------" << endl;
    output_stream << "During the tax season, every Friday, the J&J accounting"
                  << " firm provides assistance\n"
                  << "to people who prepare their own tax returns.\n"
                  << "This is a simulation of a Friday workday for J&J Accounting.\n"
                  << "Random clients will be generated with consultation times between "
                  << format_with_commas(MIN_CLIENT_MINUTES, 0)
                  << " and "
                  << format_with_commas(MAX_CLIENT_MINUTES, 0)
                  << " minutes\nuntil the day reaches or exceeds "
                  << format_with_commas(REGULAR_WORKDAY_MINUTES, 0)
                  << " minutes.\nAny time beyond "
                  << format_with_commas(REGULAR_WORKDAY_MINUTES, 0)
                  << " minutes is treated as overtime at "
                  << format_with_commas(OVERTIME_MULTIPLIER, 2)
                  << "x labor cost.\n";

    return output_stream.str();
}

/**
 * Generate a random consultation length for a simulated client.
 *
 * Parameters
 * ----------
 * random_engine : mt19937&
 *     The pseudo-random engine shared across the simulation.
 *
 * Returns
 * -------
 * consultation_minutes : int
 *     A random whole-number duration between 20 and 120 minutes.
 */
int generate_random_consultation_minutes(mt19937& random_engine)
{
    uniform_int_distribution<int> distribution(
        MIN_CLIENT_MINUTES,
        MAX_CLIENT_MINUTES
    );

    return distribution(random_engine);
}

/**
 * Randomly assign whether a simulated client is low income.
 *
 * Parameters
 * ----------
 * random_engine : mt19937&
 *     The pseudo-random engine shared across the simulation.
 *
 * Returns
 * -------
 * is_low_income : bool
 *     True when the generated client is low income.
 */
bool generate_random_low_income_status(mt19937& random_engine)
{
    bernoulli_distribution distribution(0.50);
    return distribution(random_engine);
}

/**
 * Calculate how many consultation minutes fall within regular work hours.
 *
 * Parameters
 * ----------
 * regular_minutes_remaining : double
 *     The number of regular minutes still available before overtime begins.
 *
 * consulting_minutes : double
 *     The client's total consultation time.
 *
 * Returns
 * -------
 * regular_minutes_worked : double
 *     The portion of the client's consultation counted as regular time.
 */
double calculate_regular_minutes_worked(
    double regular_minutes_remaining,
    double consulting_minutes
)
{
    if (regular_minutes_remaining <= 0.0)
    {
        return 0.0;
    }

    if (consulting_minutes <= regular_minutes_remaining)
    {
        return consulting_minutes;
    }

    return regular_minutes_remaining;
}

/**
 * Calculate how many consultation minutes fall into overtime.
 *
 * Parameters
 * ----------
 * regular_minutes_remaining : double
 *     The number of regular minutes still available before overtime begins.
 *
 * consulting_minutes : double
 *     The client's total consultation time.
 *
 * Returns
 * -------
 * overtime_minutes_worked : double
 *     The portion of the client's consultation counted as overtime.
 */
double calculate_overtime_minutes_worked(
    double regular_minutes_remaining,
    double consulting_minutes
)
{
    if (regular_minutes_remaining >= consulting_minutes)
    {
        return 0.0;
    }

    if (regular_minutes_remaining <= 0.0)
    {
        return consulting_minutes;
    }

    return consulting_minutes - regular_minutes_remaining;
}

/**
 * Calculate the employee labor cost for a consultation.
 *
 * Parameters
 * ----------
 * employee_hourly_rate : double
 *     The employee's standard hourly wage.
 *
 * regular_minutes_worked : double
 *     The number of minutes worked at the standard pay rate.
 *
 * overtime_minutes_worked : double
 *     The number of minutes worked at the overtime pay rate.
 *
 * overtime_multiplier : double
 *     The multiplier applied to overtime labor cost.
 *
 * Returns
 * -------
 * labor_cost : double
 *     The firm's labor cost for that consultation.
 */
double calculate_employee_labor_cost(
    double employee_hourly_rate,
    double regular_minutes_worked,
    double overtime_minutes_worked,
    double overtime_multiplier
)
{
    const double regular_cost =
        employee_hourly_rate * (regular_minutes_worked / 60.0);

    const double overtime_cost =
        employee_hourly_rate *
        overtime_multiplier *
        (overtime_minutes_worked / 60.0);

    return regular_cost + overtime_cost;
}

/**
 * Calculate the day's net profit after labor and overhead.
 *
 * Parameters
 * ----------
 * total_revenue : double
 *     The total amount charged to clients.
 *
 * total_labor_cost : double
 *     The total employee labor cost.
 *
 * overhead_cost : double
 *     The fixed daily operating cost.
 *
 * Returns
 * -------
 * net_profit : double
 *     Revenue minus labor cost minus overhead.
 */
double calculate_net_profit(
    double total_revenue,
    double total_labor_cost,
    double overhead_cost
)
{
    return total_revenue - total_labor_cost - overhead_cost;
}

/**
 * Format a numeric amount as currency text for aligned table output.
 *
 * Parameters
 * ----------
 * amount : double
 *     The numeric amount to render.
 *
 * Returns
 * -------
 * currency_text : string
 *     The amount formatted with a leading dollar sign and two decimals.
 */
string format_currency(double amount)
{
    ostringstream output_stream;
    output_stream << fixed << setprecision(2) << "$" << amount;
    return output_stream.str();
}

/**
 * Print the simulation table header once.
 */
void print_simulation_table_header()
{
    cout << fixed << setprecision(2);

    cout << "\nFriday Simulation" << endl;
    cout << "-----------------" << endl;

    cout << left
         << setw(8) << "Client"
         << setw(14) << "Income"
         << right
         << setw(10) << "Minutes"
         << setw(12) << "Day Total"
         << setw(10) << "Regular"
         << setw(8) << "OT"
         << setw(12) << "Charge"
         << setw(12) << "Labor"
         << setw(14) << "Profit"
         << endl;

    cout << left
         << setw(8) << "------"
         << setw(14) << "------------"
         << right
         << setw(10) << "-------"
         << setw(12) << "---------"
         << setw(10) << "-------"
         << setw(8) << "--"
         << setw(12) << "----------"
         << setw(12) << "----------"
         << setw(14) << "------"
         << endl;
}

/**
 * Print one aligned simulation row.
 *
 * Parameters
 * ----------
 * client_number : int
 *     The sequential number of the simulated client.
 *
 * is_low_income : bool
 *     Indicates whether the simulated client is low income.
 *
 * consulting_minutes : double
 *     The total consultation time in minutes.
 *
 * billing_amount : double
 *     The amount charged to the client.
 *
 * regular_minutes_worked : double
 *     The regular-time minutes worked for the client.
 *
 * overtime_minutes_worked : double
 *     The overtime minutes worked for the client.
 *
 * employee_labor_cost : double
 *     The labor cost for the client.
 *
 * consultation_profit : double
 *     Revenue minus labor cost for that client.
 *
 * total_minutes_worked : double
 *     The cumulative workday minutes after this client.
 */
void print_simulation_client_row(
    int client_number,
    bool is_low_income,
    double consulting_minutes,
    double billing_amount,
    double regular_minutes_worked,
    double overtime_minutes_worked,
    double employee_labor_cost,
    double consultation_profit,
    double total_minutes_worked
)
{
    cout << fixed << setprecision(2);

    cout << left
         << setw(8) << client_number
         << setw(14) << get_income_classification(is_low_income)
         << right
         << setw(10) << consulting_minutes
         << setw(12) << format_with_commas(total_minutes_worked, 0)
         << setw(10) << format_with_commas(regular_minutes_worked, 0)
         << setw(8)  << format_with_commas(overtime_minutes_worked, 0)
         << setw(12) << format_currency(billing_amount)
         << setw(12) << format_currency(employee_labor_cost)
         << setw(14) << format_currency(consultation_profit)
         << endl;
}

/**
 * Print the final simulation summary.
 *
 * Parameters
 * ----------
 * clients_processed : int
 *     The total number of clients processed.
 *
 * free_consultations : int
 *     The number of consultations with no client charge.
 *
 * low_income_clients : int
 *     The number of low-income clients processed.
 *
 * total_minutes_worked : double
 *     The cumulative number of consultation minutes worked.
 *
 * regular_minutes_total : double
 *     The cumulative number of regular-time minutes worked.
 *
 * overtime_minutes_total : double
 *     The cumulative number of overtime minutes worked.
 *
 * total_revenue : double
 *     The cumulative amount charged to clients.
 *
 * total_labor_cost : double
 *     The cumulative employee labor cost.
 *
 * overhead_cost : double
 *     The fixed daily overhead cost.
 *
 * net_profit : double
 *     Revenue minus labor cost minus overhead.
 */
void print_simulation_day_summary(
    int clients_processed,
    int free_consultations,
    int low_income_clients,
    double total_minutes_worked,
    double regular_minutes_total,
    double overtime_minutes_total,
    double total_revenue,
    double total_labor_cost,
    double overhead_cost,
    double net_profit
)
{
    double average_revenue_per_client = 0.0;
    double average_profit_per_client = 0.0;

    if (clients_processed > 0)
    {
        average_revenue_per_client = total_revenue / clients_processed;
        average_profit_per_client =
            (net_profit + overhead_cost) / clients_processed;
    }

    cout << fixed << setprecision(2);

    cout << "\nSimulation Day Summary" << endl;
    cout << "----------------------" << endl;
    cout << "Clients processed: "
         << format_with_commas(clients_processed, 0)
         << endl;
    cout << "Free consultations: "
         << format_with_commas(free_consultations,0)
         << endl;
    cout << "Low-income clients: "
         << format_with_commas(low_income_clients, 0)
         << endl;
    cout << "Regular minutes total: "
         << format_with_commas(regular_minutes_total, 0)
         << endl;
    cout << "Overtime minutes total: "
         << format_with_commas(overtime_minutes_total, 0)
         << endl;
    cout << "Total minutes worked: "
         << format_with_commas(total_minutes_worked, 0)
         << endl;
    cout << "Total labor cost: $"
         << format_with_commas(total_labor_cost, 2)
         << endl;
    cout << "Daily overhead: $"
         << format_with_commas(overhead_cost, 2)
         << endl;
    cout << "Average revenue per client: $"
         << format_with_commas(average_revenue_per_client, 2)
         << endl;
    cout << "Average profit per client: $"
         << format_with_commas(average_profit_per_client, 2)
         << endl;
    cout << "Total revenue: $"
         << format_with_commas(total_revenue, 2)
         << endl;
    if (net_profit > 0) {
        cout << "Net profit: $"
             << format_with_commas(net_profit, 2)
             << endl;
    }
    else if (net_profit < 0) {
        cout << "Net loss: $"
             << format_with_commas(net_profit, 2)
             << endl;
    }
    if (net_profit == 0) {
        cout << "Net profit: $"
             << format_with_commas(net_profit, 2)
             << "J&J barely broke even" << endl;
    }
}

/**
 * Run the optional Friday workday simulation.
 *
 * Notes
 * -----
 * This mode keeps the assignment billing function central, then layers a
 * randomized workday on top so profitability can be explored without
 * complicating the required path.
 *
 * The client-processing logic for generating the report is kept local
 * because passing all the running totals by reference would make the
 * helper function unnecessarily long. Once structs or classes are introduced,
 * this could be handled more cleanly.
 */
void run_simulation_mode()
{
    // ----------------------------------------------------------------
    // Constant declarations
    // ----------------------------------------------------------------
    const string REPEAT_PROMPT =
    "\nWould you like to process another Simulation? (Y/N): ";

    cout << build_simulation_mode_intro() << endl;

    // ----------------------------------------------------------------
    // Variable declarations
    // ----------------------------------------------------------------
    random_device random_device_source;
    mt19937 random_engine(random_device_source());

    double employee_hourly_rate = 0.0;
    double client_hourly_rate = 0.0;
    double overhead_cost = 0.0;

    double consulting_minutes = 0.0;
    bool is_low_income = false;
    double billing_amount = 0.0;
    double regular_minutes_remaining = 0.0;
    double regular_minutes_worked = 0.0;
    double overtime_minutes_worked = 0.0;
    double employee_labor_cost = 0.0;
    double consultation_profit = 0.0;

    double total_minutes_worked = 0.0;
    double regular_minutes_total = 0.0;
    double overtime_minutes_total = 0.0;
    double total_revenue = 0.0;
    double total_labor_cost = 0.0;
    double net_profit = 0.0;

    int clients_processed = 0;
    int free_consultations = 0;
    int low_income_clients = 0;

    // ----------------------------------------------------------------
    // Main Loop
    // ----------------------------------------------------------------
    do
    {
        // ------------------------------------------------------------
        // Variable Initializations
        // ------------------------------------------------------------
        consulting_minutes = 0.0;
        is_low_income = false;
        billing_amount = 0.0;
        regular_minutes_remaining = 0.0;
        regular_minutes_worked = 0.0;
        overtime_minutes_worked = 0.0;
        employee_labor_cost = 0.0;
        consultation_profit = 0.0;

        total_minutes_worked = 0.0;
        regular_minutes_total = 0.0;
        overtime_minutes_total = 0.0;
        total_revenue = 0.0;
        total_labor_cost = 0.0;
        net_profit = 0.0;

        clients_processed = 0;
        free_consultations = 0;
        low_income_clients = 0;

        // ------------------------------------------------------------
        // Input
        // ------------------------------------------------------------
        employee_hourly_rate = get_bounded_double(
            "Enter employee hourly wage: $",
            0.01,
            500.0
        );

        client_hourly_rate = get_bounded_double(
            "Enter client hourly billing rate: $",
            0.01,
            1000.0
        );

        overhead_cost = get_bounded_double(
            "Enter fixed daily overhead cost: $",
            0.0,
            1000000.0
        );

        print_simulation_table_header();

        // ------------------------------------------------------------
        // Processing
        // ------------------------------------------------------------
        while (total_minutes_worked < REGULAR_WORKDAY_MINUTES)
        {
            consulting_minutes = static_cast<double>(
                generate_random_consultation_minutes(random_engine)
            );

            is_low_income = generate_random_low_income_status(random_engine);

            billing_amount = calculate_billing_amount(
                client_hourly_rate,
                consulting_minutes,
                is_low_income
            );

            regular_minutes_remaining =
                REGULAR_WORKDAY_MINUTES - total_minutes_worked;

            regular_minutes_worked = calculate_regular_minutes_worked(
                regular_minutes_remaining,
                consulting_minutes
            );

            overtime_minutes_worked = calculate_overtime_minutes_worked(
                regular_minutes_remaining,
                consulting_minutes
            );

            employee_labor_cost = calculate_employee_labor_cost(
                employee_hourly_rate,
                regular_minutes_worked,
                overtime_minutes_worked,
                OVERTIME_MULTIPLIER
            );

            consultation_profit = billing_amount - employee_labor_cost;

            clients_processed++;
            total_minutes_worked += consulting_minutes;
            regular_minutes_total += regular_minutes_worked;
            overtime_minutes_total += overtime_minutes_worked;
            total_revenue += billing_amount;
            total_labor_cost += employee_labor_cost;

            if (is_low_income)
            {
                low_income_clients++;
            }

            if (billing_amount <= 0.0)
            {
                free_consultations++;
            }

        net_profit = calculate_net_profit(
            total_revenue,
            total_labor_cost,
            overhead_cost
        );

        // ------------------------------------------------------------
        // Output
        // ------------------------------------------------------------
            print_simulation_client_row(
                clients_processed,
                is_low_income,
                consulting_minutes,
                billing_amount,
                regular_minutes_worked,
                overtime_minutes_worked,
                employee_labor_cost,
                consultation_profit,
                total_minutes_worked
            );
        }

        print_simulation_day_summary(
            clients_processed,
            free_consultations,
            low_income_clients,
            total_minutes_worked,
            regular_minutes_total,
            overtime_minutes_total,
            total_revenue,
            total_labor_cost,
            overhead_cost,
            net_profit
        );
    } while (get_boolean_choice(REPEAT_PROMPT, "Y", "N"));
}