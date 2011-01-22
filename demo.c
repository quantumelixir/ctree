#include <ncurses.h>
#include <unistd.h>
#include <string.h>
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
    static char buffer[1024];
    char* str = NULL;
    int i;

    if (!infile) {
        printf ("fatal: could not open file\n");
        exit(1);
    }

    Node* root = create_tree ("/");
    Node* prev = NULL;
    int prevdepth = -1;

    while (fgets (buffer, sizeof(buffer), infile) != NULL) {
        for (i = 0; buffer[i] != '\n' && isspace (buffer[i]); i++);
        str = buffer + i;
        str[strlen(str) - 1] = '\0';
        str = (char *) malloc (strlen (str) + 1);
        strcpy (str, buffer + i);

        if (i > prevdepth) {
            prev = create_node_under (prev ? prev : root, str);
            prevdepth++;
        } else if (i == prevdepth) {
            prev = create_node_next_to (prev, str);
        } else {
            prev = create_node_next_to (prev->parent, str);
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

    FILE* fin = fopen("../test", "r");
    Node* root = create_tree_from_file (fin);
    fclose (fin);

    Node* chillu = root->firstchild->firstchild;
    Node* sandbox = chillu->firstchild->prevsibling;
    Node* foo = sandbox->prevsibling;
    Node* copy = shallow_copy (chillu);

    printf ("\nInit\n");
    traverse_node (root, print_string);

    printf ("\nMove sandbox under root\n");
    move_node_under (sandbox, root);
    traverse_node (root, print_string);

    printf ("\nPrinting the saved shallow_copy of chillu\n");
    traverse_node (copy, print_string);
    delete_node (copy);

    printf ("\nMove foo under chillu's parent\n");
    move_node_under (foo, chillu->parent);
    traverse_node (root, print_string);

    printf ("\nDelete chillu\n");
    delete_node (chillu);
    traverse_node (root, print_string);

    delete_node (root);
    return 0;
}