//============================================================================
// Name        : HashTable.cpp
// Author      : Jeff Perkinson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;
const unsigned int DEFAULT_KEY = UINT_MAX;			// max unsigned int value

// forward declarations
double strToDouble(string str, char ch);


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

void displayBid(Bid bid);

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    //(1): Define structures to hold bids
	struct BidNode {
		Bid bid;
		unsigned int key;		// key value for hashing
		BidNode* next;			// for implementing collision chaining

		// default constructor
		BidNode() {
			this->key = DEFAULT_KEY;
			this->next = nullptr;
		}

		// initialize a bid node with a bid
		BidNode(Bid bid) {
			BidNode();			// initialize an empty node with null next
			this->bid = bid;	// set the bid in the node to the bid passed in
		}

		// initialize with a bid and a key
		BidNode(Bid bid, unsigned int key) {
			BidNode();
			this->bid = bid;
			this-> key = key;			// set key to the key passed in
		}
	};

	vector<BidNode> bidNodes;			// vector to hold bid nodes

	// Define the hash table size for the modulo hash algorithm
	// DEFAULT_SIZE is 179 (smaller monthly file for testing)
	unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // (2): Initialize the structures used to hold bids
	bidNodes.resize(tableSize);					// resize the vector to the desired size
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // (3): Implement logic to free storage when class is destroyed
	// erase function removes specified elements from the vector
	// begin() denotes removal of the first element in the vector
	this->bidNodes.erase(this->bidNodes.begin());
	return;
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // (4): Implement logic to calculate a hash value
	// use modulo division to return the remainder of the key by hash table size
	return key % this->tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // (5): Implement logic to insert a bid

	// create the key- a hash of the bid's bidId
	// requires conversion of bidId from string object to string to int
	// key denotes bucket in the table to insert the bid
	unsigned int key = this->hash(atoi(bid.bidId.c_str()));

	// check whether the bucket presently holds data
	// if it does, chain a linked list together
	BidNode* keyNode = &(this->bidNodes.at(key));		// desired node bucket

	if (keyNode == nullptr) {							// bucket is empty, insert node at desired bucket
		BidNode* newNode = new BidNode(bid, key);
		bidNodes.insert(bidNodes.begin() + key, *newNode);
	} else {
		if (keyNode->key == DEFAULT_KEY) {				// Bucket has an unused node, replace it
			keyNode->key = key;							// with populated node for this bid
			keyNode->bid = bid;
			keyNode->next = nullptr;
		} else {										// Chain the nodes in the desired bucket
			while (keyNode->next != nullptr) {			// iterate to the end of the chain
				keyNode = keyNode->next;
			}
			keyNode->next = new BidNode(bid, key);		// append a new node with bid to end of last node
		}
	}

	return;
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // (6): Implement logic to print all bids
	// Iterate over each index in the vector and loop through
	// potential chains

	for (unsigned int key = 0; key < this->bidNodes.size(); ++key) {
		BidNode* iterationNode = &(this->bidNodes.at(key));
		if (iterationNode == nullptr ||
			iterationNode->key == DEFAULT_KEY) {				// bucket is empty or unused, do nothing
			continue;
		} else if (iterationNode->next == nullptr) {			// bucket has a single node
			displayBid(iterationNode->bid);
		} else {											// Chain of nodes in the desired bucket
			while (iterationNode->next != nullptr) {			// iterate to the end of the chain
				displayBid(iterationNode->bid);
				iterationNode = iterationNode->next;
			}
			displayBid(iterationNode->bid);					// print the last node in the chain
		}
	}

	return;
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // (7): Implement logic to remove a bid
	unsigned int key = this->hash(atoi(bidId.c_str()));		// the key for the bidId passed in

	BidNode* searchNode = &(this->bidNodes.at(key));

	if (searchNode == nullptr ||
	  	searchNode->key == DEFAULT_KEY) {					// no node found or is found but unused
	   	cout << "Bid ID " << bidId << " not found.";
	   	return;
	}

	if (searchNode != nullptr && 							// node found, is not empty
	   	searchNode->key != DEFAULT_KEY &&					// and matches the bidId argument
		searchNode->bid.bidId.compare(bidId) == 0) {

		// remove the bid found
		delete searchNode;

	} else {													// node is found but buried in chain

	   	while (searchNode != nullptr) {
	   		if (searchNode->next->key != DEFAULT_KEY &&
	   			searchNode->next->bid.bidId.compare(bidId) == 0) {	// next node matches, remove the bid
	   				searchNode->next = searchNode->next->next;
	   				delete searchNode->next;
	   		}
	   		searchNode = searchNode->next;
	   	}
	}

	cout << "Bid ID " << bidId << " removed." << endl;

	return;
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // (8): Implement logic to search for and return a bid
    unsigned int key = this->hash(atoi(bidId.c_str()));		// the key for the bidId passed in

    BidNode* searchNode = &(this->bidNodes.at(key));

    if (searchNode == nullptr ||
    	searchNode->key == DEFAULT_KEY) {					// no node found or is found but unused
    	return bid;											// return empty bid
    }

    if (searchNode != nullptr && 							// node found, is not empty
    	searchNode->key != DEFAULT_KEY &&					// and matches the bidId argument
		searchNode->bid.bidId.compare(bidId) == 0) {

    	bid = searchNode->bid;								// return the bid found

    } else {												// node is found but buried in chain
    	while (searchNode != nullptr) {
    		if (searchNode->key != DEFAULT_KEY &&
    			searchNode->bid.bidId.compare(bidId) == 0) {	// node matches, return the bid
    				return searchNode->bid;
    		}
    		searchNode = searchNode->next;
    	}
    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
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
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, searchValue;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        searchValue = "98109";
        break;
    case 3:
        csvPath = argv[1];
        searchValue = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
//        searchValue = "98109";
//        searchValue = "97951";
        searchValue = "98190";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(searchValue);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << searchValue << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(searchValue);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
