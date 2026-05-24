/*
 * Student Name: Daniel Preller
 * File Name: Week10.cpp
 * Date: 5/20/2026
*/

#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

// Function prototypes for functions used within classes
string boolToString(bool value, string trueString, string falseString);
int getIntInRange(int min, int max);

// Abstract class that stores name and price information for menu items
class MenuItem {
protected:
    string name;
    double price;
    string type = "None";

public:
    MenuItem() {// Default constructor
        setName("Item");
        setPrice(0);
    }

    MenuItem(string name, double price) {
        setName(name);
        setPrice(price);
    }

    // Destructor
    virtual ~MenuItem() = default;

    // Set methods
    void setName(string name) {
        this->name = name;
    }

    void setPrice(double price) {
        this->price = price;
    }

    // Get methods
    string getName() {
        return name;
    }

    // getPrice method takes an integer argument for the size, which is implemented in the DrinkItem subclass
    virtual double getPrice(int size) {
        return price;
    }

    string getType() {
        return type;
    }

    // Prints the item as a line in a menu
    void printBasicMenuLine() {
        cout << setw(30) << left << name;
        cout << "$" << fixed << setprecision(2) << price << endl;
    }

    // Pure virtual function for printing a detailed menu line
    virtual void printDetailedMenuLine() = 0;
};

// Represents a food item in a menu
class FoodItem: public MenuItem {
private:
    bool glutenFree = false;

public:
    // Default constructor
    FoodItem(): MenuItem() {
        type = "Food";
    }

    // Constructor matching that of superclass
    FoodItem(string name, double price): MenuItem(name, price) {
        type = "Food";
    }

    // Constructor for all fields
    FoodItem(string name, double price, bool glutenFree): MenuItem(name, price) {
        this->glutenFree = glutenFree;
        type = "Food";
    }

    // Prints the detailed menu line, including whether the item is gluten-free
    void printDetailedMenuLine() override{
        cout << setw(20) << left << name;
        cout << setw(15) << boolToString(glutenFree, "Yes", "No");
        cout << "$" << fixed << setprecision(2) << price << endl;
    }
};

// Represents a drink item in a menu
class DrinkItem: public MenuItem {
private:
    // Fields for prices of different sizes
    // The inherited price field is used for the 'regular' size
    double smallPrice;
    double largePrice;

public:
    // Default constructor
    DrinkItem(): DrinkItem("Item", 0) {
        type = "Drink";
    }

    // Constructor matching that of parent class
    DrinkItem(string name, double price): MenuItem(name, price) {
        // Default prices for small and large are 50 cents above and below
        // regular price, respectively, but cannot be negative
        smallPrice = max(price - 0.50, 0.0);
        largePrice = price + 0.50;
        type = "Drink";
    }

    // Constructor for all fields
    DrinkItem(string name, double price, double smallPrice, double largePrice) : MenuItem(name, price) {
        this->smallPrice = smallPrice;
        this->largePrice = largePrice;
        type = "Drink";
    }

    // Prints a detailed menu line, including prices for all sizes
    void printDetailedMenuLine() override{
        cout << setw(20) << left << name;
        cout << "$" << fixed << setprecision(2) << left << setw(9) << smallPrice;
        cout << "$" << fixed << setprecision(2) << left << setw(9) << price;
        cout << "$" << fixed << setprecision(2) << left << setw(9) << largePrice << endl;
    }

    // Gets the price for a specific size
    // 1 for small, 2 for regular, and 3 for large
    double getPrice(int size) override {
        switch (size) {
            case 1:
                return smallPrice;
                break;
            case 2:
                return price;
                break;
            case 3:
                return largePrice;
                break;
            default:
                throw invalid_argument("Invalid size");
        }
    }
};

// Menu class is used to make a menu of items and to order from it
class Menu {
private:
    string name;
    const int NUMBER_OF_ITEMS;
    MenuItem* *itemsPointer = nullptr;// Pointer to an array of menu item pointers, allowing for dynamic resizing and polymorphism

public:

    // Constructor using a name and a number of items
    Menu(string name, int numberOfItems) : NUMBER_OF_ITEMS(numberOfItems) {
        if (numberOfItems < 0) {
            throw invalid_argument("Number of items must be positive");
        }
        this->setName(name);
        itemsPointer = new MenuItem*[numberOfItems];// Creates an array according to the number of items specified
    }

    // Destructor
    ~Menu() {
        delete [] itemsPointer;
    }

    // Name set method
    void setName(string name) {
        this->name = name;
    }

    // Sets an item, which can be any concrete menu item type
    void setItem(int itemNumber, MenuItem *item) {
        itemsPointer[itemNumber] = item;
    }

    // Gets the name of the menu
    string getName() {
        return name;
    }

