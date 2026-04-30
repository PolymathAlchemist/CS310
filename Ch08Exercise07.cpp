/**
* ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M7 Assignment
 * Author: Eric J. Turman
 * Date: 2026-04-27
 *
 * Description:
 * This program allows the user to enter the last names of five local
 * election candidates and the number of votes received by each candidate.
 * It then displays each candidate's vote total, percentage of the overall
 * vote, and the winner of the election.
 *
 * Notes:
 * The program uses parallel arrays because the chapter exercise focuses on
 * array-based data organization. Structs or classes would be a cleaner way
 * to group each candidate's name and vote count, but arrays match the
 * current assignment requirements and chapter focus.
 *
 * This version extends the textbook exercise with defensive input validation
 * for candidate names and vote counts while preserving the original program
 * requirements. It also uses dynamic table sizing so candidate names, vote
 * totals, and percentages remain readable across a wider range of valid
 * input.
 *
 * Very small non-zero percentages are displayed as "< 0.01%" rather than
 * "0.00%" so the output does not imply a candidate received no measurable
 * share of the vote.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <cctype>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

using namespace std;

// ========================================================================
// Constants
// ========================================================================
constexpr int CANDIDATE_COUNT = 5;
constexpr int MINIMUM_VOTES = 0;
constexpr int MAXIMUM_VOTES = 1000000000;
constexpr int PERCENTAGE_PRECISION = 2;
constexpr int MINIMUM_CANDIDATE_NAME_WIDTH = 20;
constexpr int MAXIMUM_CANDIDATE_NAME_WIDTH = 30;
constexpr int VOTE_COUNT_WIDTH = 15;
constexpr int PERCENTAGE_WIDTH = 10;
const string ORDINAL_WORDS[CANDIDATE_COUNT] =
{
    "first",
    "second",
    "third",
    "fourth",
    "fifth"
};

// ========================================================================
// Function (Prototype) Declarations
// ========================================================================
string format_with_commas(double numeric_value, int decimal_precision);

string format_vote_percentage(int candidate_votes, int total_votes);

int get_display_candidate_name_width(
    const string candidate_names[],
    int candidate_count
);

string format_candidate_name_for_table(
    const string& candidate_name,
    int display_width
);

string trim_text(const string& text_value);

string get_candidate_last_name(const string& input_message);

bool names_match_case_insensitively(
    const string& first_name,
    const string& second_name
);

bool get_boolean_choice(
    const string& prompt_message,
    const string& true_association,
    const string& false_association
);

int get_bounded_int(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
);

void collect_election_data(
    string candidate_names[],
    int candidate_votes[],
    int candidate_count
);

int calculate_total_votes(const int candidate_votes[], int candidate_count);

double calculate_vote_percentage(int candidate_votes, int total_votes);

int find_highest_vote_count(const int candidate_votes[], int candidate_count);

int count_candidates_with_votes(
    const int candidate_votes[],
    int candidate_count,
    int target_vote_count
);

void display_election_results(
    const string candidate_names[],
    const int candidate_votes[],
    int candidate_count,
    int total_votes
);

void display_winner(
    const string candidate_names[],
    const int candidate_votes[],
    int candidate_count
);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations
    // --------------------------------------------------------------------
    const string INTRODUCTION_MESSAGE =
        "======================================================\n"
        "          LOCAL ELECTION RESULTS CALCULATOR\n"
        "======================================================\n"
        "Enter Last names and vote counts and this program will\n"
        "calculate each candidates percentage of votes and\n"
        "provide the total number of votes received.\n"
        "------------------------------------------------------\n";

    const string RUN_AGAIN_PROMPT =
        "\nRun another election? [Y] or press [Enter] to exit: ";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    string candidate_names[CANDIDATE_COUNT];
    int candidate_votes[CANDIDATE_COUNT] = {};

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRODUCTION_MESSAGE << endl;

    // --------------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------------
    do
    {
        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        for (int index = 0; index < CANDIDATE_COUNT; index++)
        {
            candidate_names[index] = "";
            candidate_votes[index] = 0;
        }

        collect_election_data(
            candidate_names,
            candidate_votes,
            CANDIDATE_COUNT
        );

        // ----------------------------------------------------------------
        // Processing
        // ----------------------------------------------------------------
        const int total_votes = calculate_total_votes(
            candidate_votes,
            CANDIDATE_COUNT
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        display_election_results(
            candidate_names,
            candidate_votes,
            CANDIDATE_COUNT,
            total_votes
        );

        display_winner(
            candidate_names,
            candidate_votes,
            CANDIDATE_COUNT
        );
    } while (get_boolean_choice(RUN_AGAIN_PROMPT, "Y", ""));

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
 *     The value to be converted into a formatted string.
 *
 * decimal_precision : int
 *     Used to set the decimal precision.
 *
 * Returns
 * -------
 * numeric_text : string
 *     A string formatted with comma separators and the requested precision.
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
 * Format a candidate's vote share for table display.
 *
 * Parameters
 * ----------
 * candidate_votes : int
 *     Number of votes received by one candidate.
 *
 * total_votes : int
 *     Number of votes received by all candidates.
 *
 * Returns
 * -------
 * percentage_text : string
 *     The candidate's vote share formatted for display.
 *
 * Notes
 * -----
 * Very small nonzero percentages should not appear as 0.00% because that
 * hides the fact that the candidate did receive votes.
 */
