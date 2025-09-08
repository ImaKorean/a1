#include <stdio.h>  //For printf(),scanf(),fopen(),fclose(),fgets()
#include <stdlib.h> // To use EXIT_FAILURE, EXIT_SUCCESS, malloc, calloc, realloc, free
#include <stdbool.h> // To use bool, true, false
#include <string.h> 
// start!
// I used shift + option + f to format clear (Shift + Alt + F) in window

int validate_input(FILE *sf)
{
    int getletter;
    int count = 0;

#ifdef DEBUG
    int line_no = 1;
    char a = 0;
    char b = 0;
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
            a = b = 0;
            line_no++;
#endif
            count = 0;

            if (getletter == EOF)
                break;
            continue;
        }
        else
        {
            if (getletter < 'A' || getletter > 'Z')
            {
#ifdef DEBUG
                fprintf(stderr, "[DEBUG] line %d: non-uppercase char '%c' -> INVALID\n", line_no, (char)getletter);
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

#ifdef DEBUG
            if (count == 0)
                a = (char)getletter;
            else
                b = (char)getletter;
#endif
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