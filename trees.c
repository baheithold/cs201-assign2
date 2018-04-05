#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "string.h"
#include "scanner.h"
#include "gst.h"
#include "avl.h"

/* options */
int vOption = 0;    /* option -v            */
int gOption = 0;    /* option -g            */
int rOption = 1;    /* option -r (default)  */

char *corpusFilename;
char *commandsFilename;

static int processOptions(int, char **);
void Fatal(char *,...);
void printAuthor(void);
void processCorpusToGST(GST *, FILE *);
void processCorpusToAVL(AVL *, FILE *);
void processCommandsGST(GST *, FILE *);
void processCommandsAVL(AVL *, FILE *);
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
    GST *gTree = newGST(displaySTRING, compareSTRING, freeSTRING);
    AVL *aTree = newAVL(displaySTRING, compareSTRING, freeSTRING);
    if (gOption) {
        processCorpusToGST(gTree, corpusFP);
        processCommandsGST(gTree, commandsFP);
    }
    else {
        processCorpusToAVL(aTree, corpusFP);
        processCommandsAVL(aTree, commandsFP);
    }

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

static int processOptions(int argc, char **argv) {
    int argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-') {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        switch (argv[argIndex][1]) {
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


void processCorpusToGST(GST *t, FILE *fp) {
    char *str;
    if (stringPending(fp)) str = readString(fp);
    else str = readToken(fp);
    while (!feof(fp)) {
        cleanString(str);
        if (strlen(str) > 0) {
            // add cleaned string to GST
            insertGST(t, newSTRING(str));
        }
        if (stringPending(fp)) str = readString(fp);
        else str = readToken(fp);
    }
}

void processCommandsGST(GST *t, FILE *fp) {
    char command;
    char *str;
    command = readChar(fp);
    while (!feof(fp)) {
        switch (command) {
            case 'i':
                if (stringPending(fp)) str = readString(fp);
                else str = readToken(fp);
                cleanString(str);
                if (strlen(str) > 0) {
                    insertGST(t, newSTRING(str));
                }
                break;
            case 'd':
                if (stringPending(fp)) str = readString(fp);
                else str = readToken(fp);
                cleanString(str);
                if (strlen(str) > 0) {
                    void *deleted = deleteGST(t, newSTRING(str));
                    if (!deleted) {
                        printf("Value %s not found.\n", str);
                    }
                }
                break;
            case 'f':
                if (stringPending(fp)) str = readString(fp);
                else str = readToken(fp);
                cleanString(str);
                STRING *temp = newSTRING(str);
                printf("Frequency of %s: %d\n", str, findGSTcount(t, temp));
                freeSTRING(temp);
                break;
            case 's':
                displayGST(t, stdout);
                break;
            case 'r':
                statisticsGST(t, stdout);
                break;
            default:
                Fatal("Command %c not understood!\n", command);
                break;
        }
        command = readChar(fp);
    }
}

void processCorpusToAVL(AVL *t, FILE *fp) {
    char *str;
    if (stringPending(fp)) str = readString(fp);
    else str = readToken(fp);
    while (!feof(fp)) {
        cleanString(str);
        if (strlen(str) > 0) {
            // add cleaned string to AVL
            STRING *cleaned = newSTRING(str);
            insertAVL(t, cleaned);
        }
        if (stringPending(fp)) str = readString(fp);
        else str = readToken(fp);
    }
}

void processCommandsAVL(AVL *t, FILE *fp) {
    char command;
    char *str;
    command = readChar(fp);
    while (!feof(fp)) {
        switch (command) {
            case 'i':
                if (stringPending(fp)) str = readString(fp);
                else str = readToken(fp);
                cleanString(str);
                if (strlen(str) > 0) {
                    insertAVL(t, newSTRING(str));
                }
                break;
            case 'd':
                if (stringPending(fp)) str = readString(fp);
                else str = readToken(fp);
                cleanString(str);
                if (strlen(str) > 0) {
                    void *deleted = deleteAVL(t, newSTRING(str));
                    if (!deleted) {
                        printf("Value %s not found.\n", str);
                    }
                }
                break;
            case 'f':
                if (stringPending(fp)) str = readString(fp);
                else str = readToken(fp);
                cleanString(str);
                STRING *temp = newSTRING(str);
                printf("Frequency of %s: %d\n", str, findAVLcount(t, temp));
                freeSTRING(temp);
                break;
            case 's':
                displayAVL(t, stdout);
                break;
            case 'r':
                statisticsAVL(t, stdout);
                break;
            default:
                Fatal("Command %c not understood!\n", command);
                break;
        }
        command = readChar(fp);
    }
}

void cleanString(char str[]) {
    int i = 0;
    int n = 0;
    int inString = 0;
    int prevSpace = 0;
    //printf("Cleaning: \"%s\"\t", str);
    while (str[i]) {
        if (isalpha(str[i])) {
            if (inString && prevSpace) {
                str[n++] = ' ';
            }
            str[n++] = tolower(str[i]);
            inString = 1;
            prevSpace = 0;
        }
        else if (isspace(str[i])) {
            prevSpace = 1;
        }
        i++;
    }
    str[n] = '\0';
    //printf("Cleaned: \"%s\"\n", str);
}
