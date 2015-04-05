#define VERBOSE 1
#define DEBUG 1
//#define DEBUG_SELECT 0

#include <cstdio>

#include "database.h"
#include "select.h"

struct dom
{
    std::string name;
    int type;
    int length;
};

int print_table_details(database *main_database)
{
    table *temp_table;
    unsigned int i, index, att_count;

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
    unsigned int i, j;
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
    printf("--------------------------------------------------------------------------------\n");

    return;
}

int create_table(database *main_database)
{
    dom temp_dom;
    std::vector< dom > domains;
    std::vector< int > fk_list;
    char temp_name[100];
    int attribute_count, i, j, pkey_span, temp_int, fk_count, table_index;
    table *temp_table, *temp_table2;

    if (VERBOSE)
        printf("Enter the table name:");

    scanf("%s", temp_name);


    if (VERBOSE)
        printf("Enter the number of attributes:");

    scanf("%d", &attribute_count);

    temp_table = new table(std::string(temp_name), attribute_count);

    bool *ins_check = new bool[attribute_count];

    for (i = 0; i < attribute_count; i++)
    {
        ins_check[i] = false;
        if (VERBOSE)
            printf("Enter the attribute name, type, length\n1: INTEGER\n2: STRING\n3: FLOAT\n");

        scanf("%s %d %d", temp_name, &temp_dom.type, &temp_dom.length);
        temp_dom.name.assign(temp_name);
        domains.push_back(temp_dom);
        //temp_table->add_attribute(type, length, std::string(temp_name));
    }

    if (VERBOSE)
        printf("Enter the number of columns in primary key:");

    scanf("%d", &pkey_span);

    if (VERBOSE)
        printf("Enter the indices of %d primary key column(s)\n", pkey_span);

    for (i = 0; i < pkey_span; i++)
    {
        scanf("%d", &temp_int);
        temp_table->add_primary_key(temp_int - 1);
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
            temp_int--;
            ins_check[temp_int] = true;
            temp_table->add_foreign_domain(domains[temp_int].name, temp_table2->get_domain(i), temp_int);
            fk_list.push_back(temp_int);
        }
        temp_table->add_foreign_key(temp_table2, fk_list);

    }

    for (i = 0; i < attribute_count; i++)
    {
        if(!ins_check[i])
        {
            temp_table->add_attribute(domains[i].type, domains[i].length, domains[i].name, i);
        }
    }

    delete[] ins_check;
    domains.clear();
    fk_list.clear();
    temp_table->add_to_size(sizeof(*temp_table));
    main_database->add_table(temp_table);
    return 0;
}

void insert_to_table(database *main_database)
{
    int index, success = -1;
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

}

void print_table(database *main_database)
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

