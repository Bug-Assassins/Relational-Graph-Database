#define VERBOSE 1
#define DEBUG 1
#define TEST 1

#include <cstdio>

#include "database.h"
#include "select.h"

#ifdef TEST
    #include "test.h"
#endif

int print_table_details(database *main_database)
{
    table *temp_table;
    unsigned int i, index, att_count;
    int temp_int;

    if (VERBOSE)
    {
        printf("Enter the table you want\n");
        for (i = 0; i < main_database->get_tables_size(); i++)
        {
            printf("%d : %s\n", i + 1, main_database->get_table(i)->get_table_name().c_str());
        }
    }
    scanf("%d", &index);
    printf("\n");

    temp_table = main_database->get_table(index - 1);
    att_count = temp_table->get_attribute_count();

    if (VERBOSE)
    {
        for (i = 0; i < att_count; i++)
        {

            printf ("%s\t", temp_table->get_attribute_name(i).c_str());
            temp_int = temp_table->get_normal_index(i)->get_data_type();
            if (temp_int == 1)
                printf("INTEGER\n");
            else if (temp_int == 2)
                printf("STRING\n");
            else
                printf("FLOAT\n");
        }
    }

    return index - 1;
}
void print_record_list(table *tab, std::set< main_node *> &record_list, std::vector< int > &attributes)
{
    unsigned int i, j;
    std::set< main_node * >::iterator it;

    printf("\n--------------------------------------------------------------------------------\n");
    //Printing the heading of each column
    for(i = 0; i < attributes.size(); i++)
    {
        printf("%s\t", tab->get_attribute_name(attributes[i]).c_str());
    }
    printf("\n--------------------------------------------------------------------------------\n");

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

void create_table(database *main_database)
{
    std::string temp_str;
    std::vector< unsigned int > type;
    std::vector< std::string > attribute_names;
    std::vector< int > fk_list;
    char temp_name[100];
    int attribute_count, i, j, pkey_span, temp_int, fk_count, table_index;
    unsigned int temp_type;
    table *temp_table, *temp_table2;

    if (VERBOSE)
        printf("Enter the table name:");

    scanf("%s", temp_name);
    temp_str.assign(temp_name);
    std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), toupper);


    if (VERBOSE)
        printf("Enter the number of attributes:");

    scanf("%d", &attribute_count);

    temp_table = new table(temp_str, attribute_count);
    bool *ins_check = new bool[attribute_count];

    for (i = 0; i < attribute_count; i++)
    {
        ins_check[i] = false;

        if (VERBOSE)
            printf("Enter the attribute name, type (1: INTEGER 2: STRING 3: FLOAT) :");

        scanf("%s %u", temp_name, &temp_type);
        temp_str.assign(temp_name);
        std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), toupper);
        attribute_names.push_back(temp_str);
        type.push_back(temp_type);
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
        temp_table2 =  main_database->get_table(table_index);

        for (j = 0; j <  temp_table2->get_primary_key_size(); j++)
        {
            scanf("%d", &temp_int);
            temp_int--;
            ins_check[temp_int] = true;
            temp_table->add_foreign_domain(attribute_names[temp_int], temp_table2->get_domain(i), temp_int);
            fk_list.push_back(temp_int);
        }

        temp_table->add_foreign_key(temp_table2, fk_list);
    }

    for (i = 0; i < attribute_count; i++)
    {
        if(ins_check[i] == false)
        {
            temp_table->add_attribute(type[i], attribute_names[i], i);
        }
    }

    delete[] ins_check;
    fk_list.clear();
    temp_table->add_to_size(sizeof(*temp_table));
    main_database->add_table(temp_table);

    return;
}

