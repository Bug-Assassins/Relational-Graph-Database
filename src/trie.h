#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED 1

#include "nodes.h"
#define MAX_SIZE 256

class trie_node {

    char val;
    trie_node *child[MAX_SIZE];
    attribute_node *node;

};

class trie{

  private :
    trie_node root;

  public :
    //Constructor to create a new trie
    trie()
    {

    }

};

#endif //TRIE_INCLUDED
