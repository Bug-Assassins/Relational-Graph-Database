#define VERBOSE 1
#include <cstdio>
#include "database.h"
#include "domain.h"
#include "foreign_key.h"
#include "nodes.h"
#include "table.h"
#include "trie.h"
int main()
{
    int choice;

    if (VERBOSE)
        printf("Enter name of database");

    while (0)
    {
        if (VERBOSE)
        {
            printf("Enter your choice :\n");
            printf("1) Create a table\n");
            printf("2) Insert to table\n");
        }
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                break;

            case 2:
                break;

            case 0:
                printf("Exiting cleanly!\n");
                break;

            default:
                printf("Not supported\n");
                break;
        }
    }

    return 0;
}