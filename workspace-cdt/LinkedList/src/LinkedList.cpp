//============================================================================
// Name        : LinkedList.cpp
// Author      : Jeff Perkinson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// forward declarations
double strToDouble(string str, char ch);
void displayBid(Bid bid);

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    // Internal structure for list entries, housekeeping variables
	// (i.e. the list node)
	struct Node {
		Bid bid;			// the actual Bid
		Node* next;			// subsequent Bid pointer

		// constructors
		Node() {			// initialize a nullptr by default
			next = nullptr;
		}

		Node(Bid bid) {		// initialize a node with a bid if provided
			this->bid = bid;
			next = nullptr;
		}
	};

	Node* head;				// head pointer (i.e. start of list)
	Node* tail;				// tail pointer (i.e. end of list)
	int size;				// count of list elements

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // Initialize housekeeping variables
	// list is initially empty (i.e. no head or tail, and no elements)
	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
}

/**
 * Append a new bid to the end of the list
 * @param bid The bid to be appended to the list
 */
void LinkedList::Append(Bid bid) {
    // Implement append logic

	// initialize a new node to hold the bid passed in
	Node* node = new Node(bid);

	// Append the node to the list
	if (this->head == nullptr) {		// list is empty
		this->head = node;
	} else {
		if (this->tail != nullptr) {	// tail presently points to a node
			this->tail->next = node;	// present tail is no longer tail
		}
	}

	this->tail = node;					// appended node is new tail
	this->size++;						// list size increases by node added

	return;
}

/**
 * Prepend a new bid to the start of the list
 * @param bid The bid to be prepended to the list
 */
void LinkedList::Prepend(Bid bid) {
    // Implement prepend logic

	// initialize a new node to hold the bid passed in
	Node* node = new Node(bid);

	// Prepend the node to the beginning of the list
	if (this->head == nullptr) {		// list is empty
		this->tail = node;
	} else {
		node->next = this->head;
	}

	this->head = node;					// prepended node is new head
	this-> size++;						// list size increases by node added

	return;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // Implement print logic
	Node* currentNode = this->head;		// start at the list head

	// iterate over each list node in succession and display the bid
	while (currentNode != nullptr) {
		displayBid(currentNode->bid);
		currentNode = currentNode->next;
	}

	return;
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // Implement remove logic

	Bid bid = this->Search(bidId);

	if (!bid.bidId.empty()) {			// bid was found, remove it
		cout << "Removed bid " << bidId << endl;
		displayBid(bid);				// display the bid removed

		// special case list head is removed
		if (this->head != nullptr) {
			if (head->bid.bidId.compare(bidId) == 0) {
				Node* tempPointer = this->head;
				delete head;
				this->head = tempPointer->next;
				delete tempPointer;
				return;
			}
		}

		// remove the bid
		// Iterate over the bids
		Node* currentNode = this->head;		// start at the list head

		// iterate over each list node in succession
		while (currentNode != nullptr) {
			// if the current node's 'next' node is the one to be removed
			// update the pointers and remove the 'next' bid
			if (currentNode->next->bid.bidId.compare(bid.bidId) == 0) {
				Node* tempPointer = currentNode->next;
				delete currentNode->next;
				currentNode->next = tempPointer->next;
				delete tempPointer;
				return;
			}

			currentNode = currentNode->next;
		}

		size--;

	} else {
		cout << "Bid Id " << bidId << " not found." << endl;
	}

	return;
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 * Returns the bid if found, empty Bid otherwise
 */
Bid LinkedList::Search(string bidId) {
    // Implement search logic
	Bid* bid = new Bid();				// bid to be returned

	// Iterate over the bids
	Node* currentNode = this->head;		// start at the list head

	// iterate over each list node in succession
	while (currentNode != nullptr) {
		// early return of found bid if found
		if (currentNode->bid.bidId.compare(bidId) == 0) {
			*bid = currentNode->bid;
			return *bid;
		}

		currentNode = currentNode->next;
	}

	return *bid;						// return empty bid if not found
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return this->size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // output for testing
            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add bid to the end of the LinkedList argument provided
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

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
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        // testing keys
//        bidKey = "98109";
//        bidKey = "98223";
//        bidKey = "97991";
//        bidKey = "12345";  // For testing empty case
        bidKey = "98346";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  6. Prepend Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 3:
            bidList.PrintList();

            break;

        case 4:
            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            bidList.Remove(bidKey);

            break;

        case 6:
        	bid = getBid();
        	bidList.Prepend(bid);
        	displayBid(bid);

        	break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
