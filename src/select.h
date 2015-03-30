#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1
#include <string>
#include <vector>
#include <climits>

class table;

#include "table.h"

bool compare_values(std::string &left, std::string &right, int op, int data_type)
{
    /*
        Function to compare 2 strings using given operator and data type and returns final result
        0 -> =
        1 -> <=
        2 -> >=
        3 -> !=
        4 -> >
        5 -> <
    */

    if(data_type == domain.INTEGER)
    {
        int l, r;
        sscanf(left.c_str(), "%d", &l);
        sscanf(right.c_str(), "%d", &r);

        if(l == r && op < 3) return true; // Values Equal
        else if(l < r && (op == 1 || op == 3 || op == 5)) return true; //Values Less Than
        else if(l > r && (op == 2 || op == 3 || op == 4)) return true; //Values Greater Than
        else return false;
    }
    else if(data_type == domain.FLOAT)
    {
        float l, r;
        sscanf(left.c_str(), "%f", &l);
        sscanf(right.c_str(), "%f", &r);

        if(l == r && op < 3) return true; // Values Equal
        else if(l < r && (op == 1 || op == 3 || op == 5)) return true; //Values Less Than
        else if(l > r && (op == 2 || op == 3 || op == 4)) return true; //Values Greater Than
        else return false;
    }
    else if(data_type == domain.STRING)
    {
        int res = left.compare(right);

        if(res == 0 && op < 3) return true; //Values Equal
        else if(res < 0 && (op == 1 || op == 3 || op == 5)) return true; //Values Less Than
        else if(res > 0 && (op == 2 || op == 3 || op == 4)) return true; //Values Greater Than
        else return false;
    }
    else
    {
        printf("Wrong Data Type Passed to Compare Function\nAborting\n");
        abort();
    }
}

//Function to select record from the table
void select(table &tab, std::vector< int > &attributes, std::vector< std::string > &values, std::vector< int > &ops, std::vector< bool > &join_ops)
{
    /*
        tab = Table on which select operation has to be performed
        attributes = indexes of attributes whose values has been specified (size = n)
        values = Values with which attribute values has to be matched (size = n)
        ops = Operator which has to be using which comparing values (size = n)
        join_ops = Logical Operators used to join comparisons (true = AND, false = OR) (size = n - 1)
    */

    int i, j, node_count, min_index = -1, min_node_count = INT_MAX;
    bool result, temp_res;
    main_node *head;
    std::vector< main_node * > *main_node_list, *min_main_node_list;

    for(i = 0; i < attributes.size(); i++)
    {
        if(ops[i] == 0)
        {
            main_node_list = tab.get_records_with_val(attributes[i], values[i]);
            node_count = main_node_list->size();

            if(node_count < min_node_count)
            {
                min_node_count = node_count;
                min_main_node_list = main_node_list;
                min_index = i;
            }
        }
    }

    if(min_index == -1)
    {
        head = tab.get_main_node_head();
        while (head != 0)
        {
            
        }
    }

    for(i = 0; i < min_main_node_list->size(); i++)
    {
        for(j = 0, result = true; j < attributes.size(); j++)
        {
            // To avoid extra string comparison
            // min_index condition is already satisfied
            if(j != min_index) 
            {
                //comparison of value with that in attribute
                temp_res = compare_values(values[j], min_main_node_list[i]->get_attribute_list_index(attributes[j])->get_value(),
                                                                    ops[j], tab.get_nomal_index(attributes[j])->get_data_type());

                if(j != 0)
                {
                    if(join_ops[j - 1]) result &= temp_res;
                    else result |= temp_res;
                }
                else
                {
                    result = temp_res;
                }
            }
        }
    }

}
