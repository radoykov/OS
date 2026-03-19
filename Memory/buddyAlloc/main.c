#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node
{
    int busy;
    struct node *left, *right;
} Node;

Node storage[2048];
int next_node = 0;

void *buddy_alloc(Node *root, int order, int target)
{
    if (root->busy || order < target)
        return NULL;
    if (order == target && !root->left)
        return (root->busy = 1, root);
    if (!root->left)
    {
        root->left = &storage[++next_node];
        root->right = &storage[++next_node];
    }
    void *res = buddy_alloc(root->left, order - 1, target);
    if (!res)
        res = buddy_alloc(root->right, order - 1, target);
    if (res)
        root->busy = (root->left->busy && root->right->busy);
    return res;
}

int main()
{
    Node *root = &storage[0];
    void *b1 = buddy_alloc(root, 10, 7);
    void *b2 = buddy_alloc(root, 10, 7);
    printf("Buddy alloc: %s, %s\n", b1 ? "Success" : "Fail", b2 ? "Success" : "Fail");

    return 0;
}