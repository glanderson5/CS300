//============================================================================
// ProjectTwo.cpp
// Author: Gregory Anderson
// Description: ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>

using namespace std;

// To uppercase method for course codes
// Allows for more robust user input when searching by course code
static inline string toUpperStr(string s) {
	for (char& c : s) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
	return s;
}

// Simple CSV split by comma
static vector<string> splitCSV(const string& line) {
	vector<string> tokens;
	string token;
	stringstream ss(line);
	while (getline(ss, token, ',')) {
		tokens.push_back(token);
	}
	return tokens;
}

// struct to hold course information
struct Course {
	string number;                 // "CSCI200"
	string title;                  // "Data Structures"
	vector<string> prerequisites;  // {"CSCI100", "MATH200"}
};


// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};
/**
 * Define a class containing data members and methods to
	 * implement a binary search tree
	 */
class BinarySearchTree {

private:
	Node* root;

	// helper methods 
	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	void postOrder(Node* node);
	void preOrder(Node* node);
	Node* removeNode(Node* node, string courseNumber);
	size_t size(Node* node) const;
	void collect(Node* node, vector<Course>& out) const;

	// destructor helper
	void destroy(Node* node) {
		if (node == nullptr) {
			return;
		}
		destroy(node->left);
		destroy(node->right);
		delete node;
	}

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();

	void InOrder();
	void PostOrder();
	void PreOrder();
	void Insert(Course course);
	void Remove(string courseNumber);
	Course Search(string courseNumber) const;
	size_t Size() const;
	void Collect(vector<Course>& out) const;
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	// initialize housekeeping variable
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
	destroy(root);
}

/**
 * Traverse the tree in-order
 */
void BinarySearchTree::InOrder() {
	// in-order from root
	inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
	// post-order from root
	postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
	// pre-order from root
	preOrder(root);
}

size_t BinarySearchTree::Size() const {
	return size(root);
}

size_t BinarySearchTree::size(Node* node) const {
	if (node == nullptr) return 0;
	return 1 + size(node->left) + size(node->right);
}

void BinarySearchTree::Collect(vector<Course>& out) const {
	collect(root, out);
}

void BinarySearchTree::collect(Node* node, vector<Course>& out) const {
	if (node == nullptr) return;
	collect(node->left, out);
	out.push_back(node->course);      // in-order to keep alphanumeric
	collect(node->right, out);
}


/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
	// if root == nullptr, root is the new node
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		// otherwise add into the tree starting at root
		addNode(root, course);
	}
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {
	// remove starting from root
	root = removeNode(root, courseNumber);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) const {
	// start at the root
	Node* current = root;

	// loop downward until bottom reached or match found
	while (current != nullptr) {
		if (courseNumber == current->course.number) {
			// match
			return current->course;
		}
		else if (courseNumber < current->course.number) {
			// go left
			current = current->left;
		}
		else {
			// go right
			current = current->right;
		}
	}

	// not found: return empty course
	return Course();
}

/**
 * Add a course to some node (recursive)
 */
void BinarySearchTree::addNode(Node* node, Course course) {
	// if course.number is less, go left
	if (course.number < node->course.number) {
		// if no left child, insert here
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			// otherwise recurse down the left subtree
			addNode(node->left, course);
		}
	}
	// else if course.number is greater, go right
	else if (course.number > node->course.number) {
		// if no right child, insert here
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			// otherwise recurse down the right subtree
			addNode(node->right, course);
		}
	}
	// else equal keys: overwrite (or ignore). We’ll overwrite to keep latest.
	else {
		node->course = course;
	}
}

/**
 * In-order traversal (Left, Node, Right)
 */
void BinarySearchTree::inOrder(Node* node) {
	if (node == nullptr) {
		return;
	}
	inOrder(node->left);
	cout << node->course.number << ", " << node->course.title << endl;
	inOrder(node->right);
}

/**
 * Post-order traversal (Left, Right, Node)
 */
void BinarySearchTree::postOrder(Node* node) {
	if (node == nullptr) {
		return;
	}
	postOrder(node->left);
	postOrder(node->right);
	cout << node->course.number << ", " << node->course.title << endl;
}

/**
 * Pre-order traversal (Node, Left, Right)
 */
void BinarySearchTree::preOrder(Node* node) {
	if (node == nullptr) {
		return;
	}
	cout << node->course.number << ", " << node->course.title << endl;
	preOrder(node->left);
	preOrder(node->right);
}

