/**
* ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M9 Assignment
 * Source: Chapter 11, Programming Exercise 13
 * File: Ch10Exercise21.cpp
 * Author: Eric J. Turman
 * Date: 2026-05-14
 *
 * Description:
 * This program defines a bankAccount base class and two derived account
 * classes named checkingAccount and savingsAccount. The derived classes
 * extend the shared account-number, balance, deposit, withdrawal, and
 * account-printing behavior from the base class.
 *
 * The program creates related checking and savings accounts from a shared
 * customer root account number, then provides a small account simulation
 * menu. The simulation demonstrates the minimum required class operations,
 * including deposits, withdrawals, checking-account service charges,
 * check writing, APY-based monthly interest, account information printing,
 * and overridden class behavior.
 *
 * Notes:
 * This assignment is sourced from Chapter 11, Programming Exercise 13,
 * while the submitted file name remains Ch10Exercise21.cpp to match the
 * course submission requirements.
 *
 * ========================================================================
 */

// ========================================================================
// Imports / Includes
// ========================================================================
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

// ========================================================================
// Namespace
// ========================================================================
using namespace std;

// ========================================================================
// Constants
// ========================================================================
constexpr int MIN_ACCOUNT_NUMBER = 10000;
constexpr int MAX_ACCOUNT_NUMBER = 99999;
constexpr int MINIMUM_ROOT_ACCOUNT_NUMBER = 10000;
constexpr int MAXIMUM_ROOT_ACCOUNT_NUMBER = 99998;

constexpr double MIN_OPENING_BALANCE = 0.0;
constexpr double MAX_MONEY_AMOUNT = 10000000.0;

constexpr double MIN_INTEREST_RATE = 0.0;
constexpr double MAX_INTEREST_RATE = 1.0;

constexpr double MIN_SERVICE_CHARGE = 0.0;
constexpr double MAX_SERVICE_CHARGE = 1000.0;

constexpr double DEFAULT_CHECKING_APY = 0.0084;
constexpr double DEFAULT_SAVINGS_APY = 0.04;
constexpr int DEFAULT_ROOT_ACCOUNT_NUMBER = 10000;
constexpr double DEFAULT_CHECKING_MINIMUM_BALANCE = 500.0;
constexpr double DEFAULT_CHECKING_OPENING_BALANCE = 1000.0;
constexpr double DEFAULT_CHECKING_SERVICE_CHARGE = 20.0;
constexpr double DEFAULT_SAVINGS_OPENING_BALANCE = 1500.0;

constexpr int MENU_PRINT_ACCOUNT_INFORMATION = 1;
constexpr int MENU_UPDATE_CHECKING_ACCOUNT = 2;
constexpr int MENU_UPDATE_SAVINGS_ACCOUNT = 3;
constexpr int MENU_ADVANCE_ONE_MONTH = 4;
constexpr int MENU_SIMULATE_MONTHS = 5;
constexpr int MENU_EXIT = 6;

// ========================================================================
// Function Declarations (Prototypes)
// ========================================================================
class bankAccount;
class checkingAccount;
class savingsAccount;

string format_with_commas(double numeric_value, int decimal_precision);

int get_bounded_integer(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
);

int get_bounded_integer_with_default(
    const string& prompt_message,
    int minimum_value,
    int maximum_value,
    int default_value
);

double get_bounded_double(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
);

string trim_text(const string& text_value);

bool parse_money_input(
    const string& input_text,
    double& parsed_value,
    string& error_message
);

bool parse_apy_input(
    const string& input_text,
    double& parsed_value,
    string& error_message
);

double get_bounded_money(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
);

double get_bounded_money_with_default(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value
);

bool get_optional_bounded_money(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double& user_value
);

double get_bounded_apy(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value
);

double get_bounded_apy_with_default(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value
);

bool get_optional_bounded_apy(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value,
    double& user_value
);

int get_menu_choice_with_enter_exit(
    const string& prompt_message,
    int minimum_value,
    int maximum_value,
    int exit_value
);

void display_section_heading(const string& heading_text);

void display_introduction_banner();

bool get_boolean_choice(
    const string& prompt_message,
    bool enter_key_value
);

int get_even_account_number_from_user();

checkingAccount get_checking_account_from_user(int account_number);

savingsAccount get_savings_account_from_user(int account_number);

void display_transaction_result(
    const string& action_description,
    bool transaction_succeeded
);

void display_account_information(
    const checkingAccount& checking_account,
    const savingsAccount& savings_account
);

void display_account_simulation_menu();

void update_checking_account(checkingAccount& checking_account);

void update_savings_account(savingsAccount& savings_account);

bool process_monthly_activity(
    checkingAccount& checking_account,
    savingsAccount& savings_account,
    int month_number,
    double monthly_checking_income,
    double monthly_checking_spending,
    double monthly_savings_contribution
);

void advance_one_month(
    checkingAccount& checking_account,
    savingsAccount& savings_account,
    int& current_month
);

void simulate_months(
    checkingAccount& checking_account,
    savingsAccount& savings_account,
    int& current_month
);

void run_account_simulation_menu(
    checkingAccount& checking_account,
    savingsAccount& savings_account
);

void display_completion_message();

bool confirm_program_exit();

void pause_program_exit();

// ========================================================================
// Class Declarations
// ========================================================================

/**
 * Store and process the basic properties shared by all bank accounts.
 */
class bankAccount
{
private:
    int account_number;

protected:
    double balance;

public:
    bankAccount();

    bankAccount(
        int new_account_number,
        double new_balance
    );

    virtual ~bankAccount() = default;

    bool set_account_number(int new_account_number);

    [[nodiscard]] int get_account_number() const;

    [[nodiscard]] double get_balance() const;

    bool deposit(double deposit_amount);

    virtual bool withdraw(double withdrawal_amount);

    virtual void print_account_information() const;
};

/**
 * Store and process a checking account with interest, minimum balance, and
 * service charge rules.
 */
class checkingAccount : public bankAccount
{
private:
    double interest_rate;
    double minimum_balance;
    double service_charges;
    double accrued_service_charges;

public:
    checkingAccount();

    checkingAccount(
        int new_account_number,
        double new_balance,
        double new_interest_rate,
        double new_minimum_balance,
        double new_service_charges
    );

    bool set_interest_rate(double new_interest_rate);

    [[nodiscard]] double get_interest_rate() const;

    bool set_minimum_balance(double new_minimum_balance);

    [[nodiscard]] double get_minimum_balance() const;

    bool set_service_charges(double new_service_charges);

    [[nodiscard]] double get_service_charges() const;

    [[nodiscard]] double get_accrued_service_charges() const;

    [[nodiscard]] bool has_accrued_service_charges() const;

    bool post_interest();

    [[nodiscard]] bool is_below_minimum_balance() const;

    void apply_monthly_service_charge_if_needed();

    void pay_accrued_service_charges_if_possible();

