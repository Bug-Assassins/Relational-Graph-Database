#include"trie.h"

class table;

class meta_domain {

    std::string attribute_name;
    attribute_node *head;
    trie *index;

};

class foreign_key {

    table *origin;
    table *destination;
    std::vector<attribute_node *> destination_domain;
    std::vector<std::string> origin_name;

};

class table {

    std::string name;
    main_node *head;
    int attribute_count;
    std::vector< meta_domain * > normal;
    std::vector< foreign_key * > foreign;
    std::vector<int> prime_in;

};


class database {

    std::vector<table *> tabs;

};

