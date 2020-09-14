/** ***********************
* Program written by Alvin Chen (nuoyanc@student.unimelb.edu.au) 9/2020
* 2020s2 UNIMELB COMP20003 assignment 2
*/

#include "myCSV_Parser.h"


/**
* Read in char pointer to a line of a csv file
* and integer lineIndex which is the index in line
* for the function start to split.
* returns char pointer to the first item from lineIndex in the line
*
*/
char *splitOneToken(char *line, int * lineIndex){
    int isInsideQuote = (line[*lineIndex]==CHAR_QUOTE), destIndex = 0;
    char * dest = (char*)malloc((MAX_FIELD_LEN+1) * sizeof(char));

    for (int i=*lineIndex + isInsideQuote;i<strlen(line);i++){

        // If inside a quoted string, just keep storing the char
        // until meets the end of the line
        if(isInsideQuote){
            if (
                (line[i]!=CHAR_QUOTE || line[i+1]!=CHAR_SEPERATOR) &&
                !(line[i+1]==CHAR_NEWLINE || line[i+1]==CHAR_NULLCHAR)
                ){

                // If not at the beginning of the line and previous char
                // and current char are both quote, then not store it
                if(
                    (destIndex>=1 &&
                    (dest[destIndex-1]!=CHAR_QUOTE || line[i]!=CHAR_QUOTE))
                    ||(destIndex==0)
                ){
                    dest[destIndex++] = line[i];
                }
            // At the end of a quoted string, return the token
            }else{
                dest[destIndex] = CHAR_NULLCHAR;
                // jump to the comma after the quote
                *lineIndex = i + 2;
                if (dest[strlen(dest) - 1] == CHAR_QUOTE){
                    dest[strlen(dest) - 1] = CHAR_NULLCHAR;
                }
                dest = (char *)realloc(dest, (strlen(dest) + 1) * sizeof(char));
                return dest;
            }
        }else{
            // Not inside quote and then check if it is at the begin or end
            if (
                line[i]!=CHAR_SEPERATOR &&
                !(line[i+1]==CHAR_NEWLINE || line[i+1]==CHAR_NULLCHAR)
            ){
                if(
                    (destIndex>=1 &&
                    (dest[destIndex-1]!=CHAR_QUOTE || line[i]!=CHAR_QUOTE))
                    ||(destIndex==0)
                ){
                    dest[destIndex++] = line[i];
                }
            // Reach the end of a quoted string, return the token
            }else{
                dest[destIndex] = CHAR_NULLCHAR;
                // jump to next comma
                *lineIndex = i + 1;
                if (dest[strlen(dest) - 1] == CHAR_QUOTE){
                    dest[strlen(dest) - 1] = CHAR_NULLCHAR;
                }
                dest = (char *)realloc(dest, (strlen(dest) + 1) * sizeof(char));
                return dest;
            }
        }
    }
    printf("ERROR! lineIndex outside the line!");
    return NULL;
}


KDT_t *
readFileToTree(KDT_t *dest, char *filename){
    int lineCount = 0;
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);

    // fields is like a truck, takes pointers that point to
    // address in the heap of each token in a line
    // and unload them using storefields(), push to the whole list,
    // Then come back again to take another line
    char *fields[FIELD_NUM];

    int lineIndex;
    char line[MAX_LINE_LEN+1];
    int depth = 0;

    // Remove header line of CSV
    fgets(line, MAX_LINE_LEN, fp);

    // Start reading CSV file
    while(fgets(line, MAX_LINE_LEN, fp)!=NULL){
        lineCount++;
        lineIndex=0;
        depth=0;
        for(int i=0;i<FIELD_NUM;i++){
            fields[i] = splitOneToken(line, &lineIndex);
        }

        dest = addToKDT(dest, storeFields(fields), &depth);
    }

    fclose(fp);
    return dest;
}
