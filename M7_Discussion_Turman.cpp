/**
* ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M7 Discussion
 * Author: Eric J. Turman
 * Date: 2026-04-26
 *
 * Description:
 * A dragon-broker simulation adapted from the M6 enumeration discussion
 * program. The program presents a randomized treasure expedition scenario
 * and asks the user to select the dragon they believe is best suited for
 * the job.
 *
 * For the M7 discussion, the M6 version was refactored so dragon
 * characteristics and treasure properties are organized with
 * two-dimensional arrays instead of separate attribute lookup functions.
 * Enumeration values are still used for clear menu choices, while array
 * indexes are used to retrieve related dragon and treasure data.
 *
 * The simulation evaluates whether the chosen dragon accepts the expedition
 * based on carrying capacity, time cost, snack cost, and the dragon's
 * treasure appetite. It then displays a detailed expedition report and a
 * broker performance review that compares the chosen dragon to the best
 * dragon available for that scenario.
 *
 * Notes:
 * This discussion version intentionally includes at least two errors so
 * classmates can identify and correct them. The program is not expected to
 * compile or execute successfully until those errors are fixed. There are
 * four intentional bugs.
 *
 * The code would still be cleaner and more elegant with structs and classes,
 * which are covered in a later week. Until then, grouped array lookups help
 * keep related dragon and treasure attributes organized without changing
 * the overall procedural structure of the program.
 * ========================================================================
 */

// ========================================================================
// Imports (Includes) and Namespace
// ========================================================================
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>

using namespace std;

// ========================================================================
// Enumerations
// ========================================================================
enum DragonType
{
    RED = 1,
    BLUE,
    GREEN,
    BLACK,
    WHITE,
    GOLD,
    SILVER,
    BRONZE,
    COPPER,
    BRASS
};

enum TreasureType
{
    GOLD_COIN = 1,
    GOLD_CHALICE,
    SILVER_COIN,
    SILVER_PLATTER,
    PLATINUM_COIN,
    PLATINUM_RING,
    RAW_GEM,
    CUT_GEM,
    JEWELRY
};

enum DragonStatIndex
{
    STRENGTH_INDEX,
    GREED_MODIFIER_INDEX,
    GREED_THRESHOLD_INDEX,
    SPEED_MPH_INDEX,
    ENDURANCE_MILES_INDEX,
    SNACK_DEMAND_INDEX,
    TIME_SENSITIVITY_INDEX,
    DRAGON_STAT_COUNT
};

enum DragonTextIndex
{
    DRAGON_NAME_INDEX,
    DRAGON_DESCRIPTION_INDEX,
    DRAGON_TEXT_COUNT
};

enum TreasureStatIndex
{
    WEIGHT_PER_UNIT_INDEX,
    VALUE_PER_POUND_INDEX,
    TREASURE_STAT_COUNT
};

enum TreasureTextIndex
{
    TREASURE_NAME_INDEX,
    TREASURE_DESCRIPTION_INDEX,
    TREASURE_TEXT_COUNT
};

// ========================================================================
// Constants
// ========================================================================
const int MIN_DISTANCE_MILES = 100;
const int MAX_DISTANCE_MILES = 500;
const int MIN_TREASURE_QUANTITY = 50;
const int MAX_TREASURE_QUANTITY = 300;
const int DRAGON_COUNT = 10;
const int TREASURE_COUNT = 9;
const double SNACK_COST = 50.0;
const double BROKER_COMMISSION_RATE = 0.10;
const int SECONDS_PER_TREASURE_UNIT = 1;
const double SECONDS_PER_MINUTE = 60.0;
const double SECONDS_PER_HOUR = 3600.0;

/**
 * Dragon display text organized by dragon type and text field.
 *
 * Rows:
 * Match DragonType order, beginning with RED and ending with BRASS.
 *
 * Columns:
 * DRAGON_NAME_INDEX stores the display name.
 * DRAGON_DESCRIPTION_INDEX stores the broker-facing description.
 */
const string DRAGON_TEXT[DRAGON_COUNT][DRAGON_TEXT_COUNT] = {
    {
        "Red",
        "A domineering force that considers lesser hoards beneath its notice.\n"
        "When it moves, it expects the reward to justify its presence."
    },
    {
        "Blue",
        "Calculating and precise, it favors outcomes that are worth the effort.\n"
        "Rarely acts without weighing the long-term gain."
    },
    {
        "Green",
        "Cunning and patient, with a preference for outcomes that offer\n"
        "more than they first appear to promise."
    },
    {
        "Black",
        "Opportunistic and enduring, willing to tolerate a great deal\n"
        "if the outcome leans in its favor."
    },
    {
        "White",
        "Quick to act and quicker to abandon an uninteresting pursuit.\n"
        "Instinct often outweighs deliberation."
    },
    {
        "Gold",
        "Ancient and deliberate, not easily swayed by mere wealth.\n"
        "Chooses its undertakings with great care."
    },
    {
        "Silver",
        "Swift and capable, with a balanced outlook on effort and reward.\n"
        "Favors efficiency and clean outcomes."
    },
    {
        "Bronze",
        "Resolute and dependable, willing to commit when the endeavor\n"
        "appears worthwhile."
    },
    {
        "Copper",
        "Easily distracted and rarely committed to long, tedious endeavors.\n"
        "Prefers engagements that are as interesting as they are rewarding."
    },
    {
        "Brass",
        "Curious and talkative, but not particularly driven by labor-intensive\n"
        "pursuits. Favors lighter engagements."
    }
};

/**
 * Treasure display text organized by treasure type and text field.
 *
 * Rows:
 * Match TreasureType order, beginning with GOLD_COIN and ending with JEWELRY.
 *
 * Columns:
 * TREASURE_NAME_INDEX stores the display name.
 * TREASURE_DESCRIPTION_INDEX stores the broker-facing description.
 */
const string TREASURE_TEXT[TREASURE_COUNT][TREASURE_TEXT_COUNT] = {
    {
        "Gold coins",
        "Compact and widely valued, a classic and reliable form of wealth."
    },
    {
        "Gold chalices",
        "Ornate and impressive, though not always the most practical to transport."
    },
        [
        "Silver coins",
        "Common and easy to handle, but not especially remarkable in value."
    ],
    {
        "Silver platters",
        "Large and cumbersome, with more presence than efficient value."
    },
    {
        "Platinum coins",
        "Dense and highly valued, offering substantial worth in a small form."
    },
    {
        "Platinum rings",
        "Small, refined, and exceptionally valuable for their size."
    },
    {
        "Raw gems",
        "Unrefined and full of potential, though not yet at their peak worth."
    },
    {
        "Cut gems",
        "Precisely shaped and brilliantly valuable, prized for their refinement."
    },
    {
        "Jewelry",
        "Crafted pieces of beauty and value, balancing form and worth."
    }
};

/**
 * Dragon numeric attributes organized by dragon type and stat field.
 *
 * Rows:
 * Match DragonType order, beginning with RED and ending with BRASS.
 *
 * Columns:
 * STRENGTH_INDEX stores carrying strength.
 * GREED_MODIFIER_INDEX stores the dragon's interest adjustment.
 * GREED_THRESHOLD_INDEX stores the minimum perceived value required.
 * SPEED_MPH_INDEX stores travel speed in miles per hour.
 * ENDURANCE_MILES_INDEX stores distance before a rest stop is needed.
 * SNACK_DEMAND_INDEX stores snacks consumed at each rest stop.
 * TIME_SENSITIVITY_INDEX stores travel-time penalty sensitivity.
 */
const double DRAGON_STATS[DRAGON_COUNT][DRAGON_STAT_COUNT] = {
    {10.0, 1.0, 20000.0, 140.0, 140.0, 6.0, 60.0},
    {8.0, 0.7, 15000.0, 135.0, 180.0, 4.0, 30.0},
    {7.0, 0.5, 13000.0, 138.0, 160.0, 3.0, 35.0},
    {7.0, 0.3, 10000.0, 120.0, 200.0, 4.0, 25.0},
    {5.0, -0.1, 6000.0, 165.0, 90.0, 2.0, 80.0},
    {10.0, 0.2, 18000.0, 130.0, 260.0, 8.0, 20.0},
    {7.0, 0.1, 12000.0, 170.0, 170.0, 3.0, 25.0},
    {8.0, 0.2, 13000.0, 145.0, 220.0, 4.0, 20.0},
    {6.0, 0.0, 9000.0, 160.0, 150.0, 3.0, 45.0},
    {6.0, -0.1, 8000.0, 155.0, 140.0, 3.0, 55.0}
};

