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

    table *get_table(unsigned int i)
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

    // Function to check whether a table "name" exists or not and returns the index of the table
    // Function returns -1 to indicate that the table does not exist
    int check_tab_name(std::string name)
    {
        int i;
        std::transform(name.begin(), name.end(), name.begin(), toupper);
        for (i = 0; i < tables.size(); i++)
        {
            if (tables[i]->get_table_name().compare(name) == 0)
            {
                printf("Table = %s\n", tables[i]->get_table_name().c_str());
                return i;
            }
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
