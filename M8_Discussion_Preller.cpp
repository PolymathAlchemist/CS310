/*
 * Student Name: Daniel Preller
 * File Name: Week8.cpp
 * Date: 5/2/2026
*/

#include <iostream>
#include <conio.h>
#include <iomanip>
#include <climits>

using namespace std;

// Stores name and price information for menu items
class MenuItem {
    private:
    string name;
    double price;

    public:
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

    double getPrice() {
        return price;
    }
};

// Menu class is used to make a menu of items and to order from it
class Menu {
    private:
    string name;
    MenuItem items[5];// For this example, the number of menu items is hardcoded to 5

    public:

    // Constructor using a name
    Menu(string name) {
        this->setName(name);
    }

    // Name set method
    void setName(string name) {
        this->name = name;
    }

    // Sets a specific item in the list
    // Validation of a valid index is left to the calling function for this example
    void setItem(int itemNumber, string name, double price) {
        items[itemNumber].setName(name);
        items[itemNumber].setPrice(price);
    }

    // Get methods
    string getName() {
        return name;
    }

    MenuItem getItem(int itemNumber) {// Validation is left to the calling function
        return items[itemNumber];
    }

    // Prints the menu information
    void printMenu() {
        cout << "  " << name << " menu:" << endl;
        int itemCounter = 1;// Used to display item numbers
        for (MenuItem item : items) {
            cout << "  " << itemCounter << " " << setw(30) << left << item.getName();
            cout << fixed << setprecision(2) << item.getPrice() << endl;
            itemCounter += 1;
        }
        cout << endl;
    }
};

// Function prototype
void orderItem(Menu& menu, double& total);

int main() {
    double total = 0;
    string cafeName;

    // arrays used for populating the menu
    string items[5] = {"Bagel", "Muffin", "Croissant", "Coffee", "Tea"};
    double prices[5] = {3.00, 4.00, 3.50, 3.75, 3.50};

    do {// Gets a cafe name from the user, ensuring it is not empty
        cout << "  \n\nPlease enter a cafe name: ";
        getline(cin, cafeName);
    }
    while (cafeName.empty());

    Menu menu(cafeName);// Creates the menu

    for (int i = 0; i < 5; i++) {// Populates the menu from the arrays
        menu.setItem(i, items[i], prices[i]);
    }

    cout << "  Welcome to " << menu.getName() << "!" << endl;

    while (true) {// Repeatedly prints the menu, instructs the user, and orders items
        menu.printMenu();
        cout << "  Enter an item number to add an item to your order." << endl;
        cout << "  To stop, enter 0: ";
        orderItem(menu, total);// the orderItem function exits the loop by exiting the program if 0 is entered
    }
}

// orderItem function is used to order items and update the total order price
void orderItem(Menu& menu, double& total) {
    int selection;

    // Executes until the user's selection is an int in menu range
    // Selection must be decreased by 1 because menu items are displayed starting from 1
    while (!(cin >> selection) || selection < 0 || selection > 5) {
        cout << "  Input error. Please try again: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    if (selection == 0) {// If user enters sentinel (0), prints message and exits the program
        cout << "  Thank you for visiting " << menu.getName() << "! Your total is $" << fixed << setprecision(2) << total << endl;
        getch();
        exit(0);

    } else {// Otherwise, gets the valid item from the menu, adds to the total, and informs the user
        MenuItem item = menu.getItem(selection - 1);
        cin.ignore(INT_MAX, '\n');// Removes any extra input entered after a space
        total += item.getPrice();
        cout << "\n  Added one " << item.getName() << " to your order." << endl;
        cout << "  Your total is now $" << fixed << setprecision(2) << total << endl << endl;
    }
}