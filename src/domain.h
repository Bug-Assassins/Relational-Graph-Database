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
    trie * index;

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
        head = NULL;
    }

	//Function to get trie Index
	trie *get_index()
    {
	    return index;
	}

    //Function to add new value to domain
    void add_new_value(attribute_node &new_node)
    {
        if(head == NULL)
        {
            head = &new_node;
        }
        else
        {
            new_node.set_next(*head);
            head = &new_node;
        }

        //Logic to add new_node it to a trie
    }


};

#endif // META_INCLUDED
