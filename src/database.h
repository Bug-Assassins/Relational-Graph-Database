#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED 1

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
    void add_table(table *tab)
    {
        tables.push_back(tab);
    }

    table *get_tables_index(unsigned int i)
    {
        if (i > tables.size())
            return NULL;
        
        return tables[i];
    }

    unsigned int get_tables_size()
    {
        return tables.size();
    }

    unsigned int get_index_table(table *tab)
    {
        unsigned int i;

        for (i = 0; i < tables.size(); i++)
        {
            if (tables[i] == tab)
                return i;
        }
        return -1;
    }

    //Function to deallocate the database
    void clear()
    {
        unsigned int i;

        name.clear();
        for(i = 0; i < tables.size(); i++)
        {
            tables[i]->clear();
            delete tables[i];
        }
        tables.clear();
    }
};

#endif // DATABASE_INCLUDED
