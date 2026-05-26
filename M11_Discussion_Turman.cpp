/**
 * ========================================================================
 * Course: CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M11 Discussion
 * Author: Eric J. Turman
 * Date: 2026-05-25
 *
 * Description:
 * A small sheep-sized-snack inventory tracker for the Dragon Broker pantry.
 * The program lets the user choose two snack flavors and quantities from a
 * small pantry, then combines those portions into a consultation kit.
 *
 * Notes:
 * This program will not compile on purpose. There are five intentional bugs
 * introduced to this code.
 *
 * This discussion program keeps the Dragon Broker theme but with a lighter
 * and more compact example. It demonstrates a class template,
 * an overloaded operator used for a kit summary, bounded input, and runtime
 * exception handling without bringing in the larger project logic.
 * ========================================================================
 */

// ========================================================================
// Includes
// ========================================================================
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

// ========================================================================
// Namespace
// ========================================================================
using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================
class SheepSizedSnack;

[[nodiscard]] string format_with_commas(
    double numeric_value,
    int decimal_precision
);

int get_bounded_integer(
    const string& prompt_message,
    int minimum_value,
    int maximum_value
);

void display_program_intro();

void display_section_heading(const string& heading_text);

void display_pantry(
    const SheepSizedSnack& first_snack,
    const SheepSizedSnack& second_snack,
    const SheepSizedSnack& third_snack
);

[[nodiscard]] SheepSizedSnack choose_snack_portion(
    const SheepSizedSnack& first_snack,
    const SheepSizedSnack& second_snack,
    const SheepSizedSnack& third_snack,
    const string& selection_prompt
);

[[nodiscard]] SheepSizedSnack get_snack_by_choice(
    int snack_choice,
    const SheepSizedSnack& first_snack,
    const SheepSizedSnack& second_snack,
    const SheepSizedSnack& third_snack
);

// ========================================================================
// Class Definitions
// ========================================================================

/**
 * Represent one snack flavor in the broker's tiny emergency pantry.
 *
 * The class protects quantity changes because snack counts are the easiest
 * place for a small inventory program to become misleading.
 */
class SheepSizedSnack
{
private:
    string flavor_name;
    int quantity;
    double value_per_snack;
    string effect_note;

public:
    SheepSizedSnack(
        const string& snack_flavor_name,
        int snack_quantity,
        double snack_value_per_snack,
        const string& snack_effect_note
    ) :
        flavor_name(snack_flavor_name),
        quantity(snack_quantity),
        value_per_snack(snack_value_per_snack),
        effect_note(snack_effect_note)
    {
    }

    [[nodiscard]] string get_flavor_name() const
    {
        return flavor_name;
    }

    [[nodiscard]] int get_quantity() const
    {
        return quantity;
    }

    [[nodiscard]] double get_total_value() const
    {
        return quantity * value_per_snack;
    }

    [[nodiscard]] double get_value_per_snack() const
    {
        return value_per_snack;
    }

    [[nodiscard]] string get_effect_note() const
    {
        return effect_note;
    }

    /**
     * Remove snacks only when the requested amount keeps the inventory true.
     *
     * Parameters
     * ----------
     * snacks_to_remove : int
     *     Number of snacks to remove from the current snack item.
     */
    void remove_snacks(int snacks_to_remove)
    {
        if (snacks_to_remove < 1 || snacks_to_remove > quantity)
        {
            throw summer_time_error(
                "Snack removal must be at least 1 and no more than the "
                "available quantity."
            );
        }

        quantity -= snacks_to_remove;
    }

    /**
     * Combine two snack selections into one consultation kit summary.
     *
     * Parameters
     * ----------
     * other_snack : SheepSizedSnack
     *     Another snack selection for the same consultation kit.
     *
     * Returns
     * -------
     * combined_snack : SheepSizedSnack
     *     A combined kit summary using a weighted value per snack.
     */
    [[nodiscard]] Snack operator+(
        const SheepSizedSnack& other_snack
    ) const
    {
        int combined_quantity = quantity + other_snack.quantity;
        double combined_total_value =
            get_total_value() + other_snack.get_total_value();
        string combined_flavor_name =
            flavor_name + " and " + other_snack.flavor_name;
        string combined_effect_note =
            effect_note + " Also: " + other_snack.effect_note;

        return SheepSizedSnack(
            combined_flavor_name,
            combined_quantity,
            combined_total_value / combined_quantity,
            combined_effect_note
        );
    }

