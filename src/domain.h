#ifndef META_INCLUDED
#define META_INCLUDED 1

#include<string>
#include<vector>

class table;
class attribute_node;

#include "nodes.h"
#include "trie.h"


class domain {

  private:
    int data_type;
    int attr_length;
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

    int insert_table_pointer(table *tab)
    {
        table_list.push_back(tab);
        return (table_list.size() - 1);
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
    size_t add_get_new_value(std::string new_val, main_node *node, table *tab)
    {
        //printf("%s\n", new_val.c_str());
        bool exists = false;
        size_t node_size = sizeof(node);

        //Add the new value to trie for indexing
        attribute_node* new_node = index->get_node(new_val, exists, table_list.size());

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
        new_node->connect_main_record(node, get_table_index(tab));
        node->add_attribute(new_node);
        return node_size;
    }

    // Function to get the list of main nodes connected to a specific attribute node which belong to the given table.
    std::vector< main_node * >  *get_main_nodes(std::string &value, int table_index)
    {
        bool exists = true;
        attribute_node *node = index->get_node(value, exists, table_list.size());

        if(!node)
        {
            return NULL;
        }

        return node->get_record_list(table_index);

    }

    //Function to deallocate the whole domain
    void clear()
    {
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
    //Returns the index of the table attached
    int add_to_table(table* tab)
    {
        table_list.push_back(tab);
        return (table_list.size() - 1);
    }
};

#endif // META_INCLUDED
