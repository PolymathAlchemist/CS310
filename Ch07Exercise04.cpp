/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M6 Assignment (Ch07Exercise04)
 * Author: Eric J. Turman
 * Date: 2026-04-23
 *
 * Description:
 * A simple program that prompts the user for a line of text, removes all
 * vowels from that text using substr(), and outputs the resulting string.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

// ========================================================================
// Function (Prototype) Declarations
// ========================================================================
bool is_vowel(char character_to_check);

string remove_vowels(const string& original_text);

bool get_boolean_choice(
    const string& prompt_message,
    const string& true_association,
    const string& false_association
);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations
    // --------------------------------------------------------------------
    const string INPUT_PROMPT = "Enter a string: ";

    const string AGAIN_PROMPT =
        "Enter another string? (Y) or enter to Exit... ";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    string original_text;
    string text_without_vowels;

    do {
        // --------------------------------------------------------------------
        // Input
        // --------------------------------------------------------------------
        cout << INPUT_PROMPT;
        getline(cin, original_text);

        // --------------------------------------------------------------------
        // Processing
        // --------------------------------------------------------------------
        text_without_vowels = remove_vowels(original_text);

        // --------------------------------------------------------------------
        // Output
        // --------------------------------------------------------------------
        cout << "\nString without vowels: " << text_without_vowels << endl;

    } while (get_boolean_choice(AGAIN_PROMPT, "Y",  ""));

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Determine whether a character is a vowel.
 *
 * Parameters
 * ----------
 * character_to_check : char
 *     The character being evaluated.
 *
 * Returns
 * -------
 * is_character_vowel : bool
 *     Returns true if the character is a vowel.
 *     Returns false if the character is not a vowel.
 *
 * Notes
 * -----
 * The character is converted to lowercase before comparison so uppercase
 * and lowercase vowels are treated consistently.
 */
bool is_vowel(char character_to_check)
{
    const string VOWELS = "aeiou";

    char lowercase_character = static_cast<char>(
        tolower(static_cast<unsigned char>(character_to_check))
    );

    return VOWELS.find(lowercase_character) != string::npos;
}

/**
 * Remove all vowels from a string.
 *
 * Parameters
 * ----------
 * original_text : const string&
 *     The original text entered by the user.
 *
 * Returns
 * -------
 * text_without_vowels : string
 *     A copy of the original text with all vowels removed.
 *
 * Notes
 * -----
 * This function uses substr() when copying non-vowel characters into the
 * result string to satisfy the assignment requirement.
 */
string remove_vowels(const string& original_text)
{
    string text_without_vowels;

    for (size_t character_index = 0;
         character_index < original_text.length();
         ++character_index)
    {
        if (!is_vowel(original_text[character_index]))
        {
            text_without_vowels += original_text.substr(character_index, 1);
        }
    }

    return text_without_vowels;
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
 */
bool get_boolean_choice(
    const string& prompt_message,
    const string& true_association,
    const string& false_association
)
{
    string user_input;
    string normalized_input;
    string normalized_true_association = true_association;
    string normalized_false_association = false_association;

    for (char& character : normalized_true_association)
    {
        character = static_cast<char>(toupper(
            static_cast<unsigned char>(character)
        ));
    }

    for (char& character : normalized_false_association)
    {
        character = static_cast<char>(toupper(
            static_cast<unsigned char>(character)
        ));
    }

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        normalized_input = user_input;

        for (char& character : normalized_input)
        {
            character = static_cast<char>(toupper(
                static_cast<unsigned char>(character)
            ));
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
