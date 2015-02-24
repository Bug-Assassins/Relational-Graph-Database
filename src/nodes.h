#ifndef NODE_INCLUDED
#define NODE_INCLUDED 1

#include<vector>
#include<string>
#include<cstdlib>

class main_node;

class attribute_node {

  private:
    std::string value;
    std::vector< main_node* > nodes;
    attribute_node *next;

  public:

    //Constructor
    attribute_node(std::string val)
    {
        value = val;
        next = NULL;
    }

    //Getter and Setter for value of attribute node
    void set_value(std::string val)
    {
        value = val;
    }

    std::string get_value()
    {
        return value;
    }

    //Getter and Setter for next pointer
    void set_next(attribute_node &n)
    {
        next = &n;
    }

    attribute_node* get_next()
    {
        return next;
    }

    //Function to added details of connected record
    void connect_main_record(main_node &node)
    {
        nodes.push_back(&node);
    }

    //Function to get number of connected records
    int get_num_records()
    {
        return nodes.size();
    }
};

class main_node {

  public:
    std::vector< attribute_node * > attribute_list;
    main_node *next;
    //std::vector<main_node *> table_list;
    //std::vector<std::vector<main_node *> > foreign_in;

  public:
    //constructor
    main_node()
    {
        next = NULL;
    }

    //Getter and Setter for next node
    void set_next(main_node &next_node)
    {
        next = &next_node;
    }

    main_node* get_next()
    {
        return next;
    }

    //Function to retr
    void add_attribute(attribute_node &attr)
    {
        attribute_list.push_back(&attr);
    }

    int attribute_count()
    {
        return attribute_list.size();
    }

    

};

#endif // NODE_INCLUDED