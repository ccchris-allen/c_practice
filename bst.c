#include <stdio.h>
#include <stdlib.h>

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

void _bst_insert_recursively(NODE *tree_node, NODE *new_node) {
    if (!tree_node) {
       tree_node = new_node;
    } else if (new_node->value <= tree_node->value) {
        _bst_insert_recursively(tree_node->left, new_node);
    } else {
        _bst_insert_recursively(tree_node->right, new_node);
    }
}

void bst_insert(BST *tree, int value) {
    NODE *new_node = (NODE *)malloc(sizeof(NODE));
    new_node->value = value;

    if (!tree->root) {
        tree->root = new_node;
    }
        
    _bst_insert_recursively(tree->root, new_node);

    tree->size++;
}


int main() { 
    BST *tree = bst_init();

    bst_insert(tree, 3);
    bst_insert(tree, 32);
    bst_insert(tree, 2);

    printf("ROOT: %d\n", tree->root->value);
}
