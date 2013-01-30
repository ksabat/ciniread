#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __SECTION_ID '['
#define __COMMENT_ID '#'
#define __BLANKS_LIST " \n\t\r\f"
#define __VALUE_DELIMITOR "="
#define __READ_BUF_SIZE 256

typedef struct arrayContainer {
    int size;
    char **data;
} arrayContainer;

typedef struct listItem {
    char *key;
    char *val;
} listItem;

typedef struct list {
    char *name;
    int count;
    listItem **items;
} list;

typedef struct listCollection {
    int count;
    list **lists;
} listCollection;

//count the lines in the ini file
int getLineCount(const char*);

//read in the config file
arrayContainer* readConfig(const char* );

//count the number of sections in file
int getSectionCount(const arrayContainer* );

//get the number of keys in a section
int getSectionKeyCount(const char *, const arrayContainer *);

//get a list of section names
arrayContainer* getSectionNames(const arrayContainer *);

//get a section (all of its keys and its section name)
arrayContainer* getSection(const char *, arrayContainer *);

//convert an array of strings into a list container with each key being a listItem
list* createSectionList(const arrayContainer*);

listCollection* getIni(const char*);

//free the char** created as temporary structs to pass data around
void freeStringArrays(arrayContainer*);

//free the memory used by a list structure
void freeList(list*);

void freeListCollection(listCollection *);

#endif