    bool write_check(double check_amount);

    bool withdraw(double withdrawal_amount) override;

    void print_account_information() const override;
};

/**
 * Store and process a savings account with interest.
 */
class savingsAccount : public bankAccount
{
private:
    double interest_rate;

public:
    savingsAccount();

    savingsAccount(
        int new_account_number,
        double new_balance,
        double new_interest_rate
    );

    bool set_interest_rate(double new_interest_rate);

    [[nodiscard]] double get_interest_rate() const;

    bool post_interest();

    bool withdraw(double withdrawal_amount) override;

    void print_account_information() const override;
};

// ========================================================================
// Main Execution
// ========================================================================
/**
 * Create one object of each account type and demonstrate the required class
 * operations.
 *
 * Returns
 * -------
 * status_code : int
 *     Zero when the program completes normally.
 */
int main()
{
    bool program_should_exit = false;

    // Input
    display_introduction_banner();

    const int customer_root_account_number =
        get_even_account_number_from_user();

    checkingAccount checking_account = get_checking_account_from_user(
        customer_root_account_number
    );
    savingsAccount savings_account = get_savings_account_from_user(
        customer_root_account_number + 1
    );

    // Processing
    do
    {
        run_account_simulation_menu(checking_account, savings_account);

        // Output
        display_completion_message();
        program_should_exit = confirm_program_exit();
    } while (!program_should_exit);

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Format a decimal value using commas as thousands separators.
 *
 * Parameters
 * ----------
 * numeric_value : double
 *     The value to convert into formatted text.
 *
 * decimal_precision : int
 *     The number of digits to display after the decimal point.
 *
 * Returns
 * -------
 * numeric_text : string
 *     The formatted number with comma separators.
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

    // Properly format negative numbers since the '-' character counts
    // toward total length.
    const int first_digit_position = numeric_text[0] == '-' ? 1 : 0;

    for (int insert_position = static_cast<int>(decimal_position) - 3;
        insert_position > first_digit_position;
        insert_position -= 3)
    {
        numeric_text.insert(insert_position, ",");
    }

    return numeric_text;
}

/**
 * Prompt for an integer value within an allowed range.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : int
 *     The smallest accepted value.
 *
 * maximum_value : int
 *     The largest accepted value.
 *
 * Returns
 * -------
 * user_value : int
 *     A validated integer in the requested range.
 */
int get_bounded_integer(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
)
{
    int user_value = 0;

    while (true)
    {
        cout << prompt_message;
        cin >> user_value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a whole number." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (user_value < minimum_value || user_value > maximum_value)
        {
            cout << "Error: Please enter a value from "
                << minimum_value << " to " << maximum_value << "." << endl;
            continue;
        }

        return user_value;
    }
}

/**
 * Prompt for an integer value, allowing Enter to accept a default.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : int
 *     The smallest accepted value.
 *
 * maximum_value : int
 *     The largest accepted value.
 *
 * default_value : int
 *     The value used when the user presses Enter.
 *
 * Returns
 * -------
 * user_value : int
 *     A validated integer in the requested range.
 */
int get_bounded_integer_with_default(
    const string& prompt_message,
    int minimum_value,
    int maximum_value,
    int default_value
)
{
    string input_text;
    int user_value = default_value;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input_text);

        if (trim_text(input_text).empty())
        {
            return default_value;
        }

        stringstream input_stream(input_text);
        input_stream >> user_value;

        if (input_stream.fail() || !input_stream.eof())
        {
            cout << "Error: Please enter a whole number." << endl;
            continue;
        }

        if (user_value < minimum_value || user_value > maximum_value)
        {
            cout << "Error: Please enter a value from "
                << minimum_value << " to " << maximum_value << "." << endl;
            continue;
        }

        return user_value;
    }
}

/**
 * Prompt for a decimal value within an allowed range.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted value.
 *
 * maximum_value : double
 *     The largest accepted value.
 *
 * Returns
 * -------
 * user_value : double
 *     A validated decimal value in the requested range.
 */
double get_bounded_double(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
)
{
    double user_value = 0.0;

    while (true)
    {
        cout << prompt_message;
        cin >> user_value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a numeric value." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (user_value < minimum_value || user_value > maximum_value)
        {
            cout << "Error: Please enter a value from "
                << format_with_commas(minimum_value, 2) << " to "
                << format_with_commas(maximum_value, 2) << "." << endl;
            continue;
        }

        return user_value;
    }
}

/**
 * Remove leading and trailing whitespace from text.
 *
 * Parameters
 * ----------
 * text_value : const string&
 *     The text to trim.
 *
 * Returns
 * -------
 * trimmed_text : string
 *     The text without leading or trailing whitespace.
 */
string trim_text(const string& text_value)
{
    const size_t first_character = text_value.find_first_not_of(" \t\r\n");

    if (first_character == string::npos)
    {
        return "";
    }

    const size_t last_character = text_value.find_last_not_of(" \t\r\n");
    return text_value.substr(
        first_character,
        last_character - first_character + 1
    );
}

/**
 * Parse money input, allowing a leading dollar sign.
 *
 * Parameters
 * ----------
 * input_text : const string&
 *     The user's raw input text.
 *
 * parsed_value : double&
 *     The parsed money amount when input is valid.
 *
 * error_message : string&
 *     A helpful message when input cannot be parsed.
 *
 * Returns
 * -------
 * input_was_parsed : bool
 *     True when the input can be converted to a money amount.
 */
bool parse_money_input(
    const string& input_text,
    double& parsed_value,
    string& error_message
)
{
    string working_text = trim_text(input_text);

    if (working_text.empty())
    {
        error_message = "Error: Please enter a money amount.";
        return false;
    }

    size_t dollar_position = working_text.find('$');
    if (dollar_position != string::npos)
    {
        if (working_text.find('$', dollar_position + 1) != string::npos)
        {
            error_message = "Error: Use at most one dollar sign.";
            return false;
        }

        const bool has_leading_negative_sign = working_text[0] == '-';
        const size_t allowed_dollar_position =
            has_leading_negative_sign ? 1 : 0;

        if (dollar_position != allowed_dollar_position)
        {
            error_message = "Error: If used, the dollar sign must appear "
                "at the beginning, such as $500.00.";
            return false;
        }

        working_text.erase(dollar_position, 1);
    }

    try
    {
        size_t parsed_character_count = 0;
        parsed_value = stod(working_text, &parsed_character_count);

        if (parsed_character_count != working_text.length())
        {
            error_message = "Error: Please enter only a money amount.";
            return false;
        }
    }
    catch (...)
    {
        error_message = "Error: Please enter a valid money amount.";
        return false;
    }

    return true;
}

/**
 * Parse APY input in percent or decimal form.
 *
 * Parameters
 * ----------
 * input_text : const string&
 *     The user's raw APY input.
 *
 * parsed_value : double&
 *     The parsed APY as a decimal rate.
 *
 * error_message : string&
 *     A helpful message when input cannot be parsed.
 *
 * Returns
 * -------
 * input_was_parsed : bool
 *     True when the input can be converted to a decimal APY.
 */
bool parse_apy_input(
    const string& input_text,
    double& parsed_value,
    string& error_message
)
{
    string working_text = trim_text(input_text);

    if (working_text.empty())
    {
        error_message = "Error: Please enter an APY value.";
        return false;
    }

    const size_t percent_position = working_text.find('%');
    const bool uses_percent_sign = percent_position != string::npos;

    if (uses_percent_sign)
    {
        if (working_text.find('%', percent_position + 1) != string::npos)
        {
            error_message = "Error: Use at most one percent sign.";
            return false;
        }

        if (percent_position != working_text.length() - 1)
        {
            error_message = "Error: If used, the percent sign must appear "
                "at the end, such as 4.65%.";
            return false;
        }

        working_text.erase(percent_position, 1);
        working_text = trim_text(working_text);
    }

    try
    {
        size_t parsed_character_count = 0;
        parsed_value = stod(working_text, &parsed_character_count);

        if (parsed_character_count != working_text.length())
        {
            error_message = "Error: Please enter only an APY value.";
            return false;
        }
    }
    catch (...)
    {
        error_message = "Error: Please enter a valid APY value.";
        return false;
    }

    if (uses_percent_sign || parsed_value > 1.0)
    {
        parsed_value /= 100.0;
    }

    return true;
}

/**
 * Prompt for a bounded money amount.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted value.
 *
 * maximum_value : double
 *     The largest accepted value.
 *
 * Returns
 * -------
 * user_value : double
 *     A validated money amount.
 */
double get_bounded_money(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
)
{
    string input_text;
    double user_value = 0.0;
    string error_message;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input_text);

        if (!parse_money_input(input_text, user_value, error_message))
        {
            cout << error_message << endl;
            continue;
        }

        if (user_value < minimum_value || user_value > maximum_value)
        {
            cout << "Error: Please enter a value from $"
                << format_with_commas(minimum_value, 2) << " to $"
                << format_with_commas(maximum_value, 2) << "." << endl;
            continue;
        }

        return user_value;
    }
}

/**
 * Prompt for a bounded money amount, allowing Enter to accept a default.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted value.
 *
 * maximum_value : double
 *     The largest accepted value.
 *
 * default_value : double
 *     The value used when the user presses Enter.
 *
 * Returns
 * -------
 * user_value : double
 *     A validated money amount.
 */
double get_bounded_money_with_default(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value
)
{
    double user_value = default_value;

    if (get_optional_bounded_money(
        prompt_message,
        minimum_value,
        maximum_value,
        user_value
    ))
    {
        return user_value;
    }

    return default_value;
}

/**
 * Prompt for an optional bounded money amount.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted value.
 *
 * maximum_value : double
 *     The largest accepted value.
 *
 * user_value : double&
 *     The parsed money amount when the user enters one.
 *
 * Returns
 * -------
 * value_was_entered : bool
 *     True when the user enters a value, false when the user presses Enter.
 */
bool get_optional_bounded_money(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double& user_value
)
{
    string input_text;
    string error_message;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input_text);

        if (trim_text(input_text).empty())
        {
            user_value = 0.0;
            return false;
        }

        if (!parse_money_input(input_text, user_value, error_message))
        {
            cout << error_message << endl;
            continue;
        }

        if (user_value < minimum_value || user_value > maximum_value)
        {
            cout << "Error: Please enter a value from $"
                << format_with_commas(minimum_value, 2) << " to $"
                << format_with_commas(maximum_value, 2) << "." << endl;
            continue;
        }

        return true;
    }
}

/**
 * Prompt for a bounded APY value with an optional default.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted decimal APY.
 *
 * maximum_value : double
 *     The largest accepted decimal APY.
 *
 * default_value : double
 *     The decimal APY used when the user presses Enter.
 *
 * Returns
 * -------
 * user_value : double
 *     A validated APY as a decimal rate.
 */
double get_bounded_apy(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value
)
{
    double user_value = default_value;
    get_optional_bounded_apy(
        prompt_message,
        minimum_value,
        maximum_value,
        default_value,
        user_value
    );

    return user_value;
}

/**
 * Prompt for a bounded APY value, allowing Enter to accept a default.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted decimal APY.
 *
 * maximum_value : double
 *     The largest accepted decimal APY.
 *
 * default_value : double
 *     The decimal APY used when the user presses Enter.
 *
 * Returns
 * -------
 * user_value : double
 *     A validated APY as a decimal rate.
 */
double get_bounded_apy_with_default(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value
)
{
    return get_bounded_apy(
        prompt_message,
        minimum_value,
        maximum_value,
        default_value
    );
}

/**
 * Prompt for an optional bounded APY value.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : double
 *     The smallest accepted decimal APY.
 *
 * maximum_value : double
 *     The largest accepted decimal APY.
 *
 * default_value : double
 *     The decimal APY used when the user presses Enter.
 *
 * user_value : double&
 *     The parsed APY as a decimal rate.
 *
 * Returns
 * -------
 * value_was_entered : bool
 *     True when the user enters an APY, false when the default is used.
 */
bool get_optional_bounded_apy(
    const string& prompt_message,
    double minimum_value,
    double maximum_value,
    double default_value,
    double& user_value
)
{
    string input_text;
    string error_message;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input_text);

        if (trim_text(input_text).empty())
        {
            user_value = default_value;
            return false;
        }

        if (!parse_apy_input(input_text, user_value, error_message))
        {
            cout << error_message << endl;
            continue;
        }

        if (user_value < minimum_value || user_value > maximum_value)
        {
            cout << "Error: Please enter an APY from "
                << fixed << setprecision(2) << minimum_value * 100.0
                << "% to " << maximum_value * 100.0 << "%." << endl;
            continue;
        }

        return true;
    }
}

/**
 * Prompt for a menu choice and allow Enter to exit.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * minimum_value : int
 *     The smallest accepted menu value.
 *
 * maximum_value : int
 *     The largest accepted menu value.
 *
 * exit_value : int
 *     The value returned when the user presses Enter.
 *
 * Returns
 * -------
 * menu_choice : int
 *     A valid menu selection, or the exit value.
 */
int get_menu_choice_with_enter_exit(
    const string& prompt_message,
    int minimum_value,
    int maximum_value,
    int exit_value
)
{
    string input_text;
    int menu_choice = exit_value;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input_text);

        if (trim_text(input_text).empty())
        {
            return exit_value;
        }

        stringstream input_stream(input_text);
        input_stream >> menu_choice;

        if (input_stream.fail() || !input_stream.eof())
        {
            cout << "Error: Enter a menu number, or press Enter to exit."
                << endl;
            continue;
        }

        if (menu_choice < minimum_value || menu_choice > maximum_value)
        {
            cout << "Error: Please enter a menu number from "
                << minimum_value << " to " << maximum_value
                << ", or press Enter to exit." << endl;
            continue;
        }

        return menu_choice;
    }
}