/**
 * Treasure numeric attributes organized by treasure type and stat field.
 *
 * Rows:
 * Match TreasureType order, beginning with GOLD_COIN and ending with JEWELRY.
 *
 * Columns:
 * WEIGHT_PER_UNIT_INDEX stores pounds per treasure unit.
 * VALUE_PER_POUND_INDEX stores treasure value per pound.
 */
const double TREASURE_STATS[TREASURE_COUNT][TREASURE_STAT_COUNT] = {
    {0.02, 950.0},
    {3.00, 700.0},
    {0.02, 120.0},
    {5.00, 90.0},
    {0.02, 1600.0},
    {0.10, 2200.0},
    {0.50, 1800.0},
    {0.30, 3500.0},
    {1.20, 2600.0}
};
// ========================================================================
// Function (Prototype) Declarations
// ========================================================================
string format_with_commas(double numeric_value, int decimal_precision);

string get_plurality(
    int quantity,
    const string& singular_word,
    const string& plural_word
);

string format_time_wdhms(double total_seconds);

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

string build_briefing_message();

void broker_notes_loop(TreasureType treasure_type);

int display_broker_notes_menu();

string get_dragon_description(DragonType dragon_type);

string get_treasure_description(TreasureType treasure_type);

void display_treasure_description(TreasureType treasure_type);

DragonType get_dragon_choice_from_user();

int get_dragon_index(DragonType dragon_type);

int get_treasure_index(TreasureType treasure_type);

mt19937& get_random_engine();

TreasureType generate_random_treasure_type();

int generate_random_distance_miles();

int generate_random_treasure_quantity();

bool is_playable_scenario(
    TreasureType treasure_type,
    int treasure_quantity,
    int distance_miles
);

void generate_playable_scenario(
    TreasureType& treasure_type,
    int& treasure_quantity,
    int& distance_miles
);

string get_dragon_name(DragonType dragon_type);

string get_treasure_name(TreasureType treasure_type);

int get_strength(DragonType dragon_type);

double get_greed_modifier(DragonType dragon_type);

double get_greed_threshold(DragonType dragon_type);

double get_speed_mph(DragonType dragon_type);

double calculate_loaded_return_speed_mph(
    double base_speed_mph,
    double total_weight,
    double carrying_capacity
);

double get_endurance_miles(DragonType dragon_type);

int get_snack_demand(DragonType dragon_type);

int get_time_sensitivity(DragonType dragon_type);

void display_dragon_descriptions(DragonType first, DragonType last);

double get_weight_per_unit(TreasureType treasure_type);

double get_value_per_pound(TreasureType treasure_type);

double calculate_total_weight(int treasure_quantity, double weight_per_unit);

double calculate_absolute_value(double total_weight, double value_per_pound);

double calculate_carrying_capacity(int strength);

double calculate_round_trip_distance(double one_way_distance_miles);

double calculate_leg_flight_time_seconds(
    double leg_distance_miles,
    double speed_mph
);

int calculate_rest_count(
    double travel_distance_miles,
    double endurance_miles
);

double calculate_loading_time_seconds(int treasure_quantity);

double calculate_break_time_seconds(int rest_count, int snack_demand);

double calculate_snack_cost(int rest_count, int snack_demand);

double calculate_perceived_value(
    double total_value,
    double greed_modifier,
    double travel_time_hours,
    int time_sensitivity,
    double snack_cost
);

bool expedition_is_accepted(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
);

string determine_decline_reason(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
);

double calculate_broker_commission(
    double perceived_value,
    bool accepted
);

double calculate_final_profit(double broker_commission, double snack_cost);

DragonType determine_best_dragon(
    TreasureType treasure_type,
    int treasure_quantity,
    int distance_miles,
    bool& best_dragon_found,
    double& best_final_profit,
    double& best_perceived_value
);

void run_balance_test();

void display_balance_test_line(
    DragonType dragon_type,
    int win_count,
    int number_of_test
);

string build_broker_review_message(
    DragonType chosen_dragon_type,
    DragonType best_dragon_type,
    bool chosen_dragon_accepted,
    bool best_dragon_found,
    double chosen_final_profit,
    double best_final_profit
);

void display_expedition_report(
    TreasureType treasure_type,
    int treasure_quantity,
    int distance_miles,
    DragonType dragon_type,
    int snack_demand,
    int destination_recovery_count,
    double total_weight,
    double total_value,
    double outbound_flight_time_seconds,
    double return_flight_time_seconds,
    int outbound_rest_count,
    int return_rest_count,
    double total_break_time_seconds,
    double loading_time_seconds,
    double snack_cost,
    double perceived_value,
    bool accepted,
    const string& decision_reason,
    double broker_commission,
    double final_profit,
    DragonType best_dragon_type,
    bool best_dragon_found,
    double best_final_profit
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
        "This program simulates a dragon broker selecting the best dragon\n"
        "for a randomized treasure expedition.";

    const string BRIEFING_PROMPT =
        "\nWould you like a briefing before starting? [Y] or press [Enter] to "
        "continue: ";

    const string BROKER_NOTES_PROMPT =
        "Would you like broker notes before choosing a dragon? "
        "[Y] or press [Enter] to continue: ";

    const string PLAY_AGAIN_PROMPT =
        "\nBroker another treasure expedition? [Y] or press [Enter] to exit: ";

    // --------------------------------------------------------------------
    // Variable declarations
    // --------------------------------------------------------------------

    TreasureType treasure_type = GOLD_COIN;
    int treasure_quantity = 0;
    int distance_miles = 0;
    DragonType dragon_type = RED;

    // --------------------------------------------------------------------
    // Introduction
    // --------------------------------------------------------------------
    cout << INTRODUCTION_MESSAGE << endl;

    if (get_boolean_choice(BRIEFING_PROMPT, "Y", ""))
    {
        cout << "\n" << build_briefing_message() << endl;
    }

    // --------------------------------------------------------------------
    // Main loop
    // --------------------------------------------------------------------
    do
    {
        // ----------------------------------------------------------------
        // Scenario generation
        // ----------------------------------------------------------------
        generate_playable_scenario(
            treasure_type,
            treasure_quantity,
            distance_miles
        );

        cout << "\nExpedition Opportunity" << endl;
        cout << "----------------------" << endl;
        cout << "Treasure type: "
             << get_treasure_name(treasure_type)
             << endl;
        cout << "Treasure quantity: "
             << format_with_commas(treasure_quantity, 0)
             << endl;
        cout << "One-way distance: "
             << format_with_commas(distance_miles, 0)
             << " miles"
             << endl;

        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        broker_notes_loop(treasure_type);

        dragon_type = get_dragon_choice_from_user();

        // ----------------------------------------------------------------
        // Dragon and treasure characteristic lookups
        // ----------------------------------------------------------------
        const int strength = get_strength(dragon_type);
        const double greed_modifier = get_greed_modifier(dragon_type);
        const double greed_threshold = get_greed_threshold(dragon_type);
        const double speed_mph = get_speed_mph(dragon_type);
        const double endurance_miles = get_endurance_miles(dragon_type);
        const int snack_demand = get_snack_demand(dragon_type);
        const int time_sensitivity = get_time_sensitivity(dragon_type);

        const double weight_per_unit = get_weight_per_unit(treasure_type);
        const double value_per_pound = get_value_per_pound(treasure_type);

        // ----------------------------------------------------------------
        // Processing
        // ----------------------------------------------------------------
        const double total_weight = calculate_total_weight(
            treasure_quantity,
            weight_per_unit
        );

        const double total_value = calculate_absolute_value(
            total_weight,
            value_per_pound
        );

        const double carrying_capacity = calculate_carrying_capacity(strength);

        const double outbound_flight_time_seconds = calculate_leg_flight_time_seconds(
            static_cast<double>(distance_miles),
            speed_mph
        );

        const double loaded_return_speed_mph = calculate_loaded_return_speed_mph(
            speed_mph,
            total_weight,
            carrying_capacity
        );

        const double return_flight_time_seconds = calculate_leg_flight_time_seconds(
            static_cast<double>(distance_miles),
            loaded_return_speed_mph
        );

        const int outbound_rest_count = calculate_rest_count(
            static_cast<double>(distance_miles),
            endurance_miles
        );

        const int destination_recovery_count = 1;

        const int return_rest_count = calculate_rest_count(
            static_cast<double>(distance_miles),
            endurance_miles
        );

        const int total_rest_count =
            outbound_rest_count +
            destination_recovery_count +
            return_rest_count;

        const double total_break_time_seconds = calculate_break_time_seconds(
            total_rest_count,
            snack_demand
        );

        const double loading_time_seconds = calculate_loading_time_seconds(
            treasure_quantity
        );

        const double snack_cost = calculate_snack_cost(
            total_rest_count,
            snack_demand
        );

        const double perceived_value = calculate_perceived_value(
            total_value,
            greed_modifier,
            (outbound_flight_time_seconds + return_flight_time_seconds) /
                SECONDS_PER_HOUR,
            time_sensitivity,
            snack_cost
        );

        const bool accepted = expedition_is_accepted(
            total_weight,
            carrying_capacity,
            perceived_value,
            greed_threshold
        );

        const string decision_reason = determine_decline_reason(
            total_weight,
            carrying_capacity,
            perceived_value,
            greed_threshold
        );

        const double broker_commission = calculate_broker_commission(
            perceived_value,
            accepted
        );

        const double final_profit = calculate_final_profit(
            broker_commission,
            snack_cost
        );

        bool best_dragon_found = false;
        double best_final_profit = 0.0;
        double best_perceived_value = 0.0;

        const DragonType best_dragon_type = determine_best_dragon(
            treasure_type,
            treasure_quantity,
            distance_miles,
            best_dragon_found,
            best_final_profit,
            best_perceived_value
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        display_expedition_report(
            treasure_type,
            treasure_quantity,
            distance_miles,
            dragon_type,
            snack_demand,
            destination_recovery_count,
            total_weight,
            total_value,
            outbound_flight_time_seconds,
            return_flight_time_seconds,
            outbound_rest_count,
            return_rest_count,
            total_break_time_seconds,
            loading_time_seconds,
            snack_cost,
            perceived_value,
            accepted,
            decision_reason,
            broker_commission,
            final_profit,
            best_dragon_type,
            best_dragon_found,
            best_final_profit
        );

    } while (get_boolean_choice(PLAY_AGAIN_PROMPT, "Y", ""));

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
 * Return the singular or plural form of a word based on a count.
 *
 * Parameters
 * ----------
 * quantity : int
 *     The quantity being described.
 *
 * singular_word : const string&
 *     The form used when quantity equals one.
 *
 * plural_word : const string&
 *     The form used for all other counts.
 *
 * Returns
 * -------
 * selected_word : string
 *     The word that matches the quantity.
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
 * Convert a duration in seconds into a weeks-days-hours-minutes-seconds
 * string suitable for report output.
 *
 * Parameters
 * ----------
 * total_seconds : double
 *     The total duration to format, expressed in seconds.
 *
 * Returns
 * -------
 * formatted_time : string
 *     A readable time string such as "1 hour, 25 minutes, 14 seconds".
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

        return numeric_value;
    }
}


