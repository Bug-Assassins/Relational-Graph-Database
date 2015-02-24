#define VERBOSE 1
#include <cstdio>
#include <iostream>
#include "database.h"
#include "domain.h"
#include "foreign_key.h"
#include "nodes.h"
#include "table.h"
#include "trie.h"

int create_table(database *main_database)
{
    char temp_name[100];
    int attribute_count, i, type, length;
    domain *temp_domain;
    table *temp_table;

    if (VERBOSE)
        printf("Enter the table name\n");
    
    scanf("%s", temp_name);

    temp_table = new table(std::string(temp_name));

    if (VERBOSE)
        printf("Enter the number of attributes:");

    scanf("%d", &attribute_count);

    for (i = 0; i < attribute_count; i++)
    {
        if (VERBOSE)
            printf("Enter the attribute name, type, length\n 1: INTEGER\n2: STRING\n3: FLOAT\n");

        scanf("%s %d %d", temp_name, &type, &length);

        temp_domain = new domain(std::string(temp_name), type, length);
        temp_table->add_attribute(temp_domain);
    }
    main_database->add_table(temp_table);
    return 0;
}

int main()
{
    int choice;
    char name[100];

    if (VERBOSE)
        printf("Enter name of database");

    scanf("%s", name);

    database *main_database = new database(std::string(name));

    while (1)
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
                create_table(main_database);
                break;

            case 2:
                break;

            case 0:
                printf("Exiting cleanly!\n");
                main_database->clear();
                break;

            default:
                printf("Not supported\n");
                break;
        }
        if (choice == 0)
            break;
    }

    return 0;
}