void query(database *main_database, int check)
{

    /*Conditions on check
        0 : select
        1 : update
        2 : delete
    */

    std::vector< int > col_list, attributes, ops;
    std::vector< value_expression > expression;
    std::vector< std::string > values, update_values;
    std::vector < std::vector< value_expression > > expression_vec;

    std::set< main_node * > result;

    int col_count, temp_int, expr_count, join_operator, i;
    char rhs[100];
    std::string temp_string;
    value_expression temp_exp;

    int table_index = print_table_details(main_database);
    table *selected_table = main_database->get_tables_index(table_index);

    if(check < 2)
    {
        if (VERBOSE)
        {
            if(check == 1)
                printf("Enter the number of columns you want to update:");
            else if(check == 0)
                printf("Enter the number of columns you want to select:");
        }

        scanf("%d", &col_count);

        if (VERBOSE)
        {
            printf("Enter the indexes of the columns");
            if(check == 1)
                printf(" and their corresponding values:\n");
        }

        for (i = 0; i < col_count; i++)
        {
            scanf("%d", &temp_int);
            col_list.push_back(temp_int - 1);
            if(check == 1)
            {
                scanf("%s", rhs);
                temp_string.assign(rhs);
                update_values.push_back(temp_string);
            }
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
    }
    expression_vec.push_back(expression);
    expression.clear();
    result = selected_table->select_single_table(expression_vec);
    if(check == 0)
    {
        print_record_list(selected_table, result, col_list);
    }
    else if(check == 1)
    {
        selected_table->update(result, col_list, update_values);
    }
    else if(check == 2)
    {
        selected_table->del(result);
    }
}
void print_main_node(main_node *result, std::vector< int > &col_list)
{
    unsigned int i;
    
    for (i = 0; i < col_list.size(); i++)
    {
        printf("%s ", result->get_attribute_list_index(col_list[i])->get_value().c_str());
    }
    return;
}
void join_tables(database *main_database)
{
    table *j_table[2], *temp_table;
    int j_index[2], table_span, i, foreign_key_index, expr_count, j, col_index, tab, op_type, j_type, t_index, col_count;
    char rhs[100];
    struct value_expression temp_expression;
    std::vector< std::vector< value_expression > > expression_vec;
    std::vector< value_expression > temp_expr_vect;
    std::vector< int > col_list[2];
    std::set< main_node * > result;
    std::set< main_node * >::iterator it;
    main_node *node;

    printf("Enter the child table index\n");
    j_index[0] = print_table_details(main_database);
    j_table[0] = main_database->get_tables_index(j_index[0]);
    table_span = j_table[0]->get_foreign_key_count();

    printf("Enter the parent table index\n");
    for (i = 0; i < table_span; i++)
    {
        temp_table = j_table[0]->get_parent_table(i);
        printf("%d: %s\n",i + 1, temp_table->get_table_name().c_str());
    }
    scanf("%d", &foreign_key_index);
    foreign_key_index--;

    j_index[1] = main_database->get_index_table(j_table[0]->get_parent_table(foreign_key_index));

    printf("Enter the number of columns you want to select:");
    scanf("%d", &col_count);

    for (i = 0; i < col_count; i++)
    {
        printf("Parent or child? 1/0 : ");
        scanf("%d", &t_index);

        printf("Enter the column index:");
        scanf("%d", &col_index);

        col_list[t_index].push_back(col_index - 1);
    }

    printf("Enter the number of expressions\n");
    scanf("%d", &expr_count);

    j = 0;
    for (i = 0; i < expr_count; i++)
    {
        printf("LHS column is child or parent? 1/2:");
        scanf("%d", &tab);

        printf("Enter the index of the column:");
        scanf("%d", &col_index);

        printf("Enter the op type:");
        scanf("%d", &op_type);

        printf("Enter the rhs value:");
        scanf("%s", rhs);
        fflush(stdout);

        tab--;
        if (tab)
            temp_expression.table = false;
        else
            temp_expression.table = true;

        temp_expression.attribute = col_index - 1;
        temp_expression.op = op_type;
        temp_expression.value.assign(rhs);

        temp_expr_vect.push_back(temp_expression);

        if (i != expr_count - 1)
        {
            printf("AND or OR? 1/0");
            scanf("%d", &j_type);

            if (j_type == 0)
            {
                j++;
                expression_vec.push_back(temp_expr_vect);
                temp_expr_vect.clear();
            }
        }
    }
    expression_vec.push_back(temp_expr_vect);
    temp_expr_vect.clear();
    result = j_table[0]->join(foreign_key_index, expression_vec);
    for (it = result.begin(); it != result.end(); it++)
    {
        node = *it;
        print_main_node(node, col_list[0]);
        print_main_node(node->get_parent_node(foreign_key_index), col_list[1]);
        printf("\n");
    }


    for (i = 0; i < (int) expression_vec.size(); i++)
        expression_vec[i].clear();

    expression_vec.clear();
    col_list[0].clear();
    col_list[1].clear();
    result.clear();
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
            printf("7) Delete from a table\n");
            printf("8) Join two tables\n");
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
                query(main_database, 0);
                break;

            case 6:
                query(main_database, 1);
                break;

            case 7:
                query(main_database, 2);
                break;

            case 8:
                join_tables(main_database);
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
