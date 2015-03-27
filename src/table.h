#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <climits>

class domain;
class foreign_node;
class main_node;

#include "domain.h"

class table {

  private:
    std::string name; //Name of the Table
    main_node *head; //Head to the main list of the table
    int attribute_count; //Number of attributes
    size_t total_size;
    std::vector< domain * > normal;
    std::vector< std::string > attribute_names;
    //std::vector< foreign_key * > foreign;
    //std::vector<table *> foreign_out;
    std::vector<int> primary_keys;

  public:
    //Constructor that creates a table with given name
    table(std::string table_name)
    {
        name = table_name;
        attribute_count = 0;
        head = NULL;
    }

    //Function that adds an attribute to the table
    void add_attribute(domain* dom, std::string name)
    {
        attribute_count++;
        normal.push_back(dom);
        attribute_names.push_back(name);
    }

    //Function to add primary key to the table
    void add_primary_key(int attr_pos)
    {
        assert(attr_pos < normal.size());
        primary_keys.push_back(attr_pos);
    }

    int check_for_primary(std::vector< std::string > &values)
    {
        int i, col_index, node_count = INT_MAX, null_keys = 0;

        for(i = 0; i < primary_keys.size(); i++)
        {
            col_index = primary_keys[i];
            node_count = std::min(normal[i]->get_num_of_nodes(values[col_index], this), node_count);
            if(!node_count)
            {
                null_keys++;
                if(values[col_index].empty())
                    return 1;
            }
        }

        if(null_keys == primary_keys.size())
        {
            return 0;
        }



    }

    //Function that adds a new record to table
    void add_new_record(std::vector<std::string > &values)
    {
        main_node* new_main = new main_node();
        for(int i = 0; i < values.size(); i++)
        {
            if(normal[i]->get_attr_length() < values[i].size())
            {
                //Logic when Given value is larger than the specified length of attribute
                abort();
            }
            add_to_size(normal[i]->add_get_new_value(values[i], new_main));
        }

        new_main->set_next(head);
        head = new_main;
        add_to_size(sizeof(*head));
    }
    std::string get_attribute_name(int index)
    {
        return attribute_names[index];
    }
    std::string get_table_name()
    {
        return name;
    }

    int get_attribute_count()
    {
        return attribute_count;
    }
    domain *get_normal_index(int i)
    {
        if (i < normal.size())
            return normal[i];
        else
            return NULL;
    }
    int add_primary_key_index(int i)
    {
        primary_keys.push_back(i);
    }
    main_node *get_main_node_head()
    {
        return head;
    }


    //Function to deallocate memory occupied by the table
    void clear()
    {
        name.clear();
        for(int i = 0; i < normal.size(); i++)
        {
            normal[i]->clear();
            delete normal[i];
        }
        normal.clear();
        primary_keys.clear();

        main_node *temp ;

        while(head != NULL)
        {
            temp = head->get_next();
            head->clear();
            delete head;
            head = temp;
        }
    }

    void add_to_size(size_t add)
    {
        total_size += add;
    }

    size_t get_size()
    {
        return total_size;
    }

};

#endif // TABLE_INCLUDED
