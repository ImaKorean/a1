#include <stdio.h> //for printf(),scanf(),fopen(),fclose(),fgets()
#include <stdlib.h> // To use EXIT_FAILURE, EXIT_SUCCESS
// start!

int main(int argc, char **argv){
    if (argc != 2) {
        fprintf(stderr, "need Your inputfile\n");;//Using stderr for error
        return EXIT_FAILURE;
    }
    char *inputFileName = argv[1];
    FILE *storeFile = fopen(inputFileName, "r");

    if (!storeFile) {
        fprintf(stderr, "Failed to open: %s\n", inputFileName);  
        return EXIT_FAILURE;
    }

    fclose(storeFile);
    return EXIT_SUCCESS;
}