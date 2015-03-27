#ifndef META_INCLUDED
#define META_INCLUDED 1

#include<string>
#include<vector>

class table;
class attribute_node;

#include "nodes.h"
#include "trie.h"
#include "table.h"

class domain {

  private:
    int data_type;
    int attr_length;
    //std::string attribute_name;
    std::vector<table *> table_list;
    attribute_node *head;
    trie *index;

  public:

    static const int INTEGER = 1;
    static const int STRING = 2;
    static const int FLOAT = 3;

    //Constructor for a new domain
    domain(int type, int length)
    {
        data_type = type;
        attr_length = length;
        index = new trie();
        head = NULL;
    }

    //Fucntion to get length of attribute
    int get_attr_length()
    {
        return attr_length;
    }

    //Function to get and set data type
    int get_data_type()
    {
        return data_type;
    }

    //Function to get trie Index
    trie *get_index()
    {
        return index;
    }

    int get_table_index(table *tab)
    {
        for(int i = 0; i < table_list.size(); i++)
        {
            if(table_list[i] == tab)
            {
                return i;
            }
        }
        return -1;
    }
    //Function to add new value to domain
    size_t add_get_new_value(std::string new_val, main_node* main)
    {
        bool exists = false;
        size_t node_size = sizeof(main);

        //Add the new value to trie for indexing
        attribute_node* new_node = index->get_node(new_val, exists);

        //Add the new node to domain
        if(head == NULL)
        {
            head = new_node;
            node_size += sizeof(*head) + new_val.length() - sizeof(std::string);
        }
        else if(!exists)
        {
            new_node->set_next(head);
            head = new_node;
            node_size += sizeof(*head) + new_val.length() - sizeof(std::string);
        }

        //new_node->connect_main_record(main);
        main->add_attribute(new_node);
        return node_size;
    }

    int get_num_of_nodes(std::string value, table *tab)
    {
        return 0;
    }

    //Function to deallocate the whole domain
    void clear()
    {
        //attribute_name.clear();
        attribute_node* temp = head;

        while(head != NULL)
        {
            temp = head->get_next();
            head->clear();
            delete head;
            head = temp;
        }
        index->clear();
        delete index;
    }

    //Function to include this domain in a new table
    void add_to_table(table* tab)
    {
        table_list.push_back(tab);
    }
};

#endif // META_INCLUDED
