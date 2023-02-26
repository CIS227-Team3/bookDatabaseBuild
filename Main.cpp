/*
 * Main.cpp
 *
 *  Created on: Jan 25, 2023
 *      Author: Group 3
 */

#include <iostream>
#include <string>

#include "UserDatabase.h"
#include "menuOptions.h"
#include "fileOperations.h"

using namespace std;

int main() {
    string password;
    string username;

    BookstoreInventory inventory;
    UserDatabase users;
    User currentUser;
    Book foundBook;

    short unsigned int menuOption;
    string title;

    string ISBN;
    string author;
    int year;
    string publisher;
    string description;
    string genre;
    float price;
    unsigned short int quantity;

    cout << "---------------------------" << endl;
    cout << "Thank you for using BOOKIN!" << endl;
    cout << "---------------------------" << endl;
    cout << endl;
    cout << "Please type in your username:" << endl;
    cin >> username;

    cout << "Please type in your password:" << endl;
    cin >> password;

    // removes newline character from buffer
    cin.clear();
    cin.ignore(1000, '\n');

    if (users.loginUser(username, password)) {
        currentUser = users.getCurrentUser();
        currentUser.getUserShoppingList(inventory);
        displayUserOptions();

        // only displays the admin options if the user is an admin
        if (currentUser.getAdminStatus() == 1) {
            displayAdminOptions();
        }

        menuOption = validateMenuOption();

        while (menuOption != 0) {
            switch (menuOption) {
                case 1: {
                    displayUserOptions();
                    break;
                }
                case 2: {
                    cout << "Please enter a title to search: " << endl;
                    getline(cin, title);
                    inventory.searchForBook(title);
                    break;
                }
                case 3: {
                    cout << "Please add the following information. " << endl;

                    cout << "Add Book-Title: " << endl;
                    getline(cin, title);

                    cout << "ISBN, Book-Title, Book-Author, Year-Of-Publication, Publisher, Price, Quantity: "
                         << endl;
                    cout << "Add ISBN: " << endl;
                    getline(cin, ISBN);
                    cout << "Add Book-Author: " << endl;
                    getline(cin, author);
                    cout << "Add Year-Of-Publication: " << endl;
                    year = validateYear(); // validates that the year entered is an integer
                    cout << "Add Publisher: " << endl;
                    getline(cin, publisher);
                    cout << "Add Price: " << endl;
                    price = validatePrice();
                    cout << "Add Quantity: " << endl;
                    quantity = validateQuantity();

                    // By default, user does not have to enter a description or a genre at the time it is added to the database
                    Book book(ISBN, title, author, year, publisher, "", "", price, quantity);

                    // adds book to inventory
                    inventory.addBook(book);

                    break;
                }
                case 4: {
                    cout << "Please enter title of book to delete:" << endl;
                    getline(cin, title);
                    inventory.deleteBook(title);
                    break;
                }
                case 5: {
                    cout << "Please enter title: " << endl;
                    getline(cin, title);
                    cout << "Please enter description/plot: " << endl;
                    getline(cin, description);
                    cout << "Please enter genre: " << endl;
                    getline(cin, genre);

                    // updates the description and genre of the specified book.
                    inventory.updateDescription(title, description);
                    inventory.updateGenre(title, genre);
                    break;
                }
                case 6: {
                    cout << "Please enter title of book to add:" << endl;
                    getline(cin, title);
                    foundBook = inventory.searchForBook(title);
                    currentUser.addToUserList(foundBook);
                    break;
                }
                case 7: {
                    currentUser.listUserList();
                    break;
                }
                case 8: {
                    exportUserListToCsv(currentUser);
                    break;
                }
                case 9: {
                    cout << "Please enter the title of the book you would like to add to your cart:" << endl;
                    getline(cin, title);

                    Book book = inventory.searchForBook(title);
                    currentUser.addToUserShoppingList(book);
                    break;
                }
                case 10: {
                    currentUser.listUserShoppingList();
                    break;
                }
                case 11: {
                    currentUser.saveUserShoppingList();
                    break;
                }
                case 12: {
                    if (currentUser.getAdminStatus() == 1) {
                        users.addUser();
                    } else {
                        cout << "Menu option not recognized." << endl;
                    }
                    break;
                }
                case 13: {
                    if (currentUser.getAdminStatus() == 1) {
                        users.updateUserPassword();
                    } else {
                        cout << "Menu option not recognized." << endl;
                    }
                    break;
                }
                case 14: {
                    if (currentUser.getAdminStatus() == 1) {
                        string filePath;
                        cout << "Please enter the name of the book file you would like to import:" << endl;
                        cin >> filePath;

                        readBookFile(inventory, filePath);
                    } else {
                        cout << "Menu option not recognized." << endl;
                    }
                    break;
                }
                case 15: {
                    if (currentUser.getAdminStatus() == 1) {
                        string filePath;
                        cout << "Please enter the name of the users file you would like to import:" << endl;
                        cin >> filePath;

                        readUsersFile(users, filePath);
                    } else {
                        cout << "Menu option not recognized." << endl;
                    }
                    break;
                }
                default: {
                    cout << "Menu option not recognized." << endl;
                    break;
                }
            }
            // displays menu options and gets/validates user input
            displayUserOptions();
            if (currentUser.getAdminStatus() == 1) {
                displayAdminOptions();
            }

            menuOption = validateMenuOption();
        }
    }
    cout << "Exiting BOOKIN. Goodbye!" << endl;

    return EXIT_SUCCESS;
}
