/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M9 Assignment, Chapter 10 Programming Exercise 21
 * Author: Eric J. Turman
 * Date: 2026-05-14
 *
 * Description:
 * This program defines a bankAccount class and demonstrates basic account
 * processing for up to ten bank customers.
 *
 * ========================================================================
 */

// ========================================================================
// Imports (Includes)
// ========================================================================
#include <cctype>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

// ========================================================================
// Namespace
// ========================================================================
using namespace std;

// ========================================================================
// Constant Declarations
// ========================================================================
const int MAX_CUSTOMERS = 10;
const int FIRST_ACCOUNT_NUMBER = 10001;
const int MENU_CREATE_ACCOUNT = 1;
const int MENU_DISPLAY_ACCOUNTS = 2;
const int MENU_DEPOSIT = 3;
const int MENU_WITHDRAW = 4;
const int MENU_APPLY_INTEREST = 5;
const int MENU_EXIT = 6;
const double MIN_OPENING_BALANCE = 0.0;
const double MAX_MONEY_AMOUNT = 10000000.0;
const double MIN_INTEREST_RATE = 0.0;
const double MAX_INTEREST_RATE = 1.0;

// ========================================================================
// Function Declarations
// ========================================================================
class bankAccount;

string format_with_commas(double numeric_value, int decimal_precision);

int get_bounded_integer(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
);

double get_bounded_double(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
);

string convert_to_lowercase(string source_text);

string trim_text(const string& text_value);

string get_valid_account_holder_name(const string& input_message);

bool account_type_is_valid(const string& account_type);

string get_account_type_from_user();

void display_menu();

void create_account(bankAccount accounts[], int& customer_count);

void display_all_accounts(
    const bankAccount accounts[],
    int customer_count
);

void make_deposit(bankAccount accounts[], int customer_count);

void make_withdrawal(bankAccount accounts[], int customer_count);

void apply_account_interest(bankAccount accounts[], int customer_count);

int find_account_index(
    const bankAccount accounts[],
    int customer_count,
    int account_number
);

// ========================================================================
// Class Declaration
// ========================================================================

/**
 * Store and process the basic properties of one bank account.
 *
 * The default constructor creates an inactive placeholder so an array can be
 * declared before the program knows how many real customers will be entered.
 * Account numbers are assigned only when initialize_account creates a real
 * customer account.
 */
class bankAccount
{
private:
    string account_holder_name;
    int account_number;
    string account_type;
    double balance;
    double interest_rate;
    bool active;
    static int next_account_number;

public:
    bankAccount();

    void initialize_account(
        const string& new_account_holder_name,
        const string& new_account_type,
        double new_opening_balance,
        double new_interest_rate
    );

    bool deposit(double deposit_amount);

    bool withdraw(double withdrawal_amount);

    void apply_interest();

    void display_account_information() const;

    [[nodiscard]]bool is_active() const;

    [[nodiscard]]int get_account_number() const;
};

// ========================================================================
// Static Member Definitions
// ========================================================================
int bankAccount::next_account_number = FIRST_ACCOUNT_NUMBER;

// ========================================================================
// Main Execution
// ========================================================================
/**
 * Demonstrate the bankAccount class with a small menu-driven program.
 *
 * Returns
 * -------
 * status_code : int
 *     Zero when the program completes normally.
 */
