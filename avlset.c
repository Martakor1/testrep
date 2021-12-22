#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) (a > b? a : b)

typedef struct NODE {
    int key;
    struct NODE *left;
    struct NODE *right;
    unsigned char height;
} node;

node *newNode(int key) {
    node *result = (node *)malloc(sizeof(node));
    result->key = key;
    result->left = NULL;
    result->right = NULL;
    result->height = 1;
    return result;
}

char height(node *a) {
    if (a) return a->height;
    else return 0;
}

void updateHeight(node *a) {
    a->height = MAX(height(a->left), height(a->right)) + 1;
}

char b(node *a) {
    if (!a) return 0;
    return height(a->right) - height(a->left);
}

node *leftRotate(node *a) {
    node *b = a->right;
    a->right = b->left;
    b->left = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

node *rightRotate(node *a) {
    node *b = a->left;
    a->left = b->right;
    b->right = a;
    updateHeight(a);
    updateHeight(b);
    return b;
}

node *balance(node *cur) {
    updateHeight(cur);
    char curB = b(cur);
    if (curB > 1) {
        if (b(cur->right) == -1) {
            cur->right = rightRotate(cur->right);
        }
        return leftRotate(cur);
    }
    else if (curB < -1) {
        if (b(cur->left) == 1) {
            cur->left = leftRotate(cur->left);
        }
        return rightRotate(cur);
    }
    return cur;
}

unsigned gi = 1;
void printTree(node *cur, FILE *file) {
    if (cur) {
        int pos;
        fprintf(file, "%i ", cur->key);
        if (cur->left) {
            ++gi;
            fprintf(file, "%i ", gi);
            pos = ftell(file);
            fprintf(file, "           ");
            printTree(cur->left, file);
            fseek(file, pos, SEEK_SET);
        } else fprintf(file, "0 ");
        if (cur->right) {
            ++gi;
            fprintf(file, "%i\n", gi);
            fseek(file, 0, SEEK_END);
            printTree(cur->right, file);
        } else fprintf(file, "0\n");
    }
}

node *add(node *cur, int key) {
    if (cur) {
        if (key < cur->key) cur->left = add(cur->left, key);
        else if (key > cur->key) cur->right = add(cur->right, key);
        return balance(cur);
    }
    else return newNode(key);
}

node *mostRight(node *cur) {
    if (cur->right) return mostRight(cur->right);
    else return cur;
}

node *del(node *cur, int key) {
    if (cur) {
        if (key < cur->key) cur->left = del(cur->left, key);
        else if (key > cur->key) cur->right = del(cur->right, key);
        else {
            if (!cur->left) {
                if (!cur->right) {
                    free(cur);
                    return NULL;
                }
                else {
                    node *rightChild = cur->right;
                    free(cur);
                    return rightChild;
                }
            }
            else {
                node *mright = mostRight(cur->left);
                cur->key = mright->key;
                cur->left = del(cur->left, mright->key);
            }
        }
        return balance(cur);
    }
    else return cur;
}

char find(node *cur, int key) {
    if (cur) {
        if (key < cur->key) return find(cur->left, key);
        else if (key > cur->key) return find(cur->right, key);
        else return 1;
    }
    return 0;
}

int main() {
    unsigned i, n;
    FILE *file = fopen("avlset.in", "r");
    fscanf(file, "%u\n", &n);
    char operator;
    int operand;
    node *root = NULL;
    FILE *output = fopen("avlset.out", "w");
    for (i = 0; i < n; i++) {
        fscanf(file, "%c %i\n", &operator, &operand);
        if (operator == 'A') {
            root = add(root, operand);
            fprintf(output, "%i\n", b(root));
        }
        else if (operator == 'D') {
            root = del(root, operand);
            fprintf(output, "%i\n", b(root));
        }
        else if (find(root, operand)) fprintf(output, "Y\n");
        else fprintf(output, "N\n");
    }
    return 0;
}