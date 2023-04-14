/*
* Project: Course Schedule
* Author: Brent Artuch
* Date: 4/10/2023
* Section: CS-300-T4190
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

// design for the course objects
struct Course {

    string courseNumber;
    string title;

    // storing the prerecs as a vector will allow for 
    // a dynamic amount.
    vector<string> prerequisites;
};

// internal structure for tree node
struct Node {

    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // intitialize with a course
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

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void PrintSampleSchedule();
    void Insert(Course course);
    Course GetCourseInformation(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order to print courses
 */
void BinarySearchTree::PrintSampleSchedule() {

    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {

    // if root equarl to null ptr
    if (root == nullptr) {

        // root is equal to new node bid
        root = new Node(course);
    }
    else {
        // add Node root and bid
        this->addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::GetCourseInformation(string courseNumber) {

    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching bidId found
    while (current != nullptr) {

        // if match found, return current course
        if (current->course.courseNumber.compare(courseNumber) == 0) return current->course;

        // if courseNumber is smaller than current node then traverse left
        if (courseNumber.compare(current->course.courseNumber) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }

    Course course;
    return course;
}


void BinarySearchTree::inOrder(Node* node) {

    //if node is not equal to null ptr
    if (node != nullptr) {

        //InOrder not left
        inOrder(node->left);

        //output course number and title
        cout << node->course.courseNumber << ", "
            << node->course.title
            << endl;

        //InOder right
        inOrder(node->right);
    }
}

/**
 * Add a course to some node (recursive)
 */
void BinarySearchTree::addNode(Node* node, Course course) {

    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {

        // if no left node
        if (node->left == nullptr) {

            // this node becomes left
            node->left = new Node(course);
        } 
        else {
            // recurse down the left node
            this->addNode(node->left, course);
        }
    }
    else {
        // if no right node
        if (node->right == nullptr) {

            // this node becomes right
            node->right = new Node(course);
        }
        else {
            // recurse down the right node
            this->addNode(node->right, course);
        }
    }
}

/*
* Display the course information to the console
*/
void displayCourse(Course course) {

    // output title and number to the console
    cout << course.courseNumber << ", "
        << course.title << endl;

    // output the prereqs
    cout << "Prerequisites: ";
    for (int i = 0; i < course.prerequisites.size(); i++) {

        // if i is at the last index indent and don't put a comma
        if (i == course.prerequisites.size()-1) {
            cout << course.prerequisites[i] << endl;
        }
        else {
            cout << course.prerequisites[i] << ", ";
        }
    }
    return;
}

/*
* Convert the alphabetical values of the course number to uppercase
*/
string upperCase(string str) {
    
    for (int i = 0; i <= 3; i++){

        // determine if ASCII matches a lowercase letter
        if (str[i] >= 'a' && str[i] <= 'z') 

            // convert the value to its uppercase counter part
            str[i] = str[i] - 32;        
    }

    return str;
}

/*
* Load the course from the txt file into the bst
*/
void loadCourses(string txtPath, BinarySearchTree* bst) {
    // open the text file
    ifstream textFile;
    textFile.open(txtPath, ios::in);

    // if it is open
    if (textFile.is_open()) {
        string line;

        // loop to read the lines of the file
        while (getline(textFile, line)) {

            // parse the line into seperate subStrings at the commas.
            stringstream ss(line);
            vector<string> parsedLine;
            while (ss.good()) {
                string subString;
                getline(ss, subString, ',');

                // add the substrings to the parsed line vector
                parsedLine.push_back(subString);
            }

            // insuficient items in line.
            if (parsedLine.size() < 2) {
                cout << "Error: Not enough inputs" << endl;
                return;
            }

            // build course object from parsed line
            Course course;

            // get course number and title from the first two items of the parsed line.
            course.courseNumber = parsedLine[0];
            course.title = parsedLine[1];

            // loop to add any prerequisites
            for (int i = 2; i < parsedLine.size(); i++) {
                course.prerequisites.push_back(parsedLine[i]);
            }

            // call the insert function to add the constructed course to the tree
            bst->Insert(course);
            
        }

        textFile.close();
    }

    // error message if the file fails to open.
    else {
        cout << "Error: Failed to open file." << endl;
    }

}

int main(int argc, char* argv[]) {
	cout << "Welcome to the course planner." << endl;

    // Declare the txt file path
    string txtPath = "ABCU_Advising_Program_Input.txt";

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    // Menu Interface
    int choice = 0;

    // Termination condition
    while (choice != 9) {

        try { // try-catch for correct choice input
            cout << endl;
            cout << "  1. Load Data Structure" << endl;
            cout << "  2. Print Course List" << endl;
            cout << "  3. Print Course" << endl;
            cout << "  9. Exit" << endl;
            cout << endl;
            cout << "What would you like to do? ";
            cin >> choice;
            
            // if the input isn't an integer will prevent infinite loop
            while (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid entry, please enter 1,2,3 or 9: ";
                cin >> choice;
            }

            if (choice != 1 && choice != 2 && choice != 3 && choice != 9) {
                throw choice;
            }
        }

        // error message for invalid choice
        catch (int choice) {
            cout << choice << " is not a valid option." << endl;
        }
       
        switch (choice) {
        case 1:
            cout << "Loading Data Structure..." << endl;

            // call load courses passing the file path and bst pointer
            loadCourses(txtPath, bst);
            cout << "Courses Loaded." << endl;
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            // call the in order traversal output function
            bst->PrintSampleSchedule();
            break;

        case 3:
            string courseNumber = "";
            try { // try-catch for correct courseNumber length
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
  
                if (courseNumber.length() != 7) throw courseNumber;

                // set course to the course found by the bst search function
                course = bst->GetCourseInformation(upperCase(courseNumber));

                if (!course.courseNumber.empty()) {
                    displayCourse(course);
                }
                // message if the corresponding course can't be found
                else {
                    cout << "Error: Course " << courseNumber << " not found." << endl;
                }
            }
            // error for invalid courseNumber length
            catch (string courseNumber) {
                cout << "Error: Invalid course number format." << endl;
            }
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}