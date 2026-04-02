/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M3 Assignment - Chapter 4, Programming Exercise 16
 * Author: Eric J. Turman
 * Date: 2026-03-30
 *
 * Description:
 * Estimates an author's royalties under three publishing contract options
 * and reports which option produces the highest royalty.
 *
 * Note:
 * This version uses named constants for all fixed payments, royalty rates,
 * and threshold values, and includes modest input validation.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <sstream>

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
bool get_yes_or_enter(const string& prompt_message);
double get_non_negative_double(const string& prompt_message);
int get_non_negative_integer(const string& prompt_message);
double calculate_option_one_royalty();
double calculate_option_two_royalty(
    double net_price_per_copy,
    int estimated_copies_sold
);
double calculate_option_three_royalty(
    double net_price_per_copy,
    int estimated_copies_sold
);
string format_with_commas(double numeric_value, int decimal_precision);
int determine_best_option(
    double option_one_royalty,
    double option_two_royalty,
    double option_three_royalty
);
double determine_best_royalty(
    double option_one_royalty,
    double option_two_royalty,
    double option_three_royalty
);

// ========================================================================
// Main Execution
// ========================================================================
int main() {
    // --------------------------------------------------------------------
    // Constant declarations (fixed values, prompts, messages)
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "This program estimates an author's royalties under three contract "
        "options \nand reports which option yields the highest royalty.\n";

    const string INTRO_STORY_CHOICE_PROMPT =
        "Would you like the accompanying story (Y) or press \"Enter\" "
        "to continue to the inputs.";

    const string INTRO_STORY_MESSAGE =
        "After saying goodbye to 'Tim the Financial Enchanter' you come "
        "across a curious clerk sitting at a desk in the middle of\n"
        "an open field. He looks up from his ledger and states, \"Well, "
        "let's have a look at it.\" gesturing to a manuscript that\n"
        "you are suddenly aware of holding. You hand it over, and after "
        "quickly flipping through the pages, he looks back up at\n"
        "you. \"I have a couple of questions for you, and I'll recommend the "
        "best option for you that yields the highest return.\"\n"
        "He hands you a paper describing three options:\n\n"
        "  Option 1: $5,000 upon delivery + $20,000 upon publication (fixed).\n"
        "  Option 2: 12.5% of net price per copy sold.\n"
        "  Option 3: 10% of net price for the first 4,000 copies,\n"
        "            then 14% for all additional copies.\n\n"
        "\"Give me a few details,\" he says, \"and I'll recommend the best "
        "option for you.\" \n";

    const string NET_PRICE_PROMPT =
        "What price will you sell each copy of the novel: $";

    const string COPIES_SOLD_PROMPT =
        "What is estimated number of copies that will be sold: ";

    const string REPEAT_PROMPT =
    "Estimate another book? (Y) or press Enter to exit:";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    double net_price_per_copy = 0.0;
    double option_one_royalty = 0.0;
    double option_two_royalty = 0.0;
    double option_three_royalty = 0.0;
    double best_royalty = 0.0;

    int estimated_copies_sold = 0;
    int best_option = 0;

    // --------------------------------------------------------------------
    // Output formatting
    // --------------------------------------------------------------------
    cout << fixed << setprecision(2);

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRO_MESSAGE << endl;
    if (get_yes_or_enter(INTRO_STORY_CHOICE_PROMPT))
    {
        cout << "\n" << INTRO_STORY_MESSAGE << endl;
    }

    // --------------------------------------------------------------------
    // Main Loop
    // --------------------------------------------------------------------
    do
    {

        // --------------------------------------------------------------------
        // Input
        // --------------------------------------------------------------------
        net_price_per_copy = get_non_negative_double(NET_PRICE_PROMPT);
        estimated_copies_sold = get_non_negative_integer(COPIES_SOLD_PROMPT);

        // --------------------------------------------------------------------
        // Processing
        // --------------------------------------------------------------------
        option_one_royalty = calculate_option_one_royalty();

        option_two_royalty = calculate_option_two_royalty(
            net_price_per_copy,
            estimated_copies_sold
        );

        option_three_royalty = calculate_option_three_royalty(
            net_price_per_copy,
            estimated_copies_sold
        );

        best_option = determine_best_option(
            option_one_royalty,
            option_two_royalty,
            option_three_royalty
        );

        best_royalty = determine_best_royalty(
            option_one_royalty,
            option_two_royalty,
            option_three_royalty
        );

        // --------------------------------------------------------------------
        // Output
        // --------------------------------------------------------------------
        cout << "\n------------------------------------------------------------\n";
        cout << "Royalty estimates based on a net price of $"
             << format_with_commas(net_price_per_copy, 2)
             << " and estimated sales of "
             << format_with_commas(estimated_copies_sold, 0)
             << " copies:\n\n";

        cout << "Option 1: $"
             << format_with_commas(option_one_royalty, 2) << endl;
        cout << "Option 2: $"
             << format_with_commas(option_two_royalty, 2) << endl;
        cout << "Option 3: $"
             << format_with_commas(option_three_royalty, 2) << endl;

        cout << "\nBest option: Option "
             << best_option
             << " with estimated royalties of $"
             << format_with_commas(best_royalty, 2)
             << ".\n";

    } while (get_yes_or_enter(REPEAT_PROMPT));

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Prompt the user for a simple yes/continue decision.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed to the user before input is read.
 *
 * Returns
 * -------
 * user_choice : bool
 *     Returns true if the user enters 'Y' or 'y'.
 *     Returns false if the user presses Enter with no input.
 *
 * Notes
 * -----
 * This function reads the entire line using getline so it can distinguish
 * between a single-character response and empty input. Any input other than
 * 'Y', 'y', or an empty line is rejected and the user is prompted again.
 */
bool get_yes_or_enter(const string& prompt_message)
{
    string user_input;

    while (true)
    {
        cout << prompt_message << " ";
        getline(cin, user_input);

        if (user_input.empty())
        {
            return false;
        }

        if (user_input.length() == 1 &&
            (user_input[0] == 'Y' || user_input[0] == 'y'))
        {
            return true;
        }

        cout << "Error: Enter 'Y' or press Enter to continue." << endl;
    }
}

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
 * This function reads the entire line first so it can distinguish between
 * valid decimal input and input that contains extra characters.
 */
double get_non_negative_double(const string& prompt_message)
{
    string user_input;
    double numeric_value = 0.0;

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        if (user_input.empty())
        {
            cout << "Error: Please enter a valid numeric value." << endl;
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

        if (numeric_value < 0.0)
        {
            cout << "Error: Value cannot be negative." << endl;
            continue;
        }

        return numeric_value;
    }
}

/**
 * Prompt for a non-negative whole number.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The prompt displayed to the user before input is read.
 *
 * Returns
 * -------
 * numeric_value : int
 *     A validated whole number that is zero or greater.
 *
 * Notes
 * -----
 * This function reads the entire line first so it can distinguish between
 * valid integers and input that contains decimals or extra characters.
 */
int get_non_negative_integer(const string& prompt_message)
{
    string user_input;
    int numeric_value = 0;

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        if (user_input.empty())
        {
            cout << "Error: Please enter a valid whole number." << endl;
            continue;
        }

        stringstream input_stream(user_input);
        input_stream >> numeric_value;

        if (input_stream.fail())
        {
            cout << "Error: Please enter a valid whole number." << endl;
            continue;
        }

        input_stream >> ws;

        if (!input_stream.eof())
        {
            if (user_input.find('.') != string::npos)
            {
                cout << "Error: Decimal values are not allowed." << endl;
            }
            else
            {
                cout << "Error: Please enter a valid whole number." << endl;
            }
            continue;
        }

        if (numeric_value < 0)
        {
            cout << "Error: Value cannot be negative." << endl;
            continue;
        }

        return numeric_value;
    }
}

