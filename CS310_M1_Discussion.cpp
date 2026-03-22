/**
 * =======================================================================
 * CS310-T301 Programming with C++
 * Instructor: Dr. Robert Flowers
 * Assignment: M2 Discussion
 * Author: Eric J. Turman
 * Date: 2026-03-21
 * Email: ejturman@my365.bellevue.edu
 *
 * Description:
 * Prompts the user for a project name and echoes it back.
 *
 * Note:
 * This version intentionally contains syntax errors and will not compile.
 * =======================================================================
 */

// =======================================================================
// Imports (Includes)
// =======================================================================
#include <iostream>
#include <string>

// ========================================================================
// Namespace
// ========================================================================

using namespace std;

// ========================================================================
// Function Declarations
// ========================================================================

string get_project_name();

// ========================================================================
// Main Execution
// ========================================================================
int main()
{
    string project_name = get_prj_name();

    cout << "Active project: " << project_name << endl

    return 0;
}


// ========================================================================
// Function Definitions
// ========================================================================

/**
 *
 * Prompt for a project name.
 *
 * Returns
 * -------
 * string
 *     The raw project name entered by the user.
 *
 * Notes
 * -----
 * getline() is used to preserve spaces in multi-word names.
 */
string get_project_name()
{
    string project_name;

    cout << "Enter the name of your current project: ";
    getline(cin, project_name);

    return project_name;
}
