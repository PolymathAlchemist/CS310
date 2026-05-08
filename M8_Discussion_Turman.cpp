/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M8 Discussion
 * Author: Eric J. Turman
 * Date: 2026-05-04
 *
 * Description:
 * A class-based Dragon Broker simulation for the M8 discussion assignment.
 * The program presents a randomized treasure expedition scenario and asks
 * the user to select the dragon they believe is best suited for the job.
 *
 * Dragon and Treasure are represented as separate classes so their related
 * data and behavior stay grouped with the concepts they describe. Simple
 * structs are used only for grouped expedition data that is passed between
 * helper functions.
 *
 * Notes:
 * [[nodiscard]] is used selectively on getters, calculations, factories,
 * and formatting helpers where ignoring the returned value would usually
 * indicate a mistake.
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

// ========================================================================
// Constants
// ========================================================================
constexpr int MIN_DISTANCE_MILES = 100;
constexpr int MAX_DISTANCE_MILES = 500;
constexpr int MIN_TREASURE_QUANTITY = 50;
constexpr int MAX_TREASURE_QUANTITY = 300;
constexpr int DRAGON_COUNT = 10;
constexpr int TREASURE_COUNT = 9;
constexpr double SNACK_COST = 50.0;
constexpr double BROKER_COMMISSION_RATE = 0.10;
constexpr int SECONDS_PER_TREASURE_UNIT = 1;
constexpr double SECONDS_PER_HOUR = 3600.0;
constexpr double GREED_INFLUENCE = 0.015;

// ========================================================================
// Helper Function Prototype Declarations
// ========================================================================
[[nodiscard]] double calculate_carrying_capacity(int strength);

[[nodiscard]] double calculate_loaded_return_speed_mph(
    double base_speed_mph,
    double total_weight,
    double carrying_capacity
);

[[nodiscard]] bool expedition_is_accepted(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
);

// ========================================================================
// Classes
// ========================================================================

/**
 * Represent one dragon in the broker roster.
 *
 * The class keeps dragon-specific data near the small behaviors that depend
 * on that data. This replaces scattered array lookups with object-oriented
 * access while still leaving the roster simple enough for the current module.
 */
class Dragon
{
private:
    DragonType type;
    string name;
    string description;
    int strength;
    double greed_modifier;
    double greed_threshold;
    double speed_mph;
    double endurance_miles;
    int snack_demand;
    int time_sensitivity;

public:
    /**
     * Initialize one dragon with the values used by the broker simulation.
     *
     * Parameters
     * ----------
     * dragon_type : DragonType
     *     Enum value used to identify the dragon.
     *
     * dragon_name : string
     *     Display name used in menus and reports.
     *
     * dragon_description : string
     *     Broker-facing explanation of the dragon's personality.
     *
     * dragon_strength : int
     *     Carrying strength used to calculate capacity.
     *
     * dragon_greed_modifier : double
     *     Adjustment used in perceived value calculations.
     *
     * dragon_greed_threshold : double
     *     Minimum perceived value required before the dragon accepts.
     *
     * dragon_speed_mph : double
     *     Unloaded flight speed in miles per hour.
     *
     * dragon_endurance_miles : double
     *     Miles the dragon travels before requiring a rest stop.
     *
     * dragon_snack_demand : int
     *     Number of sheep-sized snacks consumed at each rest stop.
     *
     * dragon_time_sensitivity : int
     *     Penalty factor applied to long expedition travel times.
     */
    Dragon(
        DragonType dragon_type,
        const string& dragon_name,
        const string& dragon_description,
        int dragon_strength,
        double dragon_greed_modifier,
        double dragon_greed_threshold,
        double dragon_speed_mph,
        double dragon_endurance_miles,
        int dragon_snack_demand,
        int dragon_time_sensitivity
    ) :
        type(dragon_type),
        name(dragon_name),
        description(dragon_description),
        greed_modifier(dragon_greed_modifier),
        greed_threshold(dragon_greed_threshold),
        speed_mph(dragon_speed_mph),
        endurance_miles(dragon_endurance_miles),
        snack_demand(dragon_snack_demand),
        time_sensitivity(dragon_time_sensitivity)
    {
    }

    /**
     * Return the enum value that identifies this dragon.
     *
     * Returns
     * -------
     * type : DragonType
     *     The dragon's enum identifier.
     */
    [[nodiscard]] DragonType get_type() const
    {
        return type;
    }

    /**
     * Return the dragon name used in menus and reports.
     *
     * Returns
     * -------
     * name : string
     *     The display name for this dragon.
     */
    [[nodiscard]] string get_name() const
    {
        return name;
    }

    /**
     * Return the broker-facing dragon description.
     *
     * Returns
     * -------
     * description : string
     *     The description shown in broker notes.
     */
    [[nodiscard]] string get_description() const
    {
        return description;
    }

    /**
     * Return the strength value used for carrying capacity.
     *
     * Returns
     * -------
     * strength : int
     *     The dragon's carrying strength rating.
     */
    [[nodiscard]] int get_strength() const
    {
        return strength;
    }

    /**
     * Return the greed adjustment used in perceived value calculations.
     *
     * Returns
     * -------
     * greed_modifier : double
     *     The dragon's value adjustment factor.
     */
    [[nodiscard]] double get_greed_modifier() const
    {
        return greed_modifier;
    }

    /**
     * Return the minimum perceived value this dragon requires.
     *
     * Returns
     * -------
     * greed_threshold : double
     *     The dragon's acceptance threshold.
     */
    [[nodiscard]] double get_greed_threshold() const
    {
        return greed_threshold;
    }

    /**
     * Return the dragon's unloaded travel speed.
     *
     * Returns
     * -------
     * speed_mph : double
     *     The flight speed in miles per hour.
     */
    [[nodiscard]] double get_speed_mph() const
    {
        return speed_mph;
    }

    /**
     * Return the distance the dragon can travel before resting.
     *
     * Returns
     * -------
     * endurance_miles : double
     *     The endurance distance in miles.
     */
    [[nodiscard]] double get_endurance_miles() const
    {
        return endurance_miles;
    }

    /**
     * Return the snack demand used at each rest stop.
     *
     * Returns
     * -------
     * snack_demand : int
     *     The number of sheep-sized snacks consumed per stop.
     */
    [[nodiscard]] int get_snack_demand() const
    {
        return snack_demand;
    }

    /**
     * Return the time sensitivity used for travel penalties.
     *
     * Returns
     * -------
     * time_sensitivity : int
     *     The dragon's travel-time penalty factor.
     */
    [[nodiscard]] int get_time_sensitivity() const
    {
        return time_sensitivity;
    }

    /**
     * Return carrying capacity derived from this dragon's strength.
     *
     * Returns
     * -------
     * carrying_capacity : double
     *     The maximum treasure weight this dragon can carry.
     */
    [[nodiscard]] double get_carrying_capacity() const
    {
        return calculate_carrying_capacity(strength);
    }