/**
 * Calculate royalties for contract option one.
 *
 * Parameters
 * ----------
 * None
 *
 * Returns
 * -------
 * option_one_royalty : double
 *     The fixed royalty payment for option one.
 *
 * Notes
 * -----
 * Option one pays a fixed amount upon manuscript delivery and an
 * additional fixed amount upon publication.
 */
double calculate_option_one_royalty()
{
    const double DELIVERY_PAYMENT = 5000.00;
    const double PUBLICATION_PAYMENT = 20000.00;

    double option_one_royalty =
        DELIVERY_PAYMENT + PUBLICATION_PAYMENT;

    return option_one_royalty;
}

/**
 * Calculate royalties for contract option two.
 *
 * Parameters
 * ----------
 * net_price_per_copy : double
 *     The net price of one copy of the novel.
 * estimated_copies_sold : int
 *     The estimated number of copies that will be sold.
 *
 * Returns
 * -------
 * option_two_royalty : double
 *     The royalty earned under option two.
 *
 * Notes
 * -----
 * Option two pays 12.5 percent of the net price for every copy sold.
 */
double calculate_option_two_royalty(
    double net_price_per_copy,
    int estimated_copies_sold
)
{
    const double OPTION_TWO_RATE = 0.125;

    double option_two_royalty =
        net_price_per_copy * estimated_copies_sold * OPTION_TWO_RATE;

    return option_two_royalty;
}

