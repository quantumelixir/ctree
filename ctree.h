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
void _traverse_node (Node* node, int depth, void (*print_data)(void* data, int depth, int islastchild, unsigned int* bitmask));
void traverse_node (Node* node, void (*print_data)(void* data, int depth, int islastchild, unsigned int* bitmask));

/* remove node */
int remove_node (Node* node);

/* delete node */
int delete_node (Node* node);
int _delete_node (Node* node, int raw);

/* move node */
int move_node_next_to (Node* node, Node* targetsibling);
int move_node_under (Node* node, Node* targetparent);

/* deep copy */
Node* shallow_copy (Node* node);

void memprint (void* data, unsigned int n, FILE* fout);

/* serialize */
void serialize (Node* node, FILE* fout, unsigned int n);
void _serialize (Node* node, int depth, FILE* fout, unsigned int n);

/* unserialize */
/*
 *Node* unserialize (FILE* fin);
 */

#endif /* __CTREE_H__ */