/**
 * Build the optional expedition briefing message.
 *
 * Returns
 * -------
 * briefing_message : string
 *     A scenario description explaining how the simulation works.
 */
string build_briefing_message()
{
    ostringstream output_stream;

    output_stream << "Dragon Broker Briefing" << endl;
    output_stream << "----------------------" << endl;
    output_stream << "A treasure opportunity will be generated with a random "
                  << "treasure type, quantity,\nand one-way distance. You "
                  << "must then select the dragon you believe is best suited\n"
                  << "for the job. Each dragon has different carrying power, "
                  << "speed, endurance,\nfeeding needs, and treasure appetite.\n"
                  << endl;
    output_stream << "The chosen dragon evaluates the expedition by "
                  << "comparing the treasure's value\nagainst carrying limits, "
                  << "time pressure, and snack cost. If the dragon accepts,\n"
                  << "you earn a broker commission of around "
                  << format_with_commas(BROKER_COMMISSION_RATE * 100.0, 0)
                  << "% of the what the dragon believes the\ntreasure is worth. "
                  << "But some dragons are hungrier or greedier than others,\n"
                     "and this will affect your commission.\n"
                  << endl;
    output_stream << "After the expedition report, the program also compares "
                  << "your selection against\nthe best dragon available for "
                  << "that same scenario so you can see whether your\nbroker "
                  << "instincts were optimal.";

    return output_stream.str();
}

/**
 * Provide access to a shared pseudo-random number generator.
 *
 * Returns
 * -------
 * random_engine : mt19937&
 *     A reference to a static Mersenne Twister engine seeded once for
 *     the duration of the program.
 *
 * Notes
 * -----
 * This centralizes random-engine ownership so helper functions can
 * generate values without forcing RNG parameters through the code base.
 */
mt19937& get_random_engine()
{
    static random_device random_device_source;
    static mt19937 random_engine(random_device_source());

    return random_engine;
}

/**
 * Generate a random treasure type for the scenario.
 *
 * Returns
 * -------
 * treasure_type : TreasureType
 *     A randomly selected treasure type.
 */
TreasureType generate_random_treasure_type()
{
    uniform_int_distribution<int> distribution(
        static_cast<int>(GOLD_COIN),
        static_cast<int>(JEWELRY)
    );

    return static_cast<TreasureType>(distribution(get_random_engine()));
}

/**
 * Generate a random one-way expedition distance in miles.
 *
 * Returns
 * -------
 * distance_miles : int
 *     A random one-way distance between the configured bounds.
 */
int generate_random_distance_miles()
{
    uniform_int_distribution<int> distribution(
        MIN_DISTANCE_MILES,
        MAX_DISTANCE_MILES
    );

    return distribution(get_random_engine());
}

/**
 * Generate a random treasure quantity for the scenario.
 *
 * Returns
 * -------
 * treasure_quantity : int
 *     A random treasure quantity between the configured bounds.
 */
int generate_random_treasure_quantity()
{
    uniform_int_distribution<int> distribution(
        MIN_TREASURE_QUANTITY,
        MAX_TREASURE_QUANTITY
    );

    return distribution(get_random_engine());
}

/**
 * Determine whether a generated scenario produces a meaningful mix of
 * accepted and declined dragon responses.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure type generated for the scenario.
 *
 * treasure_quantity : int
 *     The quantity of treasure available in the scenario.
 *
 * distance_miles : int
 *     The one-way distance to the treasure in miles.
 *
 * Returns
 * -------
 * playable : bool
 *     True when at least two dragons accept and at least two dragons
 *     decline the expedition. False otherwise.
 *
 * Notes
 * -----
 * This prevents dead scenarios where no dragons accept and avoids
 * trivial scenarios where nearly every dragon accepts.
 */
bool is_playable_scenario(
    TreasureType treasure_type,
    int treasure_quantity,
    int distance_miles
)
{
    int accepted_count = 0;
    int declined_count = 0;

    double weight_per_unit = get_weight_per_unit(treasure_type);
    double value_per_pound = get_value_per_pound(treasure_type);
    double total_weight = calculate_total_weight(
        treasure_quantity,
        weight_per_unit
    );
    double total_value = calculate_absolute_value(total_weight, value_per_pound);
    double one_way_distance_miles = static_cast<double>(distance_miles);

    for (int dragon_index = static_cast<int>(RED);
        dragon_index <= static_cast<int>(BRASS);
        ++dragon_index
    )
    {
        DragonType dragon_type = static_cast<DragonType>(dragon_index);

        double carrying_capacity = calculate_carrying_capacity(
            get_strength(dragon_type)
        );

        double speed_mph = get_speed_mph(dragon_type);

        double outbound_time_seconds = calculate_leg_flight_time_seconds(
            one_way_distance_miles,
            speed_mph
        );

        double loaded_return_speed_mph = calculate_loaded_return_speed_mph(
            speed_mph,
            total_weight,
            carrying_capacity
        );

        double return_time_seconds = calculate_leg_flight_time_seconds(
            one_way_distance_miles,
            loaded_return_speed_mph
        );

        int total_rest_count =
            calculate_rest_count(
                one_way_distance_miles,
                get_endurance_miles(dragon_type)
            ) +
            1 +
            calculate_rest_count(
                one_way_distance_miles,
                get_endurance_miles(dragon_type)
            );

        double snack_cost = calculate_snack_cost(
            total_rest_count,
            get_snack_demand(dragon_type)
        );

        double perceived_value = calculate_perceived_value(
            total_value,
            get_greed_modifier(dragon_type),
            (outbound_time_seconds + return_time_seconds) / SECONDS_PER_HOUR,
            get_time_sensitivity(dragon_type),
            snack_cost
        );

        bool accepted = expedition_is_accepted(
            total_weight,
            carrying_capacity,
            perceived_value,
            get_greed_threshold(dragon_type)
        );

        if (accepted)
        {
            accepted_count++;
        }
        else
        {
            declined_count++;
        }
    }

    return accepted_count >= 2 && declined_count >= 2;
}

