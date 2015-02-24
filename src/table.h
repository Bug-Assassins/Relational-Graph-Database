#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>

class domain;
class foreign_node;
class main_node;

#include "domain.h"

class table {

  private:
    std::string name; //Name of the Table
    main_node *head; //Head to the main list of the table
    int attribute_count; //
    std::vector< domain * > normal;
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
    void add_attribute(domain &dom)
    {
        attribute_count++;
        normal.push_back(&dom);
    }
    void add_attribute(domain* dom)
    {
        attribute_count++;
        normal.push_back(dom);
    }

    //Function to add primary key to the table
    void add_primary_key(int attr_pos)
    {
        assert(attr_pos < normal.size());
        primary_keys.push_back(attr_pos);
    }

    //Function that adds a new record to table
    void add_new_record(std::vector<std::string > values)
    {
        main_node* new_main = new main_node();
        for(int i = 0; i < values.size(); i++)
        {
            if(normal[i]->get_attribute_length() < values[i].size())
            {
                //Logic when Given value is larger than the specified length of attribute
                abort();
            }
            normal[i]->add_get_new_value(values[i], new_main);
        }
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
    

    //Function to deallocate memory occupied by the table
    void clear()
    {
        name.clear();
        for(int i = 0; i < normal.size(); i++)
        {
            normal[i]->clear();
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

};

#endif // TABLE_INCLUDED
