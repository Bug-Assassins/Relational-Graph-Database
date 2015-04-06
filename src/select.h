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

    unsigned int i, node_count, min_node_count = INT_MAX;
    int min_index = -1;
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

            if(main_node_list == NULL)
            {
                return result;
            }

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
        This function accepts a vector of vector.
        The inner vectors are all joined by AND operators only.
        The other vectors is formed by union
    */

    unsigned int i;
    main_node *head;
    std::set< main_node * > result;
    std::vector< main_node * > temp_result;

    if(expression_vec.size() == 0)
    {
        //Get the head of the main node linked list
        head = get_main_node_head();

        //Iterate all records and add it to the set
        while(head != NULL)
        {
            result.insert(head);
            head = head->get_next();
        }

        return result;
    }

    for(i = 0; i < expression_vec.size(); i++)
    {
        //Finding the result of the first total AND vector
        temp_result = select_via_and(expression_vec[i]);

        //Taking union of the current result as well as old result
        result.insert(temp_result.begin(), temp_result.end());
    }

    return result;
}

//Function to return all child nodes connected to a particular main node
std::vector< main_node *> table::get_records_as_child(int foreign_key_index, main_node *parent_record)
{
    unsigned int i, j, min_rec_count = INT_MAX, span_index, prime_index, min_index = INT_MAX;
    std::vector< main_node * > result, *child_result, *min_child_result;
    std::string temp_value;
    for(i = 0; i < foreign_key[foreign_key_index].second.size(); i++)
    {
        span_index = index_in_domain[foreign_key[foreign_key_index].second[i]];
        prime_index = foreign_key[foreign_key_index].first->get_primary_attribute(i);
        child_result = parent_record->get_attribute_list_index(prime_index)->get_record_list(span_index);

        if(child_result == NULL)
        {
            return result;
        }

        if(child_result->size() < min_rec_count)
        {
            min_index = i;
            min_rec_count = child_result->size();
            min_child_result = child_result;
        }
    }

    for(i = 0; i < min_child_result->size(); i++)
    {
        for(j = 0; j < foreign_key[foreign_key_index].second.size(); j++)
        {
            if(j != min_index)
            {
                span_index = foreign_key[foreign_key_index].second[j];
                prime_index = foreign_key[foreign_key_index].first->get_primary_attribute(j);
                temp_value = (*min_child_result)[i]->get_attribute_value(span_index);
                if(temp_value.compare(parent_record->get_attribute_value(prime_index)) != 0)
                {
                    break;
                }
            }
        }

        if(j == foreign_key[foreign_key_index].second.size())
        {
            result.push_back((*min_child_result)[i]);
        }

    }

    return result;
}

//Function to get all child node connected to any node in parent result set
//This function Assumes all logical operators used is AND
std::vector< main_node *> table::get_all_records_as_child(int foreign_key_index, std::vector< value_expression > &expression_list)
{
    unsigned int i;
    std::vector< main_node *> child_result, result, parent_list;

    //Retrieving all the records in the parent table that satisfies given conditions
    parent_list = foreign_key[foreign_key_index].first->select_via_and(expression_list);

    for(i = 0; i < parent_list.size(); i++)
    {
        //Fetching all child records that satisfies the given condition
        child_result = get_records_as_child(foreign_key_index, parent_list[i]);

        //Taking union of these results - We can use vectors because no overlapping
        result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
}

//Function to join via and
std::vector< main_node * > table::join_via_and(int foreign_key_index, std::vector< value_expression > &expression_list)
{
    unsigned int i;
    std::vector< value_expression > parent, child;
    std::vector< main_node *> parent_result, result;

    for(i = 0; i < expression_list.size(); i++)
    {
        if(expression_list[i].table) child.push_back(expression_list[i]);
        else parent.push_back(expression_list[i]);
    }


    if(parent.size() == 0) return select_via_and(child);
    if(child.size() == 0) return get_all_records_as_child(foreign_key_index, parent);

    parent_result = get_all_records_as_child(foreign_key_index, parent);

    for(i = 0; i < parent_result.size(); i++)
    {
        if(compare_record(parent_result[i], child))
        {
            result.push_back(parent_result[i]);
        }
    }

    return result;
}

std::set< main_node *> table::join(int foreign_key_index, std::vector< std::vector< value_expression > > &expression_vec)
{
    /*
     * This function joins the current table with it's parent table
     * It accepts vector of vector for expression
     * Outer Vector for OR and Inner Vector for AND
     */

    unsigned int i;
    main_node *head;
    std::set< main_node * > result;
    std::vector< main_node *> temp_result;

    if(expression_vec.size() == 0)
    {
        //Get the head of the main node linked list
        head = get_main_node_head();

        //Iterate all records and add it to the set
        while(head != NULL)
        {
            result.insert(head);
            head = head->get_next();
        }

        return result;
    }

    for(i = 0; i < expression_vec.size(); i++)
    {
        //temporary result selected via AND
        temp_result = join_via_and(foreign_key_index, expression_vec[i]);

        //Taking UNION of current result with previous result
        result.insert(temp_result.begin(), temp_result.end());
    }

    return result;
}

#endif // SELECT_INCLUDED
