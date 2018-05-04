//Program Created by Jermaine Lara
// 5/4/18
// Program to create a Red-Black Tree from a file or user input, 
// and can insert and print the Red-Black Tree on user command
// Based off example code from: https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

const bool COLOR_RED = true;
const bool COLOR_BLACK = false;

struct Node {
	int value;
	bool color;
	Node* left;
	Node* right;
	Node* parent;
};

Node* readFile(); 
Node* readInput(); 
Node* bstInsert(Node* root,Node* node);
Node* insert(Node* node,int value);
void printFullTree(Node* node);
void rotateLeft(Node* &root, Node* &node);
void rotateRight(Node* &root, Node* &node);
const char* getColorString(Node* node);
bool isRed(Node* node);
void repairTree(Node* &root, Node* &node);
void swapColor(Node* node1, Node* node2);

int main () {
	Node* root = NULL;
	cout << "Enter option:" << endl;
	cout << "1 to specify a file name" << endl;
	cout << "2 to enter numbers manually" << endl;
	int choice;
	cin >> choice;
	if (choice == 1) {
		root = readFile();
    }
    else if (choice == 2) {
		root = readInput();
	}
	else {
		cout << "Invalid option " << endl;
	}
	do {
		cout << "Enter option:" << endl;
		cout << "1 to print tree" << endl;
		cout << "2 to add a value into the tree" << endl;
		cout << "3 to exit" << endl;
		cin >> choice;
		if (choice == 1) {
			printFullTree(root);
		}
		
		else if (choice == 2) {
		    cout << "Enter value to add" << endl;
			int value;
			cin >> value;
			root = insert(root, value);
		}
		
		else if (choice != 3) {
			cout << "Invalid option " << endl;
		}
	}
	while(choice != 3);
	
}

// Read the numbers from a file and build RB Tree
Node* readFile() {
	char fileName[81];
    cout << "Enter file name (with comma separated values3): " << endl;
	cin >> fileName;
    int number;
	char separator;
	Node* root = NULL;
    ifstream file(fileName);
    if (file.is_open()) {
		while (file >> number)
		{
			if (number < 1 || number > 1000) {
				cout << number << " is out of range: number must be between 1 and 1000" << endl;
				break;
			}
			root = insert(root, number);
			//Read and discard the comma separator
			file >> separator;
		}
		file.close();
    }
    else {
		cout << "Unable to open file"; 
    }
	return root;
}

// Insert a value into the Red-Black Tree at the correct position
Node* insert(Node* root,int value) {
	Node* node = new Node;
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->color = COLOR_RED;
    // Do a normal BST insert
	Node * newRoot = bstInsert(root, node);
	// fix Red Black Tree violations
	repairTree(newRoot,node);
	return newRoot;
}

// Normal BST Insertion
Node* bstInsert(Node* root,Node* node) {
	if (root == NULL) {
		return node;
	}
    if (node->value < root->value) {
		root->left = bstInsert(root->left, node);
		root->left->parent = root;
	}
	else {
		root->right = bstInsert(root->right, node);
		root->right->parent = root;		
	}
	return root;
}

//Fixes Violations to Red Black tree caused by BST Insertion
void repairTree(Node* &root, Node* &node) {
	Node* parent = NULL;
	Node* grandParent = NULL;
	
	while (node != root && isRed(node) && isRed(node->parent)) {
		parent = node->parent;
		grandParent = parent->parent;
		
		// Case A
		// Node parent is left child of node grandParent
		if (parent == grandParent->left) {
			Node* uncle = grandParent->right;
			// Case 1
			// Node uncle is also red
			// Only recoloring required
			if (isRed(uncle)) {
				grandParent->color = COLOR_RED;
				parent->color = COLOR_BLACK;
				uncle->color = COLOR_BLACK;
				node = grandParent;
			}
			else {
				// Case 2
				// Node is right child of its parent
				// Left rotation required
				if (node == parent->right) {
					rotateLeft(root, parent);
					node = parent;
					parent = node->parent;
				}
				
				// Case 3
				// Node is left child opf its parent
				// Right rotation required
				rotateRight(root, grandParent);
				swapColor(parent, grandParent);
				node = parent;
			}
		}
		
		// Case B
		// Node parent is right child of grand parent
		else {
			Node* uncle = grandParent->left;
			// Case 1
			// The uncle of node is also red
			// Only recoloring required
			if (isRed(uncle)) {
				grandParent->color = COLOR_RED;
				parent->color = COLOR_BLACK;
				uncle->color = COLOR_BLACK;
				node = grandParent;
			}
			else {
				// Case 2
				// node is left child of its parent
				// Right rotation required
				if (node == parent->left) {
					rotateRight(root, parent);
					node = parent;
					parent = node->parent;
				}
				
				// Case 3
				// node is right child of its parent
				// Left rotation required
				rotateLeft(root, grandParent);
				swapColor(parent, grandParent);
				node = parent;
			}
		}
	}
	root->color = COLOR_BLACK;
}

// Returns string with color of the given node
const char* getColorString(Node* node) {
	 if(isRed(node)) {
		 return "(Red)";
	 }
	 else {
		 return "(Black)";
	 }
}	

// Prints the description of the Red-Black Tree			
void printFullTree(Node* node) {
	if(node != NULL) {
		cout << "Parent: " << node->value << " " << getColorString(node);
		if (node->left != NULL) {
			cout << ", Left Child: " << node->left->value << " " << getColorString(node->left);
		}
		if (node->right != NULL) {
			cout << ", Right Child: " << node->right->value << " " << getColorString(node->right);
		}
		cout << endl;
		printFullTree(node->left);
		printFullTree(node->right);
	}
}

// Reads the numbers from the user input and insert into Red-Black Tree
Node* readInput() {
	cout << "Enter space separated numbers" << endl;
	string input;
	cin.ignore();
	getline(cin, input);
	istringstream is(input);
	int number;
	Node* root = NULL;
	while (is >> number)
	{
		if (number < 1 || number > 999) {
			cout << number << " is out of range: number must be between 1 and 999" << endl;
			break;
		}
		root = insert(root, number);
	}
	return root;
}


void rotateLeft(Node* &root, Node* &node) {
	Node* right = node->right;
	node->right = right->left;
	if (node->right != NULL) {
		node->right->parent = node;
	}
	right->parent = node->parent;
	if (node->parent == NULL) {
		root = right;
	}
	else if (node == node->parent->left) {
		node->parent->left = right;
	}
	else {
		node->parent->right = right;
	}
	right->left = node;
	node->parent = right;
}

void rotateRight(Node* &root, Node* &node) {
	Node* left = node->left;
	node->left = left->right;
	if (node->left != NULL) {
		node->left->parent = node;
	}
	left->parent = node->parent;
	if (node->parent == NULL) {
		root = left;
	}
	else if (node == node->parent->left) {
		node->parent->left = left;
	}
	else {
		node->parent->right = left;
	}
	left->right = node;
	node->parent = left;
}

// Tells if the node is not null and color is red
bool isRed(Node* node) {
	return node != NULL && node->color == COLOR_RED;
}

// Swaps the color of two nodes
void swapColor(Node* node1, Node* node2) {
	bool temp = node1->color;
	node1->color = node2->color;
	node2->color = temp;
}