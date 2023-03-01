/*
 * fileOperations.cpp
 *
 *  Created on: Feb 16, 2023
 *      Author: roger
 */

#include "fileOperations.h"

using namespace std;

float generatePrice() {
	float price;

	srand(time(NULL)); // may need to include chrono to make this work

	// generates random integer between 3499 and 10397
	// reference https://cplusplus.com/forum/beginner/183358/
	// rand() % maxNum + minNum
	int randomPrice = (rand() % (10397 - 3499)) + 3499;

	// divides random number by 100 to get it to two decimal places
	price = randomPrice / 100.00;

	return price;
}

unsigned short int generateQuantity() {
	unsigned short int randomQuantity;

	/*
	// reference https://www.boost.org/doc/libs/1_66_0/doc/html/boost_random/tutorial.html#boost_random.tutorial.generating_integers_in_a_range
	// instantiates generator object
	boost::random::mt19937 randNumGen;

	// generates random number between 2 and 56
	boost::random::uniform_int_distribution<> dist(2, 56);

	return dist(randNumGen);
	*/

	srand(time(NULL));
	randomQuantity = (rand() % 55) + 2;

	return randomQuantity;
}

string generateRandomDate() {
	unsigned short int hour;
	unsigned short int minute;
	unsigned short int second;
	unsigned short int day;
	unsigned short int month;
	unsigned short int year;

	string dateTime;

	hour = (rand() % 12) + 1; // gets a random number between 1 and 12
	minute = (rand() % 60); // gets a random number between 0 and 59
	second = (rand() % 60); // gets a random number between 0 and 59

	month = (rand() % 12) + 1; // gets a random number between 1 and 12
	year = (rand() % 24) + 2000; // gets a random year between 2000 and 2023

	switch (month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		day = (rand() % 31) + 1; // creates a random number between 1 and 31
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		day = (rand() % 30) + 1; // creates a random number between 1 and 30
		break;
	case 2:
		if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { // leap year
			day = (rand() % 29) + 1; // creates a random number between 1 and 29
		}

		else { // non-leap year
			day = (rand() % 28) + 1; // creates a random number between 1 and 28
		}
		break;

	}

	// reference https://www.boost.org/doc/libs/1_55_0/doc/html/date_time/posix_time.html#date_time.posix_time.time_duration
	boost::gregorian::date randomDate(year, month, day); // sets the date to the random numbers generated for year, month, and day

	// reference: https://thispointer.com/get-current-date-time-in-c-example-using-boost-date-time-library/
	boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
	boost::gregorian::date currentDate = timeLocal.date();

	if (randomDate > currentDate) {
		month = 1;
		boost::gregorian::date randomDate(year, month, day);
		time_duration randomTime(hour, minute, second); // sets the time to the random numbers generated for hour, minute, and second
		boost::posix_time::ptime randomDateAdded(randomDate, randomTime); // creates time object
		dateTime = to_iso_extended_string(randomDateAdded);
	}

	else {
		time_duration randomTime(hour, minute, second); // sets the time to the random numbers generated for hour, minute, and second
		boost::posix_time::ptime randomDateAdded(randomDate, randomTime); // creates time object
		dateTime = to_iso_extended_string(randomDateAdded);
	}

	return dateTime;
}

int getNumRows(string filename) {
	string rowCountQuery = "SELECT COUNT(*) FROM books as rowCount";
	int numRows = 0;

	const char* file = filename.c_str();

		// reads user database information
		sqlite3 *bookDB;
		try {
			// gets the number of rows in the database
			// checks that database opened successfully
			if (sqlite3_open(file, &bookDB) == SQLITE_OK) {

				// instantiates statement object
				sqlite3_stmt *rowCount;
				sqlite3_prepare_v2(bookDB, rowCountQuery.c_str(), rowCountQuery.length(), &rowCount, nullptr);

				// checks that the end of the database rows has not been reached
				sqlite3_step(rowCount);
				numRows = sqlite3_column_int(rowCount, 0); // row count is in the first (and only) column
				cout << "Number of rows: " << numRows << endl;

				// sql statement destructor
				sqlite3_finalize(rowCount);
			}
		}
		catch(...) {
			cout << "Error accessing database and counting rows." << endl;
		}

		sqlite3_close(bookDB);

	return numRows;
}