/**
 * Generate a randomized expedition scenario until it falls within the
 * playable acceptance range for the dragon roster.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType&
 *     Output parameter receiving the generated treasure type.
 *
 * treasure_quantity : int&
 *     Output parameter receiving the generated treasure quantity.
 *
 * distance_miles : int&
 *     Output parameter receiving the generated one-way distance.
 *
 * Returns
 * -------
 * None
 *
 * Notes
 * -----
 * The function retries random generation until the resulting scenario
 * produces at least two accepting dragons and at least two declining
 * dragons, preserving meaningful choice for the player.
 */
void generate_playable_scenario(
    TreasureType& treasure_type,
    int& treasure_quantity,
    int& distance_miles
)
{
    do
    {
        treasure_type = generate_random_treasure_type();
        treasure_quantity = generate_random_treasure_quantity();
        distance_miles = generate_random_distance_miles();
    }
    while (!is_playable_scenario(
        treasure_type,
        treasure_quantity,
        distance_miles
    ));
}

/**
 * Display the broker notes menu and return the selected option.
 *
 * Returns
 * -------
 * selection : int
 *     The validated menu option selected by the user.
 *
 * Notes
 * -----
 * This menu allows the user to inspect descriptive information
 * before committing to a dragon choice.
 */
int display_broker_notes_menu()
{
    cout << "\nBroker Notes Menu\n";
    cout << "------------------------------------------------------------\n";
    cout << "1. View all dragon descriptions\n";
    cout << "2. View one dragon description\n";
    cout << "3. View treasure description\n";
    cout << "4. Run balance tests\n";
    cout << "5. Continue to dragon selection\n";

    return get_bounded_int("Selection: ", 1, 5);
}

/**
 * Prompt the user to choose a dragon from the available dragon types.
 *
 * Returns
 * -------
 * selected_dragon : DragonType
 *     The validated dragon type selected by the user.
 */
DragonType get_dragon_choice_from_user()
{
    cout << "\nAvailable Dragons" << endl;
    cout << "-----------------" << endl;
    cout << "1. Red" << endl;
    cout << "2. Blue" << endl;
    cout << "3. Green" << endl;
    cout << "4. Black" << endl;
    cout << "5. White" << endl;
    cout << "6. Gold" << endl;
    cout << "7. Silver" << endl;
    cout << "8. Bronze" << endl;
    cout << "9. Copper" << endl;
    cout << "10. Brass" << endl;

    int user_choice = get_bounded_int(
        "\nChoose the dragon you want to broker for this expedition: ",
        1,
        10
    );

    return static_cast<DragonType>(user_choice);
}

/**
 * Display descriptive summaries for a range of dragon types.
 *
 * Parameters
 * ----------
 * first : DragonType
 *     The first dragon type in the range to display.
 *
 * last : DragonType
 *     The last dragon type in the range to display.
 *
 * Returns
 * -------
 * None
 *
 * Notes
 * -----
 * This function is useful for showing all dragon descriptions at once
 * or for displaying a single dragon by passing the same value for both
 * first and last.
 */
void display_dragon_descriptions(DragonType first, DragonType last)
{
    cout << "\nDragon Descriptions\n";
    cout << "------------------------------------------------------------\n";

    for (int i = first; i <= last; ++i)
    {
        DragonType dragon = static_cast<DragonType>(i);

        cout << "\n" << get_dragon_name(dragon) << ":\n";
        cout << get_dragon_description(dragon) << "\n";
    }
}

/**
 * Display the flavor description for the current treasure type.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure type whose descriptive notes should be shown.
 *
 * Returns
 * -------
 * None
 *
 * Notes
 * -----
 * This keeps treasure-description output consistent with the dragon
 * description display flow used in the broker notes menu.
 */
void display_treasure_description(TreasureType treasure_type)
{
    cout << "\nTreasure Description\n";
    cout << "------------------------------------------------------------\n";
    cout << get_treasure_name(treasure_type) << ":\n";
    cout << get_treasure_description(treasure_type) << "\n";
}

/**
 * Manage the optional broker notes workflow before dragon selection.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure type generated for the current expedition scenario.
 *
 * Returns
 * -------
 * None
 *
 * Notes
 * -----
 * This function allows the user to review descriptive notes about
 * dragons and the current treasure before choosing a dragon.
 */
void broker_notes_loop(TreasureType treasure_type)
{
    bool continue_notes = true;

    while (continue_notes)
    {
        int selection = display_broker_notes_menu();

        switch (selection)
        {
            case 1:
                display_dragon_descriptions(RED, BRASS);
                break;

            case 2:
            {
                DragonType dragon = get_dragon_choice_from_user();
                display_dragon_descriptions(dragon, dragon);
                break;
            }

            case 3:
                display_treasure_description(treasure_type);
                break;

            case 4:
                run_balance_test();
                break;

            case 5:
                continue_notes = false;
                break;

            default:
                cout << "\nInvalid selection.\n";
        }
    }
}

/**
 * Convert a dragon enum value into a zero-based array index.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon enumeration value being translated for table lookup.
 *
 * Returns
 * -------
 * dragon_index : int
 *     The zero-based array index associated with that dragon. Returns -1
 *     when the supplied enum value falls outside the supported range.
 */
int get_dragon_index(DragonType dragon_type)
{
    int dragon_index = static_cast<int>(dragon_type) - 1;

    if (dragon_index < 0 || dragon_index >= DRAGON_COUNT)
    {
        return -1;
    }

    return dragon_index;
}

/**
 * Convert a treasure enum value into a zero-based array index.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure enumeration value being translated for table lookup.
 *
 * Returns
 * -------
 * treasure_index : int
 *     The zero-based array index associated with that treasure. Returns -1
 *     when the supplied enum value falls outside the supported range.
 */
int get_treasure_index(TreasureType treasure_type)
{
    int treasure_index = static_cast<int>(treasure_type) - 1;

    if (treasure_index < 0 || treasure_index >= TREASURE_COUNT)
    {
        return -1;
    }

    return treasure_index;
}

/**
 * Return the display name for a dragon type.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon enumeration value being translated into report text.
 *
 * Returns
 * -------
 * dragon_name : string
 *     The display-ready name associated with the supplied dragon type.
 */
string get_dragon_name(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return "Unknown Dragon";
    }

    return DRAGON_TEXT[dragon_index][DRAGON_NAME_INDEX];
}

/**
 * Provides a flavor-based description for the selected dragon type.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The enumerated dragon type selected by the user.
 *
 * Returns
 * -------
 * string
 *     A descriptive string conveying the temperament and tendencies of the
 *     specified dragon. The description intentionally avoids numeric values
 *     and instead provides suggestive language to encourage user intuition.
 *
 * Notes
 * -----
 * - These descriptions are designed to support gameplay by obscuring exact
 *   mechanical values while still reflecting underlying characteristics.
 * - The descriptions align with the internal stat model without exposing it.
 * ---------------------------------------------------------------------------
 */
string get_dragon_description(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return "An unknown dragon, its tendencies unclear.";
    }

    return DRAGON_TEXT[dragon_index][DRAGON_DESCRIPTION_INDEX];
}

/**
 * Return the display name for a treasure type.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure enumeration value being translated into report text.
 *
 * Returns
 * -------
 * treasure_name : string
 *     The display-ready name associated with the supplied treasure type.
 */
string get_treasure_name(TreasureType treasure_type)
{
    int treasure_index = get_treasure_index(treasure_type);

    if (treasure_index == -1)
    {
        return "Unknown Treasure";
    }

    return TREASURE_TEXT[treasure_index][TREASURE_NAME_INDEX];
}

