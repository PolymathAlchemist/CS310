/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M4 Assignment - Chapter 5, Programming Exercise 33
 * Author: Eric J. Turman
 * Date: 2026-04-07
 *
 * Description:
 * Determines how many dishes Bianca can prepare in a given amount of time
 * when the preparation time for each new dish increases by a fixed number
 * of minutes and reports the result to the user.
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
double get_non_negative_double(const string& prompt_message);

int calculate_number_of_dishes(
    double initial_dish_preparation_time,
    double additional_time_per_dish,
    double total_available_preparation_time
);

string get_plurality_of_dishes(int number_of_dishes_prepared);

bool get_yes_or_enter(const string& prompt_message);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations (prompts, messages)
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "This program calculates how many dishes Bianca can prepare\n"
        "when each new dish takes longer to make than the previous one.";

    const string INTRO_STORY_CHOICE_PROMPT =
        "Would you like the accompanying story (Y) or press \"Enter\" "
        "to continue to the inputs.";

    const string INTRO_STORY_MESSAGE =
        "You bump into your friend Bianca at the grocery store. She has "
        "a worried look on her face. When she sees you she says,\n"
        "\"It is my daughter's birthday today and I am trying to gather "
        "supplies to make her her favorite dishes, \nbut I don't want to buy "
        "too much. Can you please help me figure out how many dishes that "
        "I can manage?\nEach dish I make takes a little longer than "
        "the previous one and I only have so much time to complete them.\"";

    const string INITIAL_DISH_TIME_PROMPT =
        "\nEnter time to prepare the first dish (minutes): ";

    const string TIME_INCREASE_PER_DISH_PROMPT =
        "Enter additional time per dish (minutes): ";

    const string TOTAL_AVAILABLE_TIME_PROMPT =
        "Enter total available time (minutes): ";

    const string REPEAT_PROMPT =
        "Help Bianca with another estimate? (Y) or press Enter to exit: ";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    double initial_dish_preparation_time = 0.0;
    double additional_time_per_dish = 0.0;
    double total_available_preparation_time = 0.0;

    int number_of_dishes_prepared = 0;

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRO_MESSAGE << endl;
    if (get_yes_or_enter(INTRO_STORY_CHOICE_PROMPT))
    {
        cout << "\n" << INTRO_STORY_MESSAGE << endl;
    }

    // --------------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------------
    do
    {
        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        initial_dish_preparation_time =
            get_non_negative_double(INITIAL_DISH_TIME_PROMPT);

        additional_time_per_dish =
            get_non_negative_double(TIME_INCREASE_PER_DISH_PROMPT);

        total_available_preparation_time =
            get_non_negative_double(TOTAL_AVAILABLE_TIME_PROMPT);

        // ----------------------------------------------------------------
        // Processing
        // ----------------------------------------------------------------
        number_of_dishes_prepared = calculate_number_of_dishes(
            initial_dish_preparation_time,
            additional_time_per_dish,
            total_available_preparation_time
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        cout << "\nBianca can prepare "
             << number_of_dishes_prepared << " "
             << get_plurality_of_dishes(number_of_dishes_prepared)
             << " within "
             << total_available_preparation_time
             << " minutes.\n";

    } while (get_yes_or_enter(REPEAT_PROMPT));

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
 *     A validated decimal value that is zero or greater.
 *
 * Notes
 * -----
 * This function reads the entire line of user input before attempting
 * conversion. This allows the program to reject invalid input cleanly,
 * including alphabetic characters, symbols, empty input, or numeric
 * values followed by trailing characters. The function continues
 * prompting until the user provides a valid non-negative decimal value.
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
 * Calculate the number of dishes Bianca can fully prepare within the
 * available amount of time.
 *
 * Parameters
 * ----------
 * initial_dish_preparation_time : double
 *     The number of minutes required to prepare the first dish.
 *
 * additional_time_per_dish : double
 *     The number of additional minutes required for each successive dish
 *     compared with the one before it.
 *
 * total_available_preparation_time : double
 *     The total number of minutes Bianca has available for preparing
 *     dishes.
 *
 * Returns
 * -------
 * number_of_dishes_prepared : int
 *     The total number of complete dishes Bianca can prepare without
 *     exceeding the available time.
 *
 * Notes
 * -----
 * This function adds dish times one at a time, increasing each new dish
 * by the fixed increment.
 *
 * The loop runs only while the next dish fits within the available time.
 * When it no longer fits, the loop ends and the current count is returned.
 */
int calculate_number_of_dishes(
    double initial_dish_preparation_time,
    double additional_time_per_dish,
    double total_available_preparation_time
)
{

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    double cumulative_preparation_time = 0.0;
    double current_dish_preparation_time = initial_dish_preparation_time;

    int number_of_dishes_prepared = 0;

    // --------------------------------------------------------------------
    // Process loop
    // --------------------------------------------------------------------
    while (cumulative_preparation_time + current_dish_preparation_time
        <= total_available_preparation_time)
    {
        cumulative_preparation_time += current_dish_preparation_time;
        number_of_dishes_prepared++;
        current_dish_preparation_time += additional_time_per_dish;
    }

    return number_of_dishes_prepared;
}

/**
 * Determine whether the word "dish" should be singular or plural.
 *
 * Parameters
 * ----------
 * number_of_dishes_prepared : int
 *     The total number of complete dishes Bianca can prepare.
 *
 * Returns
 * -------
 * single_or_plural : string
 *     Returns "dish" when the count is exactly 1.
 *     Returns "dishes" for all other values.
 */
string get_plurality_of_dishes(int number_of_dishes_prepared)
{
    if (number_of_dishes_prepared == 1)
    {
        return "dish";
    }
    else
    {
       return "dishes";
    }
}

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

        cout << "Error: Only 'Y' or pressing Enter is accepted." << endl;
    }
}