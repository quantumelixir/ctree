#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "ctree.h"

/*
 * takes a filename and parses it to create a tree
 * depth of an item is inferred from its relative
 * indentation to the previous item
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

    Node* root = create_tree ("/", 2);
    Node* prev = NULL;
    int prevdepth = -1;

    while (fgets (buffer, sizeof(buffer), infile) != NULL) {
        for (i = 0; buffer[i] != '\n' && isspace (buffer[i]); i++);
        str = buffer + i;
        str[strlen(str) - 1] = '\0';

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
print_string (void* data, int indent, bool islastchild, unsigned int* bitmask) {
    int i;

    // print the current node's data with appropriate indentation
    for (i = 1; i < indent; ++i) {
        // use bitmasks to suppress printing unnecessary branches
        if (!((*bitmask) & (1 << i))) {
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

    Node* nroot = create_tree_from_file (stdin);
    Node* chillu = nroot->firstchild->firstchild;
    Node* sandbox = chillu->firstchild->prevsibling;
    Node* foo = sandbox->prevsibling;
    Node* copy = deep_copy (chillu);

    printf ("\nInit\n");
    traverse_node (nroot, print_string);

    printf ("\nMove sandbox under root\n");
    move_node_under (sandbox, nroot);
    traverse_node (nroot, print_string);

    printf ("\nMove foo under chillu's parent\n");
    move_node_under (foo, chillu->parent);
    traverse_node (nroot, print_string);

    printf ("\nDelete chillu\n");
    delete_node (chillu);
    traverse_node (nroot, print_string);

    printf ("\nPrinting saved deep_copy of chillu\n");
    delete_node (nroot);
    traverse_node (copy, print_string);

    printf ("\nSerializing the content of chillu\n");
    serialize(copy, stdout);

    return 0;
}
