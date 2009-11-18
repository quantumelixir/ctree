/*
 * ctree -- generic tree implementation with nodes capable of having
 * arbitrary number of children and siblings stored in a circularly linked
 * list for fast insertion/deletion/moving
 *
 * chillu
 * Date: Sunday November, 15 2009
 * Time: 10:18:44 PM IST 
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*
 * basic tree implementation supporting fast insertion and deletion
 * Every child points to its immediate parent, previous and next sibling
 * Parents also have a pointer to their first child
 * Sibling relationships form a circularly linked list
 *
 * TODO: XOR-Linking?
 */

typedef struct Node Node;
struct Node {
    void* data;
    Node* parent;
    Node* prevsibling;
    Node* nextsibling;
    Node* firstchild;
};

/*
 * creates and returns pointer to an empty root node
 * root nodes cannot have siblings
 */
Node*
create_tree (void* data, unsigned int n) {
     Node* root = (Node *) malloc(sizeof(Node));
     root->data = (void *) malloc(n);
     memcpy (root->data, data, n);
     root->parent = root->prevsibling = root->nextsibling = root->firstchild = (Node *) NULL;
     return root;
}

/*
 * insert a given node under a target node
 * as a child of the target. If target node
 * already has children the node is added at
 * the end.
 */
int
insert_node_under (Node* node, Node* targetparent) {

    Node* lastchild = (Node *) NULL;

    node->parent = targetparent;

    if (!targetparent->firstchild) { 
        targetparent->firstchild = node;
        node->nextsibling = node->prevsibling = node;
    } else {
        lastchild = targetparent->firstchild->prevsibling;

        lastchild->nextsibling = node;
        node->prevsibling = lastchild;

        node->nextsibling = targetparent->firstchild;
        targetparent->firstchild->prevsibling = node;
    }

    return 1;
}

/*
 * creates and returns poiner to a child under a given node
 */
Node*
create_node_under (Node* node, void* data, unsigned int n) {

    Node* newchild = (Node *) malloc(sizeof(Node));
    newchild->data = (void *) malloc(n);
    memcpy(newchild->data, data, n);

    newchild->firstchild = (Node *) NULL;
    insert_node_under (newchild, node);

    return newchild;
}

/*
 * insert a given node next to a target node
 * as a sibling of the target.
 */
int
insert_node_next_to (Node* node, Node* targetsibling) {

    Node* next = targetsibling->nextsibling;
    node->parent = targetsibling->parent;

    // take care of sibling links
    targetsibling->nextsibling = node;
    next->prevsibling = node;
    node->nextsibling = next;
    node->prevsibling = targetsibling;

    return 1;
}

/*
 * creates and inserts a sibling node next to a given node
 * returning pointer to the newly created sibling
 */
Node*
create_node_next_to (Node* node, void* data, unsigned int n) {

    Node* newsibling = (Node *) malloc(sizeof(Node));
    newsibling->data = (void *) malloc(n);
    memcpy(newsibling->data, data, n);

    newsibling->firstchild = (Node *) NULL;
    insert_node_next_to (newsibling, node);

    return newsibling;
}

/*
 * traverse recursively all the nodes under the given node
 * depth specifies the recursive-depth of the current node
 * callback function is called on each node
 */
void
traverse_node (Node* node, int depth, void (*print_data)(void* data, int depth)) {
    Node *start, *next;
    start = next = node->firstchild;

    // call printing function
    print_data (node->data, depth);

    // if the node has a child
    if (start) {
        traverse_node (next, depth + 1, print_data);
        // recurse without going round in circles
        while ((next = next->nextsibling) != start) {
            traverse_node (next, depth + 1, print_data);
        } 
    }
}

/*
 * remove (but don't delete) the node, preserving
 * silbing and parent-child relationships
 *
 * NOTE: Assumes that node is not root
 * It doesn't make sense to "remove" root
 * from a tree as root _is_ the tree
 */
int
remove_node (Node* node) {
    // remove the node out of the list of
    // children of which the node is a part 
    Node* prev = node->prevsibling;
    Node* next = node->nextsibling;
    if (node != next) { 
        // has at least one other sibling
        prev->nextsibling = next;
        next->prevsibling = prev;
    }

    // if the node is not root then you have to
    // manage parent-child relationships
    if (node->parent) {
        // the only child is about to be deleted
        if (node->nextsibling == node) { 
            node->parent->firstchild = (Node *) NULL;
        }
        // the first child of the parent is about to be deleted
        else if (node->parent->firstchild == node) {
            node->parent->firstchild = node->nextsibling;
        }
    }

    return 1;
}

/*
 * recursively delete all the children under a given node
 * maintaining the nextsibling relationships of the node
 * Returns 1 for success, 0 for failure
 *
 * TODO: Currently always returns 1 => Do error checking.
 */
int
delete_node (Node* node, int raw) {

    Node* start = node->firstchild;
    Node* curr  = (Node *) NULL;

    // if deletion is raw then don't bother maintaining
    // the prev/next sibling relationships as these nodes
    // will anyway be deleted later. (i.e. part of a
    // larger recursive deletion)
    // raw is 0 only for the toplevel invocation of delete_node
    if (!raw && node->parent) {
        remove_node (node);
    }

    // first delete all children of the node
    if (start) {
        // only one child
        if (start == start->nextsibling) { 
            delete_node (start, 1);
        }
        // more than one child
        else {
            curr = start->nextsibling;
            while (start != curr) {
                curr = curr->nextsibling;
                delete_node (curr->prevsibling, 1);
            }
            delete_node (start, 1);
        }
    }

    // delete the node itself
    free (node->data);
    free (node);

    return 1;
}

/*
 * move a given node next to a target node
 * as a sibling of the target
 * Does not check if the target is actually
 * a child of the given node (in which case
 * the results are undefined)
 */
int
move_node_next_to (Node* node, Node* targetsibling) {
    return remove_node (node) && insert_node_next_to (node, targetsibling);
}

/*
 * move a given node next to a target node
 * as a child of the target
 * Does not check if the target is actually
 * a child of the given node (in which case
 * the results are undefined)
 */
int
move_node_under (Node* node, Node* targetparent) {
    return remove_node (node) && insert_node_under (node, targetparent);
}