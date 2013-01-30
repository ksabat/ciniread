#include <stdio.h>
#include <stdlib.h>

#include "libs/ciniread.h"

int main()
{

    listCollection *colTest = getIni("commentfile.ini");

    printf("\n-----------------------------------------------------------------------\n");

    for (int i = 0; i < colTest->count; i++) {
        printf("List Name:\t%s\n",colTest->lists[i]->name);
        for (int c = 0; c < colTest->lists[i]->count; c++) {
            printf("key:\t%s\t| val:\t%s\n",colTest->lists[i]->items[c]->key,colTest->lists[i]->items[c]->val);
        };
        printf("---------------------------------------------------------------------\n");
    };

    freeListCollection(colTest);
    return 0;
}
