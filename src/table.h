#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1
#include<string>
#include<vector>

class table {

  private :
    std::string name;
    main_node *head;
    int attribute_count;
    std::vector< meta_domain * > normal;
    std::vector< foreign_key * > foreign;
    std::vector<int> prime_in;

  public :
    //Constructor that creates a table with given name
    table(string table_name)
    {
        name = table_name;
        attribute_count = 0;
    }

    //Function that adds an attribute to the table
    void add_normal_attribute(meta_domain &domain)
    {
        attribute_count++;
        normal.push_back(&domain);
    }

};

#endif // TABLE_INCLUDED