/**
 * Calculate royalties for contract option three.
 *
 * Parameters
 * ----------
 * net_price_per_copy : double
 *     The net price of one copy of the novel.
 * estimated_copies_sold : int
 *     The estimated number of copies that will be sold.
 *
 * Returns
 * -------
 * option_three_royalty : double
 *     The royalty earned under option three.
 *
 * Notes
 * -----
 * Option three pays 10 percent of the net price for the first 4,000
 * copies sold and 14 percent for every copy sold after 4,000.
 */
double calculate_option_three_royalty(
    double net_price_per_copy,
    int estimated_copies_sold
)
{
    const int TIER_THRESHOLD = 4000;
    const double FIRST_TIER_RATE = 0.10;
    const double SECOND_TIER_RATE = 0.14;

    double option_three_royalty = 0.0;

    if (estimated_copies_sold <= TIER_THRESHOLD)
    {
        option_three_royalty =
            net_price_per_copy * estimated_copies_sold * FIRST_TIER_RATE;

        return option_three_royalty;
    }

    option_three_royalty =
        (net_price_per_copy * TIER_THRESHOLD * FIRST_TIER_RATE)
        + (net_price_per_copy
        * (estimated_copies_sold - TIER_THRESHOLD)
        * SECOND_TIER_RATE);

    return option_three_royalty;
}

/**
 * Format a decimal value using commas as thousands separators.
 *
 * Parameters
 * ----------
 * numeric_value : double
 *     The value to be converted into a formatted string.
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
 * formatting approach was implemented.
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

    for (int insert_position = static_cast<int>(decimal_position) - 3;
         insert_position > 0;
         insert_position -= 3)
    {
        numeric_text.insert(insert_position, ",");
    }

    return numeric_text;
}

/**
 * Determine which contract option produces the highest royalty.
 *
 * Parameters
 * ----------
 * option_one_royalty : double
 *     The royalty earned under option one.
 * option_two_royalty : double
 *     The royalty earned under option two.
 * option_three_royalty : double
 *     The royalty earned under option three.
 *
 * Returns
 * -------
 * best_option : int
 *     The option number associated with the highest royalty.
 *
 * Notes
 * -----
 * In the event of a tie, the earliest matching option is returned.
 */
int determine_best_option(
    double option_one_royalty,
    double option_two_royalty,
    double option_three_royalty
)
{
    if (option_one_royalty >= option_two_royalty
        && option_one_royalty >= option_three_royalty)
    {
        return 1;
    }

    if (option_two_royalty >= option_one_royalty
        && option_two_royalty >= option_three_royalty)
    {
        return 2;
    }

    return 3;
}

/**
 * Determine the highest royalty value among the three options.
 *
 * Parameters
 * ----------
 * option_one_royalty : double
 *     The royalty earned under option one.
 * option_two_royalty : double
 *     The royalty earned under option two.
 * option_three_royalty : double
 *     The royalty earned under option three.
 *
 * Returns
 * -------
 * best_royalty : double
 *     The largest royalty value among the three options.
 */
double determine_best_royalty(
    double option_one_royalty,
    double option_two_royalty,
    double option_three_royalty
)
{
    if (option_one_royalty >= option_two_royalty
        && option_one_royalty >= option_three_royalty)
    {
        return option_one_royalty;
    }
    else if (option_two_royalty >= option_three_royalty)
    {
        return option_two_royalty;
    }
    else
    {
        return option_three_royalty;
    }
}