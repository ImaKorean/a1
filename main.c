#include <stdio.h>   //For printf(),scanf(),fopen(),fclose(),fgets()
#include <stdlib.h>  // To use EXIT_FAILURE, EXIT_SUCCESS, malloc, calloc, realloc, free
#include <stdbool.h> // To use bool, true, false

// start!
// I used shift + option + f to format clear ((Shift + Alt + F) in window)
/*During development I used 
#ifdef DEBUG blocks for error checking, 
but before submission I removed all debug/temporary 
code to improve readability and ensure no unnecessary 
output remains in the final build.”
*/

typedef struct Node
{
    int id;              // (A=0, B=1....)
    struct Node *parent; // parent  (null if no parent)
    int children[26];    // index of childrens
    int child_count;     // number of childrens
} Node;

// Node lookup table for AtoZ (each index stores the single Node* for that letter)
static Node *nodes[26]; // node[i] stores the address of the node.

static Node *new_node(int id)
{
    Node *node = (Node *)calloc(1, sizeof *node); // allocate memory for exactly one Node and initialize all fields to 0.
    if (!node)
        return NULL; // for error
    // put initial values
    node->id = id;

    node->parent = NULL;
    node->child_count = 0;
    return node;
}

// to free nodes
static void free_nodes()
{
    for (int i = 0; i < 26; i++)
    {
        free(nodes[i]);
        nodes[i] = NULL;
    }
}

static bool add_child(Node *parent, int child_id)
{
    // already present? -> ok
    for (int i = 0; i < parent->child_count; i++)
    {
        if (parent->children[i] == child_id)
        {
            return true; // ignore duplicates
        }
    }
    if (parent->child_count >= 26)
        return false;
    parent->children[parent->child_count] = child_id;
    parent->child_count = parent->child_count + 1;
    return true;
}

static bool addNodeWithletter(char mom, char son)
{
    int momid = (int)(mom - 'A'); // make char to int
    int sonid = (int)(son - 'A'); // make char to int

    if (!nodes[momid])
        nodes[momid] = new_node(momid);
    Node *parent = nodes[momid];
    if (!parent)
        return false;

    if (!nodes[sonid])
        nodes[sonid] = new_node(sonid);
    Node *child = nodes[sonid];
    if (!child)
        return false;
    // It means child has parents and parents and existed parented is different, which is false.
    if (child->parent && child->parent != parent)
        return false;

    // Start from the candidate parent
    Node *cur = parent;

    // Go up the ancestor chain until there's no parent 
    while (cur != NULL) {
        // if we find the child in this chain, linking parent->child creates a loop
        if (cur == child) {
            return false;  // cycle detected
        }
        // Move one level up
        cur = cur->parent;
    }

    child->parent = parent;
    return add_child(parent, sonid);
}

// insertion sort
static void insertion(int *child, int numChild)
{
    for (int i = 1; i < numChild; i++)
    {

        int key = child[i];

        int j = i - 1;

        while (j >= 0 && child[j] > key)
        {
            child[j + 1] = child[j]; // move right
            j--;                     // keep checking left
        }

        child[j + 1] = key;
    }
}

static void print_mytree()
{
    for (int i = 0; i < 26; i++)
    {
        Node *root = nodes[i];
        if (!root || root->parent) // if it is not root, skip
            continue;

        printf("%c\n", 'A' + i); // if you pass top of this code, print the root first

        // use BFS, level order, hierarchy ouput.
        int cur[26]; // to store what next values are in each level, ex) lev 1 - [A], lev 2 =[B,C]
        int cn = 1;  // to store the number of nodes, it is root, so 1
        cur[0] = i;

        while (cn > 0)
        {
            int next[26];      // temporary store values in next level
            int numOfNode = 0; // temporary store the number of nodes in next level
            for (int j = 0; j < cn; j++)
            {
                if (j > 0) // it can't pass if there is just one value in the same level
                    printf(" ");
                Node *parent = nodes[cur[j]];

                if (parent->child_count == 0) // there is no child, print #
                {
                    printf("#");
                }
                else
                {
                    int kids[26]; // make temporary array to protect to change children values
                    for (int k = 0; k < parent->child_count; k++)
                        kids[k] = parent->children[k]; // add that into temporary array

                    insertion(kids, parent->child_count); // use insertion sort - change DC to CD

                    for (int k = 0; k < parent->child_count; k++)
                    {
                        printf("%c", 'A' + kids[k]); // print the values in level
                        next[numOfNode++] = kids[k]; // store the next level's values into the next array
                    }
                }
            }

            printf("\n"); // line change in each level

            cn = numOfNode;
            for (int j = 0; j < cn; j++)
                cur[j] = next[j];
        }

        printf("\n"); // empty line between trees and another trees or last
    }
}

int validate_inputs_add_node(FILE *sf)
{
    int getletter;
    int count = 0;
    char a = 0, b = 0;

    while (true)
    {
        getletter = fgetc(sf);

        if (getletter == '\n' || getletter == EOF)
        {

            if (count == 0)
            { // skip empty line
                if (getletter == EOF)
                    break;
                continue;
            }

            if (count != 2)
            {
                return EXIT_FAILURE;
            }

            if (!addNodeWithletter(a, b))
            {

                return EXIT_FAILURE;
            }

            count = 0;
            a = 0;
            b = 0;
            if (getletter == EOF)
                break;
            continue;
        }
        else
        {
            if (getletter < 'A' || getletter > 'Z')
            {
                return EXIT_FAILURE;
            }
            if (count >= 2)
            {
                return EXIT_FAILURE;
            }

            if (count == 0)
            {
                a = (char)getletter;
            }
            else
            {
                b = (char)getletter;
            }
            count++;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "need Your inputfile\n");
        return EXIT_FAILURE;
    }

    const char *inputFileName = argv[1];
    FILE *storeFile = fopen(inputFileName, "r");
    if (!storeFile)
    {
        fprintf(stderr, "Failed to open: %s\n", inputFileName);
        return EXIT_FAILURE;
    }

    int result = validate_inputs_add_node(storeFile);
    fclose(storeFile);

    if (result != EXIT_SUCCESS)
    {
        printf("INVALID\n\n");
        free_nodes();
        return EXIT_SUCCESS;
    }

    print_mytree();
    free_nodes();
    return EXIT_SUCCESS;
}