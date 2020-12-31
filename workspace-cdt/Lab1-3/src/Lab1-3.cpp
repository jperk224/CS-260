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

// (1): Data structure to hold bid information together as a single unit.
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

// (4): Display the bid values passed in data structure

/**
 * Display the bid information
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

// (3): Store input values in Bid data structure

/**
 * Prompt user for bid information
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

/**
 * The one and only main() method
 */
int main() {

    // Declare instance of Bid data structure to hold bid information
	Bid userBid;

    // loop to display menu until exit chosen
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter Bid" << endl;
        cout << "  2. Display Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // Complete the method calls then test the program based on user input
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
