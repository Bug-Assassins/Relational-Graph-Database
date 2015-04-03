#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED 1

#include "nodes.h"
#define MAX_SIZE 256


class trie_node {

    private:
        char val;
        trie_node *child[MAX_SIZE];
        attribute_node *node;
    public:
        //Getter and Setter for val
        char get_val()
        {
            return val;
        }

        void set_val(char c)
        {
            val = c;
        }

        //Getter and Setter for node
        attribute_node *get_node()
        {
            return node;
        }

        void set_node(attribute_node *ptr)
        {
            node = ptr;
        }

        //Getter and Setter for child
        trie_node *get_child(int index)
        {
            return child[index];
        }

        void set_child(int index, trie_node *ptr)
        {
            child[index] = ptr;
        }

};

class trie{

  private:
        trie_node *root;

  public:
        trie()
        {
            root = new trie_node();
        }

        attribute_node *get_node(std::string &key, bool &exists, int num_tables)
        {
            int level;
            int length = key.length() - 1;
            int index;
            trie_node *temp , *node_crawl;
            node_crawl = root;

            for(level = 0; level <= length; level++)
            {
                index = key[level];
                temp = node_crawl->get_child(index);
                if(!temp)
                {
                    if(!exists)
                    {
                        return insert_node(node_crawl, key, level, length, num_tables);
                    }
                    return NULL;
                }
                node_crawl = temp;
            }

            attribute_node *node =  node_crawl->get_node();

            if(!node && !exists)
            {
                node = new attribute_node(key, num_tables);
                node_crawl->set_node(node);
            }
            else
            {
                exists = true;
            }

            return node;

        }

        attribute_node *insert_node(trie_node *node_crawl, std::string &key, int level, int length, int num_tables)
        {
            int index = key[level];
            trie_node *temp;

            while(level <= length)
            {
                temp = new trie_node();
                node_crawl->set_child(index, temp);
                node_crawl = temp;
                node_crawl->set_val(key[level++]);
                index = key[level];
            }

            attribute_node *node;
            node = new attribute_node(key, num_tables);
            node_crawl->set_node(node);
            return node;

        }

        void clear()
        {
            clear(root);
        }

        void clear(trie_node *root)
        {
            if(root != NULL)
            {
                for(int i = 0; i < MAX_SIZE; i++)
                {
                    clear(root->get_child(i));
                }

                delete root;
            }
        }

};

#endif //TRIE_INCLUDED

