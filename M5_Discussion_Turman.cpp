/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M5 Discussion
 * Author: Eric J. Turman
 * Date: 2026-04-11
 *
 * Description:
 * A simple program that uses user-defined functions to calculate how many
 * sheep-sized snacks a dragon needs for a treasure expedition based on the
 * distance to the treasure, time to load the treasure, the amount of gold
 * carried home, and the dragon's need to stop and eat at regular flight
 * intervals.
 *
 * Notes:
 * This code will not compile and includes intentionally introduced errors:
 * * 5 syntax errors
 * * 3 logic errors
 * * 1 documentation mismatch
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
double get_bounded_double(
    const string& prompt_message,
    double minimum_value,
    double maximum_value
);

bool get_yes_or_enter(const string& prompt_message);

void build_story_message(
    double unloaded_speed,
    double half_speed_weight,
    int seconds_per_snack,
    int seconds_per_break,
    double maximum_distance_miles,
    double maximum_gold_weight_pounds
);

double calculate_loaded_speed(
    double unloaded_speed_mph,
    double gold_weight_pounds,
    double load_scaling_factor
);

double calculate_flight_time_seconds(
    double travel_distance_miles,
    double flight_speed_mph
);

double calculate_number_of_snacks(
    double total_flight_time_seconds,
    int seconds_per_snack
);

double calculate_total_break_time_seconds(
    int number_of_snacks,
    int seconds_per_snack_break
);

double calculate_loading_time_seconds(double gold_weight_pounds);

string get_plurality(
    int quantity,
    const string& singular_word,
    const string& plural_word
);

string format_with_commas(double numeric_value, int decimal_precision);

