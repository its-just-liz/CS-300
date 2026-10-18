//============================================================================
// Name        : Project Two
// Author      : Elizabeth Negrete
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Final Project
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// define a structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
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
    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

// Implement binary search tree
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void deleteTree(Node* node);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course search(string courseNumber);
    void Clear();
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
    // Initialize root as empty
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    deleteTree(root);
}

// Recursively delete all node in the tree
void BinarySearchTree::deleteTree(Node* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

// Remove all courses from the tree
void BinarySearchTree::Clear() {
    deleteTree(root);
    root = nullptr;
}

// Traverse the tree in order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Insert a course into the tree
void BinarySearchTree::Insert(Course course) {
    // If the tree is empty, create a root node
    if (root == nullptr) {
        root = new Node(course);
    }

    // else, find where the course belongs
    else {
        addNode(root, course);
    }
}


// Search for a course
Course BinarySearchTree::search(string courseNumber) {

    // set current node equal to root
    Node* current = root;

    // keep looping downwards until bottom reached or matching course found
    while (current != nullptr) {

        // if match found, return current course
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }

        // if course is smaller than current node then traverse left
        if (courseNumber < current->course.courseNumber) {
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

// Add course to the tree
void BinarySearchTree::addNode(Node* node, Course course) {

    // if node is larger then add to left
    if (node->course.courseNumber > course.courseNumber) {
        
        // if no left node, make one
        if (node->left == nullptr) {
            node->left = new Node(course);
        }

        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }

    // if the course number is larger, move right
    else if (course.courseNumber > node->course.courseNumber) {
        
        // if no right node, make one
        if (node->right == nullptr) {
            node->right = new Node(course);
        }

        //else recurse down the right node
        else {
            addNode(node->right, course);
        }
    }
}

// Print courses in Alphanumeric Order
void BinarySearchTree::inOrder(Node* node) {

    //if node is not equal to null ptr
    if (node != nullptr) {

        //Visit left subtree first
        inOrder(node->left);

        // Display course number and title
        cout << node->course.courseNumber << ", "
            << node->course.courseTitle << endl;

        //Visit the right subtree
        inOrder(node->right);
    }
}

// Load course information from CSV file
bool loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // Open the CSV file
    ifstream file(csvPath);

    // Check if file opens successfully
    if (!file.is_open()) {
        cout << "Error: Cannot open file." << endl;
        return false;
    }

    string line;
    int courseCount = 0;
    vector<Course> courses;

    // Read each line of the file
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        Course course;

        // Read the course number
        if (!getline(ss, course.courseNumber, ',')) {
            continue;
        }

        // Read the course title
        if (!getline(ss, course.courseTitle, ',')) {
            continue;
        }

        // Skip courses with missing information
        if (course.courseNumber.empty() || course.courseTitle.empty()) {
            continue;
        }

        // Read any prerequisites
        while (getline(ss, value, ',')) {
            if (!value.empty()) {
                course.prerequisites.push_back(value);
            }
        }

        // Check if course already exists
        Course existingCourse = bst->search(course.courseNumber);

        if (!existingCourse.courseNumber.empty()) {
            cout << "Warning: Duplicate course " << course.courseNumber
                << " skipped." << endl;
            continue;
        }

        // Add the course to tree
        bst->Insert(course);
        courses.push_back(course);
        courseCount++;
    }

    // Validate prerequisites
    for (const Course& course : courses) {

        // Check each prerequisite
        for (const string& prerequisite : course.prerequisites) {

            // Search for the prerequisite
            Course requiredCourse = bst->search(prerequisite);

            // Display an error if the prerequisite doesn't exist
            if (requiredCourse.courseNumber.empty()) {
                cout << "Warning: " << prerequisite << " prerequisite for "
                     << course.courseNumber << " was not found." << endl;
            }
        }
    }

    // Close the file
    file.close();

    if (courseCount == 0) {
        cout << "Error: No valid courses were loaded." << endl;
        return false;
    }

    cout << courseCount << " courses loaded successfully." << endl;
    return true;
}


// Main function
int main() {

    //Create a binary search tree to store courses
    BinarySearchTree bst;

    string csvPath;
    string courseNumber;
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner!" << endl;

    // Keep displaying the menu until the user exits
    while (choice != 9) {

        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            if (cin.eof()) {
                break;
            }

            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        // Load course information
        switch (choice) {

            // Load courses
        case 1: {
            cout << "Enter the CSV filename: ";
            getline(cin >> ws, csvPath);

            // Check if file can be opened
            ifstream file(csvPath);

            if (file.is_open()) {
                file.close();

                // Clear existing courses
                bst.Clear();

                // Load course information
                dataLoaded = loadCourses(csvPath, &bst);
            }
            else {
                cout << "Error: Cannot open file" << endl;
            }

            break;
        }

            // Print course list
            case 2:
                if (dataLoaded) {
                    cout << "Here is a sample schedule:" << endl;
                    cout << endl;
                    bst.InOrder();
                }

                else {
                    cout << "Please load the course data first." << endl;
                }
                break;

            // Print course
            case 3: {
                if (dataLoaded) {
                    
                    cout << "What course do you want to know about? ";
                    cin >> courseNumber;

                    // Convert course number to uppercase
                    for (char& c : courseNumber) {
                        c = toupper(static_cast<unsigned char>(c));
                    }

                    // Search for course
                    Course course = bst.search(courseNumber);

                    // Display course information if found
                    if (!course.courseNumber.empty()) {
                        cout << course.courseNumber << ", "
                            << course.courseTitle << endl;

                        // Display prerequisites
                        cout << "Prerequisites: ";

                        if (course.prerequisites.empty()) {
                            cout << "None";
                        }

                        else {
                            for (size_t i = 0; i < course.prerequisites.size(); i++) {
                                string prerequisite = course.prerequisites[i];

                                Course requiredCourse = bst.search(prerequisite);

                                cout << prerequisite;

                                if (!requiredCourse.courseNumber.empty()) {
                                    cout << " (" << requiredCourse.courseTitle << ")";
                                }

                                if (i + 1 < course.prerequisites.size()) {
                                    cout << ", ";
                                }
                            }
                        }
                        cout << endl;
                    }
                    else {
                        cout << "Course not found." << endl;
                    }
                }

                else {
                    cout << "Please load the course data first." << endl;
                }
                break;
            }

            // Exit
            case 9: {
                cout << "Thank you for using the course planner!" << endl;
                break;
            }

            // Invalid menu options
            default: 
                cout << choice << " is not a valid option." << endl;
                break;

            }
    }
    return 0;
}

