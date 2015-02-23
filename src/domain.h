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
    //Constructor for a new domain
    domain(std::string attr_name, int type, int length)
    {
        attribute_name = attr_name;
        data_type = type;

        head = NULL;
    }
};

#endif // META_INCLUDED
