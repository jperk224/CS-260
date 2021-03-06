//============================================================================
// Name        : Lab1-3.cpp
// Author      : Jeff Perkinson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 1-3 Up to Speed in C++
//============================================================================

#include <algorithm>
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Data structure to hold bid information together as a single unit.
struct Bid {
	string articleTitle;
	string fund;
	string vehicleId;
	double amount;
};

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * Display information for a given Bid instance
 *
 * @param Bid data structure containing the bid info
 */

void displayBid(Bid userBid) {
    cout << "Title: " << userBid.articleTitle << endl;
    cout << "Fund: " << userBid.fund << endl;
    cout << "Vehicle: " << userBid.vehicleId << endl;
    cout << "Bid Amount: " << userBid.amount << endl;

    return;
}

/**
 * Prompt user for bid information and store input values in Bid data structure
 *
 * @return Bid data structure containing the bid info
 */

Bid getBid() {
    Bid userBid;

    cout << "Enter title: ";
    cin.ignore();
    getline(cin, userBid.articleTitle);

    cout << "Enter fund: ";
    cin >> userBid.fund;

    cout << "Enter vehicle: ";
    cin.ignore();
    getline(cin, userBid.vehicleId);

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    userBid.amount = strToDouble(strAmount, '$');

    return userBid;
}

int main() {

    // Bid instance to hold bid information
	Bid userBid;

    // display menu until exit chosen
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter Bid" << endl;
        cout << "  2. Display Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // Completed method calls to test the program based on user input
        switch (choice) {
            case 1:
            	userBid = getBid();
                break;
            case 2:
                displayBid(userBid);
                break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
