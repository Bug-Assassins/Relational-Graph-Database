#ifndef SELECT_INCLUDED
#define SELECT_INCLUDED 1
#include <string>
#include <vector>
#include <climits>

class table;

#include "table.h"

//Function to select record from the table
std::vector< main_node * > select_single_table(table *tab, std::vector< int > &attributes, std::vector< std::string > &values, std::vector< int > &ops, std::vector< bool > &join_ops)
{
    /*
        tab = Table on which select operation has to be performed
        attributes = indexes of attributes whose values has been specified (size = n)
        values = Values with which attribute values has to be matched (size = n)
        ops = Operator which has to be using which comparing values (size = n)
        join_ops = Logical Operators used to join comparisons (true = AND, false = OR) (size = n - 1)
    */

    int i, j, node_count, min_index = -1, min_node_count = INT_MAX;
    main_node *head;
    std::vector< main_node * > *main_node_list, *min_main_node_list, result;

    for(i = 0; i < attributes.size(); i++)
    {
        //Searching for only EQUAL operators
        if(ops[i] == 0)
        {
            main_node_list = tab->get_records_with_val(attributes[i], values[i]);
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
        /*
            This signifies that there was no EQUAL operator in the ops.
            Brute Force is the only way to get all nodes
        */
        head = tab->get_main_node_head();
        while (head != NULL)
        {
            if(tab->compare_record(head, attributes, values, ops, join_ops))
            {
                result.push_back(head);
            }
            head = head->get_next();
        }

        //Returning the list of matched records from brute search
        return result;
    }

    //This means that at-least one EQUAL operator was found. We Will iterate only through matched records
    for(i = 0; i < min_main_node_list->size(); i++)
    {
        // To avoid extra string comparison skip min_index
        if(tab->compare_record((*min_main_node_list)[i], attributes, values, ops, join_ops,  min_index))
        {
            result.push_back((*min_main_node_list)[i]);
        }
    }

    //Returning Shortlisted main-nodes that match record
    return result;
}

class condition {

  public:
    int table_index;
    int attribute_index;
    int op;
};
class value_condition : public condition {

  public:
    std::string value;
};
class column_condition : public condition {

  public:
    int dest_table;
};

std::vector< std::vector< main_node * > > join_query(std::vector< table * > &table_list, std::vector< condition * > &conditions, std::vector< bool > &join_ops)
{
    int i, min_index;

    for(i = 0; i < conditions.size(); i++)
    {
        if(conditions[i]->op == 0)
        {

        }
    }
}

#endif // SELECT_INCLUDED