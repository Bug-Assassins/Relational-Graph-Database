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
    trie index;

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

    //Function to get trie Index
    trie get_index()
    {
        return index;
    }

    //Function to add new value to domain
    attribute_node *add_new_value(std::string new_val)
    {
        //Add the new value to trie for indexing
        attribute_node* new_node = index.insert_node(new_val);

        //Add the new node to domain
        if(head == NULL)
        {
            head = new_node;
        }
        else
        {
            new_node.set_next(*head);
            head = &new_node;
        }
    }

    //Function to search for an attribute value in domain
    attribute_node* get_attr_node(std::string val)
    {
        return index.query(val);
    }
};

#endif // META_INCLUDED
