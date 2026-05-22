/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M10 Discussion
 * Author: Eric J. Turman
 * Date: 2026-05-19
 *
 * Description:
 * An M10 pointer and polymorphism refactor for the Dragon Broker
 * simulation.
 *
 * The program presents a randomized treasure expedition scenario and asks
 * the user to select the dragon they believe is best suited for the job.
 * Dragon is used as a base class for specific dragon child classes, and the
 * dragon roster is stored as a vector of Dragon pointers.
 *
 * This allows the program to preserve each child dragon's identity while
 * still working with the dragons through base-class Dragon* pointers. The
 * vector manages the roster size, while each dragon object is dynamically
 * allocated with new and cleaned up with delete before the program exits.
 *
 * Notes:
 * There are four intentional bugs added to this code, and it will not
 * compile or run properly until all have been found and fixed.
 *
 * This version uses raw pointers intentionally for the Chapter 12 discussion.
 * The dragon roster uses vector<Dragon*> to avoid object slicing when child
 * dragon objects are handled through the Dragon base class.
 *
 * Dragon includes a virtual destructor because child objects are deleted
 * through base-class pointers. Dragon also uses a virtual
 * calculate_quest_adjustment function so child classes can evaluate the same
 * expedition differently at runtime.
 *
 * [[nodiscard]] is used selectively on getters, calculations,
 * factory function, and formatting helpers where ignoring
 * the returned value would usually indicate a mistake.
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
#include <vector>

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
constexpr double SNACK_COST = 50.0;
constexpr double BROKER_COMMISSION_RATE = 0.10;
constexpr int SECONDS_PER_TREASURE_UNIT = 1;
constexpr double BASE_REST_SECONDS = 900.0;
constexpr double SECONDS_PER_SNACK = 300.0;
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

[[nodiscard]] double calculate_perceived_value(
    double total_value,
    double greed_modifier,
    double travel_time_hours,
    int time_sensitivity,
    double snack_cost,
    double quest_adjustment
);

// ========================================================================
// Classes
// ========================================================================

/**
 * Represent the expedition environment.
 *
 * The class keeps the displayed biome name separate from the broader
 * environment type that dragons use for preference adjustments.
 */
class Biome
{
private:
    string name;
    string environment_type;
    string quest_note;
    int route_difficulty;
    int weather_severity;
    int cover_density;

public:
    /**
     * Initialize one biome used by an expedition scenario.
     *
     * Parameters
     * ----------
     * biome_name : string
     *     Display name for the expedition location.
     *
     * biome_environment_type : string
     *     General environment category used by dragon preferences.
     *
     * biome_quest_note : string
     *     Vague route clue shown in Quest Notes.
     *
     * biome_route_difficulty : int
     *     General difficulty of the expedition route on a 1 to 10 scale.
     *
     * biome_weather_severity : int
     *     Environmental severity of the route on a 1 to 10 scale.
     *
     * biome_cover_density : int
     *     How dense or concealed the route is on a 1 to 10 scale.
     */
    Biome(
        const string& biome_name,
        const string& biome_environment_type,
        const string& biome_quest_note,
        int biome_route_difficulty,
        int biome_weather_severity,
        int biome_cover_density
    ) :
        name(biome_name),
        environment_type(biome_environment_type),
        quest_note(biome_quest_note),
        route_difficulty(biome_route_difficulty),
        weather_severity(biome_weather_severity),
        cover_density(biome_cover_density)
    {
    }

    /**
     * Return the expedition biome name.
     *
     * Returns
     * -------
     * name : string
     *     The biome display name.
     */
    [[nodiscard]] string get_name() const
    {
        return name;
    }

    /**
     * Return the biome's general environment type.
     *
     * Returns
     * -------
     * environment_type : string
     *     The environment category used by dragon preferences.
     */
    [[nodiscard]] string get_environment_type() const
    {
        return environment_type;
    }

    /**
     * Return the biome clue shown in Quest Notes.
     *
     * Returns
     * -------
     * quest_note : string
     *     A vague clue about the route conditions.
     */
    [[nodiscard]] string get_quest_note() const
    {
        return quest_note;
    }

    /**
     * Return the general difficulty of the biome route.
     *
     * Returns
     * -------
     * route_difficulty : int
     *     The route difficulty on a 1 to 10 scale.
     */
    [[nodiscard]] int get_route_difficulty() const
    {
        return route_difficulty;
    }

    /**
     * Return the environmental severity of the biome route.
     *
     * Returns
     * -------
     * weather_severity : int
     *     The weather or environmental severity on a 1 to 10 scale.
     */
    [[nodiscard]] int get_weather_severity() const
    {
        return weather_severity;
    }

