#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "string.h"
#include "scanner.h"

/* options */
int vOption = 0;    /* option -v            */
int gOption = 0;    /* option -g            */
int rOption = 1;    /* option -r (default)  */

char *corpusFilename;
char *commandsFilename;

static int processOptions(int, char **);
void Fatal(char *,...);
void printAuthor(void);
void processCorpusToGST(FILE *corpusFP);
void processCommandsToGST(FILE *commandsFP);
void cleanString(char []);

int main(int argc,char **argv) {
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    if (vOption) {
        printAuthor();
        return 0;
    }

    if (argIndex == argc) {
        Fatal("Missing corpus file name!\n");
    }
    else if (argIndex == argc - 1) {
        Fatal("Missing commands file name!\n");
    }

    // Open corpus and command files for reading
    corpusFilename = argv[argIndex];
    commandsFilename = argv[++argIndex];
    FILE *corpusFP = fopen(corpusFilename, "r");
    if (corpusFP == 0) {
        Fatal("Unable to open %s for reading!\n", corpusFilename);

    }
    FILE *commandsFP = fopen(commandsFilename, "r");
    if (commandsFP == 0) {
        Fatal("Unable to open %s for reading!\n", commandsFilename);
    }

    // Process Corpus File
    char str[] = "   this   is a     STRING !     ";
    cleanString(str);
    printf("%s\n", str);

    fclose(corpusFP);
    fclose(commandsFP);
    return 0;
}

void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}

/* only -oXXX  or -o XXX options */

static int processOptions(int argc, char **argv) {
    int argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-') {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        switch (argv[argIndex][1]) {
            /*
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

            case 'v':
                vOption = 1;
                break;
            case 'g':
                gOption = 1;
                break;
            case 'r':
                rOption = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }
        ++argIndex;
    }

    return argIndex;
}


void printAuthor(void) {
    printf("Written by Brett Heithold\n");
}


void processCorpusToGST(FILE *fp) {
    // TODO: Clean tokens and strings
    // TODO: insert to GST
    char *str;
    if (stringPending(fp)) str = readString(fp);
    else str = readToken(fp);
    while (!feof(fp)) {
        STRING *cleaned = newSTRING(str);
        displaySTRING(cleaned, stdout);
        printf("\n");
        if (stringPending(fp)) str = readString(fp);
        else str = readToken(fp);
    }
}

void cleanString(char str[]) {
    unsigned int i = 0;
    int n = 0;
    int inString = 0;
    for (i = 0; str[i]; ++i) {
        if (isalpha(str[i])) {
            // character is a letter a-Z
            inString = 1;
            str[n++] = tolower(str[i]);
        }
        else {
            if (inString && !isspace(str[i-1])) {
                str[n++] = str[i];
            }
        }
    }
    str[n] = '\0';
}
