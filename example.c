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

    char ch;


    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    while((ch = getch()) != KEY_F(1))
	{
        /*
         * Support the following mappings:
         * 
         * Basic:
         * u: undo --> will take time
         * h: help
         * q: quit
         *
         * Navigation:
         * P: go to root
         * p: go to parent
         * K: go to first child
         * J: go to last child
         * <C-j>: go to next sibling
         * <C-k>: go to prev sibling
         *
         * View Manipulation:
         * <TAB>: Toggle open/close node
         * c: change tree root to selected node
         * u: move tree root up one level
         * o: open & close node
         * O: recursively open nodes
         * x: close parent of node
         * X: recursively close nodes
         *
         * Node Manipulation:
         * e: edit the current node
         *
         * Tree Manipulation:
         * d: delete node, moving child nodes one level up
         * D: recursively delete node
         * y: yank node
         * Y: recursively yank node
         * p: paste node as child of current node
         * P: paste node as sibling of current node
         */

        switch(ch)
        {
            case 'j' :
                clear();
                printw("Hello");
                break;
            case 'k' :
                clear();
                printw("World");
                break;
        }
        refresh();
	}

	getch();
	endwin();

    return 0;
}

/*
 *    FILE* fin = fopen("test", "r");
 *    Node* root = create_tree_from_file (fin);
 *    Node* chillu = root->firstchild->firstchild;
 *    Node* sandbox = chillu->firstchild->prevsibling;
 *    Node* foo = sandbox->prevsibling;
 *    Node* copy = deep_copy (chillu);
 *
 *    printf ("\nInit\n");
 *    traverse_node (root, print_string);
 *
 *    printf ("\nMove sandbox under root\n");
 *    move_node_under (sandbox, root);
 *    traverse_node (root, print_string);
 *
 *    printf ("\nMove foo under chillu's parent\n");
 *    move_node_under (foo, chillu->parent);
 *    traverse_node (root, print_string);
 *
 *    printf ("\nUnserializing input...\n");
 *    traverse_node (unserialize(stdin), print_string);
 *
 *    printf ("\nDelete chillu\n");
 *    delete_node (chillu);
 *    traverse_node (root, print_string);
 *
 *    printf ("\nPrinting the saved deep_copy of chillu\n");
 *    delete_node (root);
 *    traverse_node (copy, print_string);
 *
 *    fclose (fin);
 */
