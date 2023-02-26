/*
 * readFile.cpp
 *
 *  Created on: Jan 25, 2023
 *      Author: Group 3
 */

#include "fileOperations.h"

void writeBooksFile(deque<Book> &Inventory) {
    string filename = "../printBooks.csv";
    ofstream out(filename, std::ios::out);
    cout << "File can be found as: " << filename << endl;
    out << "ISBN,Book-Title,Book-Author,Year-Of-Publication,Publisher,Genre,Description,MSRP,Quantity" << endl;

    for (auto &book: Inventory) {
        out << book.ISBN << ",";
        out << book.title << ",";
        out << book.author << ",";
        out << book.year << ",";
        out << book.publisher << ",";
        out << book.genre << ",";
        out << book.description << ",";
        out << book.msrp << ",";
        out << book.quantity << "," << endl;
    }

    out.close();
    cout << "Done writing file." << endl;
}

void readBookFile(BookstoreInventory &inventoryObject, string filePath) {
    rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, 0));

    for (int i = 0; i < doc.GetRowCount(); ++i) {
        try {
            string ISBN = doc.GetRowName(i);
            string title = doc.GetCell<string>("Book-Title", ISBN);
            string author = doc.GetCell<string>("Book-Author", ISBN);
            int year = doc.GetCell<int>("Year-Of-Publication", ISBN);
            string publisher = doc.GetCell<string>("Publisher", ISBN);
            string description = doc.GetCell<string>("Description", ISBN);
            string genre = doc.GetCell<string>("Genre", ISBN);
            float msrp = doc.GetCell<float>("MSRP", ISBN);
            int quantity = doc.GetCell<int>("Quantity", ISBN);

            Book book(ISBN, title, author, year, publisher, description, genre, msrp, quantity);

            inventoryObject.addBook(book);
        } catch (...) {

        }
    }
}

void exportUserListToCsv(User &user) {
    deque<Book> allBooks = user.getUserList();
    writeBooksFile(allBooks);
}

void readUsersFile(UserDatabase &users, string filePath) {
    rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, 0));

    for (int i = 0; i < doc.GetRowCount(); ++i) {
        try {
            string username = doc.GetRowName(i);
            string password = doc.GetCell<string>("Password", username);
            int hashed = doc.GetCell<int>("EA Applied", username);
            int isAdmin = doc.GetCell<int>("Admin", username);

            // instantiates md5 object for hashing
            MD5 md5;
            string hashedPassword;

            // if the password is not already hashed, hash algorithm is applied
            if (hashed == 0) {
                // hashes input password
                hashedPassword = md5(password);
                password = hashedPassword;
                // sets the hash status to 1
                hashed = 1;
            }

            boost::optional<User> foundUser = users.searchUser(username);

            // if user does not exist add it
            if(!foundUser){
                User user(username, hashedPassword, hashed, isAdmin);

                users.addUser(user);
            }
        }
        catch (...) {
            cout << "Error reading users file." << endl;
        }
    }
}