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

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
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
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations (fixed values, prompts, messages)
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "After saying goodbye to 'Tim the Financial Enchanter' you come "
        "across a curious clerk sitting at a desk in the middle of\n"
        "an open field. He looks up from his ledger and states, \"Well, "
        "let's have a look at it.\" gesturing to a manuscript that\n"
        "you are suddenly aware of holding. You hand it over, and after "
        "quickly flipping through the pages, he looks back up at\n"
        "you. \"I have a couple of questions for you, and I'll recommend the "
        "best option for you that yields the highest return.\"\n";
        "He hands you a paper decribing three optinos:\n\n"
        ""

    const string NET_PRICE_PROMPT =
        "What price will you sell each copy of the novel: $";

    const string COPIES_SOLD_PROMPT =
        "What is estimated number of copies that will be sold: ";

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
         << net_price_per_copy
         << " and estimated sales of "
         << estimated_copies_sold
         << " copies:\n\n";

    cout << "Option 1: $" << option_one_royalty << endl;
    cout << "Option 2: $" << option_two_royalty << endl;
    cout << "Option 3: $" << option_three_royalty << endl;

    cout << "\nBest option: Option "
         << best_option
         << " with estimated royalties of $"
         << best_royalty
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

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
 */
int get_non_negative_integer(const string& prompt_message)
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

        if (numeric_value < 0)
        {
            cout << "Error: Value cannot be negative." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Check for leftover input (like decimals)
        if (cin.peek() != '\n')
        {
            cout << "Error: Decimal values are not allowed." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    double best_royalty = option_one_royalty;

    if (option_two_royalty > best_royalty)
    {
        best_royalty = option_two_royalty;
    }

    if (option_three_royalty > best_royalty)
    {
        best_royalty = option_three_royalty;
    }

    return best_royalty;
}