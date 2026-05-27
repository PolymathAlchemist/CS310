/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M11 Assignment
 * Source: Chapter 13, Programming Exercise 17
 * File: Ch13Exercise17.cpp
 * Author: Eric J. Turman
 * Date: 2026-05-26
 *
 * Description:
 * This program runs a small Dimensional Stability Scanner demonstration.
 * Each scanner batch generates exactly 25 random real-number dimensional
 * readings between -10,000.0 and 10,000.0 using a Mersenne Twister random
 * number generator.
 *
 * In strict inspection mode, readings below -9,000.0 or above 9,000.0 are
 * treated as unstable. Those readings throw a standard exception, which is
 * caught and reported clearly before the program continues with the next
 * reading.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <cctype>
#include <exception>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

// ========================================================================
// Constants
// ========================================================================
constexpr int READING_COUNT = 25;
constexpr int READING_DISPLAY_PRECISION = 2;
constexpr int READING_NUMBER_WIDTH = 2;
constexpr int READING_VALUE_WIDTH = 12;

constexpr double MINIMUM_READING = -10000.0;
constexpr double MAXIMUM_READING = 10000.0;
constexpr double UNSTABLE_LOW_THRESHOLD = -9000.0;
constexpr double UNSTABLE_HIGH_THRESHOLD = 9000.0;

// ========================================================================
// Function (Prototype) Declarations
// ========================================================================
void display_program_intro();

string format_with_commas(double numeric_value, int decimal_precision);

string trim_text(const string& text_value);

bool get_boolean_choice(
    const string& prompt_message,
    bool enter_key_value
);

mt19937& get_random_engine();

double generate_random_reading();

void inspect_reading_for_instability(
    double reading_value,
    bool strict_inspection_enabled
);

void display_reading_result(
    int reading_number,
    double reading_value
);

void process_scanner_batch(bool strict_inspection_enabled);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations
    // --------------------------------------------------------------------
    const string STRICT_MODE_PROMPT =
        "\nEnable strict inspection mode? [Y] or [N] (Enter = Y): ";

    const string RUN_AGAIN_PROMPT =
        "\nRun another scanner batch? [Y] or press [Enter] to exit: ";

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    display_program_intro();

    // --------------------------------------------------------------------
    // Input, Processing, and Output
    // --------------------------------------------------------------------
    const bool strict_inspection_enabled =
        get_boolean_choice(STRICT_MODE_PROMPT, true);

    do
    {
        process_scanner_batch(strict_inspection_enabled);
    } while (get_boolean_choice(RUN_AGAIN_PROMPT, false));

    cout << "\nScanner powered down cleanly. Dimensional clipboard returned."
         << endl;

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Display a short introduction for the scanner program.
 */
void display_program_intro()
{
    cout << "==========================================================\n"
         << "              DIMENSIONAL STABILITY SCANNER\n"
         << "==========================================================\n"
         << "This program will generate 25 random real-number readings.\n"
         << "If strict inspection mode is enabled, readings\nbelow "
         << format_with_commas(UNSTABLE_LOW_THRESHOLD, 1)
         << "\nor above "
         << format_with_commas(UNSTABLE_HIGH_THRESHOLD, 1)
         << "\nwill trigger a caught exception.\n"
         << "Caught exceptions are expected behavior and\ndemonstrate "
         << "exception management.\n"
         << "The program continues after each caught exception.\n"
         << "----------------------------------------------------------" << endl;
}

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
 * Trim leading and trailing whitespace from a string.
 *
 * Parameters
 * ----------
 * text_value : const string&
 *     The text value to trim.
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
 * Prompt for a yes or no choice.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before reading input.
 *
 * enter_key_value : bool
 *     The value returned when the user presses Enter.
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
            const char user_choice = static_cast<char>(toupper(
                static_cast<unsigned char>(input_text[0])
            ));

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
 * Provide access to a shared pseudo-random number generator.
 *
 * Returns
 * -------
 * random_engine : mt19937&
 *     The Mersenne Twister engine reused for scanner readings.
 */
mt19937& get_random_engine()
{
    static random_device random_device_source;
    static mt19937 random_engine(random_device_source());

    return random_engine;
}

/**
 * Generate one random dimensional reading.
 *
 * Returns
 * -------
 * reading_value : double
 *     A random real-number reading within the configured range.
 */
double generate_random_reading()
{
    uniform_real_distribution<double> distribution(
        MINIMUM_READING,
        MAXIMUM_READING
    );

    return distribution(get_random_engine());
}

/**
 * Inspect a reading and throw an exception when strict mode finds instability.
 *
 * Parameters
 * ----------
 * reading_value : double
 *     The dimensional reading to inspect.
 *
 * strict_inspection_enabled : bool
 *     Determines whether unstable outer-band readings are rejected.
 */
void inspect_reading_for_instability(
    double reading_value,
    bool strict_inspection_enabled
)
{
    if (!strict_inspection_enabled)
    {
        return;
    }

    if (reading_value < UNSTABLE_LOW_THRESHOLD
        || reading_value > UNSTABLE_HIGH_THRESHOLD)
    {
        throw runtime_error(
            "outer safety band instability detected"
        );
    }
}

/**
 * Display one formatted scanner reading.
 *
 * Parameters
 * ----------
 * reading_number : int
 *     The one-based number of the reading in the current batch.
 *
 * reading_value : double
 *     The reading value to display.
 */
void display_reading_result(
    int reading_number,
    double reading_value
)
{
    cout << "Reading " << setw(READING_NUMBER_WIDTH) << reading_number
         << ": "
         << setw(READING_VALUE_WIDTH)
         << format_with_commas(reading_value, READING_DISPLAY_PRECISION);
}

/**
 * Generate and inspect exactly one scanner batch.
 *
 * Parameters
 * ----------
 * strict_inspection_enabled : bool
 *     Determines whether outer-band readings throw caught exceptions.
 */
void process_scanner_batch(bool strict_inspection_enabled)
{
    cout << "\nScanner batch started. Strict inspection mode is "
         << (strict_inspection_enabled ? "ON" : "OFF") << "." << endl;

    for (int reading_number = 1;
        reading_number <= READING_COUNT;
        reading_number++)
    {
        const double reading_value = generate_random_reading();

        display_reading_result(reading_number, reading_value);

        try
        {
            inspect_reading_for_instability(
                reading_value,
                strict_inspection_enabled
            );

            cout << "  Stable" << endl;
        }
        catch (const exception& scanner_exception)
        {
            cout << "  Warning: caught exception - "
                 << scanner_exception.what() << "." << endl;
        }
    }

    cout << "Scanner batch complete. All 25 readings were processed." << endl;
}