/**
 * Provides a flavor-based description for the selected treasure type.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The enumerated treasure type generated for the expedition scenario.
 *
 * Returns
 * -------
 * string
 *     A descriptive string conveying the general nature and implied value
 *     characteristics of the treasure. The description avoids revealing
 *     explicit numerical efficiency metrics.
 *
 * Notes
 * -----
 * - Descriptions are intentionally suggestive rather than explicit to preserve
 *   the decision-making aspect of the simulation.
 * - Each description reflects underlying weight and value relationships
 *   without directly exposing them.
 * ---------------------------------------------------------------------------
 */
string get_treasure_description(TreasureType treasure_type)
{
    int treasure_index = get_treasure_index(treasure_type);

    if (treasure_index == -1)
    {
        return "An unfamiliar treasure of uncertain value.";
    }

    return TREASURE_TEXT[treasure_index][TREASURE_DESCRIPTION_INDEX];
}

/**
 * Return the dragon's strength rating.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose hauling strength is being requested.
 *
 * Returns
 * -------
 * strength_rating : int
 *     The rating later used to derive carrying capacity for the expedition.
 *
 * Notes
 * -----
 * Strength does not directly represent pounds. It is an abstract rating
 * that is intentionally converted into carrying capacity elsewhere so the
 * relationship between dragon identity and payload remains easy to tune.
 */
int get_strength(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0;
    }

    return static_cast<int>(DRAGON_STATS[dragon_index][STRENGTH_INDEX]);
}

/**
 * Return the dragon's greed multiplier.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose treasure enthusiasm factor is being requested.
 *
 * Returns
 * -------
 * greed_modifier : double
 *     A signed greed adjustment where 0.0 is neutral, positive values
 *     increase interest, and negative values reduce interest.
 *
 * Notes
 * -----
 * This value affects perceived value only. It does not change carrying
 * capacity, range, speed, or any other physical characteristic.
 */
double get_greed_modifier(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0.0;
    }

    return DRAGON_STATS[dragon_index][GREED_MODIFIER_INDEX];
}

/**
 * Return the dragon's greed threshold.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose minimum acceptable perceived value is being
 *     requested.
 *
 * Returns
 * -------
 * greed_threshold : double
 *     The minimum perceived-value score required before that dragon will
 *     agree to the expedition.
 *
 * Notes
 * -----
 * A higher threshold means the dragon is more selective. Even a dragon
 * with a strong positive greed modifier can still decline a low-reward
 * journey if the final perceived value does not clear this bar.
 */
double get_greed_threshold(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0.0;
    }

    return DRAGON_STATS[dragon_index][GREED_THRESHOLD_INDEX];
}

/**
 * Return the dragon's cruising speed in miles per hour.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose travel speed is being requested.
 *
 * Returns
 * -------
 * speed_mph : double
 *     The dragon's travel speed in miles per hour for both journey legs.
 *
 * Notes
 * -----
 * This simulation keeps speed consistent across outbound and return legs
 * so that the report remains easy to read and compare across dragons.
 */
double get_speed_mph(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0.0;
    }

    return DRAGON_STATS[dragon_index][SPEED_INDEX];
}

/**
 * Return the dragon's endurance range in miles before rest is required.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose continuous-flight range is being requested.
 *
 * Returns
 * -------
 * endurance_miles : double
 *     The maximum miles that dragon can cover before a rest stop becomes
 *     necessary.
 *
 */
double get_endurance_miles(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0.0;
    }

    return DRAGON_STATS[dragon_index][ENDURANCE_MILES_INDEX];
}

/**
 * Return the dragon's snack demand per stop.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose appetite per stop is being requested.
 *
 * Returns
 * -------
 * snack_demand : int
 *     The number of sheep-sized snacks required at each expedition stop.
 *
 * Notes
 * -----
 * Snack demand influences break time and cost at each stop.
 */
int get_snack_demand(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0;
    }

    return static_cast<int>(DRAGON_STATS[dragon_index][SNACK_DEMAND_INDEX]);
}

/**
 * Return the dragon's time sensitivity.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon whose impatience penalty is being requested.
 *
 * Returns
 * -------
 * time_sensitivity : int
 *     The penalty applied per airborne hour when perceived value is
 *     calculated for that dragon.
 *
 * Notes
 * -----
 * Higher values mean the dragon is less tolerant of long journeys and
 * therefore loses more perceived value as flight time increases.
 */
int get_time_sensitivity(DragonType dragon_type)
{
    int dragon_index = get_dragon_index(dragon_type);

    if (dragon_index == -1)
    {
        return 0;
    }

    return static_cast<int>(DRAGON_STATS[dragon_index][TIME_SENSITIVITY_INDEX]);
}

/**
 * Return the treasure weight in pounds per unit.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure classification being evaluated.
 *
 * Returns
 * -------
 * weight_per_unit : double
 *     The weight assigned to a single unit of the selected treasure type.
 */
double get_weight_per_unit(TreasureType treasure_type)
{
    int treasure_index = get_treasure_index(treasure_type);

    if (treasure_index == -1)
    {
        return 0.0;
    }

    return TREASURE_STATS[treasure_index][VALUE_PER_UNIT_INDEX];
}

/**
 * Return the treasure value density per pound.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure classification being evaluated.
 *
 * Returns
 * -------
 * value_per_pound : double
 *     The value assigned to each pound of that treasure type.
 */
double get_value_per_pound(TreasureType treasure_type)
{
    int treasure_index = get_treasure_index(treasure_type);

    if (treasure_index == -1)
    {
        return 0.0;
    }

    return TREASURE_STATS[treasure_index][WEIGHT_PER_POUND_INDEX];
}

/**
 * Calculate the dragon's reduced return-leg speed while carrying treasure.
 *
 * Parameters
 * ----------
 * base_speed_mph : double
 *     The dragon's normal cruising speed in miles per hour.
 *
 * total_weight : double
 *     The total treasure weight being carried on the return leg.
 *
 * carrying_capacity : double
 *     The dragon's maximum carrying capacity in pounds.
 *
 * Returns
 * -------
 * loaded_speed_mph : double
 *     The adjusted return-leg speed in miles per hour.
 *
 * Notes
 * -----
 * The return leg slows as the carried load approaches the dragon's
 * maximum carrying capacity. A minimum floor is used so that the
 * dragon never slows to zero speed.
 */
double calculate_loaded_return_speed_mph(
    double base_speed_mph,
    double total_weight,
    double carrying_capacity
)
{
    if (base_speed_mph <= 0.0 || carrying_capacity <= 0.0)
    {
        return 0.0;
    }

    double load_ratio = total_weight / carrying_capacity;

    if (load_ratio < 0.0)
    {
        load_ratio = 0.0;
    }

    if (load_ratio > 1.0)
    {
        load_ratio = 1.0;
    }

    double speed_factor = 1.0 - (load_ratio * 0.35);

    if (speed_factor < 0.50)
    {
        speed_factor = 0.50;
    }

    return base_speed_mph * speed_factor;
}

/**
 * Calculate the total treasure weight.
 *
 * Parameters
 * ----------
 * treasure_quantity : int
 *     The number of treasure units being hauled.
 *
 * weight_per_unit : double
 *     The weight assigned to one unit of the selected treasure.
 *
 * Returns
 * -------
 * total_weight : double
 *     The total weight of the treasure haul in pounds.
 */
double calculate_total_weight(int treasure_quantity, double weight_per_unit)
{
    return static_cast<double>(treasure_quantity) * weight_per_unit;
}

/**
 * Calculate the base treasure value.
 *
 * Parameters
 * ----------
 * total_weight : double
 *     The full treasure haul weight in pounds.
 *
 * value_per_pound : double
 *     The value density associated with the selected treasure.
 *
 * Returns
 * -------
 * absolute_value : double
 *     The total absolute unadjusted value of the treasure haul.
 */
double calculate_absolute_value(double total_weight, double value_per_pound)
{
    return total_weight * value_per_pound;
}

/**
 * Calculate the dragon's carrying capacity in pounds.
 *
 * Parameters
 * ----------
 * strength : int
 *     The dragon's abstract strength rating.
 *
 * Returns
 * -------
 * carrying_capacity : double
 *     The number of pounds that dragon can carry in this simulation.
 */
double calculate_carrying_capacity(int strength)
{
    return static_cast<double>(strength) * 100.0;
}