/**
 * Display a consistent heading for each major output section.
 *
 * Parameters
 * ----------
 * heading_text : const string&
 *     The heading label to display.
 */
void display_section_heading(const string& heading_text)
{
    cout << "\n" << heading_text << endl;
    cout << string(heading_text.length(), '=') << endl;
}

/**
 * Display the program introduction.
 */
void display_introduction_banner()
{
    cout << "Bank Account Inheritance Demonstration" << endl;
    cout << "======================================" << endl;
    cout << "This program creates one checking account and one savings "
        << "account for a customer." << endl;
    cout << "Both accounts inherit their account number and balance fields "
        << "from the bankAccount base class." << endl;
    cout << "You will enter one even customer root account number. The "
        << "checking account uses that even number, and the savings account "
        << "uses the next odd number." << endl;
    cout << "This keeps the two account numbers visibly paired while each "
        << "account remains its own inherited bankAccount object." << endl;
    cout << "The menu lets you demonstrate deposits, withdrawals, check "
        << "writing, interest posting, and monthly activity." << endl;
}

/**
 * Prompt for a yes or no choice.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * Returns
 * -------
 * user_selected_yes : bool
 *     True when the user enters Y, false when the user enters N.
 */
bool get_boolean_choice(
    const string& prompt_message,
    bool enter_key_value
)
{
    string input_text;

    while (true)
    {
        cout << prompt_message;
        getline(cin, input_text);

        input_text = trim_text(input_text);

        if (input_text.empty())
        {
            return enter_key_value;
        }

        if (input_text.length() == 1)
        {
            const char user_choice =
                static_cast<char>(toupper(input_text[0]));

            if (user_choice == 'Y')
            {
                return true;
            }

            if (user_choice == 'N')
            {
                return false;
            }
        }

        double numeric_choice = 0.0;
        stringstream input_stream(input_text);
        input_stream >> numeric_choice;

        if (!input_stream.fail() && input_stream.eof())
        {
            return numeric_choice != 0.0;
        }

        if (input_text == "yes" || input_text == "YES"
            || input_text == "Yes")
        {
            return true;
        }

        if (input_text == "no" || input_text == "NO"
            || input_text == "No")
        {
            return false;
        }

        cout << "Error: Enter Y, N, a number, or press Enter." << endl;
    }
}

/**
 * Prompt for the customer's even account root number.
 *
 * Returns
 * -------
 * account_number : int
 *     An even account number used for checking. The next odd number is used
 *     for savings.
 */
int get_even_account_number_from_user()
{
    display_section_heading("Customer Account Number");

    while (true)
    {
        const int account_number = get_bounded_integer_with_default(
            "Enter customer root account number, or press [Enter] for "
            + to_string(DEFAULT_ROOT_ACCOUNT_NUMBER) + ": ",
            MINIMUM_ROOT_ACCOUNT_NUMBER,
            MAXIMUM_ROOT_ACCOUNT_NUMBER,
            DEFAULT_ROOT_ACCOUNT_NUMBER
        );

        if (account_number % 2 == 0)
        {
            cout << "Checking account number: " << account_number << endl;
            cout << "Savings account number: " << account_number + 1 << endl;
            return account_number;
        }

        cout << "Error: The root account number must be even." << endl;
    }
}

/**
 * Build a checkingAccount object from validated user input.
 *
 * Parameters
 * ----------
 * account_number : int
 *     The account number assigned to the checking account.
 *
 * Returns
 * -------
 * checking_account : checkingAccount
 *     A checkingAccount object initialized with the user's values.
 */
checkingAccount get_checking_account_from_user(int account_number)
{
    display_section_heading("Checking Account Input");

    const double minimum_balance = get_bounded_money_with_default(
        "Enter checking minimum balance, or press [Enter] for $"
        + format_with_commas(DEFAULT_CHECKING_MINIMUM_BALANCE, 2) + ": ",
        MIN_OPENING_BALANCE,
        MAX_MONEY_AMOUNT,
        DEFAULT_CHECKING_MINIMUM_BALANCE
    );
    const double default_checking_opening_balance =
        minimum_balance > DEFAULT_CHECKING_OPENING_BALANCE
            ? minimum_balance
            : DEFAULT_CHECKING_OPENING_BALANCE;
    const double balance = get_bounded_money_with_default(
        "Enter checking opening balance, or press [Enter] for $"
        + format_with_commas(default_checking_opening_balance, 2) + ": ",
        minimum_balance,
        MAX_MONEY_AMOUNT,
        default_checking_opening_balance
    );
    const double service_charges = get_bounded_money_with_default(
        "Enter checking service charge, or press [Enter] for $"
        + format_with_commas(DEFAULT_CHECKING_SERVICE_CHARGE, 2) + ": ",
        MIN_SERVICE_CHARGE,
        MAX_SERVICE_CHARGE,
        DEFAULT_CHECKING_SERVICE_CHARGE
    );
    const double interest_rate = get_bounded_apy_with_default(
        "Enter checking APY, or press [Enter] for 0.84%: ",
        MIN_INTEREST_RATE,
        MAX_INTEREST_RATE,
        DEFAULT_CHECKING_APY
    );

    return checkingAccount(
        account_number,
        balance,
        interest_rate,
        minimum_balance,
        service_charges
    );
}

/**
 * Build a savingsAccount object from validated user input.
 *
 * Parameters
 * ----------
 * account_number : int
 *     The account number assigned to the savings account.
 *
 * Returns
 * -------
 * savings_account : savingsAccount
 *     A savingsAccount object initialized with the user's values.
 */
savingsAccount get_savings_account_from_user(int account_number)
{
    display_section_heading("Savings Account Input");

    const double balance = get_bounded_money_with_default(
        "Enter savings opening balance, or press [Enter] for $"
        + format_with_commas(DEFAULT_SAVINGS_OPENING_BALANCE, 2) + ": ",
        MIN_OPENING_BALANCE,
        MAX_MONEY_AMOUNT,
        DEFAULT_SAVINGS_OPENING_BALANCE
    );
    const double interest_rate = get_bounded_apy_with_default(
        "Enter savings APY, or press [Enter] for 4.00%: ",
        MIN_INTEREST_RATE,
        MAX_INTEREST_RATE,
        DEFAULT_SAVINGS_APY
    );

    return savingsAccount(account_number, balance, interest_rate);
}

/**
 * Display whether an account transaction succeeded.
 *
 * Parameters
 * ----------
 * action_description : const string&
 *     A short description of the attempted transaction.
 *
 * transaction_succeeded : bool
 *     Whether the transaction completed successfully.
 */
void display_transaction_result(
    const string& action_description,
    bool transaction_succeeded
)
{
    cout << action_description << ": ";

    if (transaction_succeeded)
    {
        cout << "completed" << endl;
    }
    else
    {
        cout << "rejected" << endl;
    }
}

