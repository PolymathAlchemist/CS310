/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M8 Assignment, Chapter 10 Programming Exercise 21
 * Author: Eric J. Turman
 * Date: 2026-05-08
 *
 * Description:
 * This program defines a bankAccount class and demonstrates basic account
 * processing for up to ten bank customers.
 *
 * The array is intentionally created with ten default-constructed account
 * objects. Those placeholders remain inactive until a real customer account
 * is initialized, which keeps the static account-number counter from being
 * consumed by unused array elements.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes)
// ========================================================================
#include <iomanip>
#include <iostream>
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
const double MAX_MONEY_AMOUNT = 100000000.0;
const double MIN_INTEREST_RATE = 0.0;
const double MAX_INTEREST_RATE = 1.0;

// ========================================================================
// Function Declarations
// ========================================================================
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

bool account_type_is_valid(const string& account_type);

string get_account_type_from_user();

void display_menu();

int find_account_index(
    const class bankAccount accounts[],
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

    bool is_active() const;

    int get_account_number() const;
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

        if (menu_choice == MENU_CREATE_ACCOUNT)
        {
            if (customer_count >= MAX_CUSTOMERS)
            {
                cout << "\nThe customer list is full." << endl;
            }
            else
            {
                string account_holder_name;
                string account_type;
                double opening_balance = 0.0;
                double interest_rate = 0.0;

                cout << "\nEnter account holder name: ";
                getline(cin, account_holder_name);

                while (account_holder_name.empty())
                {
                    cout << "Error: Please enter a customer name." << endl;
                    cout << "Enter account holder name: ";
                    getline(cin, account_holder_name);
                }

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

                customer_accounts[customer_count].initialize_account(
                    account_holder_name,
                    account_type,
                    opening_balance,
                    interest_rate
                );

                cout << "\nAccount created." << endl;
                cout << "Assigned account number: "
                     << customer_accounts[customer_count].get_account_number()
                     << endl;

                customer_count++;
            }
        }
        else if (menu_choice == MENU_DISPLAY_ACCOUNTS)
        {
            if (customer_count == 0)
            {
                cout << "\nNo active accounts have been created." << endl;
            }
            else
            {
                cout << "\nActive Customer Accounts" << endl;
                cout << "------------------------" << endl;

                for (int index = 0; index < customer_count; ++index)
                {
                    customer_accounts[index].display_account_information();
                }
            }
        }
        else if (menu_choice == MENU_DEPOSIT)
        {
            int account_number = 0;
            int account_index = -1;
            double deposit_amount = 0.0;

            account_number = get_bounded_integer(
                "\nEnter account number: ",
                FIRST_ACCOUNT_NUMBER,
                FIRST_ACCOUNT_NUMBER + MAX_CUSTOMERS - 1
            );
            account_index = find_account_index(
                customer_accounts,
                customer_count,
                account_number
            );

            if (account_index == -1)
            {
                cout << "Error: Account number was not found." << endl;
            }
            else
            {
                deposit_amount = get_bounded_double(
                    "Enter deposit amount: $",
                    0.01,
                    MAX_MONEY_AMOUNT
                );

                if (customer_accounts[account_index].deposit(deposit_amount))
                {
                    cout << "Deposit completed." << endl;
                }
            }
        }
        else if (menu_choice == MENU_WITHDRAW)
        {
            int account_number = 0;
            int account_index = -1;
            double withdrawal_amount = 0.0;

            account_number = get_bounded_integer(
                "\nEnter account number: ",
                FIRST_ACCOUNT_NUMBER,
                FIRST_ACCOUNT_NUMBER + MAX_CUSTOMERS - 1
            );
            account_index = find_account_index(
                customer_accounts,
                customer_count,
                account_number
            );

            if (account_index == -1)
            {
                cout << "Error: Account number was not found." << endl;
            }
            else
            {
                withdrawal_amount = get_bounded_double(
                    "Enter withdrawal amount: $",
                    0.01,
                    MAX_MONEY_AMOUNT
                );

                if (customer_accounts[account_index].withdraw(
                    withdrawal_amount
                ))
                {
                    cout << "Withdrawal completed." << endl;
                }
            }
        }
        else if (menu_choice == MENU_APPLY_INTEREST)
        {
            int account_number = 0;
            int account_index = -1;

            account_number = get_bounded_integer(
                "\nEnter account number: ",
                FIRST_ACCOUNT_NUMBER,
                FIRST_ACCOUNT_NUMBER + MAX_CUSTOMERS - 1
            );
            account_index = find_account_index(
                customer_accounts,
                customer_count,
                account_number
            );

            if (account_index == -1)
            {
                cout << "Error: Account number was not found." << endl;
            }
            else
            {
                customer_accounts[account_index].apply_interest();
                cout << "Interest applied." << endl;
            }
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

    while (!account_type_is_valid(account_type))
    {
        cout << "Error: Account type must be checking or saving." << endl;
        cout << "Enter account type (checking/saving): ";
        getline(cin, account_type);
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
    cout << "1. Create account" << endl;
    cout << "2. Display all active accounts" << endl;
    cout << "3. Deposit money" << endl;
    cout << "4. Withdraw money" << endl;
    cout << "5. Apply interest" << endl;
    cout << "6. Exit" << endl;
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