    /**
     * Return how much cover the route provides.
     *
     * Returns
     * -------
     * cover_density : int
     *     The route cover density on a 1 to 10 scale.
     */
    [[nodiscard]] int get_cover_density() const
    {
        return cover_density;
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
    Biome biome = Biome(
        "Volcanic Crater",
        "Volcanic",
        "The route climbs through broken ground, sharp ledges, and paths that punish careless movement. "
        "Heat, fumes, and unstable footing make the journey feel dangerous even before the cargo is considered. "
        "Cover is scarce, leaving plenty of room to move but little room to hide.",
        8,
        9,
        2
    );
    int treasure_quantity = 0;
    int distance_miles = 0;
};

[[nodiscard]] double calculate_scenario_total_weight(
    const ExpeditionScenario& scenario
);

[[nodiscard]] double calculate_scenario_total_value(
    const ExpeditionScenario& scenario
);

// ========================================================================
// Classes
// ========================================================================

/**
 * Represent one dragon in the broker roster.
 *
 * The base class keeps shared dragon data near the small behaviors that
 * depend on that data. Specific dragon child classes can pass their fixed
 * roster values into this class without repeating the shared behavior.
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
    string preferred_environment_type;
    string disliked_environment_type;
    double biome_bonus;
    double biome_penalty;
    string biome_note;

public:
    virtual ~Dragon() = default;

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
     *
     * dragon_preferred_environment_type : string
     *     Biome environment type that improves the dragon's perceived value.
     *
     * dragon_disliked_environment_type : string
     *     Biome environment type that reduces the dragon's perceived value.
     *
     * dragon_biome_bonus : double
     *     Value added when the expedition biome matches the preference.
     *
     * dragon_biome_penalty : double
     *     Value added when the expedition biome matches the dislike.
     *
     * dragon_biome_note : string
     *     Vague broker note about environmental strengths and weaknesses.
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
        int dragon_time_sensitivity,
        const string& dragon_preferred_environment_type,
        const string& dragon_disliked_environment_type,
        double dragon_biome_bonus,
        double dragon_biome_penalty,
        const string& dragon_biome_note
    ) :
        type(dragon_type),
        name(dragon_name),
        description(dragon_description),
        strength(dragon_strength),
        greed_modifier(dragon_greed_modifier),
        greed_threshold(dragon_greed_threshold),
        speed_mph(dragon_speed_mph),
        endurance_miles(dragon_endurance_miles),
        snack_demand(dragon_snack_demand),
        time_sensitivity(dragon_time_sensitivity),
        preferred_environment_type(dragon_preferred_environment_type),
        disliked_environment_type(dragon_disliked_environment_type),
        biome_bonus(dragon_biome_bonus),
        biome_penalty(dragon_biome_penalty),
        biome_note(dragon_biome_note)
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
     * Return the broker-facing biome note for this dragon.
     *
     * Returns
     * -------
     * biome_note : string
     *     A vague description of environmental strengths and weaknesses.
     */
    [[nodiscard]] string get_biome_note() const
    {
        return biome_note;
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
     * Calculate the dragon-specific adjustment for one quest.
     *
     * This function is intentionally virtual so each dragon type can
     * evaluate the same quest differently through a base-class pointer.
     *
     * Parameters
     * ----------
     * scenario : const ExpeditionScenario&
     *     The quest being evaluated.
     *
     * Returns
     * -------
     * quest_adjustment : double
     *     Bonus, penalty, or zero based on this dragon's quest preferences.
     */
    [[nodiscard]] virtual double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const
    {
        if (scenario.biome.get_environment_type() ==
            preferred_environment_type)
        {
            return biome_bonus;
        }

        if (scenario.biome.get_environment_type() ==
            disliked_environment_type)
        {
            return biome_penalty;
        }

        return 0.0;
    }

    /**
     * Calculate the perceived value for one expedition.
     *
     * Parameters
     * ----------
     * total_value : double
     *     The base treasure value before dragon adjustments.
     *
     * travel_time_hours : double
     *     The combined flight time for the expedition in hours.
     *
     * snack_cost : double
     *     The food cost deducted from the dragon's perceived value.
     *
     * quest_adjustment : double
     *     The dragon's value adjustment for the expedition quest.
     *
     * Returns
     * -------
     * perceived_value : double
     *     The value this dragon uses when deciding whether to accept.
     */
    [[nodiscard]] double calculate_perceived_value(
        double total_value,
        double travel_time_hours,
        double snack_cost,
        double quest_adjustment
    ) const
    {
        return ::calculate_perceived_value(
            total_value,
            greed_modifier,
            travel_time_hours,
            time_sensitivity,
            snack_cost,
            quest_adjustment
        );
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
 * Represent the red dragon as a Dragon child class.
 *
 * Red dragons treat unusually rich quests as worth bullying through, but
 * their patience fades when the route is long.
 */
class RedDragon : public Dragon
{
public:
    /**
     * Initialize the red dragon with its fixed broker roster values.
     */
    RedDragon() :
        Dragon(
            RED,
            "Red",
            "A red dragon can carry the kind of treasure that makes lesser wings tremble, and it knows it. "
            "It favors open, direct routes where heat, height, and raw force have room to matter.",
            10,
            1.0,
            20000.0,
            140.0,
            140.0,
            6,
            60,
            "Volcanic",
            "Frozen",
            75.0,
            -50.0,
            "Cramped paths, tangled approaches, and fussy little errands tend to sour its mood before the bargain is even made."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_value = calculate_scenario_total_value(scenario);

        if (total_value >= 300000.0)
        {
            quest_adjustment += 120.0;
        }
        else if (total_value < 50000.0)
        {
            quest_adjustment -= 200.0;
        }

        if (scenario.distance_miles > 400)
        {
            quest_adjustment -= 80.0;
        }

        if (scenario.biome.get_cover_density() >= 8)
        {
            quest_adjustment -= 500.0;
        }

        if (scenario.biome.get_route_difficulty() >= 7 &&
            scenario.biome.get_cover_density() >= 7)
        {
            quest_adjustment -= 350.0;
        }

        if (total_value < 150000.0 &&
            scenario.biome.get_cover_density() >= 5)
        {
            quest_adjustment -= 400.0;
        }

        if (total_value < 250000.0 &&
            scenario.biome.get_environment_type() != "Volcanic")
        {
            quest_adjustment -= 450.0;
        }

        if (total_value < 400000.0 &&
            scenario.biome.get_environment_type() != "Volcanic" &&
            scenario.biome.get_cover_density() >= 3)
        {
            quest_adjustment -= 400.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the blue dragon as a Dragon child class.
 *
 * Blue dragons are careful logistics planners that value distance discipline
 * and dislike jobs that are too small to justify planning.
 */
class BlueDragon : public Dragon
{
public:
    /**
     * Initialize the blue dragon with its fixed broker roster values.
     */
    BlueDragon() :
        Dragon(
            BLUE,
            "Blue",
            "A blue dragon can carry a respectable haul, though it wins more by efficiency than "
            "brute force. It shines across exposed paths and clean lines of travel where discipline matters more "
            "than improvisation.",
            8,
            0.7,
            15000.0,
            135.0,
            180.0,
            4,
            30,
            "Coastal",
            "Desert",
            75.0,
            -50.0,
            "Tangled cover and awkward footing interfere with the precision it prefers."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_value = calculate_scenario_total_value(scenario);

        if (scenario.distance_miles >= 350)
        {
            quest_adjustment += 90.0;
        }

        if (total_value < 40000.0)
        {
            quest_adjustment -= 70.0;
        }

        if (scenario.biome.get_cover_density() >= 7)
        {
            quest_adjustment -= 500.0;
        }

        if (scenario.biome.get_route_difficulty() >= 7 &&
            scenario.biome.get_cover_density() >= 5)
        {
            quest_adjustment -= 300.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the green dragon as a Dragon child class.
 */
class GreenDragon : public Dragon
{
public:
    /**
     * Initialize the green dragon with its fixed broker roster values.
     */
    GreenDragon() :
        Dragon(
            GREEN,
            "Green",
            "A green dragon is not the first choice for the heaviest haul, but it rarely needs to be. "
            "It prefers routes with shade, cover, and enough complication to turn patience into advantage.",
            7,
            0.5,
            13000.0,
            138.0,
            160.0,
            3,
            35,
            "Forest",
            "Volcanic",
            75.0,
            -50.0,
            "Wide-open paths and blunt-force jobs leave it with fewer tricks to play."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);

        if (scenario.biome.get_cover_density() >= 8)
        {
            quest_adjustment += 50.0;
        }

        if (scenario.distance_miles <= 250)
        {
            quest_adjustment += 45.0;
        }
        else if (scenario.distance_miles > 425)
        {
            quest_adjustment -= 75.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the black dragon as a Dragon child class.
 */
class BlackDragon : public Dragon
{
public:
    /**
     * Initialize the black dragon with its fixed broker roster values.
     */
    BlackDragon() :
        Dragon(
            BLACK,
            "Black",
            "A black dragon does not need a pleasant road, and may distrust one on principle. "
            "Concealed paths, hostile conditions, and miserable footing all give it room to work.",
            7,
            0.3,
            10000.0,
            120.0,
            200.0,
            4,
            25,
            "Forest",
            "Desert",
            75.0,
            -50.0,
            "It is less convincing when the treasure is too heavy or too unrewarding to justify the trouble."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_weight = calculate_scenario_total_weight(scenario);
        const double total_value = calculate_scenario_total_value(scenario);

        if (scenario.biome.get_environment_type() == "Volcanic" ||
            scenario.biome.get_environment_type() == "Frozen" ||
            scenario.biome.get_environment_type() == "Desert")
        {
            quest_adjustment += 85.0;
        }

        if (scenario.biome.get_cover_density() >= 7)
        {
            quest_adjustment += 700.0;
        }

        if (scenario.biome.get_weather_severity() >= 8)
        {
            quest_adjustment += 650.0;
        }

        if (scenario.biome.get_route_difficulty() >= 8)
        {
            quest_adjustment += 300.0;
        }

        if (total_weight > get_carrying_capacity() * 0.85)
        {
            quest_adjustment -= 70.0;
        }

        if (total_value < 35000.0)
        {
            quest_adjustment -= 40.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the white dragon as a Dragon child class.
 */
class WhiteDragon : public Dragon
{
public:
    /**
     * Initialize the white dragon with its fixed broker roster values.
     */
    WhiteDragon() :
        Dragon(
            WHITE,
            "White",
            "A white dragon is built for speed, violence, and survival rather than careful freight work. "
            "Exposed routes and punishing conditions sharpen its instincts instead of slowing it down.",
            5,
            -0.1,
            6000.0,
            165.0,
            90.0,
            2,
            80,
            "Frozen",
            "Volcanic",
            75.0,
            -50.0,
            "Long hauls and bulky cargo can turn its confidence into impatience."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);

        if (scenario.distance_miles <= 200)
        {
            quest_adjustment += 800.0;
        }
        else if (scenario.distance_miles > 325)
        {
            quest_adjustment -= 50.0;
        }

        if (scenario.treasure_quantity > 240)
        {
            quest_adjustment -= 45.0;
        }

        if (scenario.biome.get_weather_severity() >= 9)
        {
            quest_adjustment += 1800.0;
        }

        if (scenario.biome.get_environment_type() == "Frozen")
        {
            quest_adjustment += 1200.0;
        }

        if (scenario.biome.get_route_difficulty() >= 8 &&
            scenario.biome.get_cover_density() <= 3)
        {
            quest_adjustment += 700.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the gold dragon as a Dragon child class.
 */
class GoldDragon : public Dragon
{
public:
    /**
     * Initialize the gold dragon with its fixed broker roster values.
     */
    GoldDragon() :
        Dragon(
            GOLD,
            "Gold",
            "A gold dragon brings strength, presence, and an expensive sense of purpose to a quest. "
            "It is most easily convinced by treasure whose value and prestige match the scale of its reputation.",
            10,
            0.2,
            18000.0,
            130.0,
            260.0,
            8,
            20,
            "Mountain",
            "Desert",
            75.0,
            -50.0,
            "Smaller or awkward jobs may not fail with a gold dragon, but they can feel beneath one."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_value = calculate_scenario_total_value(scenario);
        const double total_weight = calculate_scenario_total_weight(scenario);

        if (quest_adjustment < 0.0)
        {
            quest_adjustment *= 0.50;
        }
        else if (quest_adjustment > 0.0)
        {
            quest_adjustment *= 0.75;
        }

        if (scenario.distance_miles >= 300)
        {
            quest_adjustment += 35.0;
        }

        if (total_value < 75000.0)
        {
            quest_adjustment -= 800.0;
        }

        if (total_weight < get_carrying_capacity() * 0.35 &&
            total_value < 150000.0)
        {
            quest_adjustment -= 650.0;
        }

        if (scenario.biome.get_cover_density() >= 7)
        {
            quest_adjustment -= 650.0;
        }

        if (total_value < 250000.0 &&
            total_weight < get_carrying_capacity() * 0.50)
        {
            quest_adjustment -= 600.0;
        }

        if (total_value < 350000.0)
        {
            quest_adjustment -= 450.0;
        }

        if (total_value < 500000.0 &&
            scenario.biome.get_environment_type() != "Mountain")
        {
            quest_adjustment -= 500.0;
        }

        if (total_value < 300000.0 &&
            scenario.biome.get_cover_density() >= 5)
        {
            quest_adjustment -= 500.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the silver dragon as a Dragon child class.
 */
class SilverDragon : public Dragon
{
public:
    /**
     * Initialize the silver dragon with its fixed broker roster values.
     */
    SilverDragon() :
        Dragon(
            SILVER,
            "Silver",
            "A silver dragon carries itself with grace, but that grace has limits when the load becomes unreasonable. "
            "Difficult routes and demanding skies suit it well when the cargo stays manageable.",
            7,
            0.1,
            12000.0,
            170.0,
            170.0,
            3,
            25,
            "Frozen",
            "Volcanic",
            75.0,
            -50.0,
            "Push it too close to its limits and elegance stops being an advantage."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_weight = calculate_scenario_total_weight(scenario);

        if (scenario.distance_miles <= 220)
        {
            quest_adjustment += 70.0;
        }

        if (scenario.biome.get_route_difficulty() >= 8 &&
            total_weight <= get_carrying_capacity() * 0.75)
        {
            quest_adjustment += 1800.0;
        }

        if (scenario.biome.get_weather_severity() >= 8 &&
            total_weight <= get_carrying_capacity() * 0.70)
        {
            quest_adjustment += 900.0;
        }

        if (scenario.biome.get_route_difficulty() >= 9 &&
            scenario.biome.get_weather_severity() >= 7 &&
            total_weight <= get_carrying_capacity() * 0.80)
        {
            quest_adjustment += 700.0;
        }

        if (total_weight > get_carrying_capacity() * 0.90)
        {
            quest_adjustment -= 85.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the bronze dragon as a Dragon child class.
 */
class BronzeDragon : public Dragon
{
public:
    /**
     * Initialize the bronze dragon with its fixed broker roster values.
     */
    BronzeDragon() :
        Dragon(
            BRONZE,
            "Bronze",
            "A bronze dragon is a steady worker, more dependable than dramatic. "
            "It handles distance, weather, and demanding routes with the patience of something built for long service.",
            8,
            0.2,
            13000.0,
            145.0,
            220.0,
            4,
            20,
            "Coastal",
            "Frozen",
            75.0,
            -50.0,
            "It is best chosen when the job needs endurance and discipline more than spectacle."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_weight = calculate_scenario_total_weight(scenario);

        if (scenario.biome.get_environment_type() == "Coastal")
        {
            quest_adjustment += 50.0;
        }

        if (scenario.distance_miles >= 350)
        {
            quest_adjustment += 60.0;
        }

        if (total_weight > get_carrying_capacity() * 0.80)
        {
            quest_adjustment -= 65.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the copper dragon as a Dragon child class.
 */
class CopperDragon : public Dragon
{
public:
    /**
     * Initialize the copper dragon with its fixed broker roster values.
     */
    CopperDragon() :
        Dragon(
            COPPER,
            "Copper",
            "A copper dragon is better suited to clever work than crushing loads. "
            "Awkward routes and difficult footing can keep its interest when the cargo is light enough to manage.",
            6,
            0.0,
            9000.0,
            160.0,
            150.0,
            3,
            45,
            "Desert",
            "Coastal",
            75.0,
            -50.0,
            "Oversized treasure and dull long hauls make it look for excuses."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_value = calculate_scenario_total_value(scenario);
        const double total_weight = calculate_scenario_total_weight(scenario);

        if (total_value < 60000.0)
        {
            quest_adjustment += 95.0;
        }

        if (total_weight <= get_carrying_capacity() * 0.60)
        {
            quest_adjustment += 35.0;
        }

        if (scenario.biome.get_route_difficulty() >= 7 &&
            total_weight <= get_carrying_capacity() * 0.70)
        {
            quest_adjustment += 850.0;
        }

        if (scenario.biome.get_route_difficulty() >= 7 &&
            total_value < 120000.0)
        {
            quest_adjustment += 600.0;
        }

        if (scenario.distance_miles > 425)
        {
            quest_adjustment -= 55.0;
        }

        return quest_adjustment;
    }
};

/**
 * Represent the brass dragon as a Dragon child class.
 */
class BrassDragon : public Dragon
{
public:
    /**
     * Initialize the brass dragon with its fixed broker roster values.
     */
    BrassDragon() :
        Dragon(
            BRASS,
            "Brass",
            "A brass dragon prefers a job it can keep moving through rather than one that pins it under cargo. "
            "Exposed routes, hard glare, and long open stretches suit its restless nature.",
            6,
            -0.1,
            8000.0,
            155.0,
            140.0,
            3,
            55,
            "Desert",
            "Frozen",
            75.0,
            -50.0,
            "Cold, cramped, or overly heavy work can make the bargain lose its shine."
        )
    {
    }

    [[nodiscard]] double calculate_quest_adjustment(
        const ExpeditionScenario& scenario
    ) const override
    {
        double quest_adjustment = Dragon::calculate_quest_adjustment(scenario);
        const double total_weight = calculate_scenario_total_weight(scenario);

        if (scenario.distance_miles >= 350)
        {
            quest_adjustment += 600.0;
        }

        if (total_weight <= get_carrying_capacity() * 0.65)
        {
            quest_adjustment += 350.0;
        }

        if (scenario.biome.get_environment_type() == "Desert" &&
            scenario.biome.get_weather_severity() >= 8)
        {
            quest_adjustment += 1200.0;
        }

        if (scenario.biome.get_cover_density() <= 2 &&
            scenario.biome.get_weather_severity() >= 8)
        {
            quest_adjustment += 600.0;
        }

        if (scenario.distance_miles <= 180)
        {
            quest_adjustment -= 35.0;
        }

        return quest_adjustment;
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
 * Group the full outcome of evaluating one dragon against one scenario.
 *
 * This struct keeps the many report values together without turning the
 * report pipeline into a long list of repeated parameters.
 */
struct ExpeditionEvaluation
{
    DragonType dragon_type = RED;
    TreasureType treasure_type = GOLD_COIN;
    string biome_name;
    string biome_environment_type;
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
    double estimated_snack_cost = 0.0;
    double charged_snack_cost = 0.0;
    double quest_adjustment = 0.0;
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

[[nodiscard]] Dragon* build_dragon(DragonType dragon_type);

[[nodiscard]] vector<Dragon*> build_dragon_roster();

void delete_dragon_roster(vector<Dragon*>& dragon_roster);

[[nodiscard]] Dragon* find_dragon_by_type(
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type
);

[[nodiscard]] Treasure build_treasure(TreasureType treasure_type);

[[nodiscard]] Biome build_biome(int biome_number);

void quest_notes_loop(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster
);

int display_quest_notes_menu();

void display_quest_notes(const ExpeditionScenario& scenario);

void display_flavor_text(const string& flavor_text);

bool convert_dragon_menu_choice(
    const string& user_input,
    DragonType& dragon_type
);

DragonType get_dragon_choice_from_user(
    const vector<Dragon*>& dragon_roster
);

void display_dragon_selection_review(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type
);

DragonType confirm_dragon_selection(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type
);

int get_dragon_index(DragonType dragon_type);

mt19937& get_random_engine();

TreasureType generate_random_treasure_type();

Biome generate_random_biome();

int generate_random_distance_miles();

int generate_random_treasure_quantity();

bool is_playable_scenario(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster
);

ExpeditionScenario generate_playable_scenario(
    const vector<Dragon*>& dragon_roster
);

void display_dragon_descriptions(
    const vector<Dragon*>& dragon_roster,
    DragonType first,
    DragonType last
);

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
    const Dragon* chosen_dragon
);

DragonType determine_best_dragon(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster,
    bool& best_dragon_found,
    double& best_final_profit,
    double& best_perceived_value
);

void run_balance_test(const vector<Dragon*>& dragon_roster);

void display_balance_test_line(
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type,
    int win_count,
    int number_of_tests
);

string build_broker_review_message(
    const vector<Dragon*>& dragon_roster,
    DragonType chosen_dragon_type,
    DragonType best_dragon_type,
    bool chosen_dragon_accepted,
    bool best_dragon_found,
    double chosen_final_profit,
    double best_final_profit
);

void display_expedition_report(
    const ExpeditionEvaluation& evaluation,
    const vector<Dragon*>& dragon_roster,
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

    vector<Dragon*> dragon_roster = build_dragon_roster();

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
        const ExpeditionScenario scenario = generate_playable_scenario(
            dragon_roster
        );
        const Treasure scenario_treasure = build_treasure(
            scenario.treasure_type
        );
        const Biome scenario_biome = scenario.biome;

        cout << "\nExpedition Opportunity" << endl;
        cout << "----------------------" << endl;
        cout << "Treasure type: "
             << scenario_treasure.get_name()
             << endl;
        cout << "Treasure quantity: "
             << format_with_commas(scenario.treasure_quantity, 0)
             << endl;
        cout << "Expedition biome: "
             << scenario_biome.get_name()
             << " ("
             << scenario_biome.get_environment_type()
             << ")"
             << endl;
        cout << "One-way distance: "
             << format_with_commas(scenario.distance_miles, 0)
             << " miles"
             << endl;

        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        quest_notes_loop(scenario, dragon_roster);

        DragonType dragon_type = get_dragon_choice_from_user(dragon_roster);
        dragon_type = confirm_dragon_selection(
            scenario,
            dragon_roster,
            dragon_type
        );

        const Dragon* chosen_dragon = find_dragon_by_type(
            dragon_roster,
            dragon_type
        );

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
            dragon_roster,
            best_dragon_found,
            best_final_profit,
            best_perceived_value
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        display_expedition_report(
            evaluation,
            dragon_roster,
            best_dragon_type,
            best_dragon_found,
            best_final_profit
        );

    } while (get_boolean_choice(PLAY_AGAIN_PROMPT, "Y", ""));

    delete_dragon_roster(dragon_roster);

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
                  << "treasure type, biome, quantity,\nand one-way distance. You "
                  << "must then select the dragon you believe is best suited\n"
                  << "for the job. Each dragon has its own strengths, limits, "
                  << "and route preferences,\nwhile the treasure and biome notes "
                  << "give clues without revealing the exact scoring.\n"
                  << endl;
    output_stream << "Before your choice is final, the broker will show a "
                  << "selection review with the\ndragon, treasure, and biome "
                  << "notes together. You can press [Enter] to continue\n"
                  << "with that dragon or enter another dragon choice to review "
                  << "a different option.\n"
                  << endl;
    output_stream << "In this M10 version, the dragon roster is stored "
                  << "as Dragon* pointers\nin a vector. The specific child "
                  << "dragon classes evaluate the same expedition\nthrough a "
                  << "virtual quest-adjustment function, so different dragons "
                  << "can respond\ndifferently to the same treasure, route, and "
                  << "biome conditions.\n"
                  << endl;
    output_stream << "If the dragon accepts the expedition, you earn a broker "
                  << "commission of around\n"
                  << format_with_commas(BROKER_COMMISSION_RATE * 100.0, 0)
                  << "% of what the dragon believes the treasure is worth."
                  << endl;

    return output_stream.str();
}

/**
 * Build one dragon pointer from its enum type.
 *
 * Parameters
 * ----------
 * dragon_type : DragonType
 *     The enum value identifying which dragon to build.
 *
 * Returns
 * -------
 * dragon : Dragon*
 *     A dynamically allocated dragon object from the matching child class.
 */
Dragon* build_dragon(DragonType dragon_type)
{
    switch (static_cast<int>(dragon_type))
    {
    case RED:
        return new RedDragon();
    case BLUE:
        return new BlueDragon();
    case GREEN:
        return new GreenDragon();
    case BLACK:
        return new BlackDragon();
    case WHITE:
        return new WhiteDragon();
    case GOLD:
        return new GoldDragon();
    case SILVER:
        return new SilverDragon();
    case BRONZE:
        return new BronzeDragon();
    case COPPER:
        return new CopperDragon();
    case BRASS:
        return new BrassDragon();
    default:
        return build_dragon(RED);
    }
}

/**
 * Build the full dragon roster as base-class pointers.
 *
 * Returns
 * -------
 * dragon_roster : vector<Dragon*>
 *     One dynamically allocated dragon for each child-class roster entry.
 */
vector<Dragon*> build_dragon_roster()
{
    vector<Dragon*> dragon_roster;

    for (int dragon_value = static_cast<int>(RED);
        dragon_value <= static_cast<int>(BRASS);
        ++dragon_value)
    {
        dragon_roster.push_back(build_dragon(
            static_cast<DragonType>(dragon_value)
        ));
    }

    return dragon_roster;
}

/**
 * Delete every dynamically allocated dragon in the roster.
 *
 * Parameters
 * ----------
 * dragon_roster : vector<Dragon*>&
 *     The roster whose dragon pointers should be deleted and cleared.
 */
void delete_dragon_roster(vector<Dragon*>& dragon_roster)
{
    for (size_t index = 0; index < dragon_roster.size() - 1; ++index)
    {
        delete dragon_roster[index];
        dragon_roster[index] = nullptr;
    }

    dragon_roster.clear();
}

/**
 * Find one dragon in the roster by enum type.
 *
 * Parameters
 * ----------
 * dragon_roster : const vector<Dragon*>&
 *     The roster to search.
 *
 * dragon_type : DragonType
 *     The dragon type requested.
 *
 * Returns
 * -------
 * dragon : Dragon*
 *     The matching dragon pointer, or nullptr when no match is found.
 */
Dragon* find_dragon_by_type(
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type
)
{
    for (Dragon* dragon : dragon_roster)
    {
        if (dragon->get_type() == dragon_type)
        {
            return dragon;
        }
    }

    return nullptr;
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
            "A single gold coin is nothing to fear, but enough of them can make a sack argue with gravity. "
            "They pack cleanly and behave better under straps than awkward trophies or loose ornaments. "
            "Their value is dependable and respectable, though not always rare enough to impress the proudest bidder.",
            0.02,
            950.0
        );
    case GOLD_CHALICE:
        return Treasure(
            GOLD_CHALICE,
            "Gold chalices",
            "A gold chalice carries more weight than its elegant shape likes to admit. "
            "Bowls, stems, and rims make them awkward to stack without rattling the whole load loose. "
            "The gold makes the job worthwhile, but the form makes the profit feel earned.",
            3.00,
            700.0
        );
    case SILVER_COIN:
        return Treasure(
            SILVER_COIN,
            "Silver coins",
            "Silver coins are light, honest cargo that can be counted into shape without much fuss. "
            "They pack neatly and do not usually turn loading into a wrestling match. "
            "Their value is reliable but modest, which may make the job feel practical rather than grand.",
            0.02,
            120.0
        );
    case SILVER_PLATTER:
        return Treasure(
            SILVER_PLATTER,
            "Silver platters",
            "A silver platter is the sort of treasure that gets heavier the longer someone talks about carrying it. "
            "Its broad shape wastes space and makes secure packing more annoying than the metal alone suggests. "
            "The value is real, but the reward can feel spread thin across too much surface.",
            5.00,
            90.0
        );
    case PLATINUM_COIN:
        return Treasure(
            PLATINUM_COIN,
            "Platinum coins",
            "Platinum coins carry a surprising amount of promise in a small, dense form. "
            "They pack as cleanly as lesser coin while keeping the haul compact and controlled. "
            "Their concentrated value can make even a restrained load feel worth serious attention.",
            0.02,
            1600.0
        );
    case PLATINUM_RING:
        return Treasure(
            PLATINUM_RING,
            "Platinum rings",
            "Platinum rings are small enough to travel easily, but valuable enough to make the count matter. "
            "They settle into a pouch or coffer with little wasted space and almost no loading drama. "
            "Their refined worth gives the job more prestige than its weight suggests.",
            0.10,
            2200.0
        );
    case RAW_GEM:
        return Treasure(
            RAW_GEM,
            "Raw gems",
            "Raw gems have enough weight to remind the carrier that treasure comes from stone. "
            "Their uneven shapes make packing less tidy than coins, rings, or cut pieces. "
            "Their value is strong, though their rough condition leaves some of the glory unfinished.",
            0.50,
            1800.0
        );
    case CUT_GEM:
        return Treasure(
            CUT_GEM,
            "Cut gems",
            "Cut gems make a small container feel far more important than it looks. "
            "They are light, compact, and easier to secure than broad or delicate treasure. "
            "Their refined value gives the job a polished kind of prestige that dragons tend to notice.",
            0.30,
            3500.0
        );
    case JEWELRY:
        return Treasure(
            JEWELRY,
            "Jewelry",
            "Jewelry is rarely heavy in a simple, honest way. "
            "Chains, clasps, settings, and uneven pieces make the load delicate and irritating to secure. "
            "The crafted value is high enough to make the trouble feel intentional rather than accidental.",
            1.20,
            2600.0
        );
    default:
        return build_treasure(GOLD_COIN);
    }
}

/**
 * Calculate the total scenario treasure weight.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The scenario whose treasure weight is being calculated.
 *
 * Returns
 * -------
 * total_weight : double
 *     The total treasure weight in pounds.
 */
double calculate_scenario_total_weight(const ExpeditionScenario& scenario)
{
    const Treasure treasure = build_treasure(scenario.treasure_type);

    return treasure.calculate_total_weight(scenario.treasure_quantity);
}

/**
 * Calculate the total scenario treasure value.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The scenario whose treasure value is being calculated.
 *
 * Returns
 * -------
 * total_value : double
 *     The base treasure value before dragon adjustments.
 */
double calculate_scenario_total_value(const ExpeditionScenario& scenario)
{
    const Treasure treasure = build_treasure(scenario.treasure_type);

    return treasure.calculate_absolute_value(scenario.treasure_quantity);
}

/**
 * Build one biome object from its menu number.
 *
 * Parameters
 * ----------
 * biome_number : int
 *     Number identifying which biome to build.
 *
 * Returns
 * -------
 * biome : Biome
 *     A Biome object populated with the matching environment values.
 */
Biome build_biome(int biome_number)
{
    switch (biome_number)
    {
    case 1:
        return Biome(
            "Volcanic Crater",
            "Volcanic",
            "The route climbs through broken ground, sharp ledges, and paths that punish careless movement. "
            "Heat, fumes, and unstable footing make the journey feel dangerous even before the cargo is considered. "
            "Cover is scarce, leaving plenty of room to move but little room to hide.",
            8,
            9,
            2
        );
    case 2:
        return Biome(
            "Storm Coast",
            "Coastal",
            "The route is passable, but only for travelers willing to correct their course again and again. "
            "Wind, spray, and sudden weather shifts turn steady travel into a constant argument. "
            "The path is mostly exposed, giving clear movement to disciplined flyers but little shelter from mistakes.",
            6,
            8,
            3
        );
    case 3:
        return Biome(
            "High Mountain Pass",
            "Mountain",
            "The route rises through steep grades, thin air, and turns that make every mile feel earned. "
            "Cold winds and altitude turn even ordinary travel into a test of control. "
            "The passage is open in places, but cliffs and narrow cuts leave little forgiveness.",
            9,
            7,
            4
        );
    case 4:
        return Biome(
            "Ancient Forest",
            "Forest",
            "The route twists through old growth, uneven ground, and obstacles that slow anything trying to rush. "
            "The air is calmer than harsher regions, but the terrain itself does most of the work. "
            "Cover is dense, giving patient travelers options and denying clean lines of movement.",
            7,
            4,
            9
        );
    case 5:
        return Biome(
            "Frozen Wastes",
            "Frozen",
            "The route stretches across hard ground that offers distance without comfort. "
            "Bitter cold, wind, and exposure make the journey severe from the first mile. "
            "Cover is almost absent, so anything crossing it must face the open honestly.",
            8,
            10,
            2
        );
    case 6:
        return Biome(
            "Glass Desert",
            "Desert",
            "The route runs across sharp, dry ground that wears down the careless and the overloaded. "
            "Heat, glare, and empty air make the environment feel hostile over time. "
            "Cover is nearly nonexistent, leaving the path clear but unforgiving.",
            7,
            9,
            1
        );
    default:
        return build_biome(1);
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
 * Generate a random biome for the scenario.
 *
 * Returns
 * -------
 * biome : Biome
 *     A randomly selected expedition biome.
 */
Biome generate_random_biome()
{
    uniform_int_distribution<int> distribution(1, 6);

    return build_biome(distribution(get_random_engine()));
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
bool is_playable_scenario(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster
)
{
    int accepted_count = 0;

    for (const Dragon* dragon : dragon_roster)
    {
        const ExpeditionEvaluation evaluation = evaluate_expedition(
            scenario,
            *dragon
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
ExpeditionScenario generate_playable_scenario(
    const vector<Dragon*>& dragon_roster
)
{
    ExpeditionScenario scenario;

    do
    {
        scenario.treasure_type = generate_random_treasure_type();
        scenario.biome = generate_random_biome();
        scenario.treasure_quantity = generate_random_treasure_quantity();
        scenario.distance_miles = generate_random_distance_miles();
    } while (!is_playable_scenario(scenario, dragon_roster));

    return scenario;
}

/**
 * Display flavor text with one sentence per line.
 *
 * Parameters
 * ----------
 * flavor_text : const string&
 *     The flavor text to display.
 */
void display_flavor_text(const string& flavor_text)
{
    bool line_has_content = false;

    for (size_t index = 0; index < flavor_text.length(); ++index)
    {
        const char character = flavor_text[index];

        if (character == '\n')
        {
            if (line_has_content)
            {
                cout << endl;
                line_has_content = false;
            }

            continue;
        }

        cout << character;

        if (character != ' ')
        {
            line_has_content = true;
        }

        if (character == '.' &&
            index + 1 < flavor_text.length() &&
            flavor_text[index + 1] == ' ')
        {
            cout << endl;
            ++index;
            line_has_content = false;
        }
    }

    if (line_has_content)
    {
        cout << endl;
    }
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
void display_dragon_descriptions(
    const vector<Dragon*>& dragon_roster,
    DragonType first,
    DragonType last
)
{
    for (int dragon_value = static_cast<int>(first);
        dragon_value <= static_cast<int>(last);
        ++dragon_value)
    {
        const Dragon* dragon = find_dragon_by_type(
            dragon_roster,
            static_cast<DragonType>(dragon_value)
        );

        if (dragon != nullptr)
        {
            cout << "\n" << dragon.get_name() << ":\n";
            display_flavor_text(dragon->get_description());
            display_flavor_text(dragon->get_biome_note());
        }
    }
}

/**
 * Display the current scenario quest notes.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The current scenario used for treasure and biome notes.
 */
void display_quest_notes(const ExpeditionScenario& scenario)
{
    const Treasure treasure = build_treasure(scenario.treasure_type);
    const Biome biome = scenario.biome;

    cout << "\nQuest Notes" << endl;
    cout << "-----------" << endl;
    cout << "Treasure: " << treasure.get_name() << endl;
    display_flavor_text(treasure.get_description());
    cout << "\nExpedition biome: "
         << biome.get_name()
         << " ("
         << biome.get_environment_type()
         << ")"
         << endl;
    display_flavor_text(biome.get_quest_note());
    cout << "Some dragons may find this route more natural than others, "
         << "but the full choice still depends on weight, distance, value, "
         << "rest needs, and appetite.\n";
}

/**
 * Display the quest notes menu and return the selected option.
 *
 * Returns
 * -------
 * menu_choice : int
 *     The validated menu selection entered by the user.
 */
int display_quest_notes_menu()
{
    cout << "\nQuest Notes Menu" << endl;
    cout << "----------------" << endl;
    cout << "1. Metallic dragon notes" << endl;
    cout << "2. Chromatic dragon notes" << endl;
    cout << "3. Current quest notes" << endl;
    cout << "4. Run balance test" << endl;
    cout << "5. Continue to dragon selection" << endl;

    return get_bounded_int("Selection: ", 1, 5);
}

/**
 * Allow the user to review quest notes before making a dragon choice.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The current scenario used for quest-specific notes.
 */
void quest_notes_loop(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster
)
{
    bool notes_requested = get_boolean_choice(
        "Would you like quest notes before choosing a dragon? "
        "[Y] or press [Enter] to continue: ",
        "Y",
        ""
    );

    while (notes_requested)
    {
        int menu_choice = display_quest_notes_menu();

        if (menu_choice == 1)
        {
            display_dragon_descriptions(dragon_roster, GOLD, BRASS);
        }
        else if (menu_choice == 2)
        {
            display_dragon_descriptions(dragon_roster, RED, WHITE);
        }
        else if (menu_choice == 3)
        {
            display_quest_notes(scenario);
        }
        else if (menu_choice == 4)
        {
            run_balance_test(dragon_roster);
        }
        else
        {
            notes_requested = false;
        }
    }
}

/**
 * Convert one displayed dragon menu choice into a DragonType.
 *
 * Parameters
 * ----------
 * user_input : const string&
 *     The menu choice entered by the user.
 *
 * dragon_type : DragonType&
 *     Output value receiving the matching dragon type.
 *
 * Returns
 * -------
 * choice_is_valid : bool
 *     True when the input maps to a dragon roster entry.
 */
bool convert_dragon_menu_choice(
    const string& user_input,
    DragonType& dragon_type
)
{
    if (user_input.length() != 1)
    {
        return false;
    }

    if (user_input[0] == '0')
    {
        dragon_type = BRASS;
        return true;
    }

    if (user_input[0] >= '1' && user_input[0] <= '9')
    {
        dragon_type = static_cast<DragonType>(user_input[0] - '0');
        return true;
    }

    return false;
}

/**
 * Prompt the user to select a dragon from the roster.
 *
 * Returns
 * -------
 * dragon_type : DragonType
 *     The validated dragon selected by the user.
 */
DragonType get_dragon_choice_from_user(
    const vector<Dragon*>& dragon_roster
)
{
    cout << "\nDragon Roster" << endl;
    cout << "-------------" << endl;

    for (const Dragon* dragon : dragon_roster)
    {
        if (dragon != nullptr)
        {
            const int menu_choice =
                dragon->get_type() == BRASS ?
                0 :
                static_cast<int>(dragon->get_type());

            cout << menu_choice
                 << ". "
                 << dragon->get_name()
                 << endl;
        }
    }

    string user_input;
    DragonType dragon_type = RED;
    bool input_is_valid = false;

    while (!input_is_valid)
    {
        cout << "Choose a dragon by number [1-9, 0]: ";
        getline(cin, user_input);

        input_is_valid = convert_dragon_menu_choice(user_input, dragon_type);

        if (!input_is_valid)
        {
            cout << "Error: Please enter a dragon choice [1-9, 0]."
                 << endl;
        }
    }

    return dragon_type;
}

/**
 * Display a short review of the selected dragon and current quest.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The current quest opportunity.
 *
 * dragon_roster : const vector<Dragon*>&
 *     The dragon roster used to look up the selected dragon.
 *
 * dragon_type : DragonType
 *     The dragon selected by the user.
 */
void display_dragon_selection_review(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type
)
{
    const Dragon* dragon = find_dragon_by_type(dragon_roster, dragon_type);
    const Treasure treasure = build_treasure(scenario.treasure_type);
    const Biome biome = scenario.biome;

    cout << "\nSelection Review" << endl;
    cout << "----------------" << endl;

    if (dragon != nullptr)
    {
        cout << "Selected dragon: " << dragon->get_name() << endl;
        display_flavor_text(dragon->get_description());
        display_flavor_text(dragon->get_biome_note());
    }

    cout << "\nTreasure: " << treasure.get_name() << endl;
    display_flavor_text(treasure.get_description());
    cout << "Quantity: "
         << format_with_commas(scenario.treasure_quantity, 0)
         << endl;

    cout << "\nExpedition biome: "
         << biome.get_name()
         << " ("
         << biome.get_environment_type()
         << ")"
         << endl;
    display_flavor_text(biome.get_quest_note());

    cout << "One-way distance: "
         << format_with_commas(scenario.distance_miles, 0)
         << " miles"
         << endl;

    cout << "Route profile: ";

    if (biome.get_route_difficulty() >= 8)
    {
        cout << "difficult route";
    }
    else
    {
        cout << "manageable route";
    }

    cout << ", ";

    if (biome.get_weather_severity() >= 8)
    {
        cout << "severe conditions";
    }
    else
    {
        cout << "moderate conditions";
    }

    cout << ", ";

    if (biome.get_cover_density() >= 7)
    {
        cout << "dense cover";
    }
    else if (biome.get_cover_density() <= 3)
    {
        cout << "sparse cover";
    }
    else
    {
        cout << "some cover";
    }

    cout << "." << endl;
}

/**
 * Confirm or update the selected dragon before evaluation.
 *
 * Parameters
 * ----------
 * scenario : const ExpeditionScenario&
 *     The current quest opportunity.
 *
 * dragon_roster : const vector<Dragon*>&
 *     The dragon roster used to look up the selected dragon.
 *
 * dragon_type : DragonType
 *     The dragon selected by the user.
 *
 * Returns
 * -------
 * dragon_type : DragonType
 *     The final dragon type selected by the user.
 */
DragonType confirm_dragon_selection(
    const ExpeditionScenario& scenario,
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type
)
{
    string user_input;
    bool selection_confirmed = false;

    while (!selection_confirmed)
    {
        display_dragon_selection_review(scenario, dragon_roster, dragon_type);

        cout << "\nPress [Enter] to continue with this dragon, "
             << "or enter another dragon choice [1-9, 0]: ";
        getline(cin, user_input);

        if (user_input.empty())
        {
            selection_confirmed = true;
        }
        else if (!convert_dragon_menu_choice(user_input, dragon_type))
        {
            cout << "Error: Please enter a dragon choice [1-9, 0], "
                 << "or press [Enter] to continue."
                 << endl;
        }
    }

    return dragon_type;
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
 * Calculate rest break time from recovery and snacks.
 *
 * Parameters
 * ----------
 * rest_count : int
 *     The number of rest events.
 *
 * snack_demand : int
 *     The number of snacks consumed during each rest event. Each event also
 *     includes a fixed 15-minute recovery before snack feeding time is added.
 *
 * Returns
 * -------
 * break_time_seconds : double
 *     The total rest break duration in seconds.
 */
double calculate_break_time_seconds(int rest_count, int snack_demand)
{
    return rest_count * (
        BASE_REST_SECONDS +
        snack_demand * SECONDS_PER_SNACK
    );
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
 * quest_adjustment : double
 *     The dragon's value adjustment for the expedition quest.
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
    double snack_cost,
    double quest_adjustment
)
{
    const double greed_bonus =
        total_value * greed_modifier * GREED_INFLUENCE;
    double time_penalty = travel_time_hours * time_sensitivity;

    return total_value +
        greed_bonus -
        time_penalty -
        snack_cost +
        quest_adjustment;
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
 * chosen_dragon : const Dragon*
 *     The dragon being evaluated for the scenario.
 *
 * Returns
 * -------
 * evaluation : ExpeditionEvaluation
 *     The complete report data for the dragon and scenario.
 */
ExpeditionEvaluation evaluate_expedition(
    const ExpeditionScenario& scenario,
    const Dragon* chosen_dragon
)
{
    const Treasure treasure = build_treasure(scenario.treasure_type);

    ExpeditionEvaluation evaluation;

    if (chosen_dragon == nullptr)
    {
        return evaluation;
    }

    evaluation.dragon_type = chosen_dragon->get_type();
    evaluation.treasure_type = treasure.get_type();
    evaluation.biome_name = scenario.biome.get_name();
    evaluation.biome_environment_type = scenario.biome.get_environment_type();
    evaluation.treasure_quantity = scenario.treasure_quantity;
    evaluation.distance_miles = scenario.distance_miles;
    evaluation.snack_demand = chosen_dragon->get_snack_demand();
    evaluation.destination_recovery_count = 1;
    evaluation.total_weight = treasure.calculate_total_weight(
        scenario.treasure_quantity
    );
    evaluation.total_value = treasure.calculate_absolute_value(
        scenario.treasure_quantity
    );
    evaluation.outbound_flight_time_seconds =
        calculate_leg_flight_time_seconds(
            static_cast<double>(scenario.distance_miles),
            chosen_dragon->get_speed_mph()
        );
    evaluation.return_flight_time_seconds =
        calculate_leg_flight_time_seconds(
            static_cast<double>(scenario.distance_miles),
            chosen_dragon->calculate_loaded_return_speed_mph(
                evaluation.total_weight
            )
        );
    evaluation.outbound_rest_count = calculate_rest_count(
        static_cast<double>(scenario.distance_miles),
        chosen_dragon->get_endurance_miles()
    );
    evaluation.return_rest_count = calculate_rest_count(
        static_cast<double>(scenario.distance_miles),
        chosen_dragon->get_endurance_miles()
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
    evaluation.estimated_snack_cost = calculate_snack_cost(
        total_rest_count,
        evaluation.snack_demand
    );
    evaluation.quest_adjustment = chosen_dragon->calculate_quest_adjustment(
        scenario
    );
    evaluation.perceived_value = chosen_dragon->calculate_perceived_value(
        evaluation.total_value,
        (
            evaluation.outbound_flight_time_seconds +
            evaluation.return_flight_time_seconds
        ) / SECONDS_PER_HOUR,
        evaluation.estimated_snack_cost,
        evaluation.quest_adjustment
    );
    evaluation.accepted = chosen_dragon->accepts_expedition(
        evaluation.total_weight,
        evaluation.perceived_value
    );
    evaluation.decision_reason = determine_decline_reason(
        evaluation.total_weight,
        chosen_dragon->get_carrying_capacity(),
        evaluation.perceived_value,
        chosen_dragon->get_greed_threshold()
    );
    evaluation.broker_commission = calculate_broker_commission(
        evaluation.perceived_value,
        evaluation.accepted
    );
    evaluation.charged_snack_cost =
        evaluation.accepted ? evaluation.estimated_snack_cost : 0.0;
    evaluation.final_profit = calculate_final_profit(
        evaluation.broker_commission,
        evaluation.charged_snack_cost
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
    const vector<Dragon*>& dragon_roster,
    bool& best_dragon_found,
    double& best_final_profit,
    double& best_perceived_value
)
{
    DragonType best_dragon_type = RED;
    best_dragon_found = false;
    best_final_profit = 0.0;
    best_perceived_value = numeric_limits<double>::lowest();

    for (const Dragon* candidate_dragon : dragon_roster)
    {
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
            best_dragon_type = candidate_dragon->get_type();
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
    const vector<Dragon*>& dragon_roster,
    DragonType dragon_type,
    int win_count,
    int number_of_tests
)
{
    const Dragon* dragon = find_dragon_by_type(dragon_roster, dragon_type);
    const double win_percentage =
        static_cast<double>(win_count) / number_of_tests * 100.0;

    if (dragon != nullptr)
    {
        cout << left << setw(10) << dragon->get_name()
             << right << setw(8) << win_count << " wins"
             << setw(10) << fixed << setprecision(2) << win_percentage << "%"
             << endl;
    }
}

/**
 * Perform user-defined run-through tests of random scenarios.
 */
void run_balance_test(const vector<Dragon*>& dragon_roster)
{
    int dragon_win_counts[DRAGON_COUNT] = {};

    int number_of_tests = get_bounded_int(
        "How many tests should be run? ",
        10,
        10000
    );

    for (int i = 0; i < number_of_tests; ++i)
    {
        const ExpeditionScenario scenario = generate_playable_scenario(
            dragon_roster
        );

        bool best_dragon_found = false;
        double best_final_profit = 0.0;
        double best_perceived_value = 0.0;

        DragonType best_dragon = determine_best_dragon(
            scenario,
            dragon_roster,
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
            dragon_roster,
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
    const vector<Dragon*>& dragon_roster,
    DragonType chosen_dragon_type,
    DragonType best_dragon_type,
    bool chosen_dragon_accepted,
    bool best_dragon_found,
    double chosen_final_profit,
    double best_final_profit
)
{
    ostringstream review_stream;
    const Dragon* best_dragon = find_dragon_by_type(
        dragon_roster,
        best_dragon_type
    );

    if (!best_dragon_found)
    {
        review_stream << "No dragon accepted this expedition. The scenario "
                      << "was too poor or too burdensome for the roster.";
        return review_stream.str();
    }

    if (best_dragon == nullptr)
    {
        review_stream << "A best dragon was found, but the roster lookup "
                      << "could not identify it.";
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
                      << best_dragon->get_name()
                      << " would have been the strongest accepted choice.";
        return review_stream.str();
    }

    double missed_profit = best_final_profit - chosen_final_profit;

    if (missed_profit > 0.005)
    {
        review_stream << "You did not pick the best dragon.\n"
                      << best_dragon->get_name()
                      << " would have earned an additional $"
                      << format_with_commas(missed_profit, 2)
                      << " in final profit.";
    }
    else
    {
        review_stream << "Your dragon matched the best available final profit,\n"
                      << "though "
                      << best_dragon->get_name()
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
    const vector<Dragon*>& dragon_roster,
    DragonType best_dragon_type,
    bool best_dragon_found,
    double best_final_profit
)
{
    const Dragon* dragon = find_dragon_by_type(
        dragon_roster,
        evaluation.dragon_type
    );
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
        dragon_roster,
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
         << ": " << (dragon != nullptr ? dragon->get_name() : "Unknown")
         << endl;
    cout << left << setw(24) << "Treasure"
         << ": " << treasure.get_name() << endl;
    cout << left << setw(24) << "Biome"
         << ": " << evaluation.biome_name
         << " ("
         << evaluation.biome_environment_type
         << ")"
         << endl;
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
    cout << left << setw(24) << "Quest adjustment"
         << ": $" << format_with_commas(evaluation.quest_adjustment, 2)
         << endl;
    cout << left << setw(24) << "Perceived value"
         << ": $" << format_with_commas(evaluation.perceived_value, 2) << endl;
    cout << left << setw(24) << "Estimated snack cost"
         << ": $" << format_with_commas(evaluation.estimated_snack_cost, 2)
         << endl;
    cout << left << setw(24) << "Accepted"
         << ": " << (evaluation.accepted ? "Yes" : "No") << endl;
    cout << left << setw(24) << "Decision"
         << ": " << evaluation.decision_reason << endl;
    cout << left << setw(24) << "Broker commission"
         << ": $" << format_with_commas(evaluation.broker_commission, 2)
         << endl;
    cout << left << setw(24) << "Charged snack cost"
         << ": $" << format_with_commas(evaluation.charged_snack_cost, 2)
         << endl;
    cout << left << setw(24) << "Final profit"
         << ": $" << format_with_commas(evaluation.final_profit, 2) << endl;

    cout << endl;
    cout << "---------------------- BROKER REVIEW -----------------------" << endl;
    cout << broker_review_message << endl;

    cout << "============================================================" << endl;
}