/**
 * Print current checking and savings account information.
 *
 * Parameters
 * ----------
 * checking_account : const checkingAccount&
 *     The checking account to print.
 *
 * savings_account : const savingsAccount&
 *     The savings account to print.
 */
void display_account_information(
    const checkingAccount& checking_account,
    const savingsAccount& savings_account
)
{
    display_section_heading("Current Account Information");
    checking_account.print_account_information();

    cout << endl;
    savings_account.print_account_information();
}

/**
 * Display the account simulation menu.
 */
void display_account_simulation_menu()
{
    cout << "\nAccount Simulation Menu" << endl;
    cout << "-----------------------" << endl;
    cout << MENU_PRINT_ACCOUNT_INFORMATION
        << ". Display account information" << endl;
    cout << MENU_UPDATE_CHECKING_ACCOUNT << ". Update checking account"
        << endl;
    cout << MENU_UPDATE_SAVINGS_ACCOUNT << ". Update savings account" << endl;
    cout << MENU_ADVANCE_ONE_MONTH << ". Advance to next month" << endl;
    cout << MENU_SIMULATE_MONTHS << ". Simulate several months" << endl;
    cout << MENU_EXIT << ". Exit, or press Enter" << endl;
}

/**
 * Update the checking account with one signed activity amount.
 *
 * Parameters
 * ----------
 * checking_account : checkingAccount&
 *     The checking account to update.
 */
void update_checking_account(checkingAccount& checking_account)
{
    double activity_amount = 0.0;
    get_optional_bounded_money(
        "Enter checking activity. Use a positive number for a deposit, "
        "a negative number for a withdrawal/check, or press [Enter] "
        "for none: ",
        -MAX_MONEY_AMOUNT,
        MAX_MONEY_AMOUNT,
        activity_amount
    );

    if (activity_amount > 0.0)
    {
        display_transaction_result(
            "Deposit $" + format_with_commas(activity_amount, 2)
            + " to checking",
            checking_account.deposit(activity_amount)
        );
        checking_account.pay_accrued_service_charges_if_possible();
    }
    else if (activity_amount < 0.0)
    {
        const double check_amount = fabs(activity_amount);
        const bool check_was_written = checking_account.write_check(
            check_amount
        );

        display_transaction_result(
            "Write check for $" + format_with_commas(check_amount, 2),
            check_was_written
        );
    }
    else
    {
        cout << "Checking account activity: no activity entered" << endl;
    }
}

/**
 * Update the savings account with one signed activity amount.
 *
 * Parameters
 * ----------
 * savings_account : savingsAccount&
 *     The savings account to update.
 */
void update_savings_account(savingsAccount& savings_account)
{
    double activity_amount = 0.0;
    get_optional_bounded_money(
        "Enter savings activity. Use a positive number for a deposit, "
        "a negative number for a withdrawal, or press [Enter] for none: ",
        -MAX_MONEY_AMOUNT,
        MAX_MONEY_AMOUNT,
        activity_amount
    );

    if (activity_amount > 0.0)
    {
        display_transaction_result(
            "Deposit $" + format_with_commas(activity_amount, 2)
            + " to savings",
            savings_account.deposit(activity_amount)
        );
    }
    else if (activity_amount < 0.0)
    {
        const double withdrawal_amount = fabs(activity_amount);

        display_transaction_result(
            "Withdraw $" + format_with_commas(withdrawal_amount, 2)
            + " from savings",
            savings_account.withdraw(withdrawal_amount)
        );
    }
    else
    {
        cout << "Savings account activity: no activity entered" << endl;
    }
}

/**
 * Process one month of recurring checking and savings activity.
 *
 * Parameters
 * ----------
 * checking_account : checkingAccount&
 *     The checking account to update.
 *
 * savings_account : savingsAccount&
 *     The savings account to update.
 *
 * month_number : int
 *     The month label to print.
 *
 * monthly_checking_income : double
 *     The monthly income available for checking and savings allocation.
 *
 * monthly_checking_spending : double
 *     The checking spending applied through write_check for the month.
 *
 * monthly_savings_contribution : double
 *     The savings allocation from income, or savings withdrawal when
 *     negative.
 *
 * Returns
 * -------
 * month_was_completed : bool
 *     True when the monthly projection completed. False when projected
 *     checking spending would exceed the available checking balance.
 */
bool process_monthly_activity(
    checkingAccount& checking_account,
    savingsAccount& savings_account,
    int month_number,
    double monthly_checking_income,
    double monthly_checking_spending,
    double monthly_savings_contribution
)
{
    display_section_heading("Month " + to_string(month_number));

    cout << "Before monthly processing:" << endl;
    checking_account.print_account_information();
    cout << endl;
    savings_account.print_account_information();

    checking_account.apply_monthly_service_charge_if_needed();

    double checking_income_deposit = monthly_checking_income;

    if (monthly_savings_contribution > 0.0)
    {
        checking_income_deposit -= monthly_savings_contribution;
    }

    if (checking_income_deposit > 0.0)
    {
        display_transaction_result(
            "Apply checking income of $"
            + format_with_commas(checking_income_deposit, 2),
            checking_account.deposit(checking_income_deposit)
        );
        checking_account.pay_accrued_service_charges_if_possible();
    }
    else
    {
        cout << "Apply checking income: no income entered" << endl;
    }

    if (monthly_savings_contribution > 0.0)
    {
        display_transaction_result(
            "Apply savings contribution of $"
            + format_with_commas(monthly_savings_contribution, 2),
            savings_account.deposit(monthly_savings_contribution)
        );
    }
    else if (monthly_savings_contribution == 0.0)
    {
        cout << "Apply savings activity: no savings activity entered" << endl;
    }

    if (monthly_checking_spending > 0.0)
    {
        if (monthly_checking_spending > checking_account.get_balance())
        {
            cout << "\nSimulation stopped in month " << month_number
                << "." << endl;
            cout << "The projected checking spending exceeds the available "
                << "checking balance." << endl;
            cout << "The checks would bounce, so the remaining projection "
                << "is no longer valid." << endl;
            cout << "\nFinal account information:" << endl;
            checking_account.print_account_information();
            cout << endl;
            savings_account.print_account_information();
            return false;
        }

        display_transaction_result(
            "Apply checking spending through write_check for $"
            + format_with_commas(monthly_checking_spending, 2),
            checking_account.write_check(monthly_checking_spending)
        );
    }
    else
    {
        cout << "Apply checking spending: no spending entered" << endl;
    }

    if (monthly_savings_contribution < 0.0)
    {
        const double savings_withdrawal_amount =
            fabs(monthly_savings_contribution);
        const bool savings_withdrawal_completed =
            savings_account.withdraw(savings_withdrawal_amount);

        display_transaction_result(
            "Apply savings withdrawal of $"
            + format_with_commas(savings_withdrawal_amount, 2),
            savings_withdrawal_completed
        );

        if (savings_withdrawal_completed)
        {
            display_transaction_result(
                "Transfer savings withdrawal to checking",
                checking_account.deposit(savings_withdrawal_amount)
            );
            checking_account.pay_accrued_service_charges_if_possible();
        }
    }

    display_transaction_result(
        "Post checking interest",
        checking_account.post_interest()
    );

    display_transaction_result(
        "Post savings interest",
        savings_account.post_interest()
    );

    cout << "\nAfter monthly processing:" << endl;
    checking_account.print_account_information();
    cout << endl;
    savings_account.print_account_information();

    return true;
}

