#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED 1
#include<string>
#include<vector>

class database {

  private :
    std::string name;
    std::vector<table *> tables;

  public :

    //Constructor that creates a new database with given name
    database(string db_name)
    {
        name = db_name;
    }

    //Function that adds a table to the database
    void add_table(table &tab)
    {
        tables.push_back(&tab);
    }
};

#endif // DATABASE_INCLUDED
