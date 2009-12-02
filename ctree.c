#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
/*#include <mcheck.h>*/

#include "ctree.h"

/*
 * takes a filename and parses it to create a tree
 * depth is inferred from indentation
 */
Node*
create_tree_from_file (FILE* infile) {
    char buffer[1024];
    char* str;
    int i;

    if (!infile) {
        printf ("fatal: could not open file\n");
        exit(1);
    }

    Node* root = create_tree ("", 0);
    Node* prev = NULL;
    int prevdepth = -1;

    while (fgets (buffer, sizeof(buffer), infile) != NULL) {
        for (i = 0; buffer[i] != '\n' && isspace (buffer[i]); i++);
        str = buffer + i;
        /*printf ("|%s| %d %p\n", str, i, buffer);*/

        if (i > prevdepth) {
            prev = create_node_under (prev ? prev : root, str, strlen(str) + 1);
            prevdepth++;
        } else if (i == prevdepth) {
            prev = create_node_next_to (prev, str, strlen(str) + 1);
        } else {
            prev = create_node_next_to (prev->parent, str, strlen(str) + 1);
            prevdepth--;
        }
    }

    return root;
}

/*
 * tree style printing for string data
 */
void
print_string (void* data, int indent, int islastchild, unsigned int* bitmask) {
    int i;

    // print the current node's data with appropriate indentation
    for (i = 1; i < indent; ++i) {
        // use bitmasks to suppress printing unnecessary branches
        if (!(*bitmask & (1 << indent))) {
            printf ("|   ");
        }
        else
            printf ("    ");
    }
    if (indent) {
        if (islastchild)
            printf ("`-- ");
        else
            printf ("|-- ");
    }
    printf ("%s\n", (char *)data);
}

int
main (int argc, char **argv) {

    /*mtrace();*/

    /*
     *Node* root = create_tree_from_file (stdin);
     *traverse_node (root, 0, print_string);
     *delete_node (root, 0);
     */

    Node* nroot   = create_tree         ( "/"    , strlen("/") + 1);
    Node* home    = create_node_under   ( nroot  , "home"        , strlen("home") + 1);
    Node* chillu  = create_node_under   ( home   , "chillu"      , strlen("chillu") + 1);
    Node* code    = create_node_under   ( chillu , "code"        , strlen("code") + 1);
    Node* sandbox = create_node_next_to ( code   , "sandbox"     , strlen("sandbox") + 1);
    Node* blum    = create_node_next_to ( code   , "blum"        , strlen("blum") + 1);
    Node* shub    = create_node_next_to ( code   , "shub"        , strlen("shub") + 1);
    Node* foo     = create_node_next_to ( blum   , "foo"         , strlen("foo") + 1);
    Node* bin     = create_node_under   ( home   , "bin"         , strlen("bin") + 1);
    Node* boot    = create_node_under   ( nroot  , "boot"        , strlen("boot") + 1);
    Node* dev     = create_node_under   ( foo    , "dev"         , strlen("dev") + 1);
    Node* usr     = create_node_under   ( foo    , "usr"         , strlen("usr") + 1);

    traverse_node (nroot, print_string);
    printf ("\n");

    move_node_under (foo, nroot);

    traverse_node (nroot, print_string);
    printf ("\n");

    delete_node (chillu, 0);

    traverse_node (nroot, print_string);
    printf ("\n");

    delete_node (nroot, 0);

    /*muntrace();*/
    return 0;
}