/**
 * Calculate the round-trip expedition distance.
 *
 * Parameters
 * ----------
 * one_way_distance_miles : double
 *     The one-way distance to the treasure site.
 *
 * Returns
 * -------
 * round_trip_distance_miles : double
 *     The distance for both journey legs combined.
 */
double calculate_round_trip_distance(double one_way_distance_miles)
{
    return one_way_distance_miles * 2.0;
}

/**
 * Calculate the airborne travel time for a single leg.
 *
 * Parameters
 * ----------
 * leg_distance_miles : double
 *     The distance to be traveled for one expedition leg.
 *
 * speed_mph : double
 *     The dragon's air speed in miles per hour.
 *
 * Returns
 * -------
 * leg_flight_time_seconds : double
 *     The number of seconds required to complete that leg.
 */
double calculate_leg_flight_time_seconds(
    double leg_distance_miles,
    double speed_mph
)
{
    if (speed_mph <= 0.0)
    {
        return 0.0;
    }

    return (leg_distance_miles / speed_mph) * SECONDS_PER_HOUR;
}

/**
 * Calculate how many rest stops are required for a travel segment.
 *
 * Parameters
 * ----------
 * travel_distance_miles : double
 *     The distance to be covered for the leg being evaluated.
 *
 * endurance_miles : double
 *     The maximum miles the dragon can cover before a rest stop.
 *
 * Returns
 * -------
 * rest_count : int
 *     The number of rest stops required for that segment.
 */
int calculate_rest_count(
    double travel_distance_miles,
    double endurance_miles
)
{
    if (endurance_miles <= 0.0)
    {
        return 0;
    }

    return static_cast<int>(travel_distance_miles / endurance_miles);
}

/**
 * Calculate how long loading the treasure takes.
 *
 * Parameters
 * ----------
 * treasure_quantity : int
 *     The number of treasure units to be loaded.
 *
 * Returns
 * -------
 * loading_time_seconds : double
 *     The loading duration in seconds.
 */
double calculate_loading_time_seconds(int treasure_quantity)
{
    return static_cast<double>(treasure_quantity) *
           static_cast<double>(SECONDS_PER_TREASURE_UNIT);
}

/**
 * Calculate the total time spent on expedition breaks.
 *
 * Parameters
 * ----------
 * rest_count : int
 *     The number of rest stops.
 *
 * snack_demand : int
 *     The number of sheep-sized snacks consumed per stop.
 *
 * Returns
 * -------
 * total_break_seconds : double
 *     The total duration spent on expedition breaks.
 */
double calculate_break_time_seconds(int rest_count, int snack_demand)
{
    const int BASE_REST_MINUTES = 15;
    const int MINUTES_PER_SNACK = 5;

    const double break_seconds_per_stop =
        static_cast<double>(
            BASE_REST_MINUTES + (snack_demand * MINUTES_PER_SNACK)
        ) * SECONDS_PER_MINUTE;

    const double total_break_seconds = rest_count * break_seconds_per_stop;

    return total_break_seconds;
}

/**
 * Calculate the snack cost for the trip.
 *
 * Parameters
 * ----------
 * rest_count : int
 *     The number of rest stops across the expedition.
 *
 * snack_demand : int
 *     The number of sheep-sized snacks required at each stop.
 *
 * Returns
 * -------
 * snack_cost : double
 *     The total snack cost assigned to the expedition.
 */
double calculate_snack_cost(int rest_count, int snack_demand)
{
    int total_snacks_consumed = rest_count * snack_demand;

    return static_cast<double>(total_snacks_consumed) * SNACK_COST;
}

/**
 * Calculate how valuable the treasure seems to the dragon.
 *
 * Parameters
 * ----------
 * total_value : double
 *     The treasure's base value before dragon personality is considered.
 *
 * greed_modifier : double
 *     A signed greed adjustment where 0.0 is neutral, positive values
 *     increase interest, and negative values reduce interest.
 *
 * travel_time_hours : double
 *     The airborne duration of the expedition, expressed in hours.
 *
 * time_sensitivity : int
 *     The amount of value that dragon loses per airborne hour.
 *
 * snack_cost : double
 *     The total snack cost assigned to the expedition.
 *
 * Returns
 * -------
 * perceived_value : double
 *     The dragon's final expedition evaluation after treasure value is
 *     reduced by time and snack costs and then adjusted by greed.
 */
double calculate_perceived_value(
    double total_value,
    double greed_modifier,
    double travel_time_hours,
    int time_sensitivity,
    double snack_cost
)
{
    const double GREED_INFLUENCE = 0.015;
    const double base_value =
        total_value -
        (travel_time_hours * static_cast<double>(time_sensitivity)) -
        snack_cost;
    const double greed_adjustment =
        total_value * (greed_modifier * GREED_INFLUENCE);

    return base_value + greed_adjustment;
}

/**
 * Determine whether the dragon accepts the expedition.
 *
 * Parameters
 * ----------
 * total_weight : double
 *     The weight of the treasure haul.
 *
 * carrying_capacity : double
 *     The maximum weight the dragon can carry.
 *
 * perceived_value : double
 *     The dragon's final expedition evaluation after all penalties.
 *
 * greed_threshold : double
 *     The minimum perceived value that dragon requires.
 *
 * Returns
 * -------
 * accepted : bool
 *     True when the dragon approves the expedition and false otherwise.
 */
bool expedition_is_accepted(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
)
{
    if (total_weight > carrying_capacity)
    {
        return false;
    }

    if (perceived_value < greed_threshold)
    {
        return false;
    }

    return true;
}

/**
 * Determine the decline reason when an expedition is rejected.
 *
 * Parameters
 * ----------
 * total_weight : double
 *     The weight of the treasure haul.
 *
 * carrying_capacity : double
 *     The maximum weight the dragon can carry.
 *
 * perceived_value : double
 *     The dragon's final expedition evaluation after all penalties.
 *
 * greed_threshold : double
 *     The minimum perceived value that dragon requires.
 *
 * Returns
 * -------
 * decline_reason : string
 *     A human-readable explanation for the decision outcome.
 */
string determine_decline_reason(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
)
{
    if (total_weight > carrying_capacity)
    {
        return "Load exceeds carrying capacity.";
    }

    if (perceived_value < greed_threshold)
    {
        return "Treasure value does not justify the effort.";
    }

    return "Expedition approved.";
}

/**
 * Calculate the broker commission for an expedition outcome.
 *
 * Parameters
 * ----------
 * perceived_value : double
 *     The dragon's final expedition evaluation after time and snack costs.
 *
 * expedition_accepted : bool
 *     Whether the dragon accepted the expedition.
 *
 * Returns
 * -------
 * broker_commission : double
 *     The broker commission earned for the accepted expedition.
 *
 * Notes
 * -----
 * Declined expeditions earn no commission. Accepted expeditions earn
 * ten percent of the perceived value.
 */
double calculate_broker_commission(
    double perceived_value,
    bool expedition_accepted
)
{
    if (!expedition_accepted)
    {
        return 0.0;
    }

    return perceived_value * BROKER_COMMISSION_RATE;
}

/**
 * Calculate the broker's final profit after snack costs.
 *
 * Parameters
 * ----------
 * broker_commission : double
 *     The commission earned from the expedition outcome.
 *
 * snack_cost : double
 *     The total snack cost paid for the expedition.
 *
 * Returns
 * -------
 * final_profit : double
 *     The broker's profit after snack cost reduces the player outcome.
 */
double calculate_final_profit(double broker_commission, double snack_cost)
{
    return broker_commission - snack_cost;
}

/**
 * Determine which dragon would yield the best accepted profit for the
 * current scenario.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure opportunity being evaluated.
 *
 * treasure_quantity : int
 *     The quantity of treasure available in that scenario.
 *
 * distance_miles : int
 *     The one-way distance to the treasure.
 *
 * best_dragon_found : bool&
 *     Output flag indicating whether any dragon accepted the scenario.
 *
 * best_final_profit : double&
 *     Output value receiving the best accepted final profit.
 *
 * best_perceived_value : double&
 *     Output value receiving the best accepted perceived-value score.
 *
 * Returns
 * -------
 * best_dragon_type : DragonType
 *     The dragon type that produces the best accepted result. When no
 *     dragon accepts, RED is returned only as a placeholder and the flag
 *     communicates that no valid best dragon exists.
 */