    void display() const
    {
        cout << "Flavor: " << flavor_name << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Average snack value: $"
             << format_with_commas(value_per_snack, 2)
             << endl;
        cout << "Total value: $"
             << format_with_commas(get_total_value(), 2)
             << endl;
        cout << "Effect note: " << effect_note << endl;
    }
};

/**
 * Hold one inventory item in a labeled display slot.
 *
 * A template keeps the slot reusable while still letting the stored item
 * decide how much detail belongs in its own display.
 */
template <typename ItemType>
class InventorySlot
{
private:
    string slot_name;
    ItemType inventory_item;

public:
    InventorySlot(
        const string& inventory_slot_name,
        const ItemType& starting_inventory_item
    ) :
        slot_name(inventory_slot_name),
        inventory_item(starting_inventory_item)
    {
    }

    ItemType& get_item()
    {
        return inventory_item;
    }

    void display() const
    {
        cout << "Inventory Slot: " << slot_name << endl;
        cout << "------------------------------" << endl;
        inventory_item.display();
    }
};

// ========================================================================
// Main Function
// ========================================================================
int main()
{
    try
    {
        // ----------------------------------------------------------------
        // Constants
        // ----------------------------------------------------------------
        const string KIT_SLOT_NAME = "Dragon Consultation Snack Kit";
        const string FIRST_PORTION_HEADING = "First Selected Portion";
        const string SECOND_PORTION_HEADING = "Second Selected Portion";
        const string KIT_HEADING = "Combined Consultation Kit";

        // ----------------------------------------------------------------
        // Variables
        // ----------------------------------------------------------------
        SheepSizedSnack pantry_snack_one(
            "Panicked Mutton",
            4,
            42.50,
            "Improves dragon politeness for almost twelve seconds."
        );

        SheepSizedSnack pantry_snack_two(
            "Garlic Wool Surprise",
            3,
            39.75,
            "May reduce negotiation-related singeing."
        );

        SheepSizedSnack pantry_snack_three(
            "Mint-Condition Lamb",
            5,
            36.25,
            "A classic combo."
        );

        SheepSizedSnack first_selected_portion = pantry_snack_one;
        SheepSizedSnack second_selected_portion = pantry_snack_two;
        SheepSizedSnack consultation_kit = first_selected_portion;
        InventorySlot<Snack> kit_slot(
            KIT_SLOT_NAME,
            consultation_kit
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        display_program_intro();
        display_pantry(
            pantry_snack_one,
            pantry_snack_two,
            pantry_snack_three
        );

        // ----------------------------------------------------------------
        // Input
        // ----------------------------------------------------------------
        first_selected_portion = choose_snack_portion(
            pantry_snack_one,
            pantry_snack_two,
            pantry_snack_three,
            "Choose the first snack flavor: "
        );

        second_selected_portion = choose_snack_portion(
            pantry_snack_one,
            pantry_snack_two,
            pantry_snack_three,
            "Choose a different second snack flavor: "
        );

        while (
            second_selected_portion.get_flavor_name() ==
            first_selected_portion.get_flavor_name()
        )
        {
            cout << "Error: Please choose a different snack flavor for "
                 << "the second portion."
                 << endl;

            second_selected_portion = choose_snack_portion(
                pantry_snack_one,
                pantry_snack_two,
                pantry_snack_three,
                "Choose a different second snack flavor: "
            );
        }

        // ----------------------------------------------------------------
        // Processing
        // ----------------------------------------------------------------
        consultation_kit = first_selected_portion +;
        kit_slot = InventorySlot<SheepSizedSnack>(
            KIT_SLOT_NAME,
            consultation_kit
        );

        // ----------------------------------------------------------------
        // Output
        // ----------------------------------------------------------------
        cout << endl;
        display_section_heading(FIRST_PORTION_HEADING);
        first_selected_portion.display();
        cout << endl;

        display_section_heading(SECOND_PORTION_HEADING);
        second_selected_portion.display();
        cout << endl;

        display_section_heading(KIT_HEADING);
        kit_slot.display();
    }
    catch (const runtime_error& error)
    {
        cout << endl;
        cout << "Inventory error: " << program_error.what() << endl;
    }

    cout << endl;
    cout << "Press [Enter] to exit...";
    string exit_pause;
    getline(cin, exit_pause);

    return 0;
}

// ========================================================================
// Function Definitions
// ========================================================================

/**
 * Display the short program scenario.
 */
void display_program_intro()
{
    display_section_heading("Dragon Broker Snack Annex");
    cout << "The pantry has a few sheep-sized snack flavors ready."
         << endl;
    cout << "Choose two snack portions for one dragon consultation kit."
         << endl;
    cout << "The two portions will be combined into a kit summary."
         << endl;
    cout << endl;
}

/**
 * Display a heading and underline.
 *
 * Parameters
 * ----------
 * heading_text : string
 *     The heading text to display.
 */
void display_section_heading(const string& heading_text)
{
    cout << heading_text << endl;
    cout << string(heading_text.length(), '-') << endl;
}

/**
 * Display the small pantry of available snack flavors.
 *
 * Parameters
 * ----------
 * first_snack : SheepSizedSnack
 *     First pantry snack option.
 *
 * second_snack : SheepSizedSnack
 *     Second pantry snack option.
 *
 * third_snack : SheepSizedSnack
 *     Third pantry snack option.
 */
void display_pantry(
    const SheepSizedSnack& first_snack,
    const SheepSizedSnack& second_snack,
    const SheepSizedSnack& third_snack
)
{
    display_section_heading("Small Pantry");

    cout << "1." << endl;
    first_snack.display();
    cout << endl;

    cout << "2." << endl;
    second_snack.display();
    cout << endl;

    cout << "3." << endl;
    third_snack.display();
    cout << endl;
}

/**
 * Choose one snack flavor and quantity for the consultation kit.
 *
 * Parameters
 * ----------
 * first_snack : SheepSizedSnack
 *     First pantry snack option.
 *
 * second_snack : SheepSizedSnack
 *     Second pantry snack option.
 *
 * third_snack : SheepSizedSnack
 *     Third pantry snack option.
 *
 * selection_prompt : string
 *     Prompt used when asking the user to choose a flavor.
 *
 * Returns
 * -------
 * selected_portion : SheepSizedSnack
 *     Snack portion built from the selected pantry item and quantity.
 */
SheepSizedSnack choose_snack_portion(
    const SheepSizedSnack& first_snack,
    const SheepSizedSnack& second_snack,
    const SheepSizedSnack& third_snack,
    const string& selection_prompt
)
{
    int snack_choice = get_bounded_integer(selection_prompt, 1, 3);
    SheepSizedSnack selected_snack = get_snack_by_choice(
        snack_choice,
        first_snack,
        second_snack,
        third_snack
    );

    int snack_quantity = get_bounded_integer(
        "How many of that snack should be added to the kit? ",
        1,
        selected_snack.get_quantity()
    );

    return SheepSizedSnack(
        selected_snack.get_flavor_name(),
        snack_quantity,
        selected_snack.get_value_per_snack(),
        selected_snack.get_effect_note()
    );
}

/**
 * Return the pantry snack that matches a numbered choice.
 *
 * Parameters
 * ----------
 * snack_choice : int
 *     Numbered snack option selected by the user.
 *
 * first_snack : SheepSizedSnack
 *     First pantry snack option.
 *
 * second_snack : SheepSizedSnack
 *     Second pantry snack option.
 *
 * third_snack : SheepSizedSnack
 *     Third pantry snack option.
 *
 * Returns
 * -------
 * selected_snack : SheepSizedSnack
 *     Pantry snack matching the selected number.
 */
SheepSizedSnack get_snack_by_choice(
    int snack_choice,
    const SheepSizedSnack& first_snack,
    const SheepSizedSnack& second_snack,
    const SheepSizedSnack& third_snack
)
{
    if (snack_choice == 1)
    {
        return first_snack;
    }

    if (snack_choice == 2)
    {
        return second_snack;
    }

    return third_snack;
}

/**
 * Format a numeric value with comma group separators.
 *
 * Parameters
 * ----------
 * numeric_value : double
 *     The value to format.
 *
 * decimal_precision : int
 *     Number of digits to display after the decimal point.
 *
 * Returns
 * -------
 * formatted_value : string
 *     The formatted numeric value.
 */
string format_with_commas(double numeric_value, int decimal_precision)
{
    stringstream output_stream;
    output_stream << fixed << setprecision(decimal_precision) << numeric_value;

    string formatted_value = output_stream.str();
    size_t decimal_position = formatted_value.find('.');
    size_t insertion_position =
        decimal_position == string::npos ? formatted_value.length() :
        decimal_position;

    while (insertion_position > 3)
    {
        insertion_position -= 3;
        formatted_value.insert(insertion_position, ",");
    }

    return formatted_value;
}

/**
 * Read a whole number within the requested bounds.
 *
 * Parameters
 * ----------
 * prompt_message : string
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
int get_bounded_integer(
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
