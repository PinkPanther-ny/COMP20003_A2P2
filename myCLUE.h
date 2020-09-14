/** ***********************
* Program written by Alvin Chen (nuoyanc@student.unimelb.edu.au) 9/2020
* 2020s2 UNIMELB COMP20003 assignment 2
*/

#ifndef myCLUE
#define myCLUE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAX_LINE_LEN 512
#define MAX_FIELD_LEN 128
#define FIELD_NUM 11

#define FIELD_CENSUS_YEAR 0
#define FIELD_BLOCK_ID 1
#define FIELD_PROPERTY_ID 2
#define FIELD_BASE_PROPERTY_ID 3
#define FIELD_CLUE_SMALL_AREA 4
#define FIELD_TRADING_NAME 5
#define FIELD_INDUSTRY_CODE 6
#define FIELD_INDUSTRY_DESCRIPTION 7
#define FIELD_X_COORD 8
#define FIELD_Y_COORD 9
#define FIELD_LOCATION 10

#define CHAR_SEPERATOR ','
#define CHAR_NULLCHAR '\0'
#define CHAR_QUOTE '\"'
#define CHAR_NEWLINE '\n'

#define ROUNDING_ERROR_MARGIN 0.00000001

typedef struct CLUE{
    // Say it's better to store as string type
    char *censusYear;
    char *blockID;
    char *propertyID;
    char *basePropertyID;

    char *clueSmallArea;
    char *tradingName;
    char *industryCode;
    char *industryDescription;
    char *xCoord, *yCoord;
    char *location;

}Clue_t;

typedef struct Point{
    double x;
    double y;
}Point_t;


// Function prototypes
Clue_t *Clue(
    char* censusYear, char* blockID, char* propertyID, char* basePropertyID,
    char* clueSmallArea, char* tradingName, char* industryCode, char* industryDescription,
    char* xCoord, char* yCoord, char *location
    );
void putClueInfo(Clue_t *clue, FILE * fp, Point_t keyPoint);
Clue_t *storeFields(char *fields[]);
Point_t getClueLocation(Clue_t *data);
double distanceTo(Point_t p1, Point_t p2);
#endif
