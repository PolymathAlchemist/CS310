/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Assignment: M10 Assignment, Chapter 12 Programming Exercise 3
 * Author: Eric J. Turman
 * Date: 2026-05-26
 *
 * Description:
 * This program asks for the number of local election candidates, stores
 * candidate names and vote totals in raw dynamic arrays, and displays each
 * candidate's vote count, vote percentage, and the winner.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// ========================================================================
// Constants
// ========================================================================
constexpr int MINIMUM_CANDIDATES = 1;
constexpr int MAXIMUM_CANDIDATES = 100;
constexpr int MINIMUM_VOTES = 0;
constexpr int CANDIDATE_NAME_WIDTH = 20;
constexpr int VOTE_COUNT_WIDTH = 12;
constexpr int PERCENTAGE_WIDTH = 12;
constexpr int PERCENTAGE_PRECISION = 2;

// ========================================================================
// Function (Prototype) Declarations
// ========================================================================
int get_valid_integer(const string& prompt_message, int minimum_value,
    int maximum_value);
int get_total_votes(const int votes[], int candidate_count);
int find_winner_index(const int votes[], int candidate_count);
void display_results(const string candidate_names[],
    const int candidate_votes[], int candidate_count);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    int candidate_count = 0;
    string* candidate_names = nullptr;
    int* candidate_votes = nullptr;

    cout << "======================================================\n";
    cout << "          LOCAL ELECTION RESULTS CALCULATOR\n";
    cout << "======================================================\n";

    candidate_count = get_valid_integer(
        "How many candidates are in the election? ",
        MINIMUM_CANDIDATES,
        MAXIMUM_CANDIDATES
    );

    candidate_names = new string[candidate_count];
    candidate_votes = new int[candidate_count];

    for (int candidate_index = 0; candidate_index < candidate_count;
        candidate_index++)
    {
        cout << endl;
        cout << "Enter candidate " << candidate_index + 1
             << "'s last name: ";
        cin >> candidate_names[candidate_index];

        candidate_votes[candidate_index] = get_valid_integer(
            "Enter votes received by " + candidate_names[candidate_index] + ": ",
            MINIMUM_VOTES,
            numeric_limits<int>::max()
        );
    }

    display_results(candidate_names, candidate_votes, candidate_count);

    delete[] candidate_names;
    delete[] candidate_votes;

    cout << "\nPress any key to exit...";
    getch();

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Prompt for an integer within the supplied inclusive range.
 */
int get_valid_integer(const string& prompt_message, int minimum_value,
    int maximum_value)
{
    int user_number = 0;

    cout << prompt_message;
    cin >> user_number;

    while (cin.fail() || user_number < minimum_value ||
        user_number > maximum_value)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Please enter a whole number from "
             << minimum_value
             << " to "
             << maximum_value
             << ": ";
        cin >> user_number;
    }

    return user_number;
}

/**
 * Add all vote totals stored in the vote array.
 */
int get_total_votes(const int votes[], int candidate_count)
{
    int total_votes = 0;

    for (int candidate_index = 0; candidate_index < candidate_count;
        candidate_index++)
    {
        total_votes += votes[candidate_index];
    }

    return total_votes;
}

/**
 * Return the array index of the candidate with the most votes.
 */
int find_winner_index(const int votes[], int candidate_count)
{
    int winner_index = 0;

    for (int candidate_index = 1; candidate_index < candidate_count;
        candidate_index++)
    {
        if (votes[candidate_index] > votes[winner_index])
        {
            winner_index = candidate_index;
        }
    }

    return winner_index;
}

/**
 * Display the election report and winning candidate.
 */
void display_results(const string candidate_names[],
    const int candidate_votes[], int candidate_count)
{
    const int total_votes = get_total_votes(candidate_votes, candidate_count);
    const int winner_index = find_winner_index(candidate_votes,
        candidate_count);

    cout << endl;
    cout << left << setw(CANDIDATE_NAME_WIDTH) << "Candidate"
         << right << setw(VOTE_COUNT_WIDTH) << "Votes"
         << setw(PERCENTAGE_WIDTH) << "Percent"
         << endl;
    cout << "--------------------------------------------" << endl;

    cout << fixed << setprecision(PERCENTAGE_PRECISION);

    for (int candidate_index = 0; candidate_index < candidate_count;
        candidate_index++)
    {
        double vote_percentage = 0.0;

        if (total_votes > 0)
        {
            vote_percentage =
                (static_cast<double>(candidate_votes[candidate_index]) /
                    total_votes) * 100.0;
        }

        cout << left << setw(CANDIDATE_NAME_WIDTH) << candidate_names[candidate_index]
             << right << setw(VOTE_COUNT_WIDTH)
             << candidate_votes[candidate_index]
             << setw(PERCENTAGE_WIDTH - 1) << vote_percentage
             << "%"
             << endl;
    }

    cout << "--------------------------------------------" << endl;
    cout << left << setw(CANDIDATE_NAME_WIDTH) << "Total"
         << right << setw(VOTE_COUNT_WIDTH) << total_votes
         << endl;

    cout << endl;
    cout << "The winner of the election is "
         << candidate_names[winner_index]
         << "."
         << endl;
}