/**
 * Advance both accounts by one month without extra recurring activity.
 *
 * Parameters
 * ----------
 * checking_account : checkingAccount&
 *     The checking account to update.
 *
 * savings_account : savingsAccount&
 *     The savings account to update.
 *
 * current_month : int&
 *     The month counter to display and advance.
 */
void advance_one_month(
    checkingAccount& checking_account,
    savingsAccount& savings_account,
    int& current_month
)
{
    process_monthly_activity(
        checking_account,
        savings_account,
        current_month,
        0.0,
        0.0,
        0.0
    );
    current_month++;
}

/**
 * Simulate several months of recurring account activity.
 *
 * Parameters
 * ----------
 * checking_account : checkingAccount&
 *     The checking account to update.
 *
 * savings_account : savingsAccount&
 *     The savings account to update.
 *
 * current_month : int&
 *     The month counter to display and advance.
 */
void simulate_months(
    checkingAccount& checking_account,
    savingsAccount& savings_account,
    int& current_month
)
{
    const int number_of_months_to_simulate = get_bounded_integer(
        "Number of months to simulate: ",
        1,
        120
    );
    const double monthly_checking_income = get_bounded_money(
        "Recurring monthly checking income: ",
        0.0,
        MAX_MONEY_AMOUNT
    );
    const double monthly_checking_spending = get_bounded_money(
        "Recurring monthly checking spending: ",
        0.0,
        MAX_MONEY_AMOUNT
    );
    double monthly_savings_contribution = 0.0;

    while (true)
    {
        monthly_savings_contribution = get_bounded_money(
            "Recurring monthly savings contribution "
            "(positive deposit, negative withdrawal, zero for none): ",
            -MAX_MONEY_AMOUNT,
            MAX_MONEY_AMOUNT
        );

        if (monthly_savings_contribution <= monthly_checking_income)
        {
            break;
        }

        cout << "Error: The savings contribution cannot be greater than "
            << "monthly checking income." << endl;
    }

    for (int month_count = 1;
        month_count <= number_of_months_to_simulate;
        month_count++)
    {
        const bool month_was_completed = process_monthly_activity(
            checking_account,
            savings_account,
            current_month,
            monthly_checking_income,
            monthly_checking_spending,
            monthly_savings_contribution
        );

        if (!month_was_completed)
        {
            break;
        }

        current_month++;
    }
}

/**
 * Run the menu-driven checking and savings account simulation.
 *
 * Parameters
 * ----------
 * checking_account : checkingAccount&
 *     The checking account used in the simulation.
 *
 * savings_account : savingsAccount&
 *     The savings account used in the simulation.
 */
void run_account_simulation_menu(
    checkingAccount& checking_account,
    savingsAccount& savings_account
)
{
    int current_month = 1;
    int menu_choice = 0;

    while (menu_choice != MENU_EXIT)
    {
        display_account_simulation_menu();
        menu_choice = get_menu_choice_with_enter_exit(
            "Selection: ",
            MENU_PRINT_ACCOUNT_INFORMATION,
            MENU_EXIT,
            MENU_EXIT
        );

        switch (menu_choice)
        {
        case MENU_PRINT_ACCOUNT_INFORMATION:
            display_account_information(checking_account, savings_account);
            break;

        case MENU_UPDATE_CHECKING_ACCOUNT:
            update_checking_account(checking_account);
            break;

        case MENU_UPDATE_SAVINGS_ACCOUNT:
            update_savings_account(savings_account);
            break;

        case MENU_ADVANCE_ONE_MONTH:
            advance_one_month(
                checking_account,
                savings_account,
                current_month
            );
            break;

        case MENU_SIMULATE_MONTHS:
            simulate_months(
                checking_account,
                savings_account,
                current_month
            );
            break;

        case MENU_EXIT:
            break;

        default:
            cout << "Error: Invalid menu selection." << endl;
            break;
        }
    }
}

/**
 * Display the final completion message.
 */
void display_completion_message()
{
    cout << "\nProgram complete. The checking and savings account simulation "
         << "has ended after demonstrating the account inheritance structure."
         << endl;
}

/**
 * Confirm whether the user meant to exit after leaving the menu.
 *
 * Returns
 * -------
 * exit_was_confirmed : bool
 *     True when the user presses Enter, false when the user enters N.
 */
bool confirm_program_exit()
{
    string input_text;

    while (true)
    {
        cout << "Did you mean to exit? Enter N to return to the menu, "
            << "or press [Enter] to exit: ";
        getline(cin, input_text);

        input_text = trim_text(input_text);

        if (input_text.empty())
        {
            return true;
        }

        if (input_text.length() == 1
            && toupper(input_text[0]) == 'N')
        {
            return false;
        }

        cout << "Error: Enter N to return to the menu, or press Enter "
            << "to exit." << endl;
    }
}

/**
 * Pause before the program exits.
 */
void pause_program_exit()
{
    cout << "\nPress Enter to exit...";
    cin.get();
}

// ========================================================================
// Class Member Function Definitions
// ========================================================================

/**
 * Create a default bank account.
 */
bankAccount::bankAccount()
{
    account_number = 0;
    balance = 0.0;
}

/**
 * Create a bank account with an account number and balance.
 *
 * Parameters
 * ----------
 * new_account_number : int
 *     The account number assigned to the account.
 *
 * new_balance : double
 *     The beginning account balance.
 */
bankAccount::bankAccount(
    int new_account_number,
    double new_balance
)
{
    account_number = MIN_ACCOUNT_NUMBER;
    balance = 0.0;

    set_account_number(new_account_number);

    if (new_balance >= MIN_OPENING_BALANCE
        && new_balance <= MAX_MONEY_AMOUNT)
    {
        balance = new_balance;
    }
}

/**
 * Set the account number.
 *
 * Parameters
 * ----------
 * new_account_number : int
 *     The account number to store.
 *
 * Returns
 * -------
 * account_number_was_set : bool
 *     True when the value is within the allowed account number range.
 */
bool bankAccount::set_account_number(int new_account_number)
{
    if (new_account_number < MIN_ACCOUNT_NUMBER
        || new_account_number > MAX_ACCOUNT_NUMBER)
    {
        return false;
    }

    account_number = new_account_number;
    return true;
}