string format_vote_percentage(int candidate_votes, int total_votes)
{
    if (candidate_votes == 0 || total_votes == 0)
    {
        return "0.00%";
    }

    const double vote_percentage = calculate_vote_percentage(
        candidate_votes,
        total_votes
    );

    if (vote_percentage > 0.0 && vote_percentage < 0.01)
    {
        return "<0.01%";
    }

    stringstream percentage_stream;
    percentage_stream << fixed
                      << setprecision(PERCENTAGE_PRECISION)
                      << vote_percentage
                      << "%";

    return percentage_stream.str();
}

/**
 * Determine the candidate-name column width from the report data.
 *
 * Parameters
 * ----------
 * candidate_names : const string[]
 *     Array containing candidate last names.
 *
 * candidate_count : int
 *     Number of candidate names stored in the array.
 *
 * Returns
 * -------
 * display_width : int
 *     Candidate-name column width constrained to readable limits.
 */
int get_display_candidate_name_width(
    const string candidate_names[],
    int candidate_count
)
{
    int longest_name_length = static_cast<int>(string("Candidate").length());

    for (int index = 0; index < candidate_count; index++)
    {
        const int candidate_name_length = static_cast<int>(
            candidate_names[index].length()
        );

        if (candidate_name_length > longest_name_length)
        {
            longest_name_length = candidate_name_length;
        }
    }

    if (longest_name_length < MINIMUM_CANDIDATE_NAME_WIDTH)
    {
        return MINIMUM_CANDIDATE_NAME_WIDTH;
    }

    if (longest_name_length > MAXIMUM_CANDIDATE_NAME_WIDTH)
    {
        return MAXIMUM_CANDIDATE_NAME_WIDTH;
    }

    return longest_name_length;
}

/**
 * Prepare a candidate name for the fixed-width results table.
 *
 * Parameters
 * ----------
 * candidate_name : const string&
 *     The full candidate name stored for the report.
 *
 * display_width : int
 *     Maximum width available in the table column.
 *
 * Returns
 * -------
 * table_name : string
 *     Candidate name shortened only when needed to protect table layout.
 *
 * Notes
 * -----
 * Truncation is limited to the table so the full stored name remains
 * available anywhere precision matters, such as winner reporting.
 */
