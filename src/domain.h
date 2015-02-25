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
    std::string attribute_name;
    attribute_node *head;
    trie *index;

  public:

    static const int INTEGER = 1;
    static const int STRING = 2;
    static const int FLOAT = 3;

    //Constructor for a new domain
    domain(std::string attr_name, int type, int length)
    {
        attribute_name = attr_name;
        data_type = type;
        attr_length = length;
        index = new trie();
        head = NULL;
    }

    //Fucntion to get and set length of attribute
    void set_attribute_length(int len)
    {
        attr_length = len;
    }
    int get_attribute_length()
    {
        return attr_length;
    }

    std::string get_attribute_name()
    {
        return attribute_name;
    }
    int get_data_type()
    {
        return data_type;
    }
    int get_attr_length()
    {
        return attr_length;
    }


    //Function to get trie Index
    trie *get_index()
    {
        return index;
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
        }
        else if(!exists)
        {
            new_node->set_next(head);
            head = new_node;
            node_size += sizeof(*head);
        }

        new_node->connect_main_record(main);
        main->add_attribute(new_node);
        return node_size;
    }

    //Function to search for an attribute value in domain
    attribute_node* get_attr_node(std::string val)
    {
        //return index->query(val);
    }

    //Function to deallocate the whole domain
    void clear()
    {
        attribute_name.clear();
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
