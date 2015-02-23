#ifndef FOREIGN_KEY_INCLUDED
#define FOREIGN_KEY_INCLUDE 1
#include<string>
#include<vector>

class table;

#include "table.h"
#include "nodes.h"

class foreign_key {

  private:
    table *origin;
    table *destination;
    std::vector<attribute_node *> destination_domain;
    std::vector<std::string> origin_name;

};

#endif // FOREIGN_KEY_INCLUDED