int main()
{
    bankAccount customer_accounts[MAX_CUSTOMERS];
    int customer_count = 0;
    int menu_choice = 0;

    cout << "Bank Account Class Demonstration" << endl;
    cout << "================================" << endl;

    while (menu_choice != MENU_EXIT)
    {
        display_menu();
        menu_choice = get_bounded_integer("Selection: ", 1, MENU_EXIT);

        switch (menu_choice)
        {
        case MENU_CREATE_ACCOUNT:
            create_account(customer_accounts, customer_count);
            break;

        case MENU_DISPLAY_ACCOUNTS:
            display_all_accounts(customer_accounts, customer_count);
            break;

        case MENU_DEPOSIT:
            make_deposit(customer_accounts, customer_count);
            break;

        case MENU_WITHDRAW:
            make_withdrawal(customer_accounts, customer_count);
            break;

        case MENU_APPLY_INTEREST:
            apply_account_interest(customer_accounts, customer_count);
            break;

        case MENU_EXIT:
            break;

        default:
            cout << "Error: Invalid menu selection." << endl;
            break;
        }
    }

    cout << "\nProgram complete." << endl;

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

    // Properly format negative numbers since the '-' character
    // counts toward total length.
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
 *     The prompt displayed before input is read.
 *
 * minimum_value : int
 *     The smallest accepted integer value.
 *
 * maximum_value : int
 *     The largest accepted integer value.
 *
 * Returns
 * -------
 * numeric_value : int
 *     A validated integer within the requested bounds.
 */
int get_bounded_integer(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
)
{
    string user_input;
    int numeric_value = 0;
    bool input_is_valid = false;

    while (!input_is_valid)
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
            cout << "Error: Please enter a valid whole-number value." << endl;
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
                 << minimum_value
                 << " and "
                 << maximum_value
                 << "."
                 << endl;
            continue;
        }

        input_is_valid = true;
    }

    return numeric_value;
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
 * Convert a string input to a consistent lowercase form.
 *
 * Parameters
 * ----------
 * source_text : string
 *     The text entered by the user.
 *
 * Returns
 * -------
 * lowercase_text : string
 *     The same text with alphabetic characters converted to lowercase.
 */
string convert_to_lowercase(string source_text)
{
    for (char& current_character : source_text)
    {
        current_character = static_cast<char>(
            tolower(static_cast<unsigned char>(current_character))
        );
    }

    return source_text;
}

/**
 * Remove leading and trailing whitespace from a string.
 *
 * Parameters
 * ----------
 * text_value : const string&
 *     The text value to clean before validation or display.
 *
 * Returns
 * -------
 * trimmed_text : string
 *     A copy of the original string without leading or trailing whitespace.
 */
string trim_text(const string& text_value)
{
    size_t first_position = 0;
    size_t last_position = text_value.length();

    while (first_position < text_value.length() &&
        isspace(static_cast<unsigned char>(text_value[first_position])))
    {
        first_position++;
    }

    while (last_position > first_position &&
        isspace(static_cast<unsigned char>(text_value[last_position - 1])))
    {
        last_position--;
    }

    return text_value.substr(first_position, last_position - first_position);
}

/**
 * Prompt for an account holder name using realistic name rules.
 *
 * Parameters
 * ----------
 * input_message : const string&
 *     The message displayed before input is read.
 *
 * Returns
 * -------
 * account_holder_name : string
 *     A validated account holder name.
 *
 * Notes
 * -----
 * This adapts the name-validation style from the Chapter 8 election program
 * while allowing full account holder names. The regular expression validates
 * each space-separated segment so repeated separators and trailing
 * punctuation are rejected without forcing one capitalization style.
 */
string get_valid_account_holder_name(const string& input_message)
{
    const regex name_segment_pattern(
        R"(^[A-Za-z]+(?:['-][A-Za-z]+)*\.?$)"
    );

    string account_holder_name;

    while (true)
    {
        cout << input_message;
        getline(cin, account_holder_name);

        account_holder_name = trim_text(account_holder_name);

        if (account_holder_name.empty())
        {
            cout << "Error: Entry cannot be blank." << endl;
            continue;
        }

        if (account_holder_name.find("  ") != string::npos)
        {
            cout << "Error: Use only one space between name parts.\n";
            continue;
        }

        bool contains_digit = false;
        bool contains_invalid_symbol = false;

        for (char character : account_holder_name)
        {
            const auto normalized_character =
                static_cast<unsigned char>(character);

            if (isdigit(normalized_character))
            {
                contains_digit = true;
                break;
            }

            if (!isalpha(normalized_character) &&
                character != '\'' &&
                character != '-' &&
                character != '.' &&
                character != ' ')
            {
                contains_invalid_symbol = true;
                break;
            }
        }

        if (contains_digit)
        {
            cout << "Error: Names cannot contain digits." << endl;
            continue;
        }

        if (contains_invalid_symbol)
        {
            cout << "Error: Names can use only letters, apostrophes, "
                 << "hyphens, periods, and single spaces."
                 << endl;
            continue;
        }

        bool has_structure_error = false;
        bool previous_segment_used_period = false;
        stringstream name_stream(account_holder_name);
        string name_segment;

        while (getline(name_stream, name_segment, ' '))
        {
            if (name_segment.empty() ||
                !regex_match(name_segment, name_segment_pattern))
            {
                has_structure_error = true;
                break;
            }

            if (previous_segment_used_period)
            {
                previous_segment_used_period = false;
            }

            if (name_segment.back() == '.')
            {
                previous_segment_used_period = true;
            }
        }

        if (!has_structure_error && !previous_segment_used_period)
        {
            return account_holder_name;
        }

        cout << "Error: Enter a valid name such as John Smith, O'Connor, "
             << "Jean-Luc Picard, St. John, or de la Cruz."
             << endl;
    }
}

