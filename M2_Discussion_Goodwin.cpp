#include <iostream>

using namespace std;

int main()
{
    string favoriteGame;
    int hoursPlayed;

    cout << "What is your favorite video game? ";
    cin >> favoriteGame;

    cout << "How many hours have you played it? ";
    cin >> hoursPlayed;

    cout << "Your favorite game is " << favoriteGame
         << " and you have played it for " << hoursPlayed 
         << " hours." << endl;

    return 0;
}

/*
 * DEBUGGING FIXES
 * Line 14 : Added semicolon line terminator
 * Line 16 : Changed >> extraction operator to << insertion operator
 */
