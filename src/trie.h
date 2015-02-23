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

        attribute_node *get_node(string key)
        {
            int level;
            int length = key.length() - 1;
            int index;
            trie_node *node_crawl = root;
            for(level = 0; level < length; level++)
            {
                index = key[level];
                if(!node_crawl->child[index])
                {
                    break;
                }

                node_crawl = node_crawl->child[index];
            }

            while(level < length)
            {
                node_crawl->child[index] = new trie_node();
                node_crawl = node_crawl->child[index];
                node_crawl->val = key[level++];
                index = key[level];
            }

            if(node_crawl->node)
            {
                return node_crawl->node;
            }

            else
            {
                return node_crawl->node = new attribute_node();
            }
        }

};

#endif //TRIE_INCLUDED

