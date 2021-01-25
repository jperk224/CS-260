//============================================================================
// Name        : VectorSorting.cpp
// Author      : Jeff Perkinson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
 * Prompt user for bid information using console (std::in)
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
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

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
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
	// start at the extreme ends of the list and work in to the midpoint,
	// swapping elements along the way where necessary
	int low = begin;
	int high = end;

	// placeholder for swapping bids
	Bid* temp = new Bid();

	int midPoint = (begin + (end - begin)) / 2;
	Bid pivotValue = bids.at(midPoint);

	// flag for looping
	bool done = false;

	while (!done) {
		// compare the low point to the midpoint

		// keep incrementing low while title is less than the pivot value title
		while (bids.at(low).title.compare(pivotValue.title) < 0) {
			++low;
		}

		// keep decrementing high while title is greater than the pivot value title
		while (pivotValue.title.compare(bids.at(high).title) < 0) {
			--high;
		}

		// once the low counter meets or exceeds the high counter
		// the partitions have been defined (i.e. values lower than pivotValue
		// are in the low partition and values higher than pivotValue are in
		// the higher partition; flip the flag to exit the loop
		if (low >= high) {
			done = true;

		} else {
			// swap the element at the low index and the high index to
			// place the bid value in the the proper partition
			*temp = bids.at(low);
			bids.at(low) = bids.at(high);
			bids.at(high) = *temp;
		}

		++low;
		--high;
	}

	// return the highest index of the low partition
	return high;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {

	int midPoint = 0;

	// base case/recursion exit condition
	// if one bid or less, exit
	if (begin >= end) {
		return;
	}

	// Partition the vector
	// midPoint is the location of the last (i.e. highest)
	// indexed element in the low partition

	midPoint = partition(bids, begin, end);

	quickSort(bids, begin, midPoint);
	quickSort(bids, midPoint + 1, end);
}

// Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {

	// track the index of the minimum value for each iteration
	unsigned int minValIndex;
	Bid* temp = new Bid();					// temp Bid holder for sorting

	// outer loop to iterate over each vector element
	for (unsigned int i = 0; i < bids.size(); ++i) {
		minValIndex = i;		// start with min val at current iteration index

		// inner loop to compare each vector element against the current iteration
		for (unsigned int j = i + 1; j < bids.size(); ++j) {
			// compare the current outer iteration element against subsequent
			// elements in the list; make note of the minimum value index
			if (bids.at(j).title.compare(bids.at(minValIndex).title) < 0) {
				// element at index j is smallest found so far,
				// set minValIndex to new minimum value
				minValIndex = j;
			}
		}

		// set current i iteration to minimum value (i.e. element at j)
		// if there's been a change
		// everything less than current i iteration is sorted
		if (minValIndex != i) {
			*temp = bids.at(i);
			bids.at(i) = bids.at(minValIndex);
			bids.at(minValIndex) = *temp;
		}
	}

	return;
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
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (unsigned int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        // Invoke the selection sort and report timing results
        case 3:
        	// Initialize a timer variable before sorting bids
        	ticks = clock();

        	// sort the bids
        	selectionSort(bids);

        	// Calculate elapsed time and display the result
        	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
        	cout << "Sorted " << bids.size() << " bids alphabetically by title." << endl;
        	cout << "time: " << ticks << " clock ticks" << endl;
        	cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

        	break;

        // Invoke the quick sort and report timing results
        // Invoke the selection sort and report timing results
        case 4:
        	// Initialize a timer variable before sorting bids
        	ticks = clock();

        	// sort the bids
        	quickSort(bids, 0, bids.size() - 1);

        	// Calculate elapsed time and display the result
        	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
        	cout << "Sorted " << bids.size() << " bids alphabetically by title." << endl;
        	cout << "time: " << ticks << " clock ticks" << endl;
        	cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

        	break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
