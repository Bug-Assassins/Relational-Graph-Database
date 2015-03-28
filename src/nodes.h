#ifndef NODE_INCLUDED
#define NODE_INCLUDED 1

#include<vector>
#include<string>
#include<cstdlib>

class main_node;

class attribute_node {

  private:
    std::string value;
    std::vector< std::vector< main_node * > > nodes;
    attribute_node *next;

  public:

    //Constructor
    attribute_node(std::string val, int num_tables)
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

    std::vector< main_node * > *get_record_list(int tab_index)
    {
        return &nodes[tab_index];
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
    main_node *next;
    std::vector< main_node * > parent_table_list;

  public:
    //constructor
    main_node()
    {
        next = NULL;
    }

    //Getter and Setter for next node
    void set_next(main_node* next_node)
    {
        next = next_node;
    }
    main_node* get_next()
    {
        return next;
    }
    attribute_node *get_attribute_list_index(int i)
    {
        return attribute_list[i];
    }

    //Function to add attribute to the main node
    void add_attribute(attribute_node* attr)
    {
        attribute_list.push_back(attr);
    }

    void add_foreign_key_link(main_node *fk)
    {
        parent_table_list.push_back(fk);
    }
    //Function to return number of attributes
    int attribute_count()
    {
        return attribute_list.size();
    }

    //Function to deallocate memory
    void clear()
    {
        attribute_list.clear();
        parent_table_list.clear();
    }
};

#endif // NODE_INCLUDED