    /**
     * Calculate the dragon's return speed while carrying treasure.
     *
     * Parameters
     * ----------
     * total_weight : double
     *     The total treasure weight carried on the return leg.
     *
     * Returns
     * -------
     * loaded_return_speed_mph : double
     *     The adjusted return speed in miles per hour.
     */
    [[nodiscard]] double calculate_loaded_return_speed_mph(
        double total_weight
    ) const
    {
        return ::calculate_loaded_return_speed_mph(
            speed_mph,
            total_weight,
            get_carrying_capacity()
        );
    }

    /**
     * Determine whether this dragon accepts an evaluated expedition.
     *
     * Parameters
     * ----------
     * total_weight : double
     *     The total treasure weight the dragon would carry.
     *
     * perceived_value : double
     *     The expedition value after personality and cost adjustments.
     *
     * Returns
     * -------
     * accepted : bool
     *     True when capacity and perceived value meet this dragon's limits.
     */
    [[nodiscard]] bool accepts_expedition(
        double total_weight,
        double perceived_value
    ) const
    {
        return expedition_is_accepted(
            total_weight,
            get_carrying_capacity(),
            perceived_value,
            greed_threshold
        );
    }
};

/**
 * Represent one treasure type available to the broker.
 *
 * The class groups treasure identity, display text, and value calculations
 * so the expedition logic can ask the object for weights and values instead
 * of reaching into parallel data tables.
 */
class Treasure
{
private:
    TreasureType type;
    string name;
    string description;
    double weight_per_unit;
    double value_per_pound;

public:
    /**
     * Initialize one treasure type with the values used by the simulation.
     *
     * Parameters
     * ----------
     * treasure_type : TreasureType
     *     Enum value used to identify the treasure.
     *
     * treasure_name : string
     *     Display name used in menus and reports.
     *
     * treasure_description : string
     *     Broker-facing explanation of the treasure.
     *
     * treasure_weight_per_unit : double
     *     Pounds represented by one treasure unit.
     *
     * treasure_value_per_pound : double
     *     Base value assigned to one pound of the treasure.
     */
    Treasure(
        TreasureType treasure_type,
        const string& treasure_name,
        const string& treasure_description,
        double treasure_weight_per_unit,
        double treasure_value_per_pound
    ) :
        type(treasure_type),
        name(treasure_name),
        description(treasure_description),
        weight_per_unit(treasure_weight_per_unit),
        value_per_pound(treasure_value_per_pound)
    {
    }

    /**
     * Return the enum value that identifies this treasure.
     *
     * Returns
     * -------
     * type : TreasureType
     *     The treasure's enum identifier.
     */
    [[nodiscard]] TreasureType get_type() const
    {
        return type;
    }

    /**
     * Return the treasure name used in menus and reports.
     *
     * Returns
     * -------
     * name : string
     *     The display name for this treasure type.
     */
    [[nodiscard]] string get_name() const
    {
        return name;
    }

    /**
     * Return the broker-facing treasure description.
     *
     * Returns
     * -------
     * description : string
     *     The description shown in broker notes.
     */
    [[nodiscard]] string get_description() const
    {
        return description;
    }

    /**
     * Return how much one treasure unit weighs.
     *
     * Returns
     * -------
     * weight_per_unit : double
     *     The unit weight in pounds.
     */
    [[nodiscard]] double get_weight_per_unit() const
    {
        return weight_per_unit;
    }

    /**
     * Return the treasure value density.
     *
     * Returns
     * -------
     * value_per_pound : double
     *     The base treasure value per pound.
     */
    [[nodiscard]] double get_value_per_pound() const
    {
        return value_per_pound;
    }

    /**
     * Calculate total carried weight for a treasure quantity.
     *
     * Parameters
     * ----------
     * treasure_quantity : int
     *     The number of treasure units in the scenario.
     *
     * Returns
     * -------
     * total_weight : double
     *     The total treasure weight in pounds.
     */
    [[nodiscard]] double calculate_total_weight(int treasure_quantity) const
    {
        return treasure_quantity * weight_per_unit;
    }

    /**
     * Calculate absolute treasure value for a treasure quantity.
     *
     * Parameters
     * ----------
     * treasure_quantity : int
     *     The number of treasure units in the scenario.
     *
     * Returns
     * -------
     * absolute_value : double
     *     The base treasure value before dragon adjustments.
     */
    [[nodiscard]] double calculate_absolute_value(
        int treasure_quantity
    ) const
    {
        return calculate_total_weight(treasure_quantity) * value_per_pound;
    }
};

// ========================================================================
// Structures
// ========================================================================

/**
 * Group the randomized opportunity before any dragon is chosen.
 *
 * This struct stays simple because the scenario is passed between
 * generation, evaluation, and reporting helpers as one unit.
 */
struct ExpeditionScenario
{
    TreasureType treasure_type = GOLD_COIN;
    int treasure_quantity = 0;
    int distance_miles = 0;
};

/**
 * Group the full outcome of evaluating one dragon against one scenario.
 *
 * This struct keeps the many report values together without turning the
 * report pipeline into a long list of repeated parameters.
 */
struct ExpeditionEvaluation
{
    DragonType dragon_type = RED;
    TreasureType treasure_type = GOLD_COIN;
    int treasure_quantity = 0;
    int distance_miles = 0;
    int snack_demand = 0;
    int destination_recovery_count = 0;
    double total_weight = 0.0;
    double total_value = 0.0;
    double outbound_flight_time_seconds = 0.0;
    double return_flight_time_seconds = 0.0;
    int outbound_rest_count = 0;
    int return_rest_count = 0;
    double total_break_time_seconds = 0.0;
    double loading_time_seconds = 0.0;
    double snack_cost = 0.0;
    double perceived_value = 0.0;
    bool accepted = false;
    string decision_reason;
    double broker_commission = 0.0;
    double final_profit = 0.0;
};

// ========================================================================
// Function Prototype Declarations
// ========================================================================
[[nodiscard]] string format_with_commas(
    double numeric_value,
    int decimal_precision
);

[[nodiscard]] string get_plurality(
    int quantity,
    const string& singular_word,
    const string& plural_word
);

[[nodiscard]] string format_time_wdhms(double total_seconds);

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

[[nodiscard]] Dragon build_dragon(DragonType dragon_type);

[[nodiscard]] Treasure build_treasure(TreasureType treasure_type);

void broker_notes_loop(TreasureType treasure_type);

int display_broker_notes_menu();

void display_treasure_description(TreasureType treasure_type);

DragonType get_dragon_choice_from_user();

int get_dragon_index(DragonType dragon_type);

mt19937& get_random_engine();

TreasureType generate_random_treasure_type();

int generate_random_distance_miles();

int generate_random_treasure_quantity();