string format_time_wdhms(double total_seconds);

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    // --------------------------------------------------------------------
    // Constant declarations
    // --------------------------------------------------------------------
    const string INTRO_MESSAGE =
        "This program calculates how many sheep-sized snacks a dragon\n"
        "needs for a treasure expedition.";

    const string STORY_PROMPT =
        "Would you like the expedition briefing (Y) or press Enter to "
        "continue: ";

    const string DISTANCE_TO_TREASURE_PROMPT =
        "\nEnter one-way distance to the treasure (miles): ";

    const string GOLD_WEIGHT_PROMPT =
        "Enter amount of gold to carry home (pounds): ";

    const string REPEAT_PROMPT =
        "\nPlan another treasure expedition? (Y) or press Enter to exit: ";

    const double UNLOADED_SPEED_MPH = 120.0;
    const double HALF_SPEED_WEIGHT = 500.0;
    const int SECONDS_PER_SNACK = 2700;
    const int SECONDS_PER_SNACK_BREAK = 900;
    const double MAXIMUM_DISTANCE_MILES = 12444.0;
    const double MAXIMUM_GOLD_WEIGHT_POUNDS = 20000.0;

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------
    double distance_to_treasure_miles = 0.0;
    double gold_weight_pounds = 0.0;

    double outbound_flight_time_seconds = 0.0;
    double return_flight_time_seconds = 0.0;
    double total_flight_time_seconds = 0.0;

    double outbound_break_time_seconds = 0.0;
    double return_break_time_seconds = 0.0;
    double total_break_time_seconds = 0.0;

    double loading_time_seconds = 0.0;
    double loaded_speed_mph = 0.0;
    double load_scaling_factor = 0.0;


    double outbound_total_time_seconds = 0.0;
    double return_total_time_seconds = 0.0;
    double total_trip_time_seconds = 0.0;

    int outbound_snacks = 0;
    int return_snacks = 0;
    int number_of_snacks = 0;

    string story_message = build_story_message(
        UNLOADED_SPEED_MPH,
        HALF_SPEED_WEIGHT,
        SECONDS_PER_SNACK,
        SECONDS_PER_SNACK_BREAK,
        MAXIMUM_DISTANCE_MILES,
        MAXIMUM_GOLD_WEIGHT_POUNDS
    );

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRO_MESSAGE << endl;

    if (get_yes_or_enter(STORY_PROMPT))
    {
        cout << "\n" << story_message << endl;
    }

    // --------------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------------
    do
    {
        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        distance_to_treasure_miles = get_bounded_double(
            DISTANCE_TO_TREASURE_PROMPT,
            0.0,
            MAXIMUM_DISTANCE_MILES
        );

        gold_weight_pounds = get_bounded_double(
            GOLD_WEIGHT_PROMPT,
            MAXIMUM_GOLD_WEIGHT_POUNDS,
            0.0
        );

        // ----------------------------------------------------------------
        // Processing
        // ----------------------------------------------------------------

        outbound_flight_time_seconds = calculate_flight_time_seconds(
                      distance_to_treasure_miles,
                      UNLOADED_SPEED_MPH
        );


        outbound_snacks = calculate_number_of_snacks(
                      outbound_flight_time_seconds,
                      SECONDS_PER_SNACK
        );

        outbound_break_time_seconds = calculate_total_break_time_seconds(
                      outbound_snacks,
                      SECONDS_PER_SNACK_BREAK
        );

        outbound_total_time_seconds =
                  outbound_flight_time_seconds + outbound_break_time_seconds;

        loading_time_seconds =
                calculate_loading_time_seconds(gold_weight_pounds);

        load_scaling_factor = 1.0 + (gold_weight_pounds / HALF_SPEED_WEIGHT);

        loaded_speed_mph = calculate_loaded_speed(
            UNLOADED_SPEED_MPH,
            gold_weight_pounds,
            load_scaling_factor
        );

        return_flight_time_seconds = calculate_flight_time_seconds(
              distance_to_treasure_miles,
              loaded_speed_mph
        );


        return_snacks = calculate_number_of_snacks(
              return_flight_time_seconds,
              SECONDS_PER_SNACK
        );


        return_break_time_seconds = calculate_total_break_time_seconds(
              return_snacks,
              SECONDS_PER_SNACK_BREAK
        );

        total_break_time_seconds =
          outbound_break_time_seconds + return_break_time_seconds;

        number_of_snacks = outbound_snacks + return_snacks;

        return_total_time_seconds =
          return_flight_time_seconds +
          return_break_time_seconds +
          loading_time_seconds;

        total_flight_time_seconds =
                outbound_flight_time_seconds + return_flight_time_seconds;

        total_trip_time_seconds =
          outbound_total_time_seconds + return_total_time_seconds;

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        cout << "\nExpedition Summary" << endl;
        cout << "------------------" << endl;

        cout << "Outbound leg:" << endl;

        cout << "              Air speed: "
             << format_with_commas(UNLOADED_SPEED_MPH, 0)
             << " mph: "
             << endl;

        cout << "            Flight time: "
             << format_time_wdhms(outbound_flight_time_seconds)
             << endl;

        cout << "     Sheep-sized snacks: "
             << format_with_commas(outbound_snacks,0)
             << endl;

        cout << "       Snack-break time: "
             << format_time_wdhms(outbound_break_time_seconds)
             << endl;

        cout << "               Leg time: "
             << format_time_wdhms(outbound_total_time_seconds)
             << endl;

        cout << "~~~" << endl;

        cout << "Treasure\n"
             << "           loading time: "
             << format_time_wdhms(loading_time_seconds)
             << endl;

        cout << "            Load factor: "
             << format_with_commas(load_scaling_factor, 3)
             << endl;

        cout << "~~~" << endl;

        cout << "Return leg:" << endl;

        cout << "              Air speed: "
             << format_with_commas(loaded_speed_mph, 2)
             << " mph: "
             << endl;

        cout << "            Flight time: "
             << format_time_wdhms(return_flight_time_seconds)
             << endl;

        cout << "     Sheep-sized snacks: "
             << format_with_commas(return_snacks, 0)
             << endl;

        cout << "       Snack-break time: "
             << format_time_wdhms(return_break_time_seconds)
             << endl;

        cout << "               Leg time: "
             << format_time_wdhms(return_total_time_seconds)
             << endl;

        cout << "~~~" << endl;

        cout << "Expedition totals:" << endl;

        cout << "          Airborne time: "
             << format_time_wdhms(total_flight_time_seconds)
             << endl;

        cout << "     Sheep-sized snacks: "
             << format_with_commas(number_of_snacks) << " "
             << get_plurality(number_of_snacks, "snack", "snacks")
             << endl;

        cout << "       Snack-break time: "
             << format_time_wdhms(total_break_time_seconds)
             << endl;

        cout << "        Expedition time: "
             << format_time_wdhms(total_trip_time_seconds)
             << endl;

    } while (get_yes_or_enter(REPEAT_PRONT));

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Prompt for a numeric value within an allowed range.
 *
 * Parameters
 * ----------
 * prompt_message : const string&
 *     The prompt displayed to the user before input is read.
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
 *     A validated numeric value within the requested range.
 *
 * Notes
 * -----
 * This function reads the full line before conversion so it can reject
 * invalid input cleanly, including empty input, non-numeric characters,
 * and numeric values followed by trailing characters. It also enforces
 * a closed numeric range so the scenario remains within intended bounds.
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

        if (numeric_value < minimum_value || numeric_value > maximum_value)
        {
            cout << "Error: Value must be between "
                 << format_with_commas(minimum_value, 0)
                 << " and "
                 << format_with_commas(maximum_value, 0)
                 << "." << endl;
            continue;
        }

        return numeric_value;
    }
}

