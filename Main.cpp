/*
 * Main.cpp
 *
 *  Created on: Feb 14, 2023
 *      Author: roger
 */

#include <iostream>
#include "fileOperations.h"
#include "menuOptions.h"

using namespace std;

int main() {
		cout << "Please choose: " << endl;
		cout << "1. To create a new books database." << endl;
		cout << "2. To add random prices to items in an existing database." << endl;
		cout << "3. To remove duplicate database entries." << endl;
		cout << "0. To exit." << endl;

		unsigned short int menuOption;
		cin >> menuOption;

		while (menuOption != 0) {
			switch (menuOption) {
			case 1:
				readBooksFile();
				break;
			case 2:
				addPrices();
				break;
			case 3:
				updateQuantities();
				break;
			default:
				cout << "Not a recognized command. Please enter a valid menu option." << endl;
				break;
			}

			menuOptions();
		}

		cout << "Exiting database program. Goodbye!" << endl;
	return 0;
}

