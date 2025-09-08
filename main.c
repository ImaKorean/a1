#include <stdio.h>  //For printf(),scanf(),fopen(),fclose(),fgets()
#include <stdlib.h> // To use EXIT_FAILURE, EXIT_SUCCESS, malloc, calloc, realloc, free
#include <stdbool.h> // To use bool, true, false
#include <string.h> 
// start!
// I used shift + option + f to format clear ((Shift + Alt + F) in window)


typedef struct Node{
    int id; // (A=0, B=1....)
    struct Node *parent;
    int children[26]; // index of childrens
    int child_count; //number of childrens
    int track; // to use DFS
    bool seen; // check what upper letters is in inputs
} Node;

// Node lookup table for AtoZ (each index stores the single Node* for that letter)
static Node *nodes[26];

static Node *new_node(int id){
    Node *node = (Node*)calloc(1,sizeof *node); // allocate memory for exactly one Node and initialize all fields to 0.
    if(!node) return NULL; // for error
    // put initial values
    node -> id = id;
    node -> parent = NULL;
    node -> child_count = 0;
    node -> track = 0;
    node -> seen = true; // check true because I have seen it in input
    return node;
}




int validate_input(FILE *sf)
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

            if (count == 0) { //skip empty line
#ifdef DEBUG
                if (getletter == '\n')
                    fprintf(stderr, "[DEBUG] line %d: blank line -> skip\n", line_no++);
                else
                    fprintf(stderr, "[DEBUG] EOF after blank line -> skip/finish\n");
#endif
                if (getletter == EOF) break;
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

            if (!add_edge(a, b)) {
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

            if (count == 0){
                a = (char)getletter;
            }else{
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
        if((sf = fgetc(storeFile)) == EOF){
            printf("\n");
        }
        if (fseek(storeFile, 0, SEEK_SET) != 0)
        {
            perror("fseek");
        }
    }
#endif
    int result = validate_input(storeFile);
    if (result != EXIT_SUCCESS)
    {
        printf("INVALID\n\n");
        fclose(storeFile);
        return EXIT_SUCCESS;
    }

    fclose(storeFile);
    return EXIT_SUCCESS;
}