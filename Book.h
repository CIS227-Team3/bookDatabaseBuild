/*
 * Book.h
 *
 *  Created on: Jan 25, 2023
 *      Author: Group 3
 */

#include <string>

using namespace std;

#ifndef BOOK_H_
#define BOOK_H_

#include <chrono>

//Book class, creates variables related to the Book object
class Book {
public:
    Book();

    Book(const string &isbn, const string &title, const string &author, int year, const string &publisher);

    Book(const string &isbn, const string &title, const string &author, int year, const string &publisher,
         const string &description, const string &genre, const float &msrp, const int &quantity);

    string ISBN;
    string title;
    string author;
    int year;
    string publisher;
    string description;
    string genre;
    float msrp;
    unsigned int quantity;

private:
    float generatePrice();
};

#endif /* BOOK_H_ */
