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

#include "Book.h"

#include <fstream>
#include <list>
#include <deque>
#include <chrono>

using namespace std;

#ifndef FILEOPERATIONS_H_
#define FILEOPERATIONS_H_


void readBooksFile();
float generatePrice();
void addPrices();
int getNumRows(string filename);
deque<string> getTableRows(string filename);
void updateQuantities();

#endif /* FILEOPERATIONS_H_ */