/**
 * Retrieve the account number.
 *
 * Returns
 * -------
 * account_number : int
 *     The stored account number.
 */
int bankAccount::get_account_number() const
{
    return account_number;
}

/**
 * Retrieve the account balance.
 *
 * Returns
 * -------
 * balance : double
 *     The current account balance.
 */
double bankAccount::get_balance() const
{
    return balance;
}

/**
 * Deposit money into the account.
 *
 * Parameters
 * ----------
 * deposit_amount : double
 *     The amount to add to the account.
 *
 * Returns
 * -------
 * deposit_was_completed : bool
 *     True when the deposit amount is positive and within the money limit.
 */
bool bankAccount::deposit(double deposit_amount)
{
    if (deposit_amount <= 0.0 || deposit_amount > MAX_MONEY_AMOUNT)
    {
        return false;
    }

    balance += deposit_amount;
    return true;
}

/**
 * Withdraw money from the account.
 *
 * Parameters
 * ----------
 * withdrawal_amount : double
 *     The amount to remove from the account.
 *
 * Returns
 * -------
 * withdrawal_was_completed : bool
 *     True when the amount is positive and does not overdraw the account.
 */
bool bankAccount::withdraw(double withdrawal_amount)
{
    if (withdrawal_amount <= 0.0 || withdrawal_amount > MAX_MONEY_AMOUNT)
    {
        return false;
    }

    if (withdrawal_amount > balance)
    {
        return false;
    }

    balance -= withdrawal_amount;
    return true;
}

/**
 * Print the account's stored information.
 */
void bankAccount::print_account_information() const
{
    cout << "Account type: Base bank account" << endl;
    cout << "Account number: " << account_number << endl;
    cout << "Balance: $" << format_with_commas(balance, 2) << endl;
}

/**
 * Create a default checking account.
 */
checkingAccount::checkingAccount()
    : bankAccount()
{
    interest_rate = 0.0;
    minimum_balance = 0.0;
    service_charges = 0.0;
    accrued_service_charges = 0.0;
}

/**
 * Create a checking account with checking-specific rules.
 *
 * Parameters
 * ----------
 * new_account_number : int
 *     The account number assigned to the account.
 *
 * new_balance : double
 *     The beginning account balance.
 *
 * new_interest_rate : double
 *     The checking account interest rate as a decimal.
 *
 * new_minimum_balance : double
 *     The minimum required account balance.
 *
 * new_service_charges : double
 *     The charge applied during monthly processing when the balance is below
 *     the minimum balance.
 */
checkingAccount::checkingAccount(
    int new_account_number,
    double new_balance,
    double new_interest_rate,
    double new_minimum_balance,
    double new_service_charges
)
    : bankAccount(new_account_number, new_balance)
{
    interest_rate = 0.0;
    minimum_balance = 0.0;
    service_charges = 0.0;
    accrued_service_charges = 0.0;

    set_interest_rate(new_interest_rate);
    set_minimum_balance(new_minimum_balance);
    set_service_charges(new_service_charges);
}

/**
 * Set the checking account interest rate.
 *
 * Parameters
 * ----------
 * new_interest_rate : double
 *     The rate to store as a decimal value.
 *
 * Returns
 * -------
 * interest_rate_was_set : bool
 *     True when the rate is inside the allowed range.
 */
bool checkingAccount::set_interest_rate(double new_interest_rate)
{
    if (new_interest_rate < MIN_INTEREST_RATE
        || new_interest_rate > MAX_INTEREST_RATE)
    {
        return false;
    }

    interest_rate = new_interest_rate;
    return true;
}

/**
 * Retrieve the checking account interest rate.
 *
 * Returns
 * -------
 * interest_rate : double
 *     The stored checking account interest rate.
 */
double checkingAccount::get_interest_rate() const
{
    return interest_rate;
}

/**
 * Set the checking account minimum balance.
 *
 * Parameters
 * ----------
 * new_minimum_balance : double
 *     The minimum balance to store.
 *
 * Returns
 * -------
 * minimum_balance_was_set : bool
 *     True when the amount is within the allowed money range.
 */
bool checkingAccount::set_minimum_balance(double new_minimum_balance)
{
    if (new_minimum_balance < MIN_OPENING_BALANCE
        || new_minimum_balance > MAX_MONEY_AMOUNT)
    {
        return false;
    }

    minimum_balance = new_minimum_balance;
    return true;
}

/**
 * Retrieve the checking account minimum balance.
 *
 * Returns
 * -------
 * minimum_balance : double
 *     The stored minimum balance.
 */
double checkingAccount::get_minimum_balance() const
{
    return minimum_balance;
}

/**
 * Set the checking account service charge.
 *
 * Parameters
 * ----------
 * new_service_charges : double
 *     The service charge to store.
 *
 * Returns
 * -------
 * service_charge_was_set : bool
 *     True when the amount is within the allowed service charge range.
 */
bool checkingAccount::set_service_charges(double new_service_charges)
{
    if (new_service_charges < MIN_SERVICE_CHARGE
        || new_service_charges > MAX_SERVICE_CHARGE)
    {
        return false;
    }

    service_charges = new_service_charges;
    return true;
}

/**
 * Retrieve the checking account service charge.
 *
 * Returns
 * -------
 * service_charges : double
 *     The stored service charge.
 */
double checkingAccount::get_service_charges() const
{
    return service_charges;
}

/**
 * Retrieve the accrued unpaid service charges.
 *
 * Returns
 * -------
 * accrued_service_charges : double
 *     The total unpaid checking service charges.
 */
double checkingAccount::get_accrued_service_charges() const
{
    return accrued_service_charges;
}

/**
 * Determine whether the checking account has unpaid service charges.
 *
 * Returns
 * -------
 * charges_are_accrued : bool
 *     True when unpaid service charges remain.
 */
bool checkingAccount::has_accrued_service_charges() const
{
    return accrued_service_charges > 0.0;
}

/**
 * Add earned interest to the checking account balance.
 *
 * Returns
 * -------
 * interest_was_posted : bool
 *     True when the account has a positive balance and interest can be added.
 */
bool checkingAccount::post_interest()
{
    if (balance <= 0.0)
    {
        return false;
    }

    const double monthly_interest_rate =
        pow(1.0 + interest_rate, 1.0 / 12.0) - 1.0;
    balance += balance * monthly_interest_rate;
    return true;
}

/**
 * Determine whether the balance is below the required minimum balance.
 *
 * Returns
 * -------
 * below_minimum_balance : bool
 *     True when the current balance is less than the minimum balance.
 */
bool checkingAccount::is_below_minimum_balance() const
{
    return balance < minimum_balance;
}

/**
 * Apply the monthly checking service charge at the beginning of the month
 * when the balance is below the required minimum balance.
 *
 * Notes
 * -----
 * This method is intended to be called once at the beginning of each monthly
 * processing step. Withdrawals and checks do not apply the service charge
 * immediately.
 */