bool is_playable_scenario(const ExpeditionScenario& scenario);

ExpeditionScenario generate_playable_scenario();

void display_dragon_descriptions(DragonType first, DragonType last);

[[nodiscard]] double calculate_total_weight(
    int treasure_quantity,
    double weight_per_unit
);

[[nodiscard]] double calculate_absolute_value(
    double total_weight,
    double value_per_pound
);

[[nodiscard]] double calculate_round_trip_distance(
    double one_way_distance_miles
);

[[nodiscard]] double calculate_leg_flight_time_seconds(
    double leg_distance_miles,
    double speed_mph
);

[[nodiscard]] int calculate_rest_count(
    double travel_distance_miles,
    double endurance_miles
);

[[nodiscard]] double calculate_loading_time_seconds(int treasure_quantity);

[[nodiscard]] double calculate_break_time_seconds(
    int rest_count,
    int snack_demand
);

[[nodiscard]] double calculate_snack_cost(int rest_count, int snack_demand);

[[nodiscard]] double calculate_perceived_value(
    double total_value,
    double greed_modifier,
    double travel_time_hours,
    int time_sensitivity,
    double snack_cost
);

string determine_decline_reason(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
);

[[nodiscard]] double calculate_broker_commission(
    double perceived_value,
    bool accepted
);

[[nodiscard]] double calculate_final_profit(
    double broker_commission,
    double snack_cost
);

ExpeditionEvaluation evaluate_expedition(
    const ExpeditionScenario& scenario,
    const Dragon& chosen_dragon
);

DragonType determine_best_dragon(
    const ExpeditionScenario& scenario,
    bool& best_dragon_found,
    double& best_final_profit,
    double& best_perceived_value
);

void run_balance_test();

void display_balance_test_line(
    DragonType dragon_type,
    int win_count,
    int number_of_tests
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
    const ExpeditionEvaluation& evaluation,
    DragonType best_dragon_type,
    bool best_dragon_found,
    double best_final_profit
);