DragonType determine_best_dragon(
    TreasureType treasure_type,
    int treasure_quantity,
    int distance_miles,
    bool& best_dragon_found,
    double& best_final_profit,
    double& best_perceived_value
)
{
    const double weight_per_unit = get_weight_per_unit(treasure_type);
    const double value_per_pound = get_value_per_pound(treasure_type);
    const double total_weight = calculate_total_weight(
        treasure_quantity,
        weight_per_unit
    );
    const double total_value = calculate_absolute_value(
        total_weight,
        value_per_pound
    );
    const double one_way_distance_miles = static_cast<double>(distance_miles);

    DragonType best_dragon_type = RED;
    best_dragon_found = false;
    best_final_profit = 0.0;
    best_perceived_value = numeric_limits<double>::lowest();

    for (int dragon_index = static_cast<int>(RED);
        dragon_index <= static_cast<int>(BRASS);
        ++dragon_index
    )
    {
        DragonType candidate_dragon = static_cast<DragonType>(dragon_index);

        const double carrying_capacity = calculate_carrying_capacity(
            get_strength(candidate_dragon)
        );

        const double speed_mph = get_speed_mph(candidate_dragon);

        const double outbound_time_seconds = calculate_leg_flight_time_seconds(
            one_way_distance_miles,
            speed_mph
        );

        const double loaded_return_speed_mph = calculate_loaded_return_speed_mph(
            speed_mph,
            total_weight,
            carrying_capacity
        );

        const double return_time_seconds = calculate_leg_flight_time_seconds(
            one_way_distance_miles,
            loaded_return_speed_mph
        );

        const double endurance_miles = get_endurance_miles(candidate_dragon);
        const int outbound_rest_count = calculate_rest_count(
            one_way_distance_miles,
            endurance_miles
        );
        const int destination_recovery_count = 1;
        const int return_rest_count = calculate_rest_count(
            one_way_distance_miles,
            endurance_miles
        );
        const int total_rest_count =
            outbound_rest_count +
            destination_recovery_count +
            return_rest_count;
        const int snack_demand = get_snack_demand(candidate_dragon);

        const double snack_cost = calculate_snack_cost(
            total_rest_count,
            snack_demand
        );

        const double perceived_value = calculate_perceived_value(
            total_value,
            get_greed_modifier(candidate_dragon),
            (outbound_time_seconds + return_time_seconds) / SECONDS_PER_HOUR,
            get_time_sensitivity(candidate_dragon),
            snack_cost
        );

        const bool accepted = expedition_is_accepted(
            total_weight,
            carrying_capacity,
            perceived_value,
            get_greed_threshold(candidate_dragon)
        );

        const double commission = calculate_broker_commission(
            perceived_value,
            accepted
        );
        const double final_profit = calculate_final_profit(
            commission,
            snack_cost
        );

        if (!accepted)
        {
            continue;
        }

        if (!best_dragon_found ||
            final_profit > best_final_profit ||
            (final_profit == best_final_profit &&
             perceived_value > best_perceived_value))
        {
            best_dragon_found = true;
            best_dragon_type = candidate_dragon;
            best_final_profit = final_profit;
            best_perceived_value = perceived_value;
        }
    }

    return best_dragon_type;
}

/**
 * Displays one formatted balance test result line for a dragon.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The dragon being reported.
 * win_count : int
 *     The number of wins recorded for that dragon.
 * number_of_tests : int
 *     The total number of scenarios tested.
 *
 * Returns
 * -------
 * None
 */
void display_balance_test_line(
    DragonType dragon_type,
    int win_count,
    int number_of_tests
)
{
    const double win_percentage =
        static_cast<double>(win_count) / number_of_tests * 100.0;

    cout << left << setw(10) << get_dragon_name(dragon_type)
         << right << setw(8) << win_count << " wins"
         << setw(10) << fixed << setprecision(2) << win_percentage << "%"
         << endl;
}

/**
 * Performs a user-defined number of run-through tests of random scenarios
 * to help uncover balance issues.
 *
 * Returns
 * -------
 * None
 */
void run_balance_test()
{
    int dragon_win_counts[DRAGON_COUNT] = {};

    const string BALANCE_TEST_PROMPT =
        "How many tests should be run? ";

    int number_of_tests = get_bounded_int(
        BALANCE_TEST_PROMPT, 10, 10000
    );

    for (int i = 0; i < number_of_tests; ++i)
    {
        TreasureType treasure_type;
        int treasure_quantity;
        int distance_miles;

        generate_playable_scenario(
            treasure_type,
            treasure_quantity,
            distance_miles
        );

        bool best_dragon_found = false;
        double best_final_profit = 0.0;
        double best_perceived_value = 0.0;

        DragonType best_dragon = determine_best_dragon(
            treasure_type,
            treasure_quantity,
            distance_miles,
            best_dragon_found,
            best_final_profit,
            best_perceived_value
        );

        if (best_dragon_found)
        {
            int dragon_index = get_dragon_index(best_dragon);

            if (dragon_index != -1)
            {
                dragon_win_counts[dragon_index]++;
            }
        }
    }

    cout << "\nBalance Test Results\n";
    cout << "====================\n";
    cout << "Iterations: " << number_of_tests << "\n\n";

    for (int dragon_value = static_cast<int>(RED);
        dragon_value <= static_cast<int>(BRASS);
        ++dragon_value
    )
    {
        DragonType dragon_type = static_cast<DragonType>(dragon_value);
        int dragon_index = get_dragon_index(dragon_type);

        display_balance_test_line(
            dragon_type,
            dragon_win_counts[dragon_index],
            number_of_tests
        );
    }

    cout << endl;
}

/**
 * Build the broker performance review message.
 *
 * Parameters
 * ----------
 * chosen_dragon_type : DragonType
 *     The dragon selected by the user.
 *
 * best_dragon_type : DragonType
 *     The best accepted dragon available for that scenario.
 *
 * chosen_dragon_accepted : bool
 *     Indicates whether the user's selected dragon accepted the expedition.
 *
 * best_dragon_found : bool
 *     Indicates whether any dragon accepted the scenario.
 *
 * chosen_final_profit : double
 *     The final profit earned from the chosen dragon.
 *
 * best_final_profit : double
 *     The highest final profit available from an accepted dragon.
 *
 * Returns
 * -------
 * review_message : string
 *     A short performance summary for the broker's choice.
 */
string build_broker_review_message(
    DragonType chosen_dragon_type,
    DragonType best_dragon_type,
    bool chosen_dragon_accepted,
    bool best_dragon_found,
    double chosen_final_profit,
    double best_final_profit
)
{
    ostringstream review_stream;

    if (!best_dragon_found)
    {
        review_stream << "No dragon accepted this expedition. The scenario "
                      << "was too poor or too burdensome for the roster.";
        return review_stream.str();
    }

    if (chosen_dragon_type == best_dragon_type && chosen_dragon_accepted)
    {
        review_stream << "Excellent choice!!!\nYou picked the best dragon for "
                      << "this expedition.";
        return review_stream.str();
    }

    if (!chosen_dragon_accepted)
    {
        review_stream << "Your dragon declined the job, and\n"
                      << get_dragon_name(best_dragon_type)
                      << " would have been the strongest accepted choice.";
        return review_stream.str();
    }

    double missed_profit = best_final_profit - chosen_final_profit;

    if (missed_profit > 0.005)
    {
        review_stream << "You did not pick the best dragon.\n"
                      << get_dragon_name(best_dragon_type)
                      << " would have earned an additional $"
                      << format_with_commas(missed_profit, 2)
                      << " in final profit.";
    }
    else
    {
        review_stream << "Your dragon matched the best available final profit,\nthough "
                      << get_dragon_name(best_dragon_type)
                      << " was the stronger overall choice based on perceived value.";
    }

    return review_stream.str();
}

