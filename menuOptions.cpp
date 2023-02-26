/*
 * menuOptions.cpp
 *
 *  Created on: Feb 19, 2023
 *      Author: roger
 */


#include "menuOptions.h"

short int showMenuOptions() {
    short int userInput;

    // prompts user to enter command
    cout << "Press 0 to exit application." << endl;
	cout << "1. To create a new books database." << endl;
	cout << "2. To add random prices to items in an existing database." << endl;
	cout << "3. To remove duplicate database entries." << endl;
	cout << "4. To add randomized quantities to books database entries." << endl;
	cout << "5. To add randomized dateAdded column and entries to books database." << endl;
    userInput = validateMenuOption();

    return userInput;
}

void printAndClearError() {
    cout << "Input cannot negative, non-numeric, or out-of-range. Please enter a positive value." << endl;

    cin.clear();  // clears error state if the user entered a string or char.
    // reference: https://cplusplus.com/forum/general/207458/
    cin.ignore(1000, '\n'); // clears the input causing error from stream.
    // reference: https://cplusplus.com/forum/general/207458/
}

short int validateMenuOption() {
    short int input;
    while (true) {
        if (std::cin >> input) { // if the user entered an integer
            cin.ignore(1000, '\n');
            return input;
        } else {
            printAndClearError();
        }
    }
}

