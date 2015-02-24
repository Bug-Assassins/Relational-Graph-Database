#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED 1

#include<string>
#include<vector>

#include "table.h"

class database {

  private:
    std::string name;
    std::vector<table *> tables;

  public:

    //Constructor that creates a new database with given name
    database(std::string db_name)
    {
        name = db_name;
    }

    //Function that adds a table to the database
    void add_table(table &tab)
    {
        tables.push_back(&tab);
    }

    //Function to deallocate the database
    void clear()
    {
        name.clear();
        for(int i = 0; i < tables.size(); i++)
        {
            tables.clear();
        }
    }
};

#endif // DATABASE_INCLUDED