string format_candidate_name_for_table(
    const string& candidate_name,
    int display_width
)
{
    if (static_cast<int>(candidate_name.length()) <= display_width)
    {
        return candidate_name;
    }

    if (display_width <= 3)
    {
        return string(display_width, '.');
    }

    return candidate_name.substr(0, display_width - 3) + "...";
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
 * Prompt for a candidate last name using realistic election-style rules.
 *
 * Parameters
 * ----------
 * input_message : const string&
 *     The message displayed before input is read.
 *
 * Returns
 * -------
 * candidate_last_name : string
 *     A validated candidate last name.
 *
 * Notes
 * -----
 * The validation stays specific to candidate last names so the program can
 * accept common real-world forms such as O'Connor, McDonald-Douglas,
 * St. John, de la Cruz, and van der Meer while rejecting obvious junk
 * input. The regular expression validates each space-separated segment so
 * repeated separators and trailing punctuation are rejected without forcing
 * the user into one capitalization style.
 */
string get_candidate_last_name(const string& input_message)
{
    const regex name_segment_pattern(
        R"(^[A-Za-z]+(?:['-][A-Za-z]+)*\.?$)"
    );

    string candidate_last_name;

    while (true)
    {
        cout << input_message;
        getline(cin, candidate_last_name);

        candidate_last_name = trim_text(candidate_last_name);

        if (candidate_last_name.empty())
        {
            cout << "Error: Entry cannot be blank." << endl;
            continue;
        }

        if (candidate_last_name.find("  ") != string::npos)
        {
            cout << "Error: Use only one space between name parts.\n";
            continue;
        }

        bool contains_digit = false;
        bool contains_invalid_symbol = false;

        for (char character : candidate_last_name)
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
            cout << "Error: Last names cannot contain digits." << endl;
            continue;
        }

        if (contains_invalid_symbol)
        {
            cout << "Error: Last names can use only letters, apostrophes, "
                 << "hyphens, periods, and single spaces."
                 << endl;
            continue;
        }

        bool has_structure_error = false;
        bool previous_segment_used_period = false;
        stringstream name_stream(candidate_last_name);
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
            return candidate_last_name;
        }

        cout << "Error: Enter a valid last name such as McDonald, "
             << "O'Connor, St. John, de la Cruz, or van der Meer."
             << endl;
    }
}

/**
 * Compare two candidate last names without treating capitalization as
 * meaningful.
 *
 * Parameters
 * ----------
 * first_name : const string&
 *     The first candidate last name being compared.
 *
 * second_name : const string&
 *     The second candidate last name being compared.
 *
 * Returns
 * -------
 * names_match : bool
 *     True when both names are the same aside from capitalization.
 */
bool names_match_case_insensitively(
    const string& first_name,
    const string& second_name
)
{
    if (first_name.length() != second_name.length())
    {
        return false;
    }

    for (size_t index = 0; index < first_name.length(); index++)
    {
        if (tolower(static_cast<unsigned char>(first_name[index])) !=
            tolower(static_cast<unsigned char>(second_name[index])))
        {
            return false;
        }
    }

    return true;
}

/**
 * Prompt for a yes-or-no style choice using two allowed text responses.
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
 * Refinement: Handles empty input (Enter) by displaying it as "[Enter]"
 * so error messages remain clear. Uses ternary conditionals to keep this
 * concise and consistent with other input options.
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

        string true_display =
            true_association.empty() ? "[Enter]" : "[" + true_association + "]";

        string false_display =
            false_association.empty() ? "[Enter]" : "[" + false_association + "]";

        cout << "Error: Please enter "
             << true_display
             << " or "
             << false_display
             << "."
             << endl;
    }
}

/**
 * Prompt for an integer value within an allowed range.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The message displayed before input is read.
 *
 * minimum_value : int
 *     The smallest acceptable value, inclusive.
 *
 * maximum_value : int
 *     The largest acceptable value, inclusive.
 *
 * Returns
 * -------
 * numeric_value : int
 *     A validated integer value within the requested bounds.
 */