/**
 * Remove a course from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
	// base: not found
	if (node == nullptr) {
		return node;
	}

	if (courseNumber < node->course.number) {
		// search left subtree
		node->left = removeNode(node->left, courseNumber);
	}
	else if (courseNumber > node->course.number) {
		// search right subtree
		node->right = removeNode(node->right, courseNumber);
	}
	else {
		// match found
		// case 1: no children
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			return nullptr;
		}
		// case 2: one child (left only)
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node->left;
			delete node;
			return temp;
		}
		// case 2: one child (right only)
		else if (node->left == nullptr && node->right != nullptr) {
			Node* temp = node->right;
			delete node;
			return temp;
		}
		// case 3: two children
		else {
			// find inorder successor (smallest in right subtree)
			Node* succParent = node;
			Node* succ = node->right;
			while (succ->left != nullptr) {
				succParent = succ;
				succ = succ->left;
			}
			// copy successor's course into current node
			node->course = succ->course;

			// delete successor node from right subtree
			if (succParent == node) {
				// successor was immediate right child
				succParent->right = removeNode(succParent->right, succ->course.number);
			}
			else {
				succParent->left = removeNode(succParent->left, succ->course.number);
			}
		}
	}

	return node;
};

static bool loadCoursesFromFile(const string& filename, BinarySearchTree& bst) {
	ifstream in(filename);  // open file with filename for reading
	if (!in.is_open()) {  // if file cannot be opened:
		cout << "Error: could not open file \"" << filename << "\".\n";
		return false;
	}

	size_t before = bst.Size();
	string line;
	size_t lineNum = 0;

	while (getline(in, line)) {  // for each line in file:
		++lineNum;
		if (line.empty()) continue;  // if line is empty: continue

		vector<string> cols = splitCSV(line);  //split line by comma into values
		if (cols.size() < 2) {  // if length of values < 2:
			cout << "Format error (line " << lineNum << "): need at least CourseNumber,Title\n";
			continue;
		}


		Course c;
		c.number = toUpperStr(cols[0]);  // first column = course number
		c.title = cols[1];  // second column = course title

		for (size_t i = 2; i < cols.size(); ++i) {  // everything else is a prereq
			if (!cols[i].empty()) c.prerequisites.push_back(toUpperStr(cols[i]));
		}

		bst.Insert(c);  // add node into bst
	}

	in.close();

	cout << "Loaded " << (bst.Size() - before) << " courses from \"" << filename << "\".\n";
	return true;
}

static void validatePrerequisites(const BinarySearchTree& bst) {
	vector<Course> all;
	bst.Collect(all);
	bool anyMissing = false;

	for (const auto& c : all) {  // for each course in the bst
		for (const auto& pre : c.prerequisites) {  // for each prereq in each course
			if (bst.Search(pre).number.empty()) {  // check that the prereq is a valid course
				if (!anyMissing) {
					cout << "\nWARNING: One or more prerequisites were not found in the file:\n";
					anyMissing = true;
				}
				cout << "  - " << c.number << " lists missing prerequisite " << pre << '\n';
			}
		}
	}
	if (anyMissing) {
		cout << "Missing prerequisites will be shown as numbers without titles when printing details.\n\n";
	}
}

static void printCourseDetails(const BinarySearchTree& bst, const string& rawCourseNumber) {
	string key = toUpperStr(rawCourseNumber);
	if (key.empty()) {
		cout << "Course \"\" not found.\n";
		return;
	}

	Course c = bst.Search(key);
	if (c.number.empty()) {
		cout << "Course \"" << key << "\" not found.\n";
		return;
	}

	cout << c.number << ", " << c.title << '\n';

	if (c.prerequisites.empty()) {
		cout << "Prerequisites: None\n";
	}
	else {
		cout << "Prerequisites: ";
		for (size_t i = 0; i < c.prerequisites.size(); ++i) {
			const string& preNum = c.prerequisites[i];
			Course pc = bst.Search(preNum);
			if (!pc.number.empty()) {
				cout << pc.number; 
			}
			else {
				cout << preNum << " (not found)";
			}
			if (i + 1 < c.prerequisites.size()) cout << ", ";
		}
		cout << '\n';
	}
}

static void printMenu() {
	cout << "\n1. Load Data Structure.\n"
		<< "2. Print Course List.\n"
		<< "3. Print Course.\n"
		<< "9. Exit\n\n"
		<< "What would you like to do? ";
}

int main() {
	BinarySearchTree bst;
	bool dataLoaded = false;
	int choice = 0;

	cout << "Welcome to the course planner.\n\n";

	while (true) {
		printMenu();
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter a valid number.\n";
			continue;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consume rest of line

		if (choice == 9) {
			cout << "\nThank you for using the course planner!\n";
			break;
		}

		switch (choice) {
		case 1: {
			cout << "Enter filename: ";
			string filename;
			getline(cin, filename);
			if (loadCoursesFromFile(filename, bst)) {
				dataLoaded = true;
				validatePrerequisites(bst);
			}
			break;
		}
		case 2: {
			if (!dataLoaded) {
				cout << "\nPlease load data first (option 1).\n";
				break;
			}
			cout << "\nHere is a sample schedule:\n\n";
			bst.InOrder(); // in-order => alphanumeric
			break;
		}
		case 3: {
			if (!dataLoaded) {
				cout << "\nPlease load data first (option 1).\n";
				break;
			}
			cout << "What course do you want to know about? ";
			string courseNumber;
			getline(cin, courseNumber);
			printCourseDetails(bst, courseNumber);
			break;
		}
		default:
			cout << "\n" << choice << " is not a valid option.\n";
			break;
		}
	}

	return 0;
}