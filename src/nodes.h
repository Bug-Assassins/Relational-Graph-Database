#ifndef NODE_INCLUDED
#define NODE_INCLUDED 1

#include<vector>
#include<string>

class main_node;

class attribute_node {
  public:
    std::string value;
    std::vector< main_node * > nodes;
    attribute_node *next;

};

class main_node {
  public:
    std::vector< attribute_node * > attribute_list;
    main_node *next;
    std::vector<main_node *> table_list;
    std::vector<std::vector<main_node *> > foreign_in;
};

#endif // NODE_INCLUDED