    // Gets the type for a specific item in the menu
    string getItemType(int itemNumber) {
        if (itemNumber >= 0 && itemNumber < NUMBER_OF_ITEMS) {
            return itemsPointer[itemNumber]->getType();
        } else {
            throw invalid_argument("Invalid item number");
        }
    }

    // Prints the menu information
    void printMenu() {
        // Header
        cout << "  " << name << " Menu:" << endl;
        cout << "    " << setw(30) << left << "Item" << "Price" << endl;

        for (int itemNumber = 0; itemNumber < NUMBER_OF_ITEMS; itemNumber++) {
            cout << "  " << itemNumber + 1 << " ";// Item numbers are displayed starting from 1
            itemsPointer[itemNumber]->printBasicMenuLine();
        }
        cout << endl;
    }

    // Prints the drink menu with details on sizes
    void printDrinkMenu() {
        // Header
        cout << "  " << name << " Drink Menu:" << endl;
        cout << "    " << setw(20) << left << "Item" << setw(10) << "Small" << setw(10) << "Regular"
        << setw(10) << "Large" << endl;

        for (int itemNumber = 0; itemNumber < NUMBER_OF_ITEMS; itemNumber++) {
            if (itemsPointer[itemNumber]->getType() == "Drink") {// Only prints drink items
                cout << "  " << itemNumber + 1 << " ";// Item numbers are displayed starting from 1
                itemsPointer[itemNumber]->printDetailedMenuLine();
            }
        }
    }

    // Prints the food menu with details on whether items are gluten-free
    void printFoodMenu() {
        // Header
        cout << "  " << name << " Food Menu:" << endl;
        cout << "    " << setw(20) << left << "Item" << setw(15) << "Gluten-free" << setw(10) << "Price" << endl;

        for (int itemNumber = 0; itemNumber < NUMBER_OF_ITEMS; itemNumber++) {
            if (itemsPointer[itemNumber]->getType() == "Food") {// Only prints food items
                cout << "  " << itemNumber + 1<< " ";// Item numbers are displayed starting from 1
                itemsPointer[itemNumber]->printDetailedMenuLine();
            }
        }
    }

    // Takes an item number, a size (size only matters for drink items), and a total value
    // and adds the appropriate price to the total
    void orderItem(int itemNumber, int size, double& total) {
        itemNumber--;// Because user-facing item numbers are indexed from 1, decrements the item number

        if (itemNumber < 0 || itemNumber >= NUMBER_OF_ITEMS) {// Ensures item is on the menu
            throw invalid_argument("Invalid item number");
        }

        string sizeName = "";// Used to display the size in the output

        if (itemsPointer[itemNumber]->getType() == "Drink") {// Gets the size to display for a drink item
            switch (size) {
                case 1:
                    sizeName = "small ";
                    break;
                default:
                case 2:
                    sizeName = "regular ";
                    break;
                case 3:
                    sizeName = "large ";
                    break;
            }
        }

        // Adds the item's price to the total and displays the results
        total += itemsPointer[itemNumber]->getPrice(size);
        cout << "\n  Added one " << sizeName << itemsPointer[itemNumber]->getName() << " to your order." << endl;
        cout << "  Your total is now $" << fixed << setprecision(2) << total << endl;
    }
};

// Function prototypes
void mainMenuLoop(Menu& menu, double& total, int numberOfItems);
void basicMenuLoop(Menu& menu, double& total, int numberOfItems);
void foodMenuLoop(Menu& menu, double& total, int numberOfItems);
void drinkMenuLoop(Menu& menu, double& total, int numberOfItems);

int main() {
    double total = 0.0;
    string cafeName;

    const int NUMBER_OF_FOOD_ITEMS = 4;
    const int NUMBER_OF_DRINK_ITEMS = 3;
    const int NUMBER_OF_ITEMS = NUMBER_OF_FOOD_ITEMS + NUMBER_OF_DRINK_ITEMS;

    // Arrays used to create objects and populate menu
    string foodItems[NUMBER_OF_FOOD_ITEMS] = {"Bagel", "Muffin", "Parfait", "Croissant"};
    double foodPrices[NUMBER_OF_FOOD_ITEMS] = {3.00, 3.50, 4.00, 3.50};
    bool foodGlutenFreeStatus[NUMBER_OF_FOOD_ITEMS] = {false, false, true, false};

    string drinkItems[NUMBER_OF_DRINK_ITEMS] = {"Coffee", "Tea", "Espresso"};
    double drinkPrices[NUMBER_OF_DRINK_ITEMS] = {3.75, 3.50, 4.00};


    do {// Gets a cafe name from the user, ensuring it is not empty
        cout << "\n\n  Please enter a cafe name: ";
        getline(cin, cafeName);
    }
    while (cafeName.empty());

    Menu menu(cafeName, NUMBER_OF_FOOD_ITEMS + NUMBER_OF_DRINK_ITEMS);// Creates the menu

    // Populates menu with food and drink items
    for (int i = 0; i < NUMBER_OF_FOOD_ITEMS; i++) {
        menu.setItem(i, new FoodItem(foodItems[i], foodPrices[i], foodGlutenFreeStatus[i]));
    }
    for (int i = 0; i < NUMBER_OF_DRINK_ITEMS; i++) {
        menu.setItem(i +  NUMBER_OF_FOOD_ITEMS, new DrinkItem(drinkItems[i], drinkPrices[i]));
    }

    cout << "  Welcome to " << menu.getName() << "!" << endl;

    mainMenuLoop(menu, total, NUMBER_OF_ITEMS);// Main execution loop
    getch();
    return 0;
}

