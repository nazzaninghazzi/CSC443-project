#include "AVLTree.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;


memtable::memtable(int memtable_size) {
	this->current_size = 0;
	this->memtable_size = memtable_size;
	this->sst_count = 0;
	this->root = NULL;
	this->all_nodes_ptrs = new node*[memtable_size];
	allocate();
}


void memtable::allocate() {
	/**
	 * Allocate memory for the memtable
	 */
	for (int i = 0; i < memtable_size; ++i) {
		node* allocated_node = (node*) malloc(sizeof(node));
		all_nodes_ptrs[i] = allocated_node;
	}
}

void memtable::deallocate() {
	/**
	* Free the allocated memory for the memtable
	*/
	for (int i = 0; i <= memtable_size; i++) {
		free(all_nodes_ptrs[i]);
	}
	root = NULL;
	current_size = 0;
}

int memtable::height(node* t) {
	/**
	 * Get the height of the tree
	 */
	return (t == NULL ? -1 : t->height);
}

memtable::node* memtable::right_rotate(node* x) {
	node* y = x->left;
	x->left = y->right;
	y->right = x;
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), x->height)+1;
	return y;
 }

memtable::node* memtable::left_rotate(node* x) {
	node* y = x->right;
	x->right = y->left;
	y->left = x;
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), x->height)+1;
	return y;
 }

memtable::node* memtable::left_right_rotate(node* &x) {
	x->left = left_rotate(x->left);
	return right_rotate(x);
 }

memtable::node* memtable::right_left_rotate(node* &x) {
	x->right = right_rotate(x->right);
	return left_rotate(x);
 }

memtable::node* memtable::put_helper(int key, int value, node* tree) {
	if (tree == NULL) {
		// store the new node at the specified allocated location
		node* new_tree = new(all_nodes_ptrs[current_size]) node();
		new_tree->key = key;
		new_tree->value = value;
		new_tree->left = NULL;
		new_tree->right = NULL;
		new_tree->height = 0;
		tree = new_tree;
		return tree;
	} else if (key < tree->key) {
		tree->left = put_helper(key, value, tree->left);
	} else {
		tree->right = put_helper(key, value, tree->right);
	}
	tree->height = max(height(tree->left), height(tree->right))+1;
	int balance = height(tree->left) - height(tree->right);

	if(balance > 1) {
		if(key < tree->left->key) {
			return right_rotate(tree);
		} else {
			return left_right_rotate(tree);
		}

	}

	else if(balance < -1) {
		if(key > tree->right->key) {
			return left_rotate(tree);
		} else {
			return right_left_rotate(tree);
		}
	}

	return tree;
}

int memtable::put(int key, int value) {
	root = put_helper(key, value, root);
	current_size ++;

	if (current_size == memtable_size) {
		 // reached the capacity so perform a scan of the entire tree i.e. implement a traversal

		 // TODO: NOTE: the traverse function below scans the entire tree. I think we should generalize it to scan the
		 // tree within a specific (start, end) range.
		 std::pair<int, int> SST[memtable_size];
		 int curr_idx = 0;
		 traverse(root, SST, curr_idx);

		 // uncomment to see all the values in the SST
//			 for (int i = 0; i < memtable_size; ++i) {
//			     std::cout << "Pair " << i << ": (" << SST[i].first << ", " << SST[i].second << ")\n";
//			 }

		std::string dir = "SSTs";
        if (mkdir(dir.c_str(), 0777) == -1) {
            if (errno != EEXIST) {
                std::cerr << "Failed to create directory!" << std::endl;
                return 1;
            }
        }

        std::string file_path = dir + "/" + std::to_string(this->sst_count) + "_SST";
        std::ofstream file(file_path);
        if (!file) {
            std::cerr << "Failed to open file!" << std::endl;
            return 1;
        }

        for (int i = 0; i < memtable_size; ++i) {
            file << "(" << SST[i].first << ", " << SST[i].second << ")\n";
        }
        
        file.close();

        this->sst_count++;

		 // free the previous memtable
		 deallocate();
		 // allocate a new memtable
		 allocate();
	}
	return 0;
}

int memtable::get_helper(int key, node* tree) {
	if (tree == NULL) {
		return 0;
	}
	if (key < tree->key) {
		return get_helper(key, tree->left);
	} else if (key > tree->key) {
		return get_helper(key, tree->right);

	} else {
		return tree->value;
	}
}

int memtable::get(int key) {
	return get_helper(key, root);
}

void memtable::traverse(node* t, pair<int, int>* traversed_nodes, int& curr_idx) {
	/**
	 * Traverse the entire memtable based on the order of keys
	 */
	if(t == NULL) { return;}
	traverse(t->left, traversed_nodes, curr_idx);
	traversed_nodes[curr_idx++] = std::make_pair(t->key, t->value);
	traverse(t->right, traversed_nodes, curr_idx);
}





/**
 *	TODO: 1. Implement the Open command to create a directory or prepare the database for operation if a directory
 *	already exists. (Nomingere)
 *	TODO: 2. Implement scans over the memtable and write the SST from memory to storage (Nomingere)
 *	TODO: 3. Extend the scan and get to search the memtable and the SSTs (Kaavya)
 *	TODO: 4. Implement the close command (Nomingere/Kaavya/Nazanin)
 */
