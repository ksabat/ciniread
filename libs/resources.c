#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getLineCount(const char *fName)
{
    char line[__READ_BUF_SIZE];
    int lineCount = 0;

    FILE *fp = fopen(fName,"r");

     while (!feof(fp)) {
        fgets(line,__READ_BUF_SIZE,fp);
        lineCount++;
    };

    fclose(fp);

    return lineCount;
}

arrayContainer* readConfig(const char *fName )
{

    arrayContainer *contents;
    char *outputChar;

    int count = 0;

    contents = malloc(sizeof(arrayContainer *));
    contents->size = getLineCount(fName);
    contents->data = malloc(sizeof(char *) * contents->size);

    FILE *fp = fopen(fName,"r");

    while (!feof(fp)) {
        outputChar = malloc(sizeof(char) * __READ_BUF_SIZE);
        fgets(outputChar,__READ_BUF_SIZE,fp);
        outputChar[strlen(outputChar) -1 ] = '\0';
        contents->data[count] = malloc(sizeof(char) * strlen(outputChar));
        strcpy(contents->data[count], outputChar);
        count++;

        free(outputChar);
    };


    fclose(fp);

    return contents;
}

int getSectionCount(const arrayContainer *sec)
{
    int count = 0;

    for (int i = 0; i < sec->size; i++) {
        if (sec->data[i][0] == __SECTION_ID) {
            count++;
        };
    };

    return count;
}

int getSectionKeyCount(const char *secName, const arrayContainer *searchList)
{
    int count = 0;
    int foundSection = 0;

    int tempStringLength = 0;

    for (int i = 0; i < searchList->size - 1; i++) {

        char *tempSearchString = calloc( __READ_BUF_SIZE, sizeof(char));

        tempStringLength = strlen(searchList->data[i]);
        strncpy(tempSearchString,searchList->data[i] + 1,tempStringLength-2);

        if (0 == strcmp(tempSearchString,secName)) {
            foundSection = 1;
            free(tempSearchString);
            continue;
        };
        if (foundSection == 1) {
            if (searchList->data[i][0] != __SECTION_ID) {
                count++;
            } else {
                free(tempSearchString);
                break;
            };
        };

        free(tempSearchString);
    };

    return count;
}

arrayContainer* getSectionNames(const arrayContainer *inStringList)
{
    int foundCount = 0;

    arrayContainer *outList = malloc(sizeof(arrayContainer));
    outList->size = getSectionCount(inStringList);

    outList->data = malloc(sizeof(char *) * outList->size);

    for (int i = 0; i < inStringList->size; i++) {
        if(inStringList->data[i][0] == __SECTION_ID) {
            outList->data[foundCount] = calloc(__READ_BUF_SIZE, sizeof(char) );
            strncpy(outList->data[foundCount],inStringList->data[i] + 1,strlen(inStringList->data[i])-2);
            foundCount++;
        };
    };

    return outList;
}

arrayContainer* getSection(const char *secName, arrayContainer *rawList)
{
    arrayContainer* section;
    int secKeyCount = getSectionKeyCount(secName,rawList) + 1;
    int sectionFound = 0;
    int sectionCount = 0;

    int tempStringLength = 0;

    section = malloc(sizeof(arrayContainer));
    section->data = malloc(sizeof(char *) * secKeyCount);

    for (int i = 0; i < rawList->size-1; i++) {

        tempStringLength = strlen(rawList->data[i]);
        char *tempSearchString = calloc( __READ_BUF_SIZE, sizeof(char));

        tempStringLength = strlen(rawList->data[i]);
        strncpy(tempSearchString,rawList->data[i] + 1,tempStringLength-2);

        if (0 == strcmp(tempSearchString,secName)) {
            sectionFound = 1;
            section->data[sectionCount] = malloc(sizeof(char) * strlen(tempSearchString));
            strcpy(section->data[sectionCount],tempSearchString);
            sectionCount++;

            free(tempSearchString);
            continue;
        };
        if (sectionFound == 1) {
            if(rawList->data[i][0] != __SECTION_ID)
            {
                section->data[sectionCount] = malloc(sizeof(char) * strlen(rawList->data[i]));
                strcpy(section->data[sectionCount],rawList->data[i]);
                sectionCount++;
            } else {
                free(tempSearchString);
                break;
            };
        };

        free(tempSearchString);
    };
    section->size = sectionCount;

    return section;
}

list* createSectionList(arrayContainer *inItems)
{
    list *outList = malloc(sizeof (list));

    outList->count  = inItems->size - 1;

    outList->items = malloc(sizeof(listItem *) * (outList->count)) ;

    for (int i = 0; i < inItems->size; i++) {
        if (i == 0) {
            outList->name = malloc(sizeof(char) * strlen(inItems->data[i]));
            strcpy(outList->name,inItems->data[i]);
        } else {
            char *temp;

            listItem *tempItem = malloc(sizeof(listItem));

            temp = strtok(inItems->data[i],__VALUE_DELIMITOR);
            tempItem->key = malloc(sizeof(char)*strlen(temp));
            strcpy(tempItem->key,temp);

            temp = strtok(NULL,__VALUE_DELIMITOR);
            tempItem->val = malloc(sizeof(char)*strlen(temp));
            strcpy(tempItem->val,temp);

            outList->items[i-1] = tempItem;

        };
    };

    return outList;
}

listCollection* getIni(const char* fileName)
{
    arrayContainer *rawFile = readConfig(fileName);
    arrayContainer *listOfSections = getSectionNames(rawFile);
    listCollection *outCollection = malloc(sizeof(listCollection));

    outCollection->count = listOfSections->size;
    outCollection->lists = malloc(sizeof(list *) * outCollection->count );

    for(int i = 0; i < listOfSections->size; i++) {
        arrayContainer *tempStrings = getSection(listOfSections->data[i],rawFile);
        outCollection->lists[i] = createSectionList(tempStrings);

        freeStringArrays(tempStrings);
    }

    freeStringArrays(listOfSections);
    freeStringArrays(rawFile);

    return outCollection;
}

void freeStringArrays(arrayContainer *toFree)
{
    for (int i = 0; i < toFree->size; i++) {
        free(toFree->data[i]);
    };

    free(toFree->data);

    free(toFree);
}

void freeList(list *inList)
{
    for (int i = 0; i < inList->count; i++) {
        free(inList->items[i]->key);
        free(inList->items[i]->val);
        free(inList->items[i]);
    };

    free(inList->items);
    free(inList->name);
    free(inList);
}

void freeListCollection(listCollection *inCollection)
{
    for (int i = 0; i < inCollection->count; i++) {
        freeList(inCollection->lists[i]);
    };

    free(inCollection->lists);
    free(inCollection);
}