void insert_to_table(database *main_database)
{
    int index, success = -1;
    char temp_char_arr[100];
    std::string temp_string;
    table *temp_table;

    printf("Enter table name:");
    fflush(stdout);
    scanf("%s", temp_char_arr);
    temp_string.assign(temp_char_arr);
    index = main_database->check_tab_name(temp_string);
    if (index == -1)
    {
        printf("Wrong table name\n");
        return;
    }
    temp_table = main_database->get_table(index);

    std::vector< std::string > values(temp_table->get_attribute_count());

    if (VERBOSE)
    {
        printf("Enter column name, value pair. Enter 0 to finish entering info\n");
    }

    while (1)
    {
        scanf("%s", temp_char_arr);
        temp_string.assign(temp_char_arr);
        index = temp_table->check_column_name(temp_string);
        if (index == -1)
            break;
        scanf("%s", temp_char_arr);
        values[index].assign(temp_char_arr);
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
    int attribute_count, i, index;
    table *temp_table;
    main_node *temp_main_node, *next_node;
    char temp_char_arr[100];
    std::string temp_string;

    printf("Enter the name of table:");
    fflush(stdout);
    scanf("%s", temp_char_arr);
    printf("\n");
    temp_string.assign(temp_char_arr);
    index = main_database->check_tab_name(temp_string);
    if (index == -1)
    {
        printf("Table does not exist\n");
        return;
    }

    temp_table = main_database->get_table(index);
    temp_main_node = temp_table->get_main_node_head();
    attribute_count = temp_table->get_attribute_count();

    printf("\n--------------------------------------------------------------------------------\n");
    for (i = 0; i < attribute_count; i++)
    {
        printf("%s\t", temp_table->get_attribute_name(i).c_str());
    }
    printf("\n--------------------------------------------------------------------------------\n");

    while(temp_main_node && temp_main_node->lazy_delete())
    {
        next_node = temp_main_node;
        temp_main_node = temp_main_node->get_next();
        delete next_node;
    }

    temp_table->set_main_node_head(temp_main_node);

    if(temp_main_node == NULL) return;

    for (i = 0; i < attribute_count; i++)
    {
        printf("%s\t", temp_main_node->get_attribute_list_index(i)->get_value().c_str());
    }
    printf("\n");

    next_node = temp_main_node->get_next();
    while (next_node)
    {
        if(next_node->lazy_delete())
        {
            temp_main_node->delete_next();
        }

        else
        {
            for (i = 0; i < attribute_count; i++)
            {
                printf("%s\t", next_node->get_attribute_list_index(i)->get_value().c_str());
            }
            printf("\n");
            temp_main_node = next_node;
        }
        next_node = temp_main_node->get_next();
    }
    printf("\n--------------------------------------------------------------------------------\n");
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

    int col_count, temp_int, expr_count = 0, join_operator, i;
    char rhs[100], lhs[100], op_string[100];
    std::string temp_string;
    value_expression temp_exp;
    char t_name[100], col_name[100], temp_char_arr[100];


    printf("\nEnter the table name: ");
    fflush(stdout);
    scanf("%s", t_name);
    temp_string.assign(t_name);
    int table_index = main_database->check_tab_name(temp_string);
    if (table_index == -1)
    {
        printf("Wrong table name\n");
        return;
    }
    table *selected_table = main_database->get_table(table_index);
    printf("\n");
    if(check < 2)
    {
        if (VERBOSE)
        {
            if(check == 1)
                printf("Enter the number of columns you want to update: ");
            else if(check == 0)
                printf("Enter the number of columns you want to select: ");
            fflush(stdout);
        }

        scanf("%d", &col_count);

        if (VERBOSE)
        {
            printf("Enter the column name(s)");
            if(check == 1)
                printf(" and their corresponding value(s)");
            printf(":\n");
        }

        for (i = 0; i < col_count; i++)
        {
            scanf("%s", col_name);
            temp_string.assign(col_name);
            temp_int = selected_table->check_column_name(temp_string);
            if (temp_int == -1)
            {
                printf("Wrong column\n");
            }
            col_list.push_back(temp_int);
            if(check == 1)
            {
                scanf("%s", rhs);
                temp_string.assign(rhs);
                update_values.push_back(temp_string);
            }
        }
    }

    if (VERBOSE)
        printf("\nEnter the number of conditions:");
    fflush(stdout);

    scanf("%d", &expr_count);

    for (i = 0; i < expr_count; i++)
    {
        if (VERBOSE)
        {
            printf("Enter condition no. %d:", i + 1);
            fflush(stdout);
        }
        scanf("%s %s %s", lhs, op_string, rhs);
        temp_exp.value.assign(rhs);

        temp_string.assign(lhs);
        temp_int = selected_table->check_column_name(temp_string);
        if (temp_int == -1)
        {
            printf("Wrong column name\n");
            return;
        }
        temp_exp.attribute = temp_int;

        temp_string.assign(op_string);
        if (temp_string.compare("=") == 0)
            temp_exp.op = 0;
        else if (temp_string.compare("<=") == 0)
            temp_exp.op = 1;
        else if (temp_string.compare(">=") == 0)
            temp_exp.op = 2;
        else if (temp_string.compare("!=") == 0)
            temp_exp.op = 3;
        else if (temp_string.compare(">") == 0)
            temp_exp.op = 4;
        else if (temp_string.compare("<") == 0)
            temp_exp.op = 5;
        else
        {
            printf("Wrong operation\n");
            return;
        }

        expression.push_back(temp_exp);

        if (i != expr_count - 1)
        {
            if (VERBOSE)
                printf("AND/OR ? :");
            fflush(stdout);

            scanf("%s", temp_char_arr);
            temp_string.assign(temp_char_arr);
            if (temp_string.compare("AND") == 0)
                join_operator = 1;
            else if(temp_string.compare("OR") == 0)
                join_operator = 0;
            else
            {
                printf("Wrong option\n");
                return;
            }

            if(join_operator == 0)
            {
                expression_vec.push_back(expression);
                expression.clear();
            }
        }
    }

    if(expr_count)
    {
        expression_vec.push_back(expression);
        expression.clear();
    }

    else
    {
        switch(check)
        {
            case 0:
                selected_table->select_all(col_list);
                break;
            case 1:
                selected_table->update_all(col_list, update_values);
                break;
            case 2:
                selected_table->delete_all();
                break;
            default:
                printf("Wrong query condition\n");
        }
        return;
    }

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
        printf("%s\t", result->get_attribute_list_index(col_list[i])->get_value().c_str());
    }
    return;
}
void join_tables(database *main_database)
{
    table *j_table[2], *temp_table;
    int j_index[2], table_span, i, foreign_key_index, expr_count, j, col_index, tab, op_type, j_type, t_index, col_count;
    struct value_expression temp_expression;
    std::vector< std::vector< value_expression > > expression_vec;
    std::vector< value_expression > temp_expr_vect;
    std::vector< int > col_list[2];
    std::set< main_node * > result;
    std::set< main_node * >::iterator it;
    main_node *node;
    std::string temp_string;
    char t_name[100], col_name[100], temp_char_arr[500];
    char lhs[100], op_string[4], rhs[100];

    printf("\nEnter the child table name:");
    fflush(stdout);
    scanf("%s", t_name);
    temp_string.assign(t_name);
    j_index[0] = main_database->check_tab_name(temp_string);
    if (j_index[0] == -1)
    {
        printf("Wrong table name\n");
        return;
    }

    j_table[0] = main_database->get_table(j_index[0]);
    table_span = j_table[0]->get_foreign_key_count();

    printf("Enter the parent table name:");
    fflush(stdout);
    scanf("%s", t_name);
    temp_string.assign(t_name);
    foreign_key_index = j_table[0]->check_parent_name(temp_string);
    if (foreign_key_index == -1)
    {
        printf("Wrong parent name\n");
        return;
    }
    j_index[1] = main_database->get_index_table(j_table[0]->get_parent_table(foreign_key_index));
    j_table[1] = main_database->get_table(j_index[1]);

    printf("\nEnter the number of columns you want to select:");
    fflush(stdout);
    scanf("%d", &col_count);

    for (i = 0; i < col_count; i++)
    {
        printf("\nColumn name(TABLE.COLUMN_NAME) : ");
        fflush(stdout);
        scanf("%s", temp_char_arr);
        temp_string.assign(temp_char_arr);
        t_index = main_database->check_tab_name(temp_string.substr(0, temp_string.find(".")));

        if (t_index == -1)
        {
            printf("Wrong table name\n");
            return;
        }
        else if (t_index == j_index[0])
        {
            t_index = 0;
        }
        else if (t_index == j_index[1])
        {
            t_index = 1;
        }
        else
        {
            printf("This table was not selected\n");
            return;
        }

        temp_string.assign(temp_string.substr(temp_string.find(".") + 1));
        col_index = j_table[t_index]->check_column_name(temp_string);
        if (col_index == -1)
        {
            printf("Wrong column selected\n");
            return;
        }
        col_list[t_index].push_back(col_index);
    }

    printf("\nEnter the number of conditions:");
    fflush(stdout);
    scanf("%d", &expr_count);

    j = 0;
    for (i = 0; i < expr_count; i++)
    {

        printf("\nCondition:");
        fflush(stdout);
        scanf("%s %s %s", lhs, op_string, rhs);
        temp_string.assign(lhs);
        t_index = main_database->check_tab_name(temp_string.substr(0, temp_string.find(".")));

        if (t_index == -1)
        {
            printf("Wrong table name\n");
            return;
        }
        else if (t_index == j_index[0])
        {
            t_index = 0;
        }
        else if (t_index == j_index[1])
        {
            t_index = 1;
        }
        else
        {
            printf("This table was not selected\n");
            return;
        }

        temp_string.assign(temp_string.substr(temp_string.find(".") + 1));
        col_index = j_table[t_index]->check_column_name(temp_string);
        if (col_index == -1)
        {
            printf("Wrong column selected\n");
            return;
        }

        temp_string.assign(op_string);
        if (temp_string.compare("=") == 0)
            op_type = 0;
        else if (temp_string.compare("<=") == 0)
            op_type = 1;
        else if (temp_string.compare(">=") == 0)
            op_type = 2;
        else if (temp_string.compare("!=") == 0)
            op_type = 3;
        else if (temp_string.compare(">") == 0)
            op_type = 4;
        else if (temp_string.compare("<") == 0)
            op_type = 5;
        else
        {
            printf("Wrong operation\n");
            return;
        }

        if (t_index)
            temp_expression.table = false;
        else
            temp_expression.table = true;

        temp_expression.attribute = col_index;
        temp_expression.op = op_type;
        temp_expression.value.assign(rhs);

        temp_expr_vect.push_back(temp_expression);

        if (i != expr_count - 1)
        {
            printf("AND/OR: ");
            fflush(stdout);
            scanf("%s", temp_char_arr);
            temp_string.assign(temp_char_arr);
            if (temp_string.compare("AND") == 0)
                j_type = 1;
            else if(temp_string.compare("OR") == 0)
                j_type = 0;
            else
            {
                printf("Wrong option\n");
                return;
            }

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
    for (i = 0; i < col_list[0].size(); i++)
    {
        printf("%s.", j_table[0]->get_table_name().c_str());
        printf("%s  ", j_table[0]->get_attribute_name(col_list[0][i]).c_str());
    }
    for (i = 0; i < col_list[1].size(); i++)
    {
        printf("%s.", j_table[1]->get_table_name().c_str());
        printf("%s  ", j_table[1]->get_attribute_name(col_list[1][i]).c_str());
    }
    printf("\n");


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

#ifdef TEST
    create_dept(main_database);
    create_emp(main_database);
	insert_dept(main_database);
	insert_emp(main_database);
#endif


    while (1)
    {
        if (VERBOSE)
        {
            printf("\n");
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