/**
 * Display the full expedition report, including the detailed expedition
 * breakdown and broker performance review.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure opportunity generated for the scenario.
 *
 * treasure_quantity : int
 *     The number of treasure units available.
 *
 * distance_miles : int
 *     The one-way distance to the treasure site.
 *
 * dragon_type : DragonType
 *     The dragon selected by the user.
 *
 * snack_demand : int
 *     The number of sheep-sized snacks consumed at each stop.
 *
 * destination_recovery_count : int
 *     The guaranteed recovery event that occurs at the destination between
 *     the outbound and return legs.
 *
 * total_weight : double
 *     The total treasure weight in pounds.
 *
 * total_value : double
 *     The base treasure value before dragon personality is applied.
 *
 * outbound_flight_time_seconds : double
 *     The airborne time for the outbound leg.
 *
 * return_flight_time_seconds : double
 *     The airborne time for the return leg.
 *
 * outbound_rest_count : int
 *     The number of rest stops on the outbound leg.
 *
 * return_rest_count : int
 *     The number of rest stops on the return leg.
 *
 * total_break_time_seconds : double
 *     The total duration spent on expedition breaks.
 *
 * loading_time_seconds : double
 *     The time required to load the treasure haul.
 *
 * snack_cost : double
 *     The total snack cost assigned to the expedition.
 *
 * perceived_value : double
 *     The dragon's final expedition evaluation after time and snack costs
 *     reduce treasure value and a moderated greed bonus is added.
 *
 * accepted : bool
 *     Indicates whether the chosen dragon accepted the expedition.
 *
 * decision_reason : const string&
 *     The explanation associated with the decision outcome.
 *
 * broker_commission : double
 *     The commission earned by the broker from the chosen dragon.
 *
 * final_profit : double
 *     The broker's commission after snack cost reduces the player outcome.
 *
 * best_dragon_type : DragonType
 *     The best accepted dragon available for the scenario.
 *
 * best_dragon_found : bool
 *     Indicates whether any dragon accepted the scenario.
 *
 * best_final_profit : double
 *     The highest final profit available for the scenario.
 */
void display_expedition_report(
    TreasureType treasure_type,
    int treasure_quantity,
    int distance_miles,
    DragonType dragon_type,
    int snack_demand,
    int destination_recovery_count,
    double total_weight,
    double total_value,
    double outbound_flight_time_seconds,
    double return_flight_time_seconds,
    int outbound_rest_count,
    int return_rest_count,
    double total_break_time_seconds,
    double loading_time_seconds,
    double snack_cost,
    double perceived_value,
    bool accepted,
    const string& decision_reason,
    double broker_commission,
    double final_profit,
    DragonType best_dragon_type,
    bool best_dragon_found,
    double best_final_profit
)
{
    const double outbound_break_time_seconds = calculate_break_time_seconds(
        outbound_rest_count,
        snack_demand
    );
    const double destination_recovery_break_time_seconds =
        calculate_break_time_seconds(
            destination_recovery_count,
            snack_demand
        );
    const double return_break_time_seconds = calculate_break_time_seconds(
        return_rest_count,
        snack_demand
    );

    const int outbound_snacks_consumed = outbound_rest_count * snack_demand;
    const int destination_recovery_snacks_consumed =
        destination_recovery_count * snack_demand;
    const int return_snacks_consumed = return_rest_count * snack_demand;
    const int total_rest_count =
        outbound_rest_count +
        destination_recovery_count +
        return_rest_count;
    const int total_snacks_consumed =
        outbound_snacks_consumed +
        destination_recovery_snacks_consumed +
        return_snacks_consumed;
    const double outbound_leg_total_seconds =
        outbound_flight_time_seconds + outbound_break_time_seconds;
    const double destination_total_seconds =
    destination_recovery_break_time_seconds + loading_time_seconds;
    const double return_leg_total_seconds =
        return_flight_time_seconds + return_break_time_seconds;
    const double total_elapsed_time_seconds =
        outbound_flight_time_seconds +
        return_flight_time_seconds +
        total_break_time_seconds +
        loading_time_seconds;
    const string broker_review_message = build_broker_review_message(
        dragon_type,
        best_dragon_type,
        accepted,
        best_dragon_found,
        final_profit,
        best_final_profit
    );

    cout << endl;
    cout << "============================================================" << endl;
    cout << "                DRAGON BROKER EXPEDITION REPORT             " << endl;
    cout << "============================================================" << endl;

    cout << left << setw(24) << "Dragon"
         << ": " << get_dragon_name(dragon_type) << endl;
    cout << left << setw(24) << "Treasure"
         << ": " << get_treasure_name(treasure_type) << endl;
    cout << left << setw(24) << "Distance"
         << ": " << format_with_commas(distance_miles, 0)
         << " miles each way" << endl;
    cout << left << setw(24) << "Treasure quantity"
         << ": " << format_with_commas(treasure_quantity, 0) << endl;
    cout << left << setw(24) << "Treasure weight"
         << ": " << format_with_commas(total_weight, 2)
         << " pounds" << endl;

    cout << endl;
    cout << "----------------------- OUTBOUND LEG -----------------------" << endl;
    cout << left << setw(24) << "Flight time"
         << ": " << format_time_wdhms(outbound_flight_time_seconds) << endl;
    cout << left << setw(24) << "Rest stops"
         << ": " << format_with_commas(outbound_rest_count, 0) << endl;
    cout << left << setw(24) << "Sheep-sized snacks"
         << ": " << format_with_commas(outbound_snacks_consumed, 0) << endl;
    cout << left << setw(24) << "Break time"
         << ": " << format_time_wdhms(outbound_break_time_seconds) << endl;
    cout << left << setw(24) << "Outbound total"
         << ": " << format_time_wdhms(outbound_leg_total_seconds) << endl;

    cout << endl;
    cout << "------------ DESTINATION LOADING AND RECOVERY --------------" << endl;
    cout << left << setw(24) << "Loading time"
         << ": " << format_time_wdhms(loading_time_seconds) << endl;
    cout << left << setw(24) << "Sheep-sized snacks"
         << ": " << format_with_commas(destination_recovery_snacks_consumed, 0)
         << endl;
    cout << left << setw(24) << "Break time"
         << ": "
         << format_time_wdhms(destination_recovery_break_time_seconds)
         << endl;
    cout << left << setw(24) << "Loading and break time"
         << ": "
         << format_time_wdhms(destination_total_seconds)
         << endl;

    cout << endl;
    cout << "------------------------ RETURN LEG ------------------------" << endl;
    cout << left << setw(24) << "Flight time"
         << ": " << format_time_wdhms(return_flight_time_seconds) << endl;
    cout << left << setw(24) << "Rest stops"
         << ": " << format_with_commas(return_rest_count, 0) << endl;
    cout << left << setw(24) << "Sheep-sized snacks"
         << ": " << format_with_commas(return_snacks_consumed, 0) << endl;
    cout << left << setw(24) << "Break time"
         << ": " << format_time_wdhms(return_break_time_seconds) << endl;
    cout << left << setw(24) << "Return total"
         << ": " << format_time_wdhms(return_leg_total_seconds) << endl;

    cout << endl;
    cout << "------------------------ TOTALS ----------------------------" << endl;
    cout << left << setw(24) << "Total rest stops"
         << ": " << format_with_commas(total_rest_count, 0) << endl;
    cout << left << setw(24) << "Total snacks consumed"
         << ": " << format_with_commas(total_snacks_consumed, 0) << endl;
    cout << left << setw(24) << "Total break time"
         << ": " << format_time_wdhms(total_break_time_seconds) << endl;
    cout << left << setw(24) << "Loading time"
         << ": " << format_time_wdhms(loading_time_seconds) << endl;
    cout << left << setw(24) << "Total elapsed time"
         << ": " << format_time_wdhms(total_elapsed_time_seconds) << endl;

    cout << endl;
    cout << "-------------------- EXPEDITION STATUS ---------------------" << endl;
    cout << left << setw(24) << "Treasure value"
         << ": $" << format_with_commas(total_value, 2) << endl;
    cout << left << setw(24) << "Perceived value"
         << ": $" << format_with_commas(perceived_value, 2) << endl;
    cout << left << setw(24) << "Snack cost"
         << ": $" << format_with_commas(snack_cost, 2) << endl;
    cout << left << setw(24) << "Accepted"
         << ": " << (accepted ? "Yes" : "No") << endl;
    cout << left << setw(24) << "Decision"
         << ": " << decision_reason << endl;
    cout << left << setw(24) << "Broker commission"
         << ": $" << format_with_commas(broker_commission, 2) << endl;
    cout << left << setw(24) << "Final profit"
         << ": $" << format_with_commas(final_profit, 2) << endl;

    cout << endl;
    cout << "---------------------- BROKER REVIEW -----------------------" << endl;
    cout << broker_review_message << endl;

    cout << "============================================================" << endl;
}
