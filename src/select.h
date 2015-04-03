#ifndef SELECT_INCLUDED
#define SELECT_INCLUDED 1

class table;

#include "table.h"

//Function to select record from the table given that all logical operators used are AND
std::vector< main_node * > table::select_via_and(std::vector< value_expression > &expression_list)
{
    /*
        tab = Table on which select operation has to be performed
        attributes = indexes of attributes whose values has been specified (size = n)
        values = Values with which attribute values has to be matched (size = n)
        ops = Operator which has to be using which comparing values (size = n)
        All Logical Operators involved is assumed to be AND
    */

    int i, j, node_count, min_index = -1, min_node_count = INT_MAX;
    main_node *head;
    std::vector< main_node * > *main_node_list, *min_main_node_list, result;

    #ifdef DEBUG_SELECT
        printf("Selecting from table %s. %d conditions\n", get_table_name().c_str(),
                                                                    (int) expression_list.size());
        fflush(stdout);
    #endif

    for(i = 0; i < expression_list.size(); i++)
    {
        //Searching for only EQUAL operators
        if(expression_list[i].op == 0)
        {
            #ifdef DEBUG_SELECT
                printf("Found an Equal Operator Condition at %d", i);
                fflush(stdout);
            #endif // DEBUG

            main_node_list = get_records_with_val(expression_list[i].attribute, expression_list[i].value);
            node_count = main_node_list->size();

            if(node_count < min_node_count)
            {
                min_node_count = node_count;
                min_main_node_list = main_node_list;
                min_index = i;
            }
        }
    }

    #ifdef DEBUG_SELECT
        printf("min_index = %d count = %d\n", min_index, min_node_count);
        fflush(stdout);
    #endif // DEBUG

    if(min_index == -1)
    {
        /*
            This signifies that there was no EQUAL operator in the ops.
            Brute Force is the only way to get all nodes
        */
        head = get_main_node_head();
        while (head != NULL)
        {
            if(compare_record(head, expression_list))
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
        if(compare_record((*min_main_node_list)[i], expression_list,  min_index))
        {
            result.push_back((*min_main_node_list)[i]);
        }
    }

    #ifdef DEBUG_SELECT
        printf("Number of records selected = %d\n", (int) result.size());
        fflush(stdout);
    #endif //DEBUG

    //Returning Shortlisted main-nodes that match record
    return result;
}

//Function that executes a generic select query on a single table
std::set< main_node * > table::select_single_table(std::vector< std::vector< value_expression > > &expression_vec)
{
    /*
        tab = Table on which select operation has to be performed
        This function accepts a vector of vector.
        The inner vectors are all joined by AND operators only.
        The other vectors is formed by union
    */

    int i;
    std::set< main_node * > result;
    std::vector< main_node * > temp_result;

    for(i = 0; i < expression_vec.size(); i++)
    {
        //Finding the result of the first total AND vector
        temp_result = select_via_and(expression_vec[i]);

        //Taking union of the current result as well as old result
        result.insert(temp_result.begin(), temp_result.end());
    }

    return result;
}

/*std::set< main_node *> table::join(int foreign_key_index, std::vector< std::vector< value_expression > > &expression_vec)
{
    int i, j, min_index, min_rec_count, temp_rec_count;
    std::set< main_node * > result;
    std::vector< main_node * > temp_result, *child_result, *min_child_result;
    std::vector< value_expression > temp_val_expr;
    value_expression temp_expression
    table *parent = get_parent_table(foreign_key_index);

    for(i = 0; i < conditions.size(); i++)
    {
        //Finding the result of the AND block
        if(condition_list[i].table)
        {
            temp_result = select_via_and(expression_vec[i]);

            //Taking Union of current result and previous result
            result.insert(temp_result.begin(), temp_result.end());
        }
        else
        {
            temp_result = parent->select_via_and(expression_vec[i]);

            for(j = 0; j < temp_result.size(); j++)
            {
                min_index = -1, min_rec_count = INT_MAX;

                for(k = 0; k < foreign_key->get_primary_key_size(); k++)
                {
                    child_result = temp_result[j]->get_child_table_records(k, parent->);
                    temp_rec_count = child_result.size();

                    if(temp_rec_count < min_rec_count)
                    {
                        min_index = k;
                        min_rec_count = temp_rec_count;
                        min_child_result = child_result;
                    }
                }

                for(l = 0; l < min_child_result.size(); l++)
                {
                    for(k = 0; k < )
                }
            }

            //Taking Union of current result and previous result
            result.insert(parent_result.begin(), );
        }
    }


    int i, j, min_index = -1, min_rec_count = INT_MAX, temp_rec_count;  
    std::vector< std::vector< main_node *> > result;
    std::vector<int> tab_indexes;
    std::vector< main_node * > *main_node_list;

    for(i = 0; i < conditions.size(); i++)
    {
        if(conditions[i]->op == 0)
        {
            if(conditions[i].type)
            {
                main_node_list = table_list[conditions[i]->table_index]->get_records_with_val(conditions[i]->attribute_index, conditions[i]->value);
                temp_rec_count = 1;
                for(j = 0; j < table_list.size(); j++)
                {
                    if(j == conditions[i]->table_index)
                    {
                        temp_rec_count *= main_node_list->size();
                    }
                    else
                    {
                        temp_rec_count *= table_list[conditions[i]->table_index]->get_num_records();
                    }
                }
            }
            else
            {
                for(j = 0; j < conditions[i]->)
                right_attribute_index = table_list[conditions[i]->table_index]->get_parent_table_index(
                                    table_list[conditions[i]->dest_table], conditions[i]->attribute_index);

            }

            if(temp_rec_count < min_rec_count)
            {
                min_index = i;
                min_rec_count = temp_rec_count;
            }
        }
    }
}*/

#endif // SELECT_INCLUDED