/**
 * Check whether a text account type matches the assignment choices.
 *
 * Parameters
 * ----------
 * account_type : const string&
 *     The account type entered by the user.
 *
 * Returns
 * -------
 * type_is_valid : bool
 *     True when the value is checking or saving.
 */
bool account_type_is_valid(const string& account_type)
{
    return account_type == "checking" || account_type == "saving";
}

/**
 * Prompt until the user enters one of the supported account types.
 *
 * Returns
 * -------
 * account_type : string
 *     A validated account type for a new account.
 */
string get_account_type_from_user()
{
    string account_type;

    cout << "Enter account type (checking/saving): ";
    getline(cin, account_type);
    account_type = convert_to_lowercase(account_type);

    while (!account_type_is_valid(account_type))
    {
        cout << "Error: Account type must be checking or saving." << endl;
        cout << "Enter account type (checking/saving): ";
        getline(cin, account_type);
        account_type = convert_to_lowercase(account_type);
    }

    return account_type;
}

/**
 * Display the main program choices.
 *
 * This small menu keeps the class demonstration focused on the operations
 * required by the assignment.
 */
void display_menu()
{
    cout << "\nMenu" << endl;
    cout << "----" << endl;
    cout << MENU_CREATE_ACCOUNT << ". Create account" << endl;
    cout << MENU_DISPLAY_ACCOUNTS << ". Display all active accounts" << endl;
    cout << MENU_DEPOSIT << ". Deposit money" << endl;
    cout << MENU_WITHDRAW << ". Withdraw money" << endl;
    cout << MENU_APPLY_INTEREST << ". Apply interest" << endl;
    cout << MENU_EXIT << ". Exit" << endl;
}

/**
 * Create one real customer account in the fixed account array.
 *
 * Parameters
 * ----------
 * accounts : bankAccount[]
 *     The fixed array used by the program.
 *
 * customer_count : int&
 *     The number of active customer accounts currently stored.
 *
 * Notes
 * -----
 * Account-number assignment stays inside initialize_account so unused array
 * placeholders do not consume account numbers.
 */
void create_account(bankAccount accounts[], int& customer_count)
{
    if (customer_count >= MAX_CUSTOMERS)
    {
        cout << "\nThe customer list is full." << endl;
        return;
    }

    string account_holder_name;
    string account_type;
    double opening_balance = 0.0;
    double interest_rate = 0.0;

    account_holder_name = get_valid_account_holder_name(
        "\nEnter account holder name: "
    );
    account_type = get_account_type_from_user();
    opening_balance = get_bounded_double(
        "Enter opening balance: $",
        MIN_OPENING_BALANCE,
        MAX_MONEY_AMOUNT
    );
    interest_rate = get_bounded_double(
        "Enter interest rate as a decimal, such as 0.035: ",
        MIN_INTEREST_RATE,
        MAX_INTEREST_RATE
    );

    accounts[customer_count].initialize_account(
        account_holder_name,
        account_type,
        opening_balance,
        interest_rate
    );

    cout << "\nAccount created." << endl;
    cout << "Assigned account number: "
         << accounts[customer_count].get_account_number()
         << endl;

    customer_count++;
}

/**
 * Display every active account currently stored in the customer array.
 *
 * Parameters
 * ----------
 * accounts : const bankAccount[]
 *     The fixed array used by the program.
 *
 * customer_count : int
 *     The number of active customer accounts currently stored.
 */
void display_all_accounts(
    const bankAccount accounts[],
    int customer_count
)
{
    if (customer_count == 0)
    {
        cout << "\nNo active accounts have been created." << endl;
        return;
    }

    cout << "\nActive Customer Accounts" << endl;
    cout << "------------------------" << endl;

    for (int index = 0; index < customer_count; ++index)
    {
        accounts[index].display_account_information();
    }
}

/**
 * Prompt for an account and deposit amount, then process the deposit.
 *
 * Parameters
 * ----------
 * accounts : bankAccount[]
 *     The fixed array used by the program.
 *
 * customer_count : int
 *     The number of active customer accounts currently stored.
 */
void make_deposit(bankAccount accounts[], int customer_count)
{
    int account_number = get_bounded_integer(
        "\nEnter account number: ",
        FIRST_ACCOUNT_NUMBER,
        FIRST_ACCOUNT_NUMBER + MAX_CUSTOMERS - 1
    );
    int account_index = find_account_index(
        accounts,
        customer_count,
        account_number
    );

    if (account_index == -1)
    {
        cout << "Error: Account number was not found." << endl;
        return;
    }

    double deposit_amount = get_bounded_double(
        "Enter deposit amount: $",
        0.01,
        MAX_MONEY_AMOUNT
    );

    if (accounts[account_index].deposit(deposit_amount))
    {
        cout << "Deposit completed." << endl;
    }
}

/**
 * Prompt for an account and withdrawal amount, then process the withdrawal.
 *
 * Parameters
 * ----------
 * accounts : bankAccount[]
 *     The fixed array used by the program.
 *
 * customer_count : int
 *     The number of active customer accounts currently stored.
 */
void make_withdrawal(bankAccount accounts[], int customer_count)
{
    int account_number = get_bounded_integer(
        "\nEnter account number: ",
        FIRST_ACCOUNT_NUMBER,
        FIRST_ACCOUNT_NUMBER + MAX_CUSTOMERS - 1
    );
    int account_index = find_account_index(
        accounts,
        customer_count,
        account_number
    );

    if (account_index == -1)
    {
        cout << "Error: Account number was not found." << endl;
        return;
    }

    double withdrawal_amount = get_bounded_double(
        "Enter withdrawal amount: $",
        0.01,
        MAX_MONEY_AMOUNT
    );

    if (accounts[account_index].withdraw(withdrawal_amount))
    {
        cout << "Withdrawal completed." << endl;
    }
}

/**
 * Prompt for an account and apply that account's stored interest rate.
 *
 * Parameters
 * ----------
 * accounts : bankAccount[]
 *     The fixed array used by the program.
 *
 * customer_count : int
 *     The number of active customer accounts currently stored.
 */
void apply_account_interest(bankAccount accounts[], int customer_count)
{
    int account_number = get_bounded_integer(
        "\nEnter account number: ",
        FIRST_ACCOUNT_NUMBER,
        FIRST_ACCOUNT_NUMBER + MAX_CUSTOMERS - 1
    );
    int account_index = find_account_index(
        accounts,
        customer_count,
        account_number
    );

    if (account_index == -1)
    {
        cout << "Error: Account number was not found." << endl;
        return;
    }

    accounts[account_index].apply_interest();
    cout << "Interest applied." << endl;
}

/**
 * Locate an active account in the portion of the array used by customers.
 *
 * Parameters
 * ----------
 * accounts : const bankAccount[]
 *     The fixed array used by the program.
 *
 * customer_count : int
 *     The number of active customer accounts currently stored.
 *
 * account_number : int
 *     The permanent account number being searched for.
 *
 * Returns
 * -------
 * account_index : int
 *     The matching array index, or -1 when no active account matches.
 */