void checkingAccount::apply_monthly_service_charge_if_needed()
{
    if (!is_below_minimum_balance() || service_charges <= 0.0)
    {
        cout << "Monthly checking service charge: not required" << endl;
        return;
    }

    double unpaid_service_charge = service_charges;
    double amount_paid_from_checking = 0.0;

    if (balance > 0.0)
    {
        if (balance < unpaid_service_charge)
        {
            amount_paid_from_checking = balance;
        }
        else
        {
            amount_paid_from_checking = unpaid_service_charge;
        }

        balance -= amount_paid_from_checking;
        unpaid_service_charge -= amount_paid_from_checking;
    }

    if (unpaid_service_charge > 0.0)
    {
        accrued_service_charges += unpaid_service_charge;
    }

    cout << "Monthly checking service charge assessed: $"
        << format_with_commas(service_charges, 2)
        << endl;
    cout << "Paid from checking balance: $"
        << format_with_commas(amount_paid_from_checking, 2) << endl;

    if (unpaid_service_charge > 0.0)
    {
        cout << "Unpaid service charge added: $"
            << format_with_commas(unpaid_service_charge, 2) << endl;
        cout << "Unpaid service charges now total: $"
            << format_with_commas(accrued_service_charges, 2) << endl;
    }
}

/**
 * Pay accrued checking service charges from the available checking balance.
 *
 * Notes
 * -----
 * Deposits and monthly checking income call this method after the money is
 * added to the account.
 */
void checkingAccount::pay_accrued_service_charges_if_possible()
{
    if (accrued_service_charges <= 0.0 || balance <= 0.0)
    {
        return;
    }

    double amount_paid = 0.0;

    if (balance < accrued_service_charges)
    {
        amount_paid = balance;
    }
    else
    {
        amount_paid = accrued_service_charges;
    }

    balance -= amount_paid;
    accrued_service_charges -= amount_paid;

    cout << "Accrued service charge payment: $"
        << format_with_commas(amount_paid, 2) << " paid." << endl;
    cout << "Accrued service charges remaining: $"
        << format_with_commas(accrued_service_charges, 2) << endl;
}

/**
 * Write a check against the checking account.
 *
 * Parameters
 * ----------
 * check_amount : double
 *     The amount of the check to process.
 *
 * Returns
 * -------
 * check_was_written : bool
 *     True when the check amount is valid.
 */
bool checkingAccount::write_check(double check_amount)
{
    return withdraw(check_amount);
}

/**
 * Withdraw money from the checking account.
 *
 * Parameters
 * ----------
 * withdrawal_amount : double
 *     The amount to remove from the checking account.
 *
 * Returns
 * -------
 * withdrawal_was_completed : bool
 *     True when the withdrawal amount is valid.
 *
 * Notes
 * -----
 * Checking withdrawals do not apply monthly service charges. Those charges
 * are evaluated during monthly processing.
 */
bool checkingAccount::withdraw(double withdrawal_amount)
{
    if (withdrawal_amount <= 0.0 || withdrawal_amount > MAX_MONEY_AMOUNT)
    {
        return false;
    }

    if (withdrawal_amount > balance)
    {
        return false;
    }

    balance -= withdrawal_amount;
    return true;
}

/**
 * Print inherited and checking-specific account information.
 */
void checkingAccount::print_account_information() const
{
    cout << "Account type: Checking account" << endl;
    cout << "Account number: " << get_account_number() << endl;
    cout << "Balance: $" << format_with_commas(balance, 2) << endl;
    cout << "APY: " << fixed << setprecision(2)
        << interest_rate * 100.0 << "%" << endl;
    cout << "Minimum balance: $"
        << format_with_commas(minimum_balance, 2) << endl;
    cout << "Service charge: $"
        << format_with_commas(service_charges, 2) << endl;
    cout << "Accrued unpaid service charges: $"
        << format_with_commas(accrued_service_charges, 2) << endl;
    cout << "Minimum balance status: ";

    if (is_below_minimum_balance())
    {
        cout << "Below minimum" << endl;
    }
    else
    {
        cout << "Meets minimum" << endl;
    }
}

/**
 * Create a default savings account.
 */
savingsAccount::savingsAccount()
    : bankAccount()
{
    interest_rate = 0.0;
}

/**
 * Create a savings account with an interest rate.
 *
 * Parameters
 * ----------
 * new_account_number : int
 *     The account number assigned to the account.
 *
 * new_balance : double
 *     The beginning account balance.
 *
 * new_interest_rate : double
 *     The savings account interest rate as a decimal.
 */
savingsAccount::savingsAccount(
    int new_account_number,
    double new_balance,
    double new_interest_rate
)
    : bankAccount(new_account_number, new_balance)
{
    interest_rate = 0.0;
    set_interest_rate(new_interest_rate);
}

/**
 * Set the savings account interest rate.
 *
 * Parameters
 * ----------
 * new_interest_rate : double
 *     The rate to store as a decimal value.
 *
 * Returns
 * -------
 * interest_rate_was_set : bool
 *     True when the rate is inside the allowed range.
 */
bool savingsAccount::set_interest_rate(double new_interest_rate)
{
    if (new_interest_rate < MIN_INTEREST_RATE
        || new_interest_rate > MAX_INTEREST_RATE)
    {
        return false;
    }

    interest_rate = new_interest_rate;
    return true;
}

/**
 * Retrieve the savings account interest rate.
 *
 * Returns
 * -------
 * interest_rate : double
 *     The stored savings account interest rate.
 */
double savingsAccount::get_interest_rate() const
{
    return interest_rate;
}

/**
 * Add earned interest to the savings account balance.
 *
 * Returns
 * -------
 * interest_was_posted : bool
 *     True when the account has a positive balance and interest can be added.
 */
bool savingsAccount::post_interest()
{
    if (balance <= 0.0)
    {
        return false;
    }

    const double monthly_interest_rate =
        pow(1.0 + interest_rate, 1.0 / 12.0) - 1.0;
    balance += balance * monthly_interest_rate;
    return true;
}

/**
 * Withdraw money from the savings account.
 *
 * Parameters
 * ----------
 * withdrawal_amount : double
 *     The amount to remove from the savings account.
 *
 * Returns
 * -------
 * withdrawal_was_completed : bool
 *     True when the withdrawal amount is valid and funds are available.
 */
bool savingsAccount::withdraw(double withdrawal_amount)
{
    return bankAccount::withdraw(withdrawal_amount);
}

/**
 * Print inherited and savings-specific account information.
 */
void savingsAccount::print_account_information() const
{
    cout << "Account type: Savings account" << endl;
    cout << "Account number: " << get_account_number() << endl;
    cout << "Balance: $" << format_with_commas(balance, 2) << endl;
    cout << "APY: " << fixed << setprecision(2)
        << interest_rate * 100.0 << "%" << endl;
}
