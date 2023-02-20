/*
 * Book.cpp
 *
 *  Created on: Jan 25, 2023
 *      Author: Group 3
 */

#include "Book.h"
float Book::generatePrice() {
	float price;

	srand(time(NULL)); // may need to include chrono to make this work

	// generates random integer between 3499 and 10397
	// reference https://cplusplus.com/forum/beginner/183358/
	// rand() % maxNum + minNum
	int randomPrice = rand() % 10397 + 3499;

	// divides random number by 100 to get it to two decimal places
	price = randomPrice / 100;

	return price;
}

Book::Book() {
    ISBN = "none";
    title = "none";
    author = "none";
    publisher = "none";
    year = 2000;
    msrp = generatePrice();
    quantity = 1;
}

Book::Book(const string &isbn, const string &title, const string &author, int year, const string &publisher) {
	ISBN = isbn;
	this->title = title;
	this->author = author;
	this->year = year;
	this->publisher = publisher;
	msrp = generatePrice();
	quantity = 1;
}

Book::Book(const string &isbn, const string &title, const string &author, int year, const string &publisher,
           const string &description, const string &genre, const float &msrp, const int &quantity) :
        ISBN(isbn), title(title), author(author), year(year), publisher(publisher), description(description),
        genre(genre), msrp(msrp), quantity(quantity) {}