// ========================================================================
// Main Execution
// ========================================================================
/**
 * Run the Dragon Broker simulation.
 *
 * Returns
 * -------
 * status_code : int
 *     Zero when the program completes normally.
 */
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

    const string PLAY_AGAIN_PROMPT =
        "\nBroker another treasure expedition? [Y] or press [Enter] to exit: ";

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
        const ExpeditionScenario scenario = generate_playable_scenario();
        const Treasure scenario_treasure = build_treasure(
            scenario.treasure_type
        );

        cout << "\nExpedition Opportunity" << endl;
        cout << "----------------------" << endl;
        cout << "Treasure type: "
             << scenario_treasure.get_name()
             << endl;
        cout << "Treasure quantity: "
             << format_with_commas(scenario.quantity, 0)
             << endl;
        cout << "One-way distance: "
             << format_with_commas(scenario.distance_miles, 0)
             << " miles"
             << endl;

        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        broker_notes_loop(scenario.treasure_type);

        const DragonType dragon_type = get_dragon_choice_from_user();
        const Dragon chosen_dragon = build_dragon(dragon_type);

        // ----------------------------------------------------------------
        // Processing
        // ----------------------------------------------------------------
        const ExpeditionEvaluation evaluation = evaluate_expedition(
            scenario,
            chosen_dragon
        );

        bool best_dragon_found = false;
        double best_final_profit = 0.0;
        double best_perceived_value = 0.0;

        const DragonType best_dragon_type = determine_best_dragon(
            scenario,
            best_dragon_found,
            best_final_profit,
            best_perceived_value
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        display_expedition_report(
            evaluation,
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
 * Return the singular or plural form of a word based on a count.
 *
 * Parameters
 * ----------
 * quantity : int
 *     The count used to choose the word form.
 *
 * singular_word : const string&
 *     The word form used when quantity equals one.
 *
 * plural_word : const string&
 *     The word form used for all other quantities.
 *
 * Returns
 * -------
 * selected_word : string
 *     The word form that matches the quantity.
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
 *     The elapsed time to convert into readable text.
 *
 * Returns
 * -------
 * formatted_time : string
 *     The duration formatted as weeks, days, hours, minutes, and seconds.
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

    if (whole_seconds == 0)
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
 *     The prompt displayed before input is read.
 *
 * true_association : const string&
 *     The accepted input value that maps to true.
 *
 * false_association : const string&
 *     The accepted input value that maps to false.
 *
 * Returns
 * -------
 * user_choice : bool
 *     True when the user enters the true association; otherwise false.
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
    bool user_choice = false;
    bool input_is_valid = false;

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

    while (!input_is_valid)
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
            user_choice = true;
            input_is_valid = true;
        }
        else if (normalized_input == normalized_false_association)
        {
            user_choice = false;
            input_is_valid = true;
        }
        else
        {
            string true_display =
                true_association.empty() ?
                "[Enter]" :
                "[" + true_association + "]";

            string false_display =
                false_association.empty() ?
                "[Enter]" :
                "[" + false_association + "]";

            cout << "Error: Please enter "
                 << true_display
                 << " or "
                 << false_display
                 << "."
                 << endl;
        }
    }

    return user_choice;
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
int get_bounded_int(
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
 * Build the optional expedition briefing message.
 *
 * Returns
 * -------
 * briefing_message : string
 *     The explanatory text shown when the user requests a briefing.
 */
string build_briefing_message()
{
    ostringstream output_stream;

    output_stream << "Dragon Broker Briefing" << endl;
    output_stream << "----------------------" << endl;
    output_stream << "A treasure opportunity will be generated with a random "
                  << "treasure type, quantity,\nand one-way distance. You "
                  << "must then select the dragon you believe is best suited\n"
                  << "for the job. In this version, each dragon and treasure "
                  << "type is represented\nby a class object instead of a row "
                  << "in a two-dimensional array.\n"
                  << endl;
    output_stream << "The chosen dragon evaluates the expedition by comparing "
                  << "the treasure's value\nagainst carrying limits, time "
                  << "pressure, and snack cost. If the dragon accepts,\nyou "
                  << "earn a broker commission of around "
                  << format_with_commas(BROKER_COMMISSION_RATE * 100.0, 0)
                  << "% of what the dragon believes the\ntreasure is worth."
                  << endl;

    return output_stream.str();
}

/**
 * Build one dragon object from its enum type.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The enum value identifying which dragon to build.
 *
 * Returns
 * -------
 * dragon : Dragon
 *     A Dragon object populated with the matching roster values.
 */
Dragon build_dragon(DragonType dragon_type)
{
    switch (static_cast<int>(dragon_type))
    {
    case RED:
        return Dragon(
            RED,
            "Red",
            "A domineering force that considers lesser hoards beneath its notice.\n"
            "When it moves, it expects the reward to justify its presence.",
            10,
            1.0,
            20000.0,
            140.0,
            140.0,
            6,
            60
        );
    case BLUE:
        return Dragon(
            BLUE,
            "Blue",
            "Calculating and precise, it favors outcomes that are worth the effort.\n"
            "Rarely acts without weighing the long-term gain.",
            8,
            0.7,
            15000.0,
            135.0,
            180.0,
            4,
            30
        );
    case GREEN:
        return Dragon(
            GREEN,
            "Green",
            "Cunning and patient, with a preference for outcomes that offer\n"
            "more than they first appear to promise.",
            7,
            0.5,
            13000.0,
            138.0,
            160.0,
            3,
            35
        );
    case BLACK:
        return Dragon(
            BLACK,
            "Black",
            "Opportunistic and enduring, willing to tolerate a great deal\n"
            "if the outcome leans in its favor.",
            7,
            0.3,
            10000.0,
            120.0,
            200.0,
            4,
            25
        );
    case WHITE:
        return Dragon(
            WHITE,
            "White",
            "Quick to act and quicker to abandon an uninteresting pursuit.\n"
            "Instinct often outweighs deliberation.",
            5,
            -0.1,
            6000.0,
            165.0,
            90.0,
            2,
            80
        );
    case GOLD:
        return Dragon(
            GOLD,
            "Gold",
            "Ancient and deliberate, not easily swayed by mere wealth.\n"
            "Chooses its undertakings with great care.",
            10,
            0.2,
            18000.0,
            130.0,
            260.0,
            8,
            20
        );
    case SILVER:
        return Dragon(
            SILVER,
            "Silver",
            "Swift and capable, with a balanced outlook on effort and reward.\n"
            "Favors efficiency and clean outcomes.",
            7,
            0.1,
            12000.0,
            170.0,
            170.0,
            3,
            25
        );
    case BRONZE:
        return Dragon(
            BRONZE,
            "Bronze",
            "Resolute and dependable, willing to commit when the endeavor\n"
            "appears worthwhile.",
            8,
            0.2,
            13000.0,
            145.0,
            220.0,
            4,
            20
        );
    case COPPER:
        return Dragon(
            COPPER,
            "Copper",
            "Easily distracted and rarely committed to long, tedious endeavors.\n"
            "Prefers engagements that are as interesting as they are rewarding.",
            6,
            0.0,
            9000.0,
            160.0,
            150.0,
            3,
            45
        );
    case BRASS:
        return Dragon(
            BRASS,
            "Brass",
            "Curious and talkative, but not particularly driven by labor-intensive\n"
            "pursuits. Favors lighter engagements.",
            6,
            -0.1,
            8000.0,
            155.0,
            140.0,
            3,
            55
        );
    default:
        return build_dragon(RED);
    }
}

/**
 * Build one treasure object from its enum type.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The enum value identifying which treasure to build.
 *
 * Returns
 * -------
 * treasure : Treasure
 *     A Treasure object populated with the matching treasure values.
 */
Treasure build_treasure(TreasureType treasure_type)
{
    switch (static_cast<int>(treasure_type))
    {
    case GOLD_COIN:
        return Treasure(
            GOLD_COIN,
            "Gold coins",
            "Compact and widely valued, a classic and reliable form of wealth.",
            0.02,
            950.0
        );
    case GOLD_CHALICE:
        return Treasure(
            GOLD_CHALICE,
            "Gold chalices",
            "Ornate and impressive, though not always the most practical to transport.",
            3.00,
            700.0
        );
    case SILVER_COIN:
        return Treasure(
            SILVER_COIN,
            "Silver coins",
            "Common and easy to handle, but not especially remarkable in value.",
            0.02,
            120.0
        );
    case SILVER_PLATTER:
        return Treasure(
            SILVER_PLATTER,
            "Silver platters",
            "Large and cumbersome, with more presence than efficient value.",
            5.00,
            90.0
        );
    case PLATINUM_COIN:
        return Treasure(
            PLATINUM_COIN,
            "Platinum coins",
            "Dense and highly valued, offering substantial worth in a small form.",
            0.02,
            1600.0
        );
    case PLATINUM_RING:
        return Treasure(
            PLATINUM_RING,
            "Platinum rings",
            "Small, refined, and exceptionally valuable for their size.",
            0.10,
            2200.0
        );
    case RAW_GEM:
        return Treasure(
            RAW_GEM,
            "Raw gems",
            "Unrefined and full of potential, though not yet at their peak worth.",
            0.50,
            1800.0
        );
    case CUT_GEM:
        return Treasure(
            CUT_GEM,
            "Cut gems",
            "Precisely shaped and brilliantly valuable, prized for their refinement.",
            0.30,
            3500.0
        );
    case JEWELRY:
        return Treasure(
            JEWELRY,
            "Jewelry",
            "Crafted pieces of beauty and value, balancing form and worth.",
            1.20,
            2600.0
        );
    default:
        return build_treasure(GOLD_COIN);
    }
}

/**
 * Provide access to a shared pseudo-random number generator.
 *
 * Returns
 * -------
 * random_engine : mt19937&
 *     The single random engine reused by scenario-generation helpers.
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
 *     A randomly selected treasure enum value.
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
 *     A random distance within the configured scenario range.
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
 *     A random quantity within the configured treasure range.
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
 * Determine whether a generated scenario has a meaningful mix of dragon
 * decisions.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The generated opportunity to test against the dragon roster.
 *
 * Returns
 * -------
 * playable : bool
 *     True when at least two dragons accept and at least two decline.
 */
bool is_playable_scenario(const ExpeditionScenario& scenario)
{
    int accepted_count = 0;

    for (int dragon_index = static_cast<int>(RED);
        dragon_index <= static_cast<int>(BRASS);
        ++dragon_index)
    {
        const Dragon dragon = build_dragon(
            static_cast<DragonType>(dragon_index)
        );
        const ExpeditionEvaluation evaluation = evaluate_expedition(
            scenario,
            dragon
        );

        accepted_count += static_cast<int>(evaluation.accepted);
    }

    const int declined_count = DRAGON_COUNT - accepted_count;

    return accepted_count >= 2 && declined_count >= 2;
}

/**
 * Generate a randomized expedition scenario until it falls within the
 * playable acceptance range for the dragon roster.
 *
 * Returns
 * -------
 * scenario : ExpeditionScenario
 *     A randomized scenario with a useful mix of dragon outcomes.
 */
ExpeditionScenario generate_playable_scenario()
{
    ExpeditionScenario scenario;

    do
    {
        scenario.treasure_type = generate_random_treasure_type();
        scenario.treasure_quantity = generate_random_treasure_quantity();
        scenario.distance_miles = generate_random_distance_miles();
    } while (!is_playable_scenario(scenario));

    return scenario;
}

/**
 * Display a grouped set of dragon descriptions.
 *
 * Parameters
 * ----------
 * first : DragonType
 *     The first dragon enum value in the displayed range.
 *
 * last : DragonType
 *     The last dragon enum value in the displayed range.
 */
void display_dragon_descriptions(DragonType first, DragonType last)
{
    for (int dragon_value = static_cast<int>(first);
        dragon_value <= static_cast<int>(last);
        ++dragon_value)
    {
        const Dragon dragon = build_dragon(
            static_cast<DragonType>(dragon_value)
        );

        cout << "\n" << dragon.get_name() << ":\n";
        cout << dragon.get_description() << "\n";
    }
}

/**
 * Display the current scenario treasure description.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The treasure type whose notes should be displayed.
 */
void display_treasure_description(TreasureType treasure_type)
{
    const Treasure treasure = build_treasure(treasure_type);

    cout << "\n" << treasure.get_name() << ":\n";
    cout << treasure.get_description() << "\n";
}

/**
 * Display the broker notes menu and return the selected option.
 *
 * Returns
 * -------
 * menu_choice : int
 *     The validated menu selection entered by the user.
 */
int display_broker_notes_menu()
{
    cout << "\nBroker Notes Menu" << endl;
    cout << "-----------------" << endl;
    cout << "1. Metallic dragon notes" << endl;
    cout << "2. Chromatic dragon notes" << endl;
    cout << "3. Current treasure notes" << endl;
    cout << "4. Run balance test" << endl;
    cout << "5. Continue to dragon selection" << endl;

    return get_bounded_int("Selection: ", 1, 5);
}

/**
 * Allow the user to review notes before making a dragon choice.
 *
 * Parameters
 * ----------
 * treasure_type : TreasureType
 *     The current scenario treasure used for treasure-specific notes.
 */
void broker_notes_loop(TreasureType treasure_type)
{
    bool notes_requested = get_boolean_choice(
        "Would you like broker notes before choosing a dragon? "
        "[Y] or press [Enter] to continue: ",
        "Y",
        ""
    );

    while (notes_requested)
    {
        int menu_choice = display_broker_notes_menu();

        if (menu_choice == 1)
        {
            display_dragon_descriptions(GOLD, BRASS);
        }
        else if (menu_choice == 2)
        {
            display_dragon_descriptions(RED, WHITE);
        }
        else if (menu_choice == 3)
        {
            display_treasure_description(treasure_type);
        }
        else if (menu_choice == 4)
        {
            run_balance_test();
        }
        else
        {
            notes_requested = false;
        }
    }
}

/**
 * Prompt the user to select a dragon from the roster.
 *
 * Returns
 * -------
 * dragon_type : DragonType
 *     The validated dragon selected by the user.
 */
DragonType get_dragon_choice_from_user()
{
    cout << "\nDragon Roster" << endl;
    cout << "-------------" << endl;

    for (int dragon_value = static_cast<int>(RED);
        dragon_value <= static_cast<int>(BRASS);
        ++dragon_value)
    {
        const Dragon dragon = build_dragon(
            static_cast<DragonType>(dragon_value)
        );

        cout << dragon_value << ". " << dragon.get_name() << endl;
    }

    int user_choice = get_bounded_int(
        "Choose a dragon by number: ",
        static_cast<int>(RED),
        static_cast<int>(BRASS)
    );

    return static_cast<DragonType>(user_choice);
}

/**
 * Convert a dragon enum value into a zero-based array index.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The enum value to convert.
 *
 * Returns
 * -------
 * dragon_index : int
 *     The zero-based position used by balance-test counters.
 */
int get_dragon_index(DragonType dragon_type)
{
    return static_cast<int>(dragon_type) - 1;
}

/**
 * Calculate total treasure weight from quantity and unit weight.
 *
 * Parameters
 * ----------
 * treasure_quantity : int
 *     The number of treasure units being transported.
 *
 * weight_per_unit : double
 *     The weight of one treasure unit in pounds.
 *
 * Returns
 * -------
 * total_weight : double
 *     The total treasure weight in pounds.
 */
double calculate_total_weight(int treasure_quantity, double weight_per_unit)
{
    return treasure_quantity * weight_per_unit;
}

/**
 * Calculate absolute treasure value from total weight and value density.
 *
 * Parameters
 * ----------
 * total_weight : double
 *     The total treasure weight in pounds.
 *
 * value_per_pound : double
 *     The base treasure value assigned to each pound.
 *
 * Returns
 * -------
 * absolute_value : double
 *     The treasure value before dragon-specific adjustments.
 */
double calculate_absolute_value(double total_weight, double value_per_pound)
{
    return total_weight * value_per_pound;
}

/**
 * Calculate carrying capacity from dragon strength.
 *
 * Parameters
 * ----------
 * strength : int
 *     The dragon strength rating.
 *
 * Returns
 * -------
 * carrying_capacity : double
 *     The maximum treasure weight the dragon can carry.
 */
double calculate_carrying_capacity(int strength)
{
    return strength * 100.0;
}

/**
 * Calculate round-trip distance from one-way distance.
 *
 * Parameters
 * ----------
 * one_way_distance_miles : double
 *     The distance from broker to treasure site.
 *
 * Returns
 * -------
 * round_trip_distance : double
 *     The combined outbound and return distance.
 */
double calculate_round_trip_distance(double one_way_distance_miles)
{
    return one_way_distance_miles * 2.0;
}

/**
 * Calculate flight time for one expedition leg.
 *
 * Parameters
 * ----------
 * leg_distance_miles : double
 *     The distance traveled during the leg.
 *
 * speed_mph : double
 *     The travel speed used for the leg.
 *
 * Returns
 * -------
 * flight_time_seconds : double
 *     The airborne time for the leg in seconds.
 */
double calculate_leg_flight_time_seconds(
    double leg_distance_miles,
    double speed_mph
)
{
    return leg_distance_miles / speed_mph * SECONDS_PER_HOUR;
}

/**
 * Reduce the dragon's return speed as the carried weight approaches capacity.
 *
 * Parameters
 * ----------
 * base_speed_mph : double
 *     The dragon's unloaded flight speed.
 *
 * total_weight : double
 *     The total treasure weight carried on the return trip.
 *
 * carrying_capacity : double
 *     The maximum weight the dragon can carry.
 *
 * Returns
 * -------
 * loaded_return_speed_mph : double
 *     The adjusted return speed after applying the load penalty.
 */
double calculate_loaded_return_speed_mph(
    double base_speed_mph,
    double total_weight,
    double carrying_capacity
)
{
    if (carrying_capacity <= 0.0)
    {
        return base_speed_mph * 0.25;
    }

    double load_ratio = total_weight / carrying_capacity;
    double speed_modifier = 1.0 - (load_ratio * 0.45);

    if (speed_modifier < 0.25)
    {
        speed_modifier = 0.25;
    }

    return base_speed_mph * speed_modifier;
}

/**
 * Calculate how many rest stops are needed for a travel leg.
 *
 * Parameters
 * ----------
 * travel_distance_miles : double
 *     The distance covered by the travel leg.
 *
 * endurance_miles : double
 *     The distance the dragon can travel before resting.
 *
 * Returns
 * -------
 * rest_count : int
 *     The number of required rest stops for the leg.
 */
int calculate_rest_count(
    double travel_distance_miles,
    double endurance_miles
)
{
    if (travel_distance_miles <= endurance_miles)
    {
        return 0;
    }

    return static_cast<int>((travel_distance_miles - 0.001) / endurance_miles);
}

/**
 * Calculate how long treasure loading takes.
 *
 * Parameters
 * ----------
 * treasure_quantity : int
 *     The number of treasure units to load.
 *
 * Returns
 * -------
 * loading_time_seconds : double
 *     The loading duration in seconds.
 */
double calculate_loading_time_seconds(int treasure_quantity)
{
    return treasure_quantity * SECONDS_PER_TREASURE_UNIT;
}

/**
 * Calculate rest break time from the number of stops and snacks required.
 *
 * Parameters
 * ----------
 * rest_count : int
 *     The number of rest events.
 *
 * snack_demand : int
 *     The number of snacks consumed during each rest event.
 *
 * Returns
 * -------
 * break_time_seconds : double
 *     The total rest break duration in seconds.
 */
double calculate_break_time_seconds(int rest_count, int snack_demand)
{
    return rest_count * snack_demand * 300.0;
}

/**
 * Calculate the snack cost charged to the broker.
 *
 * Parameters
 * ----------
 * rest_count : int
 *     The number of rest events.
 *
 * snack_demand : int
 *     The number of snacks consumed during each rest event.
 *
 * Returns
 * -------
 * snack_cost : double
 *     The total snack expense for the expedition.
 */
double calculate_snack_cost(int rest_count, int snack_demand)
{
    return rest_count * snack_demand * SNACK_COST;
}

/**
 * Calculate the value as the dragon perceives it after personality and cost.
 *
 * Parameters
 * ----------
 * total_value : double
 *     The base treasure value before dragon adjustments.
 *
 * greed_modifier : double
 *     The dragon's personality adjustment to treasure value.
 *
 * travel_time_hours : double
 *     The combined flight time for the expedition in hours.
 *
 * time_sensitivity : int
 *     The dragon's penalty factor for long travel times.
 *
 * snack_cost : double
 *     The food cost deducted from the dragon's perceived value.
 *
 * Returns
 * -------
 * perceived_value : double
 *     The value the dragon uses when deciding whether to accept.
 */
double calculate_perceived_value(
    double total_value,
    double greed_modifier,
    double travel_time_hours,
    int time_sensitivity,
    double snack_cost
)
{
    const double greed_bonus =
        total_value * greed_modifier * GREED_INFLUENCE;
    double time_penalty = travel_time_hours * time_sensitivity;

    return total_value + greed_bonus - time_penalty - snack_cost;
}

/**
 * Determine whether a dragon accepts the expedition.
 *
 * Parameters
 * ----------
 * total_weight : double
 *     The treasure weight the dragon would carry.
 *
 * carrying_capacity : double
 *     The maximum weight the dragon can carry.
 *
 * perceived_value : double
 *     The value the dragon assigns to the expedition.
 *
 * greed_threshold : double
 *     The minimum perceived value the dragon requires.
 *
 * Returns
 * -------
 * accepted : bool
 *     True when the expedition satisfies weight and value requirements.
 */
bool expedition_is_accepted(
    double total_weight,
    double carrying_capacity,
    double perceived_value,
    double greed_threshold
)
{
    return total_weight <= carrying_capacity &&
        perceived_value >= greed_threshold;
}

/**
 * Explain why a dragon accepted or declined the expedition.
 *
 * Parameters
 * ----------
 * total_weight : double
 *     The treasure weight the dragon would carry.
 *
 * carrying_capacity : double
 *     The maximum weight the dragon can carry.
 *
 * perceived_value : double
 *     The value the dragon assigns to the expedition.
 *
 * greed_threshold : double
 *     The minimum perceived value the dragon requires.
 *
 * Returns
 * -------
 * decision_reason : string
 *     A report-friendly explanation of the dragon's decision.
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
        return "Declined because the treasure is too heavy.";
    }

    if (perceived_value < greed_threshold)
    {
        return "Declined because the reward is not tempting enough.";
    }

    return "Accepted because the burden and reward are worthwhile.";
}

/**
 * Calculate broker commission from the dragon's perceived value.
 *
 * Parameters
 * ----------
 * perceived_value : double
 *     The value the dragon assigns to the expedition.
 *
 * accepted : bool
 *     Indicates whether the dragon accepted the expedition.
 *
 * Returns
 * -------
 * broker_commission : double
 *     The broker's commission before snack cost is deducted.
 */
double calculate_broker_commission(
    double perceived_value,
    bool accepted
)
{
    if (!accepted)
    {
        return 0.0;
    }

    return perceived_value * BROKER_COMMISSION_RATE;
}

/**
 * Calculate final profit after snack cost.
 *
 * Parameters
 * ----------
 * broker_commission : double
 *     The commission earned from an accepted expedition.
 *
 * snack_cost : double
 *     The food expense assigned to the broker.
 *
 * Returns
 * -------
 * final_profit : double
 *     The broker's profit after snack cost is deducted.
 */
double calculate_final_profit(double broker_commission, double snack_cost)
{
    return broker_commission - snack_cost;
}

/**
 * Evaluate one dragon against one expedition scenario.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The generated treasure opportunity.
 *
 * chosen_dragon : const Dragon&
 *     The dragon being evaluated for the scenario.
 *
 * Returns
 * -------
 * evaluation : ExpeditionEvaluation
 *     The complete report data for the dragon and scenario.
 */
ExpeditionEvaluation evaluate_expedition(
    const ExpeditionScenario& scenario,
    const Dragon& chosen_dragon
)
{
    const Treasure treasure = build_treasure(scenario.treasure_type);

    ExpeditionEvaluation evaluation;
    evaluation.dragon_type = chosen_dragon;
    evaluation.treasure_type = treasure.get_type();
    evaluation.treasure_quantity = scenario.treasure_quantity;
    evaluation.distance_miles = scenario.distance_miles;
    evaluation.snack_demand = chosen_dragon.get_snack_demand();
    evaluation.destination_recovery_count = 1;
    evaluation.total_weight = Treasure.calculate_total_weight(
        scenario.treasure_quantity
    );
    evaluation.total_value = treasure.calculate_absolute_value(
        scenario.treasure_quantity
    );
    evaluation.outbound_flight_time_seconds =
        calculate_leg_flight_time_seconds(
            static_cast<double>(scenario.distance_miles),
            chosen_dragon.get_speed_mph()
        );
    evaluation.return_flight_time_seconds =
        calculate_leg_flight_time_seconds(
            static_cast<double>(scenario.distance_miles),
            chosen_dragon.calculate_loaded_return_speed_mph(
                evaluation.total_weight
            )
        );
    evaluation.outbound_rest_count = calculate_rest_count(
        static_cast<double>(scenario.distance_miles),
        chosen_dragon.get_endurance_miles()
    );
    evaluation.return_rest_count = calculate_rest_count(
        static_cast<double>(scenario.distance_miles),
        chosen_dragon.get_endurance_miles()
    );

    const int total_rest_count =
        evaluation.outbound_rest_count +
        evaluation.destination_recovery_count +
        evaluation.return_rest_count;

    evaluation.total_break_time_seconds = calculate_break_time_seconds(
        total_rest_count,
        evaluation.snack_demand
    );
    evaluation.loading_time_seconds = calculate_loading_time_seconds(
        scenario.treasure_quantity
    );
    evaluation.snack_cost = calculate_snack_cost(
        total_rest_count,
        evaluation.snack_demand
    );
    evaluation.perceived_value = calculate_perceived_value(
        evaluation.total_value,
        chosen_dragon.get_greed_modifier(),
        (
            evaluation.outbound_flight_time_seconds +
            evaluation.return_flight_time_seconds
        ) / SECONDS_PER_HOUR,
        chosen_dragon.get_time_sensitivity(),
        evaluation.snack_cost
    );
    evaluation.accepted = chosen_dragon.accepts_expedition(
        evaluation.total_weight,
        evaluation.perceived_value
    );
    evaluation.decision_reason = determine_decline_reason(
        evaluation.total_weight,
        chosen_dragon.get_carrying_capcity(),
        evaluation.perceived_value,
        chosen_dragon.get_greed_threshold()
    );
    evaluation.broker_commission = calculate_broker_commission(
        evaluation.perceived_value,
        evaluation.accepted
    );
    evaluation.final_profit = calculate_final_profit(
        evaluation.broker_commission,
        evaluation.snack_cost
    );

    return evaluation;
}

/**
 * Determine which dragon would yield the best accepted profit.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The generated treasure opportunity.
 *
 * best_dragon_found : bool&
 *     Output flag that becomes true when any dragon accepts the scenario.
 *
 * best_final_profit : double&
 *     Output value receiving the highest accepted final profit.
 *
 * best_perceived_value : double&
 *     Output value receiving the perceived value used for tie-breaking.
 *
 * Returns
 * -------
 * best_dragon_type : DragonType
 *     The best accepted dragon, or RED as a placeholder when none accept.
 */
DragonType determine_best_dragon(
    const ExpeditionScenario& scenario,
    bool& best_dragon_found,
    double& best_final_profit,
    double& best_perceived_value
)
{
    DragonType best_dragon_type = RED;
    best_dragon_found = false;
    best_final_profit = 0.0;
    best_perceived_value = numeric_limits<double>::lowest();

    for (int dragon_index = static_cast<int>(RED);
        dragon_index <= static_cast<int>(BRASS);
        ++dragon_index)
    {
        const Dragon candidate_dragon = build_dragon(
            static_cast<DragonType>(dragon_index)
        );
        const ExpeditionEvaluation candidate_evaluation = evaluate_expedition(
            scenario,
            candidate_dragon
        );

        if (candidate_evaluation.broker_commission > 0.0 &&
            (
                !best_dragon_found ||
                candidate_evaluation.final_profit > best_final_profit ||
                (
                    candidate_evaluation.final_profit == best_final_profit &&
                    candidate_evaluation.perceived_value > best_perceived_value
                )
            ))
        {
            best_dragon_found = true;
            best_dragon_type = candidate_dragon.get_type();
            best_final_profit = candidate_evaluation.final_profit;
            best_perceived_value = candidate_evaluation.perceived_value;
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
 *     The dragon whose balance-test result is being displayed.
 *
 * win_count : int
 *     The number of scenarios won by the dragon.
 *
 * number_of_tests : int
 *     The total number of balance-test scenarios run.
 */
void display_balance_test_line(
    DragonType dragon_type,
    int win_count,
    int number_of_tests
)
{
    const Dragon dragon = build_dragon(dragon_type);
    const double win_percentage =
        static_cast<double>(win_count) / number_of_tests * 100.0;

    cout << left << setw(10) << dragon.name()
         << right << setw(8) << win_count << " wins"
         << setw(10) << fixed << setprecision(2) << win_percentage << "%"
         << endl;
}

/**
 * Perform user-defined run-through tests of random scenarios.
 */
void run_balance_test()
{
    int dragon_win_counts[DRAGON_COUNT] = {};

    int number_of_tests = get_bounded_int(
        "How many tests should be run? ",
        10,
        10000
    );

    for (int i = 0; i < number_of_tests; ++i)
    {
        const ExpeditionScenario scenario = generate_playable_scenario();

        bool best_dragon_found = false;
        double best_final_profit = 0.0;
        double best_perceived_value = 0.0;

        DragonType best_dragon = determine_best_dragon(
            scenario,
            best_dragon_found,
            best_final_profit,
            best_perceived_value
        );

        if (best_dragon_found)
        {
            dragon_win_counts[get_dragon_index(best_dragon)]++;
        }
    }

    cout << "\nBalance Test Results\n";
    cout << "====================\n";
    cout << "Iterations: " << number_of_tests << "\n\n";

    for (int dragon_value = static_cast<int>(RED);
        dragon_value <= static_cast<int>(BRASS);
        ++dragon_value)
    {
        DragonType dragon_type = static_cast<DragonType>(dragon_value);

        display_balance_test_line(
            dragon_type,
            dragon_win_counts[get_dragon_index(dragon_type)],
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
 *     The best accepted dragon found for the scenario.
 *
 * chosen_dragon_accepted : bool
 *     Indicates whether the user's dragon accepted the expedition.
 *
 * best_dragon_found : bool
 *     Indicates whether any dragon accepted the expedition.
 *
 * chosen_final_profit : double
 *     The final profit from the user's selected dragon.
 *
 * best_final_profit : double
 *     The highest final profit available from the roster.
 *
 * Returns
 * -------
 * review_message : string
 *     The broker review text shown at the end of the report.
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
    const Dragon best_dragon = build_dragon(best_dragon_type);

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
                      << best_dragon.get_name()
                      << " would have been the strongest accepted choice.";
        return review_stream.str();
    }

    double missed_profit = best_final_profit - chosen_final_profit;

    if (missed_profit > 0.005)
    {
        review_stream << "You did not pick the best dragon.\n"
                      << best_dragon.get_name()
                      << " would have earned an additional $"
                      << format_with_commas(missed_profit, 2)
                      << " in final profit.";
    }
    else
    {
        review_stream << "Your dragon matched the best available final profit,\n"
                      << "though "
                      << best_dragon.get_name()
                      << " was the stronger overall choice based on perceived value.";
    }

    return review_stream.str();
}

/**
 * Display the full expedition report and broker performance review.
 *
 * Parameters
 * ----------
 * evaluation : const ExpeditionEvaluation&
 *     The completed evaluation data for the chosen dragon.
 *
 * best_dragon_type : DragonType
 *     The best accepted dragon found for the same scenario.
 *
 * best_dragon_found : bool
 *     Indicates whether any dragon accepted the scenario.
 *
 * best_final_profit : double
 *     The highest final profit available for the scenario.
 */
void display_expedition_report(
    const ExpeditionEvaluation& evaluation,
    DragonType best_dragon_type,
    bool best_dragon_found,
    double best_final_profit
)
{
    const Dragon dragon = build_dragon(evaluation.dragon_type);
    const Treasure treasure = build_treasure(evaluation.treasure_type);

    const double outbound_break_time_seconds = calculate_break_time_seconds(
        evaluation.outbound_rest_count,
        evaluation.snack_demand
    );
    const double destination_recovery_break_time_seconds =
        calculate_break_time_seconds(
            evaluation.destination_recovery_count,
            evaluation.snack_demand
        );
    const double return_break_time_seconds = calculate_break_time_seconds(
        evaluation.return_rest_count,
        evaluation.snack_demand
    );
    const int outbound_snacks_consumed =
        evaluation.outbound_rest_count * evaluation.snack_demand;
    const int destination_recovery_snacks_consumed =
        evaluation.destination_recovery_count * evaluation.snack_demand;
    const int return_snacks_consumed =
        evaluation.return_rest_count * evaluation.snack_demand;
    const int total_rest_count =
        evaluation.outbound_rest_count +
        evaluation.destination_recovery_count +
        evaluation.return_rest_count;
    const int total_snacks_consumed =
        outbound_snacks_consumed +
        destination_recovery_snacks_consumed +
        return_snacks_consumed;
    const double outbound_leg_total_seconds =
        evaluation.outbound_flight_time_seconds +
        outbound_break_time_seconds;
    const double destination_total_seconds =
        destination_recovery_break_time_seconds +
        evaluation.loading_time_seconds;
    const double return_leg_total_seconds =
        evaluation.return_flight_time_seconds + return_break_time_seconds;
    const double total_elapsed_time_seconds =
        evaluation.outbound_flight_time_seconds +
        evaluation.return_flight_time_seconds +
        evaluation.total_break_time_seconds +
        evaluation.loading_time_seconds;
    const string broker_review_message = build_broker_review_message(
        evaluation.dragon_type,
        best_dragon_type,
        evaluation.accepted,
        best_dragon_found,
        evaluation.final_profit,
        best_final_profit
    );

    cout << endl;
    cout << "============================================================" << endl;
    cout << "                DRAGON BROKER EXPEDITION REPORT             " << endl;
    cout << "============================================================" << endl;

    cout << left << setw(24) << "Dragon"
         << ": " << dragon.get_name() << endl;
    cout << left << setw(24) << "Treasure"
         << ": " << treasure.get_name() << endl;
    cout << left << setw(24) << "Distance"
         << ": " << format_with_commas(evaluation.distance_miles, 0)
         << " miles each way" << endl;
    cout << left << setw(24) << "Treasure quantity"
         << ": " << format_with_commas(evaluation.treasure_quantity, 0) << endl;
    cout << left << setw(24) << "Treasure weight"
         << ": " << format_with_commas(evaluation.total_weight, 2)
         << " pounds" << endl;

    cout << endl;
    cout << "----------------------- OUTBOUND LEG -----------------------" << endl;
    cout << left << setw(24) << "Flight time"
         << ": " << format_time_wdhms(evaluation.outbound_flight_time_seconds)
         << endl;
    cout << left << setw(24) << "Rest stops"
         << ": " << format_with_commas(evaluation.outbound_rest_count, 0)
         << endl;
    cout << left << setw(24) << "Sheep-sized snacks"
         << ": " << format_with_commas(outbound_snacks_consumed, 0) << endl;
    cout << left << setw(24) << "Break time"
         << ": " << format_time_wdhms(outbound_break_time_seconds) << endl;
    cout << left << setw(24) << "Outbound total"
         << ": " << format_time_wdhms(outbound_leg_total_seconds) << endl;

    cout << endl;
    cout << "------------ DESTINATION LOADING AND RECOVERY --------------" << endl;
    cout << left << setw(24) << "Loading time"
         << ": " << format_time_wdhms(evaluation.loading_time_seconds) << endl;
    cout << left << setw(24) << "Sheep-sized snacks"
         << ": " << format_with_commas(destination_recovery_snacks_consumed, 0)
         << endl;
    cout << left << setw(24) << "Break time"
         << ": " << format_time_wdhms(destination_recovery_break_time_seconds)
         << endl;
    cout << left << setw(24) << "Loading and break time"
         << ": " << format_time_wdhms(destination_total_seconds) << endl;

    cout << endl;
    cout << "------------------------ RETURN LEG ------------------------" << endl;
    cout << left << setw(24) << "Flight time"
         << ": " << format_time_wdhms(evaluation.return_flight_time_seconds)
         << endl;
    cout << left << setw(24) << "Rest stops"
         << ": " << format_with_commas(evaluation.return_rest_count, 0)
         << endl;
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
         << ": " << format_time_wdhms(evaluation.total_break_time_seconds)
         << endl;
    cout << left << setw(24) << "Loading time"
         << ": " << format_time_wdhms(evaluation.loading_time_seconds) << endl;
    cout << left << setw(24) << "Total elapsed time"
         << ": " << format_time_wdhms(total_elapsed_time_seconds) << endl;

    cout << endl;
    cout << "-------------------- EXPEDITION STATUS ---------------------" << endl;
    cout << left << setw(24) << "Treasure value"
         << ": $" << format_with_commas(evaluation.total_value, 2) << endl;
    cout << left << setw(24) << "Perceived value"
         << ": $" << format_with_commas(evaluation.perceived_value, 2) << endl;
    cout << left << setw(24) << "Snack cost"
         << ": $" << format_with_commas(evaluation.snack_cost, 2) << endl;
    cout << left << setw(24) << "Accepted"
         << ": " << (evaluation.accepted ? "Yes" : "No") << endl;
    cout << left << setw(24) << "Decision"
         << ": " << evaluation.decision_reason << endl;
    cout << left << setw(24) << "Broker commission"
         << ": $" << format_with_commas(evaluation.broker_commission, 2)
         << endl;
    cout << left << setw(24) << "Final profit"
         << ": $" << format_with_commas(evaluation.final_profit, 2) << endl;

    cout << endl;
    cout << "---------------------- BROKER REVIEW -----------------------" << endl;
    cout << broker_review_message << endl;

    cout << "============================================================" << endl;
}
