//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Jeff Perkinson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>

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

// FIXME (1): Internal structure for tree node
// Binary tree nodes have up to two child nodes- left and right
struct Node {
	Bid bid;		// the node's present bid
	// pointers to left and right nodes
	Node* left;
	Node* right;

	// default constructor
	Node() {
		this->left = nullptr;
		this->right = nullptr;
	}

	// construct Node when given a Bid argument
	Node(Bid bid) {
		this->bid = bid;
		this->left = nullptr;
		this->right = nullptr;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
	this->root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // FIXME (2a) Implement inserting a bid into the tree
	if (this->root == nullptr) {		// tree is empty
		this->root = new Node(bid);
	} else {
		// call to private recursive method that will systematically
		// insert the bid in the intended order for the binary tree
		this->addNode(root, bid);
	}
	return;
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // FIXME (4a) Implement removing a bid from the tree
	this->removeNode(root, bidId);

	return;
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // FIXME (3) Implement searching the tree for a bid

	Bid bid;

	Node* current = this->root;		// start at the root
	while (current != nullptr) {
		if (current->bid.bidId.compare(bidId) == 0) {	// bidId matches
			bid = current->bid;
			return bid;
		} else {
			// bidId is less than current node, move down the left side
			if (current->bid.bidId.compare(bidId) > 0) {
				current = current->left;
			}
			// bidId is greater than current node, move down the right side
			else {
				current = current->right;
			}
		}
	}

    return bid;		// if reached, returns an empty bid, nothing was found
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // FIXME (2b) Implement inserting a bid into the tree

	// bidId is less than current node, make it a left subtree node
	if (node->bid.bidId.compare(bid.bidId) > 0) {
		if (node->left == nullptr) {		// no left subtree node, assign bid there in a new node
			node->left = new Node(bid);
		} else {							// a left node exists, go deeper in the tree
			this->addNode(node->left, bid);	// recursive call with current left sub node as start
		}
	}
	// bidId is greater than current node, make it a right subtree node
	else {
		if (node->right == nullptr) {			// no right subtree node, assign bid there in a new node
			node->right = new Node(bid);
		} else {								// a right node exists, go deeper in tree
			this->addNode(node->right, bid);	// recursive call with current right sub node as start
		}
	}

	return;
}
void BinarySearchTree::inOrder(Node* node) {
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) {
	Node* temp = nullptr;			// temp node for swapping

	// if tree is empty, return the nullptr
	if (node == nullptr) {
		return node;
	}

	// bidId is less than current node, traverse the left subtree node
	if (node->bid.bidId.compare(bidId) > 0) {
		node->left = removeNode(node->left, bidId);
	}
	// bidId is greater than current node, traverse the right subtree node
	else if (node->bid.bidId.compare(bidId) < 0) {
		node->right = removeNode(node->right, bidId);
	}
	// bidId matches, remove the node
	else {
		// node has no children
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// node has a left child
		else if (node->left != nullptr && node->right == nullptr) {
			temp = node->left;
			node = temp;
			delete temp;
		}
		// node has a right child
		else if (node->left == nullptr && node->right != nullptr) {
			temp = node->right;
			node = temp;
			delete temp;
		}
		// node has 2 children
		// find the left most leaf of the right subtree
		else {
			temp = node->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			node->bid = temp->bid;
			node->right = removeNode(node->right, temp->bid.bidId);
		}
	}

	return node;
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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
            bst->Insert(bid);
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
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

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
            bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