int get_bounded_int(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
)
{
    string user_input;
    int numeric_value = 0;
    char extra_character = '\0';

    while (true)
    {
        cout << prompt_message;
        getline(cin, user_input);

        stringstream input_stream(user_input);

        if (input_stream >> numeric_value &&
            !(input_stream >> extra_character) &&
            numeric_value >= minimum_value &&
            numeric_value <= maximum_value)
        {
            return numeric_value;
        }

        cout << "Error: Please enter a whole number from "
             << minimum_value
             << " to "
             << maximum_value
             << "."
             << endl;
    }
}

/**
 * Collect each candidate's name and vote count from the user.
 *
 * Parameters
 * ----------
 * candidate_names : string[]
 *     Array that stores candidate last names.
 *
 * candidate_votes : int[]
 *     Array that stores the vote count for each candidate.
 *
 * candidate_count : int
 *     Number of candidates to collect.
 *
 * Returns
 * -------
 * None
 */
void collect_election_data(
    string candidate_names[],
    int candidate_votes[],
    int candidate_count
)
{
    for (int index = 0; index < candidate_count; index++)
    {
        cout << endl;

        while (true)
        {
            candidate_names[index] = get_candidate_last_name(
                "What is the " + ORDINAL_WORDS[index] +
                " candidate's last name? "
            );

            bool duplicate_name_found = false;

            for (int previous_index = 0;
                previous_index < index;
                previous_index++)
            {
                if (names_match_case_insensitively(
                    candidate_names[index],
                    candidate_names[previous_index]))
                {
                    duplicate_name_found = true;
                    break;
                }
            }

            if (!duplicate_name_found)
            {
                break;
            }

            // Duplicate last names make the final vote report ambiguous
            // because the same label could refer to more than one person.
            cout << "Error: Candidate names must be unique for this report."
                 << endl;
        }

        candidate_votes[index] = get_bounded_int(
            "How many votes did " + candidate_names[index] + " receive? ",
            MINIMUM_VOTES,
            MAXIMUM_VOTES
        );
    }
}

/**
 * Calculate the total number of votes entered.
 *
 * Parameters
 * ----------
 * candidate_votes : const int[]
 *     Array containing each candidate's vote count.
 *
 * candidate_count : int
 *     Number of candidates stored in the vote array.
 *
 * Returns
 * -------
 * total_votes : int
 *     Sum of all candidate vote counts.
 */
int calculate_total_votes(const int candidate_votes[], int candidate_count)
{
    int total_votes = 0;

    for (int index = 0; index < candidate_count; index++)
    {
        total_votes += candidate_votes[index];
    }

    return total_votes;
}

/**
 * Calculate one candidate's percentage of the total vote.
 *
 * Parameters
 * ----------
 * candidate_votes : int
 *     Number of votes received by one candidate.
 *
 * total_votes : int
 *     Number of votes received by all candidates.
 *
 * Returns
 * -------
 * vote_percentage : double
 *     Candidate's percentage of the total vote. Returns 0.0 when the total
 *     vote count is zero to avoid division by zero.
 */
double calculate_vote_percentage(int candidate_votes, int total_votes)
{
    if (total_votes == 0)
    {
        return 0.0;
    }

    return (static_cast<double>(candidate_votes) / total_votes) * 100.0;
}

/**
 * Find the highest vote count in the candidate vote array.
 *
 * Parameters
 * ----------
 * candidate_votes : const int[]
 *     Array containing each candidate's vote count.
 *
 * candidate_count : int
 *     Number of candidates stored in the vote array.
 *
 * Returns
 * -------
 * highest_vote_count : int
 *     The largest vote count found in the array.
 */
int find_highest_vote_count(const int candidate_votes[], int candidate_count)
{
    int highest_vote_count = candidate_votes[0];

    for (int index = 1; index < candidate_count; index++)
    {
        if (candidate_votes[index] > highest_vote_count)
        {
            highest_vote_count = candidate_votes[index];
        }
    }

    return highest_vote_count;
}

/**
 * Count how many candidates received a target number of votes.
 *
 * Parameters
 * ----------
 * candidate_votes : const int[]
 *     Array containing each candidate's vote count.
 *
 * candidate_count : int
 *     Number of candidates stored in the vote array.
 *
 * target_vote_count : int
 *     Vote count being searched for.
 *
 * Returns
 * -------
 * matching_candidate_count : int
 *     Number of candidates whose vote count matches the target.
 */
int count_candidates_with_votes(
    const int candidate_votes[],
    int candidate_count,
    int target_vote_count
)
{
    int matching_candidate_count = 0;

    for (int index = 0; index < candidate_count; index++)
    {
        if (candidate_votes[index] == target_vote_count)
        {
            matching_candidate_count++;
        }
    }

    return matching_candidate_count;
}

/**
 * Display each candidate's vote count and vote percentage.
 *
 * Parameters
 * ----------
 * candidate_names : const string[]
 *     Array containing candidate last names.
 *
 * candidate_votes : const int[]
 *     Array containing each candidate's vote count.
 *
 * candidate_count : int
 *     Number of candidates to display.
 *
 * total_votes : int
 *     Sum of all candidate vote counts.
 *
 * Returns
 * -------
 * None
 */
void display_election_results(
    const string candidate_names[],
    const int candidate_votes[],
    int candidate_count,
    int total_votes
)
{
    const int candidate_name_width = get_display_candidate_name_width(
        candidate_names,
        candidate_count
    );
    const int table_width =
        candidate_name_width +
        VOTE_COUNT_WIDTH +
        PERCENTAGE_WIDTH;

    cout << endl;
    cout << left << setw(candidate_name_width) << "Candidate"
         << right << setw(VOTE_COUNT_WIDTH) << "Votes"
         << setw(PERCENTAGE_WIDTH) << "Percent"
         << endl;
    cout << string(table_width, '-') << endl;

    for (int index = 0; index < candidate_count; index++)
    {
        cout << left << setw(candidate_name_width)
             << format_candidate_name_for_table(
                 candidate_names[index],
                 candidate_name_width
             )
             << right << setw(VOTE_COUNT_WIDTH)
             << format_with_commas(candidate_votes[index], 0)
             << setw(PERCENTAGE_WIDTH)
             << format_vote_percentage(candidate_votes[index], total_votes)
             << endl;
    }

    cout << string(table_width, '-') << endl;
    cout << left << setw(candidate_name_width) << "Total"
         << right << setw(VOTE_COUNT_WIDTH) << format_with_commas(total_votes, 0)
         << endl;
}

/**
 * Display the election winner or tie result.
 *
 * Parameters
 * ----------
 * candidate_names : const string[]
 *     Array containing candidate last names.
 *
 * candidate_votes : const int[]
 *     Array containing each candidate's vote count.
 *
 * candidate_count : int
 *     Number of candidates stored in the arrays.
 *
 * Returns
 * -------
 * None
 */
void display_winner(
    const string candidate_names[],
    const int candidate_votes[],
    int candidate_count
)
{
    const int highest_vote_count = find_highest_vote_count(
        candidate_votes,
        candidate_count
    );

    const int winner_count = count_candidates_with_votes(
        candidate_votes,
        candidate_count,
        highest_vote_count
    );

    cout << endl;

    if (highest_vote_count == 0)
    {
        cout << "No winner can be determined because no votes were entered."
             << endl;
        return;
    }

    if (winner_count == 1)
    {
        for (int index = 0; index < candidate_count; index++)
        {
            if (candidate_votes[index] == highest_vote_count)
            {
                cout << "The winner of the election is "
                     << candidate_names[index]
                     << "."
                     << endl;
                return;
            }
        }
    }

    cout << "The election ended in a tie between: ";

    bool first_candidate_displayed = true;

    for (int index = 0; index < candidate_count; index++)
    {
        if (candidate_votes[index] == highest_vote_count)
        {
            if (!first_candidate_displayed)
            {
                cout << ", ";
            }

            cout << candidate_names[index];
            first_candidate_displayed = false;
        }
    }

    cout << "." << endl;
}
