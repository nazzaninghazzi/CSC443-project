// AVLTree.h
#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <utility>

class memtable {
public:
    class node {
    public:
    	int key;
    	int value;
    	node* left;
    	node* right;
    	int height;
    };

    int memtable_size;
    int current_size;
    int sst_count;
    node** all_nodes_ptrs;
    node* root;

    memtable(int memtable_size);

    void allocate();
    void deallocate();
    int height(node* t);
    node* right_rotate(node* x);
    node* left_rotate(node* x);
    node* left_right_rotate(node* &x);
    node* right_left_rotate(node* &x);
    node* put_helper(int key, int value, node* tree);
    int put(int key, int value);
    int get_helper(int key, node* tree);
    int get(int key);
    void traverse(node* t, std::pair<int, int>* traversed_nodes, int& curr_idx);
};

#endif // AVL_TREE_H
