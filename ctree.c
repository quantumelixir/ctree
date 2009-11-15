#include <ncurses.h>
#include <unistd.h>

#include "ctree.h"

int
main (int argc, char **argv) {

    Node* root    = create_tree         ( "/");
    Node* home    = create_node_under   ( root, "home");
    Node* chillu  = create_node_under   ( home, "chillu");
    Node* code    = create_node_under   ( chillu, "code");
    Node* sandbox = create_node_next_to ( code, "sandbox");
    Node* blum    = create_node_next_to ( code, "blum");
    Node* shub    = create_node_next_to ( code, "shub");
    Node* foo     = create_node_next_to ( blum, "foo");
    Node* bin     = create_node_under   ( home, "bin");
    Node* boot    = create_node_under   ( root, "boot");
    Node* dev     = create_node_under   ( foo, "dev");
    Node* usr     = create_node_under   ( foo, "usr");

    traverse_node (root, 0);
    printf ("\n");

    move_node_under (foo, chillu);

    traverse_node (root, 0);
    printf ("\n");

    delete_node (chillu, 0);

    traverse_node (root, 0);
    printf ("\n");

    return 0;
}
