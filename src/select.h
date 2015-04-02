#ifndef SELECT_INCLUDED
#define SELECT_INCLUDED 1
#include <string>
#include <vector>
#include <climits>

class table;

#include "table.h"

//Function to select record from the table
std::vector< main_node * > select_single_table(table *tab, std::vector< int > &attributes, std::vector< std::string > &values, std::vector< int > &ops)
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

    #ifdef DEBUG_SELECT
        printf("Selecting from table %s. %d conditions\n", tab->get_table_name().c_str(), (int) attributes.size());
        fflush(stdout);
    #endif

    for(i = 0; i < attributes.size(); i++)
    {
        //Searching for only EQUAL operators
        if(ops[i] == 0)
        {
            #ifdef DEBUG_SELECT
                printf("Found an Equal Operator Condition at %d", i);
                fflush(stdout);
            #endif // DEBUG

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
        head = tab->get_main_node_head();
        while (head != NULL)
        {
            if(tab->compare_record(head, attributes, values, ops))
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
        if(tab->compare_record((*min_main_node_list)[i], attributes, values, ops,  min_index))
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

class condition {

  public:
    int table_index, attribute_index, op, foreign_key_index;
    // True = Value, False = Column
    bool type;
    std::string value;
};

/*std::vector< std::vector< main_node * > > select_multiple_table(std::vector< table * > &table_list, std::vector< condition * > &conditions, std::vector< bool > &join_ops)
{
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