// Main execution loop, which allows users to show other menus or complete an order
void mainMenuLoop(Menu& menu, double& total, int numberOfItems) {
    int selection;

    bool continuing = true;
    while (continuing) {// Displays instructions and gets the user's choice
        cout << "\n  Please enter a number to select an option:" << endl;
        cout << "  0 Complete order" << endl;
        cout << "  1 Display simple menu" << endl;
        cout << "  2 Display detailed food menu" << endl;
        cout << "  3 Display detailed drink menu" << endl;
        cout << "  ";
        selection = getIntInRange(0, 3);

        switch (selection) {// Processes user's choice
            case 0:// Prints closing message and ends program
                cout << "  Thank you for visiting " << menu.getName() << "! Your total is $" << fixed << setprecision(2) << total << endl;
                continuing = false;
                break;
            default:
            case 1:// Opens the basic menu
                basicMenuLoop(menu, total, numberOfItems);
                break;
            case 2:// Opens the food menu
                foodMenuLoop(menu, total, numberOfItems);
                break;
            case 3:// Opens the drink menu
                drinkMenuLoop(menu, total, numberOfItems);
                break;
        }
    }
}

// Prints the menu and allows the user to order an item or go back
// As this is the basic menu, the drink size defaults to regular
void basicMenuLoop(Menu& menu, double& total, int numberOfItems) {
    while (true) {// Loops until user exits
        cout << endl;
        menu.printMenu();
        cout << "  Enter an item number to add an item to your order." << endl;
        cout << "  To go back, enter 0: ";

        // Gets the user's selection and directs execution accordingly
        int itemSelection = getIntInRange(0, numberOfItems);
        if (itemSelection == 0) {// Ends the loop if the sentinel is entered
            break;
        } else {// Orders an item
            menu.orderItem(itemSelection, 2, total);// Defaults to regular drink size if a drink is ordered
        }
    }
}

// Prints the food menu and allows the user to order an item or go back
void foodMenuLoop(Menu& menu, double& total, int numberOfItems) {
    while (true) {
        cout << endl;
        menu.printFoodMenu();
        cout << "  Enter an item number to add an item to your order." << endl;
        cout << "  To go back, enter 0: ";

        // Gets the user's selection and directs execution accordingly
        int itemSelection = getIntInRange(0, numberOfItems);

        if (itemSelection == 0) {// Ends the loop if the sentinel is entered
            break;
        } else if (menu.getItemType(itemSelection - 1) != "Food") {// Displays an error message if a valid item is entered but it is not on the food menu
            cout << "  That item is not on the food menu." << endl;
        } else {// Orders an item
            menu.orderItem(itemSelection, 2, total);// Any value can be used for size for a food item
        }
    }
}

// Prints the drink menu and allows the user to order an item or go back
void drinkMenuLoop(Menu& menu, double& total, int numberOfItems) {
    while (true) {
        cout << endl;
        menu.printDrinkMenu();
        cout << "  Enter an item number to add an item to your order." << endl;
        cout << "  To go back, enter 0: ";

        // Gets the user's selection and directs execution accordingly
        int itemSelection = getIntInRange(0, numberOfItems);

        if (itemSelection == 0) {// Ends the loop if the sentinel is entered
            break;
        } else if (menu.getItemType(itemSelection - 1) != "Drink") {// Displays an error message if a valid item is entered but it is not on the drink menu
            cout << "  That item is not on the drink menu." << endl;
        } else {// Gets the user's size selection and then orders the item
            cout << "\n  Please select a size." << endl;
            cout << "  Enter 1 for small, 2 for regular, and 3 for large: ";
            int size = getIntInRange(1, 3);
            menu.orderItem(itemSelection, size, total);
        }
    }
}

// Gets an int value in a specified  range from the user
int getIntInRange(int min, int max) {
    int userInput;

    // Executes until a valid integer is entered
    while (!(cin >> userInput) || userInput < min || userInput > max) {
        cout << "  Input error. Please try again: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    cin.ignore(INT_MAX, '\n');// Clears the input stream of any excess data after a space
    return userInput;
}

// Returns a string depending on the value of a bool
string boolToString(bool value, string trueString, string falseString) {
    if (value) {
        return trueString;
    } else {
        return falseString;
    }
}