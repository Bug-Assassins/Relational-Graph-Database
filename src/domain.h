#ifndef META_INCLUDED
#define META_INCLUDED 1

class table;
class attribute_node;

#include "nodes.h"
#include "trie.h"


class domain {

  private:
    unsigned int data_type;
    std::vector<table *> table_list;
    attribute_node *head;
    trie *index;

  public:

    static const unsigned int INTEGER = 1;
    static const unsigned int STRING = 2;
    static const unsigned int FLOAT = 3;

    //Constructor for a new domain
    domain(unsigned int type)
    {
        data_type = type;
        index = new trie();
        head = NULL;
    }

    //Function to get and set data type
    unsigned int get_data_type()
    {
        return data_type;
    }

    //Function to get trie Index
    trie *get_index()
    {
        return index;
    }

    unsigned int insert_table_pointer(table *tab)
    {
        table_list.push_back(tab);
        return (table_list.size() - 1);
    }

    unsigned int get_table_index(table *tab)
    {
        unsigned int i;

        for(i = 0; i < table_list.size(); i++)
        {
            if(table_list[i] == tab)
            {
                return i;
            }
        }
        return -1;
    }

    //Function to add new value to domain
    attribute_node* add_get_new_value(std::string &new_val, main_node *node, unsigned int tab_index, bool ins, size_t &node_size)
    {
        bool exists = false;

        //Add the new value to trie for indexing
        attribute_node* new_node = index->get_node(new_val, exists, table_list.size());

        //Add the new node to domain
        if(head == NULL)
        {
            head = new_node;
            node_size += sizeof(attribute_node *) + new_val.length();
        }
        else if(!exists)
        {
            new_node->set_next(head);
            head = new_node;
            node_size += sizeof(attribute_node *) + new_val.length();
        }
        new_node->connect_main_record(node, tab_index);
        if(ins)
            node->add_attribute(new_node);
        return new_node;
    }

    // Function to get the list of main nodes connected to a specific attribute node which belong to the given table.
    std::vector< main_node * >  *get_main_nodes(std::string &value, unsigned int table_index)
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
};

#endif // META_INCLUDED
