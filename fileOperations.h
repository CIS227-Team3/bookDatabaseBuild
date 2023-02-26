/*
 * fileOperations.h
 *
 *  Created on: Feb 16, 2023
 *      Author: roger
 */

#include <iostream>
#include <string>
#include "includes/rapidcsv.h"
#include "includes/json.hpp"
#include "includes/sqlite3.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#include "Book.h"

#include <fstream>
#include <list>
#include <deque>
#include <chrono>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/date_time/posix_time/posix_time_io.hpp"

using namespace std;
using namespace boost::posix_time;

#ifndef FILEOPERATIONS_H_
#define FILEOPERATIONS_H_


void readBooksFile();
float generatePrice();
void addPrices();
int getNumRows(string filename);
deque<string> getTableRows(string filename);
void removeDuplicates();
unsigned short int generateQuantity();
void randomQuantities();
void addDateTimeColumn(string filename);
void randomDateTime();
string generateRandomDate();

#endif /* FILEOPERATIONS_H_ */
