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
    std::vector< int > primary_keys;
    std::vector< std::pair< table *, std::vector< int > > > foreign_key;

  public:
    //Constructor that creates a table with given name
    table(std::string table_name)
    {
        name = table_name;
        attribute_count = 0;
        head = NULL;
    }

    int get_primary_key_size()
    {
        return primary_keys.size();
    }

    void add_foreign_key(table *temp_table, std::vector< int > &fk_list)
    {
        std::pair <table *, std::vector< int > > temp_pair = std::make_pair(temp_table, fk_list);
        foreign_key.push_back(temp_pair);
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

    //Function to return a main node having the given primary key
    main_node* check_for_primary(std::vector< std::string > &values)
    {
        int i, j, col_index, node_count = INT_MAX, index;
        std::vector< main_node * > *fetch_nodes, min_nodes;

        for(i = 0; i < primary_keys.size(); i++)
        {
            col_index = primary_keys[i];
            fetch_nodes = normal[col_index]->get_main_nodes(values[i], this);

            if(!fetch_nodes || !fetch_nodes->size())
            {
                return NULL;
            }

            if(node_count > fetch_nodes->size())
            {
                node_count = fetch_nodes->size();
                min_nodes = *fetch_nodes;
                index = i;
            }

        }

        for(i = 0; i < min_nodes.size(); i++)
        {
            for(j = 0; j < primary_keys.size(); j++)
            {
                col_index = primary_keys[j];
                if(j != index && values[j].compare(min_nodes[i]->get_attribute_list_index(col_index)->get_value()) != 0)
                {
                    break;
                }
            }

            if(j == primary_keys.size())
            {
                return min_nodes[i];
            }
        }

        return NULL;

    }

    // Function to add foreign key links
    int add_foreign_key_links(std::vector< std::string > &values, main_node *new_main_node)
    {
        std::vector< std::string > temp_values;
        table *temp_table;
        main_node *temp_main_node;
        int i, j;

        for (i = 0; i < foreign_key.size(); i++)
        {
            temp_table = foreign_key[i].first;
            for (j = 0; j < foreign_key[i].second.size(); j++)
            {
                temp_values.push_back(values[foreign_key[i].second[j]]);
                printf("%s\n", temp_values[j].c_str());
            }

            temp_main_node = temp_table->check_for_primary(temp_values);
            if (temp_main_node == NULL)
            {
                temp_values.clear();
                return 1;
            }
            new_main_node->add_foreign_key_link(temp_main_node);
            temp_values.clear();
        }
        return 0;
    }

    //Function that adds a new record to table
    int add_new_record(std::vector< std::string > &values)
    {
        std::vector< std::string > prime(primary_keys.size());
        int i;

        for(i = 0; i < primary_keys.size(); i++)
        {
            if(!values[primary_keys[i]].empty())
            {
                break;
            }
        }

        if(i == primary_keys.size())
        {
            return -1;
        }

        for(i = 0; i < primary_keys.size(); i++)
        {
            prime[i].assign(values[primary_keys[i]]);
        }

        if(check_for_primary(prime))
        {
            prime.clear();
            return 0;
        }

        prime.clear();

        main_node* new_main = new main_node();
        if (add_foreign_key_links(values, new_main) == 1)
        {
            new_main->clear();
            delete new_main;
            return -2;
        }

        for(i = 0; i < values.size(); i++)
        {
            if(normal[i]->get_attr_length() < values[i].size())
            {
                //Logic when Given value is larger than the specified length of attribute
                abort();
            }
            add_to_size(normal[i]->add_get_new_value(values[i], new_main, this));
        }

        new_main->set_next(head);
        head = new_main;
        add_to_size(sizeof(*head));
        return 1;
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
        attribute_names.clear();
        for (int i = 0; i < foreign_key.size(); i++)
        {
            foreign_key[i].second.clear();
        }
        foreign_key.clear();
        primary_keys.clear();
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
