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

		unsigned short int menuOption;
		menuOption = showMenuOptions();

		while (menuOption != 0) {
			switch (menuOption) {
			case 1:
				readBooksFile();
				break;
			case 2:
				addPrices();
				break;
			case 3:
				removeDuplicates();
				break;
			case 4:
				randomQuantities();
				break;
			case 5:
				randomDateTime();
				break;
			default:
				cout << "Not a recognized command. Please enter a valid menu option." << endl;
				break;
			}

			menuOption = showMenuOptions();
		}

		cout << "Exiting database program. Goodbye!" << endl;
	return 0;
}

