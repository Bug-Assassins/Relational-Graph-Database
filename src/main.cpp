#define VERBOSE 1
#include <cstdio>
#include <cstring>
#include <iostream>
#include "database.h"
#include "domain.h"
#include "foreign_key.h"
#include "nodes.h"
#include "table.h"
#include "trie.h"

int create_table(database *main_database)
{
    std::vector<std::string>names;
    char temp_name[100];
    int attribute_count, i, type, length, pkey_span, temp_int, fk_count, table_index, col_index;
    domain *temp_domain;
    table *temp_table;

    if (VERBOSE)
        printf("Enter the table name:");

    scanf("%s", temp_name);

    temp_table = new table(std::string(temp_name));

    if (VERBOSE)
        printf("Enter the number of attributes:");

    scanf("%d", &attribute_count);

    if (VERBOSE)
        printf("Enter the number of foreign keys:\n"); // Foreign keys will be accepted first

    scanf("%d", &fk_count);

    for (i = 0; i < attribute_count; i++)
    {
        if (VERBOSE)
            printf("Enter the attribute name, type, length\n1: INTEGER\n2: STRING\n3: FLOAT\n");

        scanf("%s %d %d", temp_name, &type, &length);

        if (i < fk_count)
        {
            scanf("%d %d", &table_index, &col_index);
            //temp_domain = main_database->get_table_index(table_index - 1)->get_normal_index(col_index - 1)->
        }
        else
        {
            temp_domain = new domain(type, length);
        }
        temp_table->add_to_size(sizeof(*temp_domain) + strlen(temp_name));
        temp_table->add_attribute(temp_domain, std::string(temp_name));
    }

    if (VERBOSE)
        printf("Enter the number of columns in primary key:");

    scanf("%d", &pkey_span);

    if (VERBOSE)
        printf("Enter the indices of %d primary key columns\n", pkey_span);

    for (i = 0; i < pkey_span; i++)
    {
        scanf("%d", &temp_int);
        temp_table->add_primary_key_index(temp_int - 1);
    }

    temp_table->add_to_size(sizeof(*temp_table));
    main_database->add_table(temp_table);
    return 0;
}


table *print_table_details(database *main_database)
{
    table *temp_table;
    int i, index, att_count;

    if (VERBOSE)
    {
        printf("Enter the table you want\n");
        for (i = 0; i < main_database->get_tables_size(); i++)
        {
            printf("%d : %s\n", i + 1, main_database->get_tables_index(i)->get_table_name().c_str());
        }
    }
    scanf("%d", &index);

    temp_table = main_database->get_tables_index(index - 1);
    att_count = temp_table->get_attribute_count();

    if (VERBOSE)
    {
        for (i = 0; i < att_count; i++)
        {

            printf ("%s\t%d\t%d\n", temp_table->get_attribute_name(i).c_str(),
                                    temp_table->get_normal_index(i)->get_data_type(),
                                    temp_table->get_normal_index(i)->get_attr_length());
        }
    }

    return temp_table;
}

int insert_to_table(database *main_database)
{
    int index;
    char temp_char_arr[100];
    table *temp_table;
    temp_table = print_table_details(main_database);

    std::vector<std::string> values(temp_table->get_attribute_count());

    if (VERBOSE)
    {
        printf("Enter the index of the column and the value. Enter 0 as the index to finish entering info\n");
    }

    while (1)
    {
        scanf("%d", &index);
        if (index == 0)
            break;
        scanf("%s", temp_char_arr);
        values[index - 1].assign(temp_char_arr);
    }
    temp_table->add_new_record(values);
    values.clear();
    printf("%lu\n", temp_table->get_size());
}

int print_table(database *main_database)
{
    int attribute_count, i;
    table *temp_table;
    main_node *temp_main_node;

    temp_table = print_table_details(main_database);
    temp_main_node = temp_table->get_main_node_head();
    attribute_count = temp_table->get_attribute_count();

    while (temp_main_node != NULL)
    {
        for (i = 0; i < attribute_count; i++)
        {
            printf("%s\t",temp_main_node->get_attribute_list_index(i)->get_value().c_str());
        }
        printf("\n");
        temp_main_node = temp_main_node->get_next();
    }
}

int main()
{
    int choice;
    char name[100];

    if (VERBOSE)
        printf("Enter name of database:");

    scanf("%s", name);

    database *main_database = new database(std::string(name));

    while (1)
    {
        if (VERBOSE)
        {
            printf("Enter your choice :\n");
            printf("1) Create a table\n");
            printf("2) Insert to table\n");
            printf("3) Describe table\n");
            printf("4) Print a table\n");
            printf("0) Exit\n");
        }
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                create_table(main_database);
                break;

            case 2:
                insert_to_table(main_database);
                break;

            case 3:
                print_table_details(main_database);
                break;

            case 4:
                print_table(main_database);
                break;

            case 0:
                //printf("Exiting cleanly!\n");
                main_database->clear();
                delete main_database;
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