deque<string> getTableRows(string filename) {
	deque<string> isbnRows;
	string row;

	string selectQuery = "SELECT isbn FROM books";

	const char* file = filename.c_str();

	// reads user database information
	sqlite3 *booksDB;
	try {
		// gets the number of rows in the database
		// checks that database opened successfully
		if (sqlite3_open(file, &booksDB) == SQLITE_OK) {

			// instantiates statement object
			sqlite3_stmt *select;
			sqlite3_prepare_v2(booksDB, selectQuery.c_str(), selectQuery.length(), &select, nullptr);

			while (sqlite3_step(select) != SQLITE_DONE) {
				row = (const char*)sqlite3_column_text(select, 0);
				isbnRows.push_back(row);
			}

			// sql statement destructor
			sqlite3_finalize(select);
		}
	}
	catch(...) {
		cout << "Error accessing database and counting rows." << endl;
	}

	sqlite3_close(booksDB);

	return isbnRows;
}

void removeDuplicates() {
	string filename;
	cout << "Please enter the name of the file you would like to update: " << endl;
	cin >> filename;

	const char* file = filename.c_str();
	deque<Book> duplicates;
	string isbn;
	string title;
	string author;
	int year;
	string publisher;
	string description;
	string genre;
	float msrp;
	int numDuplicates;


	// gets the rows with duplicate entries
	string dupQuery = "SELECT isbn, title, author, MIN(year), publisher, description, genre, msrp, COUNT(title) FROM books GROUP BY title HAVING COUNT(title) > 1;";
	string copyTableQuery = "CREATE TABLE copyBooks(isbn text PRIMARY KEY NOT NULL, title text NOT NULL, author text NOT NULL, year integer NOT NULL, publisher text NOT NULL, description text, genre text, msrp real NOT NULL, quantity integer NOT NULL)";
	string removeDupQuery = "INSERT INTO copyBooks SELECT * FROM books GROUP BY title";
	string dropTableQuery = "DROP TABLE books";
	string renameTableQuery = "ALTER TABLE copyBooks RENAME TO books";
	string updateQuery = "UPDATE books SET quantity = ? WHERE title = ?";
	sqlite3* booksDB;

	try {
		if (sqlite3_open(file, &booksDB) == SQLITE_OK) {
			// constructs dupQuery statement
			sqlite3_stmt *duplicate;
			if (sqlite3_prepare_v2(booksDB, dupQuery.c_str(), dupQuery.length(), &duplicate, nullptr) == SQLITE_OK) {

				while (sqlite3_step(duplicate) != SQLITE_DONE) {
					isbn = (const char*)sqlite3_column_text(duplicate, 0);
					title = (const char*)sqlite3_column_text(duplicate, 1);
					author = (const char*)sqlite3_column_text(duplicate, 2);
					year = sqlite3_column_int(duplicate, 3);
					publisher = (const char*)sqlite3_column_text(duplicate, 4);
					description = (const char*)sqlite3_column_text(duplicate, 5);
					genre = (const char*)sqlite3_column_text(duplicate, 6);
					msrp = sqlite3_column_double(duplicate, 7);
					numDuplicates = sqlite3_column_int(duplicate, 8);

					cout << "Title: " << title << " NumDuplicates: " << numDuplicates << endl;
					Book book(isbn, title, author, year, publisher, description, genre, msrp, numDuplicates);
					duplicates.push_back(book);

				}

				sqlite3_finalize(duplicate);

				// executes the copy table query
				sqlite3_stmt *copyTable;
				sqlite3_prepare_v2(booksDB, copyTableQuery.c_str(), copyTableQuery.length(), &copyTable, nullptr);
				sqlite3_step(copyTable);
				sqlite3_finalize(copyTable);

				// inserts rows without duplicates to the copied table
				sqlite3_stmt *removeDups;
				sqlite3_prepare_v2(booksDB, removeDupQuery.c_str(), removeDupQuery.length(), &removeDups, nullptr);
				sqlite3_step(removeDups);
				sqlite3_finalize(removeDups);

				// drops original table
				sqlite3_stmt *dropTable;
				sqlite3_prepare_v2(booksDB, dropTableQuery.c_str(), dropTableQuery.length(), &dropTable, nullptr);
				sqlite3_step(dropTable);
				sqlite3_finalize(dropTable);

				// renames the copy table to original name
				sqlite3_stmt *renameTable;
				sqlite3_prepare_v2(booksDB, renameTableQuery.c_str(), renameTableQuery.length(), &renameTable, nullptr);
				sqlite3_step(renameTable);
				sqlite3_finalize(renameTable);

				// updates the quantities with the number of duplicate entries
				sqlite3_stmt *update;
				sqlite3_prepare_v2(booksDB, updateQuery.c_str(), updateQuery.length(), &update, nullptr);


				for (unsigned int i = 0; i < duplicates.size(); ++i) {
					title = duplicates.at(i).title;
					numDuplicates = duplicates.at(i).quantity;

					cout << "Title: " << title << " Duplicates: " << numDuplicates << " updated." << endl;

					sqlite3_bind_int(update, 1, numDuplicates);
					sqlite3_bind_text(update, 2, title.c_str(), title.length(), NULL);
					sqlite3_step(update);
					sqlite3_reset(update);

					cout << "Remaining records to update: " << duplicates.size() - i << endl;
				}
				sqlite3_finalize(update);
 			}

			else {
				cout << "Problems preparing statement" << endl;
			}
		}
	}
	catch (...) {
		cout << "Error creating new table" << endl;
	}

	sqlite3_close(booksDB);
}

void randomQuantities() {
	string filename;

	string updateQuery = "UPDATE books SET quantity = ? WHERE isbn = ?";

	cout << "Please enter the name of the books database you would like to generate random quantities for: " << endl;
	cin >> filename;
	filename = "books.db";

	deque<string> isbns = getTableRows(filename);
	unsigned int numRows = getNumRows(filename);
	string isbn;
	unsigned short int randomQuantity;

	const char* file = filename.c_str();
	sqlite3 *booksDB;
	try {
		if (sqlite3_open(file, &booksDB) == SQLITE_OK) {
			sqlite3_stmt *update;
			if (sqlite3_prepare_v2(booksDB, updateQuery.c_str(), updateQuery.length(), &update, nullptr) == SQLITE_OK) {
				for (unsigned int i = 0; i < numRows; ++i) {
					isbn = isbns.at(i);
					randomQuantity = generateQuantity();

					cout << "Quantity: " << randomQuantity << endl;
					cout << "Records remaining: " << numRows - i << endl;

					sqlite3_bind_int(update, 1, randomQuantity);
					sqlite3_bind_text(update, 2, isbn.c_str(), isbn.length(), NULL);
					sqlite3_step(update);
					sqlite3_reset(update);
				}
			}
			sqlite3_finalize(update);
		}
	}

	catch (...) {
		cout << "Error adding randomized quantities to database." << endl;
	}

	sqlite3_close(booksDB);
	cout << "Randomized quantity update complete." << endl;
}

void readBooksFile() {
    unsigned int added = 0;
    unsigned int failed = 0;

    string filePath;
    string tempDBName;

    cout << "Please enter the name of the file you would like to convert to a database: " << endl;
    cin >> filePath;

    cout << "Please enter the name of the database you want to create." << endl;
    cin >> tempDBName;
    const char* dbName = tempDBName.c_str();
    rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, 0));

    sqlite3 *booksDB;
    if (sqlite3_open(dbName, &booksDB) != SQLITE_OK) {
    	cout << "Error opening database." << endl;
    }

    // column names not necessary in query, added for clarity
    string insertQuery = "INSERT INTO books(isbn, title, author, year, publisher, description, genre) VALUES(?, ?, ?, ?, ?, ?, ?)";
	sqlite3_stmt *insert;
	// strings must be converted to c_str to work with sqlite3
	sqlite3_prepare_v2(booksDB, insertQuery.c_str(), insertQuery.length(), &insert, nullptr);

    for (unsigned int i = 0; i < doc.GetRowCount(); ++i) {
        try {
            string ISBN = doc.GetRowName(i);
            string title = doc.GetCell<string>("Book-Title", ISBN);
            string author = doc.GetCell<string>("Book-Author", ISBN);
            int year = doc.GetCell<int>("Year-Of-Publication", ISBN);
            string publisher = doc.GetCell<string>("Publisher", ISBN);
            string description = doc.GetCell<string>("Description/Plot", ISBN);
            string genre = doc.GetCell<string>("Genre", ISBN);


            sqlite3_bind_text(insert, 1, ISBN.c_str(), ISBN.length(), NULL);
            sqlite3_bind_text(insert, 2, title.c_str(), title.length(), NULL);
            sqlite3_bind_text(insert, 3, author.c_str(), author.length(), NULL);
            sqlite3_bind_int(insert, 4, year);
            sqlite3_bind_text(insert, 5, publisher.c_str(), publisher.length(), NULL);
            sqlite3_bind_text(insert, 6, description.c_str(), description.length(), NULL);
            sqlite3_bind_text(insert, 7, genre.c_str(), genre.length(), NULL);

            sqlite3_step(insert);
            sqlite3_reset(insert);

            added++;

        }
        catch (...) {
            cout << "Problem reading book in CSV with ISBN: " << doc.GetRowName(i) << ", did not add to inventory."
                 << endl;
            failed++;
        }
    }
    sqlite3_finalize(insert);
    sqlite3_close(booksDB);
    cout << "Added " << added << " books to inventory. Failed adding: " << failed << endl;
}

void addPrices() {

	string tempFilename;
	cout << "Please enter the name of the file to update: " << endl;
	cin >> tempFilename;
	const char* filename = tempFilename.c_str();

	float price;
	int numRows = getNumRows(tempFilename);
	string row;

	deque<string> rows = getTableRows(tempFilename);

	string updateQuery = "UPDATE books SET msrp = ROUND(?, 2) WHERE isbn = ?";

	sqlite3 *booksDB;

	try {
		if (sqlite3_open(filename, &booksDB) == SQLITE_OK) {
			// creates statement object
			sqlite3_stmt *update;
			// prepares statement object
			if (sqlite3_prepare_v2(booksDB, updateQuery.c_str(), updateQuery.length(), &update, nullptr) == SQLITE_OK) {
				// binds the randomly generated price to the first parameter of the update statement

				for (int i = 0; i < numRows; ++i) {
					row = rows.at(i);
					price = generatePrice();
					cout << "Price: " << price << endl;
					cout << "Records remaining: " << numRows - i << endl;
					sqlite3_bind_double(update, 1, price);
					sqlite3_bind_text(update, 2, row.c_str(), row.length(), nullptr);
					sqlite3_step(update);
					sqlite3_reset(update);
				}
			}
			sqlite3_finalize(update);
		}
	}

	catch(...) {
		cout << "Error adding prices to " << tempFilename << "." << endl;
	}

	sqlite3_close(booksDB);

	cout << "Price update complete." << endl;
}

void addDateTimeColumn(string filename) {

	const char* file = filename.c_str();
	string addColQuery = "ALTER TABLE books ADD COLUMN dateAdded TEXT";

	sqlite3* booksDB;

	try {
		if (sqlite3_open(file, &booksDB) == SQLITE_OK) {
			sqlite3_stmt *addCol = NULL;
			if (sqlite3_prepare_v2(booksDB, addColQuery.c_str(), addColQuery.length(), &addCol, nullptr) == SQLITE_OK) {
				sqlite3_step(addCol);
				cout << "dateAdded column successfully added to " << filename << endl;
			}
			sqlite3_finalize(addCol);
		}
	}

	catch (...) {
		cout << "Error adding dateAdded column to database." << endl;
	}

	sqlite3_close(booksDB);
}

void randomDateTime() {
	string filename;
	cout << "Please enter the name of the file you would like to add a dateAdded column and randomized entries to: " << endl;
	cin >> filename;

	addDateTimeColumn(filename);
	string randomDate;
	unsigned int numRows = getNumRows(filename);
	deque<string> isbns = getTableRows(filename);
	string isbn;

	const char* file = filename.c_str();
	string updateQuery = "UPDATE books SET dateAdded = ? WHERE isbn = ?";

	sqlite3 *booksDB;
	try {
		if (sqlite3_open(file, &booksDB) == SQLITE_OK) {
			sqlite3_stmt *update;
			if (sqlite3_prepare_v2(booksDB, updateQuery.c_str(), updateQuery.length(), &update, nullptr) == SQLITE_OK) {
				for (unsigned int i = 0; i < numRows; ++i) {
					randomDate = generateRandomDate();
					isbn = isbns.at(i);

					sqlite3_bind_text(update, 1, randomDate.c_str(), randomDate.length(), NULL);
					sqlite3_bind_text(update, 2, isbn.c_str(), isbn.length(), NULL);

					sqlite3_step(update);
					sqlite3_reset(update);

					cout << "ISBN: " << isbn << " Date generated: " << randomDate << ". Records remaining: " << numRows - i << endl;
				}
			}
			sqlite3_finalize(update);
		}
	}

	catch(...) {
		cout << "Error adding randomized time entries to " << filename << endl;
	}

	sqlite3_close(booksDB);
}
