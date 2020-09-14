/** ***********************
* Program written by Alvin Chen (nuoyanc@student.unimelb.edu.au) 9/2020
* 2020s2 UNIMELB COMP20003 assignment 2
*/

#include "myCLUE.h"

/* Constructor for Clue_t */
Clue_t *
Clue(
    char* censusYear, char* blockID, char* propertyID, char* basePropertyID,
    char* clueSmallArea, char* tradingName, char* industryCode, char* industryDescription,
    char* xCoord, char* yCoord, char *location
    ){

    Clue_t *this = (Clue_t*)malloc(sizeof(Clue_t));
    assert(this != NULL);

    this -> censusYear = censusYear;
    this -> blockID = blockID;
    this -> propertyID = propertyID;
    this -> basePropertyID = basePropertyID;

    this -> clueSmallArea = clueSmallArea;
    this -> tradingName = tradingName;
    this -> industryCode = industryCode;
    this -> industryDescription = industryDescription;

    this -> xCoord = xCoord;
    this -> yCoord = yCoord;
    this -> location = location;
    return this;
}

/**
* Put a Clue_t type object to a given file
*/
void
putClueInfo(Clue_t *clue, FILE * fp, Point_t keyPoint){

    fprintf(
        fp,
        "%.6f %.6f --> Census year: %s || Block ID: %s || "
        "Property ID: %s || Base property ID: %s || "
        "CLUE small area: %s || "
        "Trading Name: %s || "
        "Industry (ANZSIC4) code: %s || "
        "Industry (ANZSIC4) description: %s || "
        "x coordinate: %s || y coordinate: %s || "
        "Location: %s || \n",
        keyPoint.x, keyPoint.y,
        clue -> censusYear, clue -> blockID,
        clue -> propertyID, clue -> basePropertyID,
        clue -> clueSmallArea,
        clue -> tradingName,
        clue -> industryCode,
        clue -> industryDescription,
        clue -> xCoord, clue -> yCoord,
        clue->location
    );
}

/**
* param fields -A 2D array that store the information of one line of csv file
* return clue -A pointer to a Clue_t data which store the line
*/
Clue_t *
storeFields(char *fields[]){
    // Call the Clue_t constructor, return its address
    Clue_t * clue = Clue(

        fields[FIELD_CENSUS_YEAR],
        fields[FIELD_BLOCK_ID],
        fields[FIELD_PROPERTY_ID],
        fields[FIELD_BASE_PROPERTY_ID],

        fields[FIELD_CLUE_SMALL_AREA],
        fields[FIELD_TRADING_NAME],
        fields[FIELD_INDUSTRY_CODE],
        fields[FIELD_INDUSTRY_DESCRIPTION],

        fields[FIELD_X_COORD],
        fields[FIELD_Y_COORD],
        fields[FIELD_LOCATION]

    );
    return clue;
}



Point_t getClueLocation(Clue_t *data){
    Point_t p;
    char* str = data->location;
    char* pEnd;
    p.y = strtod (str+1, &pEnd);
    p.x = strtod (pEnd+1, NULL);
    return p;

}

double distanceTo(Point_t p1, Point_t p2){
    return sqrt( pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2) );

}