int find_account_index(
    const bankAccount accounts[],
    int customer_count,
    int account_number
)
{
    for (int index = 0; index < customer_count; ++index)
    {
        if (accounts[index].is_active() &&
            accounts[index].get_account_number() == account_number)
        {
            return index;
        }
    }

    return -1;
}

// ========================================================================
// Class Member Function Definitions
// ========================================================================

/**
 * Create an inactive placeholder account.
 *
 * The constructor does not assign an account number because all ten array
 * elements are constructed before the program knows which elements will
 * become real customer accounts.
 */
bankAccount::bankAccount()
{
    account_holder_name = "";
    account_number = 0;
    account_type = "";
    balance = 0.0;
    interest_rate = 0.0;
    active = false;
}

/**
 * Initialize one placeholder as a real customer account.
 *
 * Parameters
 * ----------
 * new_account_holder_name : const string&
 *     The customer's name.
 *
 * new_account_type : const string&
 *     The validated checking or saving account type.
 *
 * new_opening_balance : double
 *     The starting balance for the account.
 *
 * new_interest_rate : double
 *     The account interest rate stored as a decimal value.
 */
void bankAccount::initialize_account(
    const string& new_account_holder_name,
    const string& new_account_type,
    double new_opening_balance,
    double new_interest_rate
)
{
    account_holder_name = new_account_holder_name;
    account_type = new_account_type;
    balance = new_opening_balance;
    interest_rate = new_interest_rate;
    account_number = next_account_number;
    next_account_number++;
    active = true;
}

/**
 * Add money to the account balance.
 *
 * Parameters
 * ----------
 * deposit_amount : double
 *     The positive amount to add.
 *
 * Returns
 * -------
 * deposit_was_completed : bool
 *     True when the deposit amount is valid and the account is active.
 */
bool bankAccount::deposit(double deposit_amount)
{
    if (!active)
    {
        cout << "Error: This account is not active." << endl;
        return false;
    }

    if (deposit_amount <= 0.0)
    {
        cout << "Error: Deposit amount must be greater than zero." << endl;
        return false;
    }

    balance += deposit_amount;
    return true;
}

/**
 * Remove money from the account balance without allowing an overdraft.
 *
 * Parameters
 * ----------
 * withdrawal_amount : double
 *     The positive amount to remove.
 *
 * Returns
 * -------
 * withdrawal_was_completed : bool
 *     True when the withdrawal is valid and funds are available.
 */
bool bankAccount::withdraw(double withdrawal_amount)
{
    if (!active)
    {
        cout << "Error: This account is not active." << endl;
        return false;
    }

    if (withdrawal_amount <= 0.0)
    {
        cout << "Error: Withdrawal amount must be greater than zero." << endl;
        return false;
    }

    if (withdrawal_amount > balance)
    {
        cout << "Error: Withdrawal would overdraw the account." << endl;
        return false;
    }

    balance -= withdrawal_amount;
    return true;
}

/**
 * Apply the stored interest rate to the current balance.
 *
 * Interest is applied only to active accounts. The account number is left
 * unchanged because it is a permanent identifier once assigned.
 */
void bankAccount::apply_interest()
{
    if (active)
    {
        balance += balance * interest_rate;
    }
}

/**
 * Display one account's stored information.
 *
 * The inactive check allows the function to be called safely on an array
 * element without printing placeholder data.
 */
void bankAccount::display_account_information() const
{
    if (active)
    {
        cout << "\nAccount number: " << account_number << endl;
        cout << "Account holder: " << account_holder_name << endl;
        cout << "Account type: " << account_type << endl;
        cout << "Balance: $" << format_with_commas(balance, 2) << endl;
        cout << "Interest rate: "
             << format_with_commas(interest_rate * 100.0, 2)
             << "%"
             << endl;
    }
}

/**
 * Report whether this array element represents a real customer account.
 *
 * Returns
 * -------
 * active : bool
 *     True when the account has been initialized for a customer.
 */
bool bankAccount::is_active() const
{
    return active;
}

/**
 * Return the permanent account number assigned during initialization.
 *
 * Returns
 * -------
 * account_number : int
 *     The account number for a real account, or zero for a placeholder.
 */
int bankAccount::get_account_number() const
{
    return account_number;
}
