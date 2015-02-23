#ifndef META_INCLUDED
#define META_INCLUDED 1

#include<string>
#include<vector>

class table;
class attribute_node;

#include "nodes.h"
#include "trie.h"

class meta_domain {

  private:
    int data_type;
    int attr_length;
    std::string attribute_name;
    attribute_node *head;
    trie * index;

  public :
    meta_domain(string attr_name)
    {
        attribute_name = attr_name;
        head = NULL;
    }

};

#endif // META_INCLUDED
