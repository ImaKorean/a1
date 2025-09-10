#include <stdio.h>   //For printf(),scanf(),fopen(),fclose(),fgets()
#include <stdlib.h>  // To use EXIT_FAILURE, EXIT_SUCCESS, malloc, calloc, realloc, free
#include <stdbool.h> // To use bool, true, false
#include <string.h>
// start!
// I used shift + option + f to format clear ((Shift + Alt + F) in window)

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

#ifdef DEBUG
static void dbg_print_up_chain(Node *n, const char *tag)
{
    fprintf(stderr, "[DEBUG] %s: ", tag);
    for (Node *cur = n; cur; cur = cur->parent)
    {
        fprintf(stderr, "%c", 'A' + cur->id);
        if (cur->parent)
            fprintf(stderr, " <- ");
    }
    fprintf(stderr, "\n");
}
#endif

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
#ifdef DEBUG
    dbg_print_up_chain(parent, "parent-chain-before-link");
    dbg_print_up_chain(child, "child-node");
#endif

    for (Node *cur = parent; cur != NULL; cur = cur->parent)
    {
        if (cur == child)
        {

#ifdef DEBUG
            fprintf(stderr, "[DEBUG] CYCLE DETECTED: adding %c->%c makes a loop\n",
                    'A' + parent->id, 'A' + child->id);
            // 경로를 보기 좋게 다시 출력
            dbg_print_up_chain(parent, "loop-path (parent chain contains child)");
#endif
            return false; // cycle checking
        }
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

            printf("\n");// line change in each level

            cn = numOfNode;
            for (int j = 0; j < cn; j++) cur[j] = next[j];
        }
        
        printf("\n"); //empty line between trees and another trees or last
    }
}

#ifdef DEBUG
static void dbg_dump_graph(void)
{
    fprintf(stderr, "\n==== GRAPH DUMP ====\n");
    for (int i = 0; i < 26; i++)
    {
        Node *n = nodes[i];
        if (!n)
            continue; // not created
        char me = 'A' + n->id;
        char par = (n->parent ? ('A' + n->parent->id) : '-');
        fprintf(stderr, "[%c] parent=%c  children=", me, par);
        if (n->child_count == 0)
        {
            fprintf(stderr, "(none)");
        }
        else
        {
            for (int k = 0; k < n->child_count; k++)
            {
                fprintf(stderr, "%c", 'A' + n->children[k]);
            }
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "====================\n\n");
}
#endif

int validate_inputs_add_node(FILE *sf)
{
    int getletter;
    int count = 0;
    char a = 0, b = 0;

#ifdef DEBUG
    int line_no = 1;
#endif

    while (true)
    {
        getletter = fgetc(sf);

        if (getletter == '\n' || getletter == EOF)
        {

            if (count == 0)
            { // skip empty line
#ifdef DEBUG
                if (getletter == '\n')
                    fprintf(stderr, "[DEBUG] line %d: blank line -> skip\n", line_no++);
                else
                    fprintf(stderr, "[DEBUG] EOF after blank line -> skip/finish\n");
#endif
                if (getletter == EOF)
                    break;
                continue;
            }

            if (count != 2)
            {
#ifdef DEBUG
                fprintf(stderr, "[DEBUG] line %d: count=%d (must be 2) -> INVALID\n", line_no, count);
#endif
                return EXIT_FAILURE;
            }

#ifdef DEBUG
            fprintf(stderr, "[DEBUG] line %d: pair=%c%c\n", line_no, a, b);
#endif

            if (!addNodeWithletter(a, b))
            {
#ifdef DEBUG
                fprintf(stderr, "[DEBUG] add_edge(%c,%c) failed -> INVALID\n", a, b);
#endif
                return EXIT_FAILURE;
            }
#ifdef DEBUG
            fprintf(stderr, "[DEBUG] pair=%c%c added\n", a, b);
            line_no++;
#endif
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
#ifdef DEBUG
                fprintf(stderr, "[DEBUG] line %d: non-uppercase char '%c' -> INVALID\n",
                        line_no, (char)getletter);
#endif
                return EXIT_FAILURE;
            }
            if (count >= 2)
            {
#ifdef DEBUG
                fprintf(stderr, "[DEBUG] line %d: too many chars in line -> INVALID\n", line_no);
#endif
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

#ifdef DEBUG
    {
        int sf;
        while ((sf = fgetc(storeFile)) != EOF)
        {
            fputc(sf, stderr);
        }
        fprintf(stderr, "\n");
        if (fseek(storeFile, 0, SEEK_SET) != 0)
        {
            perror("fseek");
        }
    }
#endif

    int result = validate_inputs_add_node(storeFile);
    fclose(storeFile);

    if (result != EXIT_SUCCESS)
    {
        printf("INVALID\n\n");
        free_nodes();
        return EXIT_SUCCESS;
    }

#ifdef DEBUG
    dbg_dump_graph();
#endif

    print_mytree();
    free_nodes();
    return EXIT_SUCCESS;
}