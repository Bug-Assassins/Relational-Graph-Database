#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1
#include <string>
#include <vector>
#include <climits>

class table;

#include "table.h"

int compare_values(std::string &x, std::string &y, int op, int data_type)
{
	/*
		Function to compare 2 strings using given operator and data type
		0 -> =
		1 -> <=
		2 -> >=
		3 -> !=
		4 -> >
		5 -> <
	*/
	switch(data_type)
	{
		case 
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

	int i, j, node_count, min_index, min_node_count = INT_MAX;
	bool result;
	std::vector< main_node * > *main_node_list, *min_main_node_list;

	for(i = 0; i < attributes.size(); i++)
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

	for(i = 0; i < min_main_node_list->size(); i++)
	{
		for(j = 1, result = true; j < attributes.size(); j++)
		{
			// To avoid extra string comparison
			// min_index condition is already satisfied
			if(j != min_index) 
			{
				//comparison of value with that in attribute
				compare_value(values[j], min_main_node_list[i]->get_attribute_list_index(attributes[j])->get_value())
				{

				}
			}
		}
	}

}
