#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1
#include <string>
#include <vector>

class domain;
class foreign_node;
class main_node;

#include "domain.h"

class table {

  private:
    std::string name; //Name of the Table
    main_node *head; //Head to the main list of the table
    int attribute_count; //
    std::vector< domain * > normal;
    //std::vector< foreign_key * > foreign;
    //std::vector<table *> foreign_out;
    std::vector<int> prime_in;

  public:
    //Constructor that creates a table with given name
    table(std::string table_name)
    {
        name = table_name;
        attribute_count = 0;
    }

    //Function that adds an attribute to the table
    void add_normal_attribute(domain &dom)
    {
        attribute_count++;
        normal.push_back(&dom);
    }

    

};

#endif // TABLE_INCLUDED
