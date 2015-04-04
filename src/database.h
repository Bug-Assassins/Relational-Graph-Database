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

    table *get_tables_index(int i)
    {
        if (i < tables.size())
            return tables[i];
        else
            return NULL;
    }
    int get_tables_size()
    {
        return tables.size();
    }
    int get_index_table(table *tab)
    {
        int i;
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
        name.clear();
        for(int i = 0; i < tables.size(); i++)
        {
            tables[i]->clear();
            delete tables[i];
        }
        tables.clear();
    }
};

#endif // DATABASE_INCLUDED
