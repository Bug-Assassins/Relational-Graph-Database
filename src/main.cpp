#define VERBOSE 1
#define DEBUG 1
#define DEBUG_SELECT 0

#include <cstdio>

#include "database.h"
#include "select.h"

int print_table_details(database *main_database)
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

    return index - 1;
}
void print_record_list(table *tab, std::set< main_node *> &record_list, std::vector< int > &attributes)
{
    int i, j;
    std::set< main_node * >::iterator it;

    //Printing the heading of each column
    for(i = 0; i < attributes.size(); i++)
    {
        printf("%s\t", tab->get_attribute_name(attributes[i]).c_str());
    }
    printf("\n");

   // if(VERBOSE)
      //  printf("Record List Print Size = %d attributes = %d\n", (int) record_list.size(), (int) attributes.size());


    //Printing the Actual Records
    for(it = record_list.begin(); it != record_list.end(); it++)
    {
        for(j = 0; j < attributes.size(); j++)
        {
            printf("%s\t", (*it)->get_attribute_list_index(attributes[j])->get_value().c_str());
        }
        printf("\n");
    }

    printf("------------------------------------------------------------------------------------\n\n");
    return;
}

int create_table(database *main_database)
{
    std::vector< int > fk_list;
    char temp_name[100];
    int attribute_count, i, j, type, length, pkey_span, temp_int, fk_count, table_index;
    table *temp_table, *temp_table2;

    if (VERBOSE)
        printf("Enter the table name:");

    scanf("%s", temp_name);

    temp_table = new table(std::string(temp_name));

    if (VERBOSE)
        printf("Enter the number of attributes:");

    scanf("%d", &attribute_count);

    for (i = 0; i < attribute_count; i++)
    {
        if (VERBOSE)
            printf("Enter the attribute name, type, length\n1: INTEGER\n2: STRING\n3: FLOAT\n");

        scanf("%s %d %d", temp_name, &type, &length);

        temp_table->add_attribute(type, length, std::string(temp_name));
    }

    if (VERBOSE)
        printf("Enter the number of columns in primary key:");

    scanf("%d", &pkey_span);

    if (VERBOSE)
        printf("Enter the indices of %d primary key column(s)\n", pkey_span);

    for (i = 0; i < pkey_span; i++)
    {
        scanf("%d", &temp_int);
        temp_table->add_primary_key_index(temp_int - 1);
    }

    if (VERBOSE)
        printf("Enter the number of table the foreign key spans:");

    scanf("%d", &fk_count);

    for (i = 0; i < fk_count; i++)
    {
        table_index = print_table_details(main_database);
        fk_list.clear();

        temp_table2 =  main_database->get_tables_index(table_index);
        for (j = 0; j <  temp_table2->get_primary_key_size(); j++)
        {
            scanf("%d", &temp_int);
            fk_list.push_back(temp_int - 1);
        }
        temp_table->add_foreign_key(temp_table2, fk_list);
        /*
        printf("PRINTING FK INFO\n");
        for (j = 0; j < fk_list.size(); j++)
        {
            printf("%d ", fk_list[j]);
        }
        printf("\n");
        */
    }
    fk_list.clear();
    temp_table->add_to_size(sizeof(*temp_table));
    main_database->add_table(temp_table);
    return 0;
}

int insert_to_table(database *main_database)
{
    int index, success;
    char temp_char_arr[100];
    table *temp_table;
    temp_table = main_database->get_tables_index(print_table_details(main_database));

    std::vector< std::string > values(temp_table->get_attribute_count());

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

    success = temp_table->add_new_record(values);
    if(success == 1)
    {
        printf("1 Row Inserted\n");
    }
    else if(success == -1)
    {
        printf("NULL Primary Key not allowed\n");
    }
    else if (success == 0)
    {
        printf("Primary Key already exists\n");
    }
    else if (success == -2)
    {
        printf("Integrity Violation\n");
    }
    values.clear();
    printf("%lu\n", temp_table->get_size());
}

int print_table(database *main_database)
{
    int attribute_count, i;
    table *temp_table;
    main_node *temp_main_node;

    temp_table = main_database->get_tables_index(print_table_details(main_database));
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
//table &tab, std::vector< int > &attributes, std::vector< std::string > &values, std::vector< int > &ops, std::vector< bool > &join_ops
void select_or_update(database *main_database, bool update)
{
    std::vector< int > col_list, attributes, ops;
    std::vector< value_expression > expression;
    std::vector< std::string > values, update_values;
    std::vector < std::vector< value_expression > > expression_vec;

    std::set< main_node * > result;

    int col_count, temp_int, expr_count, op, join_operator, i, l_index;
    char rhs[100];
    std::string temp_string;
    value_expression temp_exp;

    int table_index = print_table_details(main_database);
    table *selected_table = main_database->get_tables_index(table_index);

    if (VERBOSE)
    {
        if(update)
            printf("Enter the number of columns you want to update:");
        else
            printf("Enter the number of columns you want to select:");
    }

    scanf("%d", &col_count);

    if (VERBOSE)
    {
        printf("Enter the indexes of the columns");
        if(update)
            printf(" and their corresponding values:\n");
    }

    for (i = 0; i < col_count; i++)
    {
        scanf("%d", &temp_int);
        col_list.push_back(temp_int - 1);
        if(update)
        {
            scanf("%s", rhs);
            temp_string.assign(rhs);
            update_values.push_back(temp_string);
        }
    }

    if (VERBOSE)
        printf("Enter the number of expressions\n");

    scanf("%d", &expr_count);

    for (i = 0; i < expr_count; i++)
    {
        if (VERBOSE)
        {
            printf("Enter the %d expression\n", i + 1);
            printf("Enter the LHS col index\n");
        }

        scanf("%d", &temp_exp.attribute);
        temp_exp.attribute--;

        if (VERBOSE)
            printf("Enter the operator index that you want\n");

        scanf("%d", &temp_exp.op);
        temp_exp.op--;

        if (VERBOSE)
            printf("Enter the value of RHS:");

        scanf("%s", rhs);
        temp_exp.value.assign(rhs);

        expression.push_back(temp_exp);

        if (i != expr_count - 1)
        {
            if (VERBOSE)
                printf("AND(1) / OR(0) ? :");

            scanf("%d", &join_operator);
            if(join_operator == 0)
            {
                expression_vec.push_back(expression);
                expression.clear();
            }
        }
        else
        {
            expression_vec.push_back(expression);
            expression.clear();
        }
    }
    result = selected_table->select_single_table(expression_vec);
    if(update)
    {
        selected_table->update(result, col_list, update_values);
    }
    else
    {
        print_record_list(selected_table, result, col_list);
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
            printf("5) Select from single table\n");
            printf("6) Update a table\n");
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

            case 5:
                select_or_update(main_database, false);
                break;

            case 6:
                select_or_update(main_database, true);
                break;

            case 0:
                printf("Exiting cleanly!\n");
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
