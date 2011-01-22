#ifndef __CTREE_H__
#define __CTREE_H__

/* Buffer size */
#define BSIZE 1024

/* Node structure */
typedef struct Node Node;
struct Node {
    // data
    void* data;

    // links
    Node* parent;
    Node* prevsibling;
    Node* nextsibling;
    Node* firstchild;
};

/* create tree */
Node* create_tree (void* data);

/* insert node */
int insert_node_under (Node* node, Node* targetparent);
int insert_node_next_to (Node* node, Node* targetsibling);

/* create node */
Node* create_node_under (Node* node, void* data);
Node* create_node_next_to (Node* node, void* data);

/* traverse tree */
void traverse_node (Node* node,
        void (*print_data)(void*, int, int, unsigned int*));

/* detach node */
int detach_node (Node* node);

/* delete node */
int delete_node (Node* node);

/* move node */
int move_node_next_to (Node* node, Node* targetsibling);
int move_node_under (Node* node, Node* targetparent);

/* deep copy */
Node* shallow_copy (Node* node);

#endif /* __CTREE_H__ */
