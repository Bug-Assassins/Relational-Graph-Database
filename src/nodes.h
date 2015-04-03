#ifndef NODE_INCLUDED
#define NODE_INCLUDED 1

#include <vector>
#include <set>
#include <string>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <algorithm>

class main_node;

class attribute_node {

  private:
    std::string value;
    std::vector< std::vector< main_node * > > nodes;
    attribute_node *next;

  public:

    //Constructor
    attribute_node(std::string &val, int num_tables)
    {
        value = val;
        next = NULL;
        nodes.resize(num_tables);
    }

    //Getter and Setter for value of attribute node
    void set_value(std::string val)
    {
        value = val;
    }

    std::string get_value()
    {
        return value;
    }

    //Getter and Setter for next pointer
    void set_next(attribute_node* n)
    {
        next = n;
    }

    attribute_node* get_next()
    {
        return next;
    }

    //Function to add details of connected record of a given table
    //Index number follows from the domain class
    void connect_main_record(main_node *node, int tab_index)
    {
        nodes[tab_index].push_back(node);
    }

    //Function to get number of connected records
    int get_num_records(int tab_index)
    {
        return nodes[tab_index].size();
    }

    std::vector< main_node * > *get_record_list(int table_index)
    {
        if(table_index < 0 || table_index > nodes.size())
        {
            printf("Passed Table index %d do not exist !!\nAborting!!", table_index);
            fflush(stdout);
            abort();
        }

        return &nodes[table_index];
    }

    void delete_edge(main_node *tab_node, int index)
    {
        nodes[index].erase(std::remove(nodes[index].begin(), nodes[index].end(), tab_node), nodes[index].end());
    }
    //Function to clear the node
    void clear()
    {
        value.clear();
        for(int i = 0; i < nodes.size(); i++)
        {
            nodes[i].clear();
        }
        nodes.clear();
    }

};

class main_node {

  private:
    std::vector< attribute_node * > attribute_list;
    main_node *next, *pre;
    std::vector< main_node * > parent_table_list;

  public:
    //constructor
    main_node()
    {
        next = pre = NULL;
    }

    //Getter and Setter for next node
    void set_next(main_node* next_node)
    {
        next = next_node;
    }

    void set_pre(main_node* pre_node)
    {
        pre = pre_node;
    }

    main_node* get_next()
    {
        return next;
    }

    //Function to return the corresponding connected attribute node
    attribute_node *get_attribute_list_index(int attribute_index)
    {
        if(attribute_index < 0 || attribute_index > attribute_list.size())
        {
            printf("Passed attribute index %d do not exist !!\nAborting!!", attribute_index);
            fflush(stdout);
            abort();
        }
        return attribute_list[attribute_index];
    }

    //Function to get an attribute node value
    std::string get_attribute_value(int attribute_index)
    {
        return get_attribute_list_index(attribute_index)->get_value();
    }

    //Function to add attribute to the main node
    void add_attribute(attribute_node* attr)
    {
        attribute_list.push_back(attr);
    }

    void update_attribute(int index, attribute_node *value)
    {
        attribute_list[index] = value;
    }

    void add_foreign_key_link(main_node *fk)
    {
        parent_table_list.push_back(fk);
    }

    //Function to deallocate memory
    void clear()
    {
        attribute_list.clear();
        parent_table_list.clear();
    }
};

#endif // NODE_INCLUDED
