#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct NODE {
    int value;
    struct NODE *left;
    struct NODE *right;
} NODE;

typedef struct BST {
    int size;
    NODE *root;
} BST;


BST * bst_init() {
    BST *tree = (BST *)malloc(sizeof(BST));

    return tree;
}

void _bst_insert_recurse(NODE *tree_node, NODE *new_node) {
    // ug, this seems to work but looks ugly... 

    if (new_node->value <= tree_node->value) {
        if (tree_node->left) {
            _bst_insert_recurse(tree_node->left, new_node);
        } else {
            tree_node->left = new_node;
        }
    } else {
        if (tree_node->right) {
            _bst_insert_recurse(tree_node->right, new_node);
        } else {
            tree_node->right = new_node;
        }
    }
}

void bst_insert(BST *tree, int value) {
    NODE *new_node = (NODE *)malloc(sizeof(NODE));
    new_node->value = value;
        
    if (!tree->root) {
        tree->root = new_node;
    } else {
        _bst_insert_recurse(tree->root, new_node);
    }

    tree->size++;
}

int _bst_exists_recurse(NODE *tree_node, int value) {
    if (!tree_node) {
        return 0;
    } else if (tree_node->value == value) { 
        return 1;
    } else if (tree_node->value > value) {
        return _bst_exists_recurse(tree_node->left, value);
    } else if (tree_node->value < value) {
        return _bst_exists_recurse(tree_node->right, value);
    } else {
        return 0; // not sure what happened!
    }
}

int bst_exists(BST *tree, int value) {
   return _bst_exists_recurse(tree->root, value); 
}

NODE * _bst_max_recurse(NODE *tree_node) {
    return (tree_node->right) ?  _bst_max_recurse(tree_node->right) : tree_node;
} 

int bst_max(BST *tree) {
    NODE *result = _bst_max_recurse(tree->root);

    return result->value;
}

NODE * _bst_min_recurse(NODE *tree_node) {
    return (tree_node->left) ?  _bst_min_recurse(tree_node->left) : tree_node;
} 

int bst_min(BST *tree) {
    NODE *result = _bst_min_recurse(tree->root);

    return result->value;
}

int main() { 
    int i;

    BST *tree = bst_init();

    // seed random numbers
    srand(time(NULL));

    for (i = 0; i < 500; i++) {
        bst_insert(tree, rand() % 1000);
    }

    printf("SIZE: %d\n", tree->size);
    printf("ROOT: %d\n", tree->root->value);
    printf("LEFT: %d\n", tree->root->left->value);
    printf("LEFT, LEFT: %d\n", tree->root->left->left->value);


    printf("Max VALUE: %d\n", bst_max(tree));
    printf("MIN VALUE: %d\n", bst_min(tree));

    printf("32 exists? %s\n", bst_exists(tree, 32) ? "YES" : "NO");
    printf("55 exists? %s\n", bst_exists(tree, 55) ? "YES" : "NO");
    printf("12 exists? %s\n", bst_exists(tree, 12) ? "YES" : "NO");
    printf("3 exists? %s\n", bst_exists(tree, 3) ? "YES" : "NO");

    
    
    return 0;
}