/**
 * Prompt the user for a simple yes-or-continue decision.
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
 * This function reads the full line so it can distinguish between a
 * deliberate single-character response and an empty input. Any response
 * other than 'Y', 'y', or an empty line is rejected and the user is
 * prompted again.
 */
bool get_yes_or_enter(const string& prompt_message)
{
    string user_input;

    while (true)
    {
        cout << prompt_message;
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

/**
 * Build the expedition story message using current simulation parameters.
 *
 * Parameters
 * ----------
 * unloaded_speed : double
 *     The dragon's flight speed when not carrying any treasure (mph).
 *
 * half_speed_weight : double
 *     The weight of gold in pounds that reduces the dragon's speed to
 *     half of its unloaded speed.
 *
 * seconds_per_snack : int
 *     The maximum airborne duration in seconds before the dragon needs
 *     another snack.
 *
 * seconds_per_break : int
 *     The duration of each snack break in seconds.
 *
 * maximum_distance_miles : double
 *     The greatest one-way treasure distance the program allows.
 *
 * maximum_gold_weight_pounds : double
 *     The greatest gold weight the dragon is willing to carry home.
 *
 * Returns
 * -------
 * story_message : string
 *     A fully constructed narrative describing the expedition scenario
 *     and the constraints applied to the dragon's journey.
 *
 * Notes
 * -----
 * This function dynamically incorporates simulation constants into the
 * story so that the narrative always reflects the actual model used in
 * calculations. This avoids duplication of values and ensures that any
 * changes to constants are consistently represented in both logic and
 * output.
 */
void build_story_message[
    double unloaded_speed,
    double half_speed_weight,
    int seconds_per_snack,
    int seconds_per_break,
    double maximum_distance_miles,
    double maximum_gold_weight_pounds
]
{
    stringstream story;

    story << "A dragon has learned of a treasure hoard of gold hidden deep in "
          << "the hills.\n\nIt flies at " << unloaded_speed << " mph when "
          << "unburdened, but carrying gold reduces its flight\nefficiency. "
          << "For every " << half_speed_weight << " pounds of treasure, "
          << "its load factor increases,\neffectively halving its flight speed "
          << "compared to its unloaded speed.\n\n"
          << "The dragon can remain airborne for "
          << format_time_wdhms(seconds_per_snack)
          << " before needing\na sheep-sized snack, and each snack break lasts "
          << format_time_wdhms(seconds_per_break) << ".\n\n"
          << "Loading the treasure also takes 1 second per pound of gold.\n\n"
          << "Even a dragon has limits. It will fly no farther than "
          << format_with_commas(maximum_distance_miles, 0)
          << " miles one way,\nand can carry no more than "
          << format_with_commas(maximum_gold_weight_pounds, 0)
          << " pounds of gold.\n\n"
          << "Once the journey begins, the dragon will not sleep until it "
          << "returns to its lair.";

    return story.str();
}

/**
 * Calculate the dragon's loaded return speed.
 *
 * Parameters
 * ----------
 * unloaded_speed_mph : double
 *     The dragon's normal flight speed without treasure.
 *
 * gold_weight_pounds : double
 *     The weight of the gold carried during the return trip.
 *
 * half_speed_weight : double
 *     The amount of carried gold that reduces the dragon's speed to
 *     half of its unloaded speed.
 *
 * Returns
 * -------
 * loaded_speed_mph : double
 *     The reduced return speed after accounting for the gold weight.
 *
 * Notes
 * -----
 * This function uses a simple diminishing-speed model rather than a
 * physically realistic one. The goal is to create a clear and readable
 * example for practicing user-defined functions.
 */
double calculate_loaded_speed(
    double unloaded_speed_mph,
    double gold_weight_pounds,
    double load_scaling_factor
)
{
    return unloaded_speed_mph / load_scaling_factor;
}

/**
 * Calculate flight time in seconds.
 *
 * Parameters
 * ----------
 * travel_distance_miles : double
 *     The distance to be traveled in miles.
 *
 * flight_speed_mph : double
 *     The rate of travel in miles per hour.
 *
 * Returns
 * -------
 * flight_time_seconds : double
 *     The number of seconds required to travel the given distance.
 */
double calculate_flight_time_seconds(
    double travel_distance_miles,
    double flight_speed_mph
)
{
    if (flight_speed_mph <= 0.0)
    {
        return 0.0;
    }

    return (travel_distance_miles / flight_speed_mph) * 3600.0;
}

/**
 * Calculate how many snacks the dragon needs for the journey.
 *
 * Parameters
 * ----------
 * total_flight_time_seconds : double
 *     The total airborne time for the journey segment, expressed in
 *     seconds.
 *
 * seconds_per_snack : int
 *     The maximum number of airborne seconds the dragon can manage
 *     before needing another snack.
 *
 * Returns
 * -------
 * number_of_snacks : int
 *     The total number of snack stops needed during that journey segment.
 *
 * Notes
 * -----
 * The dragon begins each journey segment already fed. Because of that,
 * the number of snacks needed is based on how many full flight intervals
 * are completed during the segment.
 */
int calculate_number_of_snacks(
    double total_flight_time_seconds,
    int seconds_per_snack
)
{
    if (seconds_per_snack <= 0)
    {
        return 0;
    }

    return static_cast<int>(total_flight_time_seconds / seconds_per_snack);
}

/**
 * Calculate the total amount of time spent on snack breaks.
 *
 * Parameters
 * ----------
 * number_of_snacks : int
 *     The number of snack stops required for the journey segment.
 *
 * seconds_per_snack_break : int
 *     The duration of each snack break in seconds.
 *
 * Returns
 * -------
 * total_break_time_seconds : double
 *     The total break time required for all snack stops.
 */
double calculate_total_break_time_seconds(
    int number_of_snacks,
    int seconds_per_snack_break
)
{
    return number_of_snacks * seconds_per_snack_break;
}

/**
 * Calculate the amount of time required to load the treasure.
 *
 * Parameters
 * ----------
 * gold_weight_pounds : double
 *     The total amount of gold to be loaded for the return trip.
 *
 * Returns
 * -------
 * loading_time_seconds : double
 *     The loading time expressed in seconds.
 *
 * Notes
 * -----
 * This model assumes that each pound of gold requires exactly one second
 * to load onto the dragon before departure on the return trip.
 */
double calculate_loading_time_seconds(double gold_weight_pounds)
{
    return gold_weight_pounds;
}

/**
 * Return the singular or plural form of a word based on quantity.
 *
 * Parameters
 * ----------
 * quantity : int
 *     The quantity used to determine which word form should be returned.
 *
 * singular_word : const string&
 *     The word form used when the quantity is exactly 1.
 *
 * plural_word : const string&
 *     The word form used for all other quantities.
 *
 * Returns
 * -------
 * selected_word : string
 *     The singular or plural word that matches the quantity.
 */
string get_plurality(
    int quantity,
    const string& singular_word,
    const string& plural_word
)
{
    if (quantity == 1)
    {
        return singular_word;
    }

    return plural_word;
}

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
 * Convert a total number of seconds into a formatted
 * weeks/days/hours/minutes/seconds string.
 *
 * Parameters
 * ----------
 * total_seconds : double
 *     The total elapsed time expressed in seconds.
 *
 * Returns
 * -------
 * formatted_time : string
 *     A human-readable string representing the time broken into
 *     weeks, days, hours, minutes, and seconds. Any component with a
 *     value of zero is omitted from the output.
 *
 * Notes
 * -----
 * This function converts total elapsed seconds into discrete time
 * components using integer division and modulo operations. The result is
 * formatted for readability rather than strict precision, making it more
 * suitable for user-facing output.
 *
 * Examples
 * --------
 * 6588 seconds -> "1 hour, 49 minutes, 48 seconds"
 * 2700 seconds -> "45 minutes"
 * 172800 seconds -> "2 days"
 */
string format_time_wdhms(double total_seconds)
{
    int whole_seconds = static_cast<int>(total_seconds);

    int weeks = whole_seconds / 604800;
    int remaining_seconds = whole_seconds % 604800;

    int days = remaining_seconds / 86400;
    remaining_seconds %= 86400;

    int hours = remaining_seconds / 3600;
    remaining_seconds %= 3600;

    int minutes = remaining_seconds / 60;
    int seconds = remaining_seconds % 60;

    stringstream output;
    bool first = true;

    if (weeks > 0)
    {
        output << format_with_commas(weeks, 0) << " "
               << get_plurality(weeks, "week", "weeks");
        first = false;
    }

    if (days > 0)
    {
        if (!first)
        {
            output << ", ";
        }

        output << format_with_commas(days, 0) << " "
               << get_plurality(days, "day", "days");
        first = false;
    }

    if (hours > 0)
    {
        if (!first)
        {
            output << ", ";
        }

        output << format_with_commas(hours, 0) << " "
               << get_plurality(hours, "hour", "hours");
        first = false;
    }

    if (minutes > 0)
    {
        if (!first)
        {
            output << ", ";
        }

        output << format_with_commas(minutes, 0) << " "
               << get_plurality(minutes, "minute", "minutes");
        first = false;
    }

    if (seconds > 0)
    {
        if (!first)
        {
            output << ", ";
        }

        output << format_with_commas(seconds, 0) << " "
               << get_plurality(seconds, "second", "seconds");
    }

    if (weeks == 0 &&
        days == 0 &&
        hours == 0 &&
        minutes == 0 &&
        seconds == 0)
    {
        return "0 seconds";
    }

    return output.str();
}