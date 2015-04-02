#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED 1


class domain;
class foreign_node;
class main_node;
class domain;

#include "domain.h"

bool compare_values(std::string &left, std::string &right, int op, int data_type)
{
    /*
        Function to compare 2 strings using given operator and data type and returns final result
        0 -> =
        1 -> <=
        2 -> >=
        3 -> !=
        4 -> >
        5 -> <
    */

    if(data_type == domain::INTEGER)
    {
        int l, r;
        sscanf(left.c_str(), "%d", &l);
        sscanf(right.c_str(), "%d", &r);

        if(l == r && op < 3) return true; // Values Equal
        else if(l < r && (op == 1 || op == 3 || op == 5)) return true; //Values Less Than
        else if(l > r && (op == 2 || op == 3 || op == 4)) return true; //Values Greater Than
        else return false;
    }
    else if(data_type == domain::FLOAT)
    {
        float l, r;
        sscanf(left.c_str(), "%f", &l);
        sscanf(right.c_str(), "%f", &r);

        if(l == r && op < 3) return true; // Values Equal
        else if(l < r && (op == 1 || op == 3 || op == 5)) return true; //Values Less Than
        else if(l > r && (op == 2 || op == 3 || op == 4)) return true; //Values Greater Than
        else return false;
    }
    else if(data_type == domain::STRING)
    {
        int res = left.compare(right);

        if(res == 0 && op < 3) return true; //Values Equal
        else if(res < 0 && (op == 1 || op == 3 || op == 5)) return true; //Values Less Than
        else if(res > 0 && (op == 2 || op == 3 || op == 4)) return true; //Values Greater Than
        else return false;
    }
    else
    {
        printf("Wrong Data Type Passed to Compare Function\nAborting\n");
        fflush(stdout);
        abort();
    }
}

struct value_expression{
    int attribute;
    int op;
    std::string value;
};

class table {

  private:
    std::string name; //Name of the Table
    main_node *head; //Head to the main list of the table
    int attribute_count, record_count; //Number of attributes
    size_t total_size;
    std::vector< domain * > normal;
    std::vector< std::string > attribute_names;
    std::vector< int > index_in_domain;
    //std::vector< foreign_key * > foreign;
    //std::vector<table *> foreign_out;
    std::vector< int > primary_keys;
    std::vector< std::pair< table *, std::vector< int > > > foreign_key;

  public:
    //Constructor that creates a table with given name
    table(std::string table_name)
    {
        name = table_name;
        attribute_count = 0;
        record_count = 0;
        head = NULL;
    }

    int get_primary_key_size()
    {
        return primary_keys.size();
    }

    void add_foreign_key(table *temp_table, std::vector< int > &fk_list)
    {
        std::pair <table *, std::vector< int > > temp_pair = std::make_pair(temp_table, fk_list);
        foreign_key.push_back(temp_pair);
    }

    //Function that adds an attribute to the table
    void add_attribute(int type, int length, std::string name)
    {
        int tab_index;
        domain *temp_domain;

        temp_domain = new domain(type, length);
        tab_index = temp_domain->insert_table_pointer(this);
        add_to_size(sizeof(*temp_domain) + name.length());
        attribute_count++;
        normal.push_back(temp_domain);
        attribute_names.push_back(name);
        index_in_domain.push_back(tab_index);
    }

    //Function to return the index in primary_key list
    int get_primary_attribute(int i)
    {
        if(i < 0 || i > primary_keys.size())
        {
            printf("Wrong Primary Key Attribute Index Requested!\nAborting!!");
            fflush(stdout);
            abort();
        }
        return primary_keys[i];
    }

    //Function to return attribute index in parent table given index here
    int get_parent_table_index(table *parent, int idx)
    {
        int i, j;

        for(i = 0; i < foreign_key.size(); i++)
        {
            if(foreign_key[i].first == parent)
            {
                for(j = 0; j < foreign_key[i].second.size(); j++)
                {
                    if(foreign_key[i].second[j] == idx)
                    {
                        return parent->get_primary_attribute(j);
                    }
                }

                printf("Passed an attribute index which is foreign key span!!\nAborting!!\n");
                fflush(stdout);
                abort();
            }
        }
    }

    //Function to find all the main_nodes with a certain attribute value
    std::vector< main_node * >  *get_records_with_val(int attribute_index, std::string &value)
    {
        return normal[attribute_index]->get_main_nodes(value, index_in_domain[attribute_index]);
    }

    //Function to add primary key to the table
    void add_primary_key(int attr_pos)
    {
        assert(attr_pos < normal.size());
        primary_keys.push_back(attr_pos);
    }

    //Function to return a main node having the given primary key
    main_node* check_for_primary(std::vector< std::string > &values)
    {
        int i, j, col_index, node_count = INT_MAX, index;
        std::vector< main_node * > *fetch_nodes, min_nodes;

        for(i = 0; i < primary_keys.size(); i++)
        {
            col_index = primary_keys[i];
            fetch_nodes = normal[col_index]->get_main_nodes(values[i], index_in_domain[col_index]);

            if(!fetch_nodes || !fetch_nodes->size())
            {
                return NULL;
            }

            if(node_count > fetch_nodes->size())
            {
                node_count = fetch_nodes->size();
                min_nodes = *fetch_nodes;
                index = i;
            }

        }

        for(i = 0; i < min_nodes.size(); i++)
        {
            for(j = 0; j < primary_keys.size(); j++)
            {
                col_index = primary_keys[j];
                if(j != index && values[j].compare(min_nodes[i]->get_attribute_list_index(col_index)->get_value()) != 0)
                {
                    break;
                }
            }

            if(j == primary_keys.size())
            {
                return min_nodes[i];
            }
        }

        return NULL;

    }

    // Function to add foreign key links
    int add_foreign_key_links(std::vector< std::string > &values, main_node *new_main_node)
    {
        std::vector< std::string > temp_values;
        table *temp_table;
        main_node *temp_main_node;
        int i, j;

        for (i = 0; i < foreign_key.size(); i++)
        {
            temp_table = foreign_key[i].first;
            for (j = 0; j < foreign_key[i].second.size(); j++)
            {
                temp_values.push_back(values[foreign_key[i].second[j]]);
                printf("%s\n", temp_values[j].c_str());
            }

            temp_main_node = temp_table->check_for_primary(temp_values);
            if (temp_main_node == NULL)
            {
                temp_values.clear();
                return 1;
            }
            new_main_node->add_foreign_key_link(temp_main_node);
            temp_values.clear();
        }
        return 0;
    }

    //Function that adds a new record to table
    int add_new_record(std::vector< std::string > &values)
    {
        std::vector< std::string > prime(primary_keys.size());
        int i;

        for(i = 0; i < primary_keys.size(); i++)
        {
            if(!values[primary_keys[i]].empty())
            {
                break;
            }
        }

        if(i == primary_keys.size())
        {
            return -1;
        }

        for(i = 0; i < primary_keys.size(); i++)
        {
            prime[i].assign(values[primary_keys[i]]);
        }

        if(check_for_primary(prime))
        {
            prime.clear();
            return 0;
        }

        prime.clear();

        main_node* new_main = new main_node();
        if (add_foreign_key_links(values, new_main) == 1)
        {
            new_main->clear();
            delete new_main;
            return -2;
        }

        for(i = 0; i < values.size(); i++)
        {
            if(normal[i]->get_attr_length() < values[i].size())
            {
                //Logic when Given value is larger than the specified length of attribute
                abort();
            }
            add_to_size(normal[i]->add_get_new_value(values[i], new_main, index_in_domain[i]));
        }

        new_main->set_next(head);
        head = new_main;
        add_to_size(sizeof(*head));
        record_count++;
        return 1;
    }

    //Function that compares a particular attribute value of a record and returns result
    bool compare_attribute(main_node *record, value_expression expr)
    {
        std::string attr_val = record->get_attribute_list_index(expr.attribute)->get_value();
        return compare_values(attr_val, expr.value, expr.op, normal[expr.attribute]->get_data_type());
    }

    //Function to compare all given values of a record with attributes are returns result
    bool compare_record(main_node *record, std::vector< value_expression > &expr, int skip)
    {
        /*
            Skip was added to avoid extra string comparison when it is certain that condition in the given
            position will evaluate to true. All Logical operatos assumed to be true. Shortcircuiting
            expressions also implemented
        */

        int i;

        for(i = 0; i < expr.size(); i++)
        {
            if((expr[i].attribute != skip) && (!compare_attribute(record, expr[i])))
            {
                return false;
            }
        }

        return true;
    }

    //Overloaded Function to compare all given values with attributes without skipping
    bool compare_record(main_node *record, std::vector< value_expression > &expr)
    {
        return compare_record(record, expr, -1);
    }

    void update(std::set< main_node * > &nodes,  std::vector< int > &index, std::vector< std::string > &values)
    {
        std::set< main_node * >::iterator it;
        int j;
        attribute_node *old_node;
        for(it = nodes.begin(); it != nodes.end(); it++)
        {
            for(j = 0; j < index.size(); j++)
            {
                old_node = (*it)->get_attribute_list_index(index[j]);
                if(values[j].compare(old_node->get_value()))
                {
                    old_node->delete_edge((*it), index_in_domain[index[j]]);
                    attribute_node *new_attribute_node = normal[index[j]]->add_new_attribute(values[j],
                                                                    (*it), index_in_domain[index[j]]);
                    (*it)->update_attribute(index[j], new_attribute_node);
                }
            }
        }
    }

    std::string get_attribute_name(int index)
    {
        return attribute_names[index];
    }

    std::string get_table_name()
    {
        return name;
    }

    int get_attribute_count()
    {
        return attribute_count;
    }

    domain *get_normal_index(int i)
    {
        if (i < normal.size())
            return normal[i];
        else
            return NULL;
    }

    int add_primary_key_index(int i)
    {
        primary_keys.push_back(i);
    }

    main_node *get_main_node_head()
    {
        return head;
    }

    //Function to deallocate memory occupied by the table
    void clear()
    {
        name.clear();
        for(int i = 0; i < normal.size(); i++)
        {
            normal[i]->clear();
            delete normal[i];
        }
        normal.clear();
        primary_keys.clear();

        main_node *temp ;

        while(head != NULL)
        {
            temp = head->get_next();
            head->clear();
            delete head;
            head = temp;
        }
        attribute_names.clear();
        for (int i = 0; i < foreign_key.size(); i++)
        {
            foreign_key[i].second.clear();
        }
        foreign_key.clear();
    }

    void add_to_size(size_t add)
    {
        total_size += add;
    }

    size_t get_size()
    {
        return total_size;
    }

    int get_num_records()
    {
        return record_count;
    }

};

#endif // TABLE_INCLUDED
