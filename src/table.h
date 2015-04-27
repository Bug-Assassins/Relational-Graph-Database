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

    /*
     * table signifies if it's parent table or child table expression
     * True = Child
     * False = Parent
     */
    bool table;
    int attribute;
    int op;
    std::string value;

    value_expression()
    {
        table = true;
    }
};

class table {

  private:
    std::string name; //Name of the Table
    main_node *head; //Head to the main list of the table
    int record_count; //Number of records
    size_t total_size;
    std::vector< domain * > normal;
    std::vector< std::string > attribute_names;
    std::vector< int > index_in_domain;
    std::vector< int > primary_keys;
    std::vector< std::pair< table *, std::vector< int > > > foreign_key;

  public:
    //Constructor that creates a table with given name
    table(std::string table_name, int attribute_count)
    {
        name = table_name;
        record_count = 0;
        head = NULL;
        normal.resize(attribute_count);
        attribute_names.resize(attribute_count);
        index_in_domain.resize(attribute_count);
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

    //Function to retrive attribute count in the table
    int get_attribute_count()
    {
        return normal.size();
    }

    //Function to set main node head
    void set_main_node_head(main_node *new_head)
    {
        head = new_head;
    }
    //Function that adds an attribute to the table
    void add_attribute(int type, std::string name, int index)
    {
        int tab_index;
        domain *temp_domain;
        temp_domain = new domain(type);
        tab_index = temp_domain->insert_table_pointer(this);
        normal[index] = temp_domain;
        attribute_names[index] = name;
        index_in_domain[index] = tab_index;
    }

    //Function to link to already existing domain
    void add_foreign_domain(std::string name, domain *dom, unsigned int index)
    {
        unsigned int tab_index;

        tab_index = dom->insert_table_pointer(this);
        normal[index] = dom;
        attribute_names[index] = name;
        index_in_domain[index] = tab_index;
    }

    //Function to return the index in primary_key list
    int get_primary_attribute(unsigned int i)
    {
        if(i > primary_keys.size())
        {
            printf("Wrong Primary Key Attribute Index Requested!\nAborting!!");
            fflush(stdout);
            abort();
        }
        return primary_keys[i];
    }

    //Funtion to return the domain from primary key list
    domain *get_domain(unsigned int i)
    {
        return normal[primary_keys[i]];
    }

    //Function to find all the main_nodes with a certain attribute value
    std::vector< main_node * >  *get_records_with_val(unsigned int attribute_index, std::string &value)
    {
        return normal[attribute_index]->get_main_nodes(value, index_in_domain[attribute_index]);
    }

    //Function to add primary key to the table
    void add_primary_key(unsigned int attribute_index)
    {
        if (attribute_index >= normal.size())
        {
            printf("%d attribute doesn't exist !!\nFailed to add primary key !!\nAborting", attribute_index);
            fflush(stdout);
            abort();
        }

        primary_keys.push_back(attribute_index);
        return;
    }

    //Function to return a main node having the given primary key
    main_node* check_for_primary(std::vector< std::string > &values)
    {
        unsigned int i, j, col_index, node_count = INT_MAX, index = INT_MAX;
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
                if(j != index && values[j].compare(min_nodes[i]->get_attribute_value(col_index)) != 0)
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
        unsigned int i, j;

        for (i = 0; i < foreign_key.size(); i++)
        {
            temp_table = foreign_key[i].first;
            for (j = 0; j < foreign_key[i].second.size(); j++)
            {
                temp_values.push_back(values[foreign_key[i].second[j]]);
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
        unsigned int i;

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

        size_t node_size;
        for(i = 0; i < values.size(); i++)
        {
            node_size = sizeof(main_node *);
            normal[i]->add_get_new_value(values[i], new_main, index_in_domain[i], true, node_size);
            total_size += node_size;
        }

        new_main->set_next(head);
        head = new_main;
        total_size += head->get_size();
        record_count++;
        return 1;
    }

    //Function that compares a particular attribute value of a record and returns result
    bool compare_attribute(main_node *record, value_expression expr)
    {
        std::string attr_val = record->get_attribute_value(expr.attribute);
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

        unsigned int i;

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
        unsigned int j;
        attribute_node *old_node;
        size_t temp_size;
        for(it = nodes.begin(); it != nodes.end(); it++)
        {
            for(j = 0; j < index.size(); j++)
            {
                old_node = (*it)->get_attribute_list_index(index[j]);
                if(values[j].compare(old_node->get_value()))
                {
                    old_node->delete_edge((*it), index_in_domain[index[j]]);
                    attribute_node *new_attribute_node = normal[index[j]]->add_get_new_value(values[j],
                                                                    (*it), index_in_domain[index[j]], false, temp_size);
                    (*it)->update_attribute(index[j], new_attribute_node);
                }
            }
        }
    }


    void update_all(std::vector< int > &index, std::vector< std::string > &values)
    {
        unsigned int j;
        attribute_node *old_node;
        size_t temp_size;
        main_node *temp, *temp_main;

        while(head && head->lazy_delete())
        {
            temp = head;
            head = head->get_next();
            delete temp;
        }

        if(head == NULL) return;

        for(j = 0; j < index.size(); j++)
        {
            old_node = head->get_attribute_list_index(index[j]);
            if(values[j].compare(old_node->get_value()))
            {
                old_node->delete_edge(head, index_in_domain[index[j]]);
                attribute_node *new_attribute_node = normal[index[j]]->add_get_new_value(values[j],
                                                                head, index_in_domain[index[j]], false, temp_size);
                head->update_attribute(index[j], new_attribute_node);
            }
        }

        temp_main = head;
        temp = head->get_next();

        while(temp)
        {
            if(temp->lazy_delete())
            {
                temp_main->delete_next();
            }

            else
            {
                for(j = 0; j < index.size(); j++)
                {
                    old_node = temp->get_attribute_list_index(index[j]);
                    if(values[j].compare(old_node->get_value()))
                    {
                        old_node->delete_edge(temp, index_in_domain[index[j]]);
                        attribute_node *new_attribute_node = normal[index[j]]->add_get_new_value(values[j],
                                                                    temp, index_in_domain[index[j]], false, temp_size);
                        temp->update_attribute(index[j], new_attribute_node);
                    }
                }
                temp_main = temp;
            }
            temp = temp_main->get_next();
        }

   }

    void del(std::set< main_node * > &nodes)
    {
        std::set< main_node * >::iterator it;
        unsigned int i;
        std::vector< attribute_node * > *old_nodes;
        for(it = nodes.begin(); it != nodes.end(); it++)
        {
            old_nodes = (*it)->get_attribute_list();
            for(i = 0; i < old_nodes->size(); i++)
            {
                 (*old_nodes)[i]->delete_edge((*it), index_in_domain[i]);
            }

            (*it)->del_node();

        }

    }

    void delete_all()
    {
        unsigned int i;
        std::vector< attribute_node * > *old_nodes;
        main_node *temp;
        temp = head;
        while(head)
        {
            if(head->lazy_delete() == false)
            {
                old_nodes = head->get_attribute_list();
                for(i = 0; i < old_nodes->size(); i++)
                {
                    (*old_nodes)[i]->delete_edge(head, index_in_domain[i]);
                }
                head->del_node();
            }
            temp = head;
            head = head->get_next();
            delete temp;
        }

    }

    std::string get_attribute_name(unsigned int attribute_index)
    {
        if(attribute_index >= attribute_names.size())
        {
            printf("Attribute Index %d do not exist!!\nAborting!!", attribute_index);
            fflush(stdout);
            abort();
        }

        return attribute_names[attribute_index];
    }

    //Function to find table's name
    std::string get_table_name()
    {
        return name;
    }

    //Function get the domain connect at a corresponding index
    domain *get_normal_index(unsigned int normal_index)
    {
        if (normal_index >= normal.size())
        {
            printf("%d Normal Index do not exist !!\nAborting!!", normal_index);
            fflush(stdout);
            abort();
        }

        return normal[normal_index];
    }

    //Function to get head of main_node list for iteration
    main_node *get_main_node_head()
    {
        return head;
    }

    //Function to return the number of parent tables
    int get_foreign_key_count()
    {
        return foreign_key.size();
    }

    //Function to get parent table of a foreign_key
    table *get_parent_table(unsigned int foreign_key_index)
    {
        if(foreign_key_index > foreign_key.size())
        {
            printf("%d foreign_key do not exist !!\nAborting!!", foreign_key_index);
            fflush(stdout);
            abort();
        }

        return foreign_key[foreign_key_index].first;
    }

    // Function to check whether the table has a parent table named "parent_name" or not
    // Returns the foreign_key_index if it exists else -1
    int check_parent_name(std::string parent_name)
    {
        int i;
        std::transform(parent_name.begin(), parent_name.end(), parent_name.begin(), toupper);
        for (i = 0; i < foreign_key.size(); i++)
        {
            if (foreign_key[i].first->get_table_name().compare(parent_name) == 0)
            {
                printf("Parent = %s\n", foreign_key[i].first->get_table_name().c_str());
                return i;
            }
        }
        return -1;
    }

    // Function to check whether the table has a column named "col_name" or not
    // Returns the column number if it exists else -1
    int check_column_name(std::string column_name)
    {
        int i;
        std::transform(column_name.begin(), column_name.end(), column_name.begin(), toupper);
        for (i = 0; i < attribute_names.size(); i++)
        {
            if (attribute_names[i].compare(column_name) == 0)
            {
                printf("Column = %s\n", attribute_names[i].c_str());
                return i;
            }
        }
        return -1;
    }

    //Function to deallocate memory occupied by the table
    void clear()
    {
        unsigned int i, j;
        bool *check = new bool[attribute_names.size()];

        for(i = 0; i < attribute_names.size(); i++)
        {
            check[i] = false;
        }

        for(i = 0; i < foreign_key.size(); i++)
        {
            for(j = 0; j < foreign_key[i].second.size(); j++)
                check[foreign_key[i].second[j]] = true;
        }

        name.clear();
        for(i = 0; i < normal.size(); i++)
        {
            if(!check[i])
            {
                normal[i]->clear();
                delete normal[i];
            }
        }
        delete[] check;
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

        for (i = 0; i < foreign_key.size(); i++)
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

    //Function Definition in table
    std::vector< main_node * > select_via_and(std::vector< value_expression > &expression_list);
    std::set< main_node * > select_single_table(std::vector< std::vector< value_expression > > &expression_vec);
    std::vector< main_node *> get_records_as_child(int foreign_key_index, main_node *parent_record);
    std::vector< main_node *> get_all_records_as_child(int foreign_key_index, std::vector< value_expression > &expression_list);
    std::vector< main_node * > join_via_and(int foreign_key_index, std::vector< value_expression > &expression_list);
    std::set< main_node *> join(int foreign_key_index, std::vector< std::vector< value_expression > > &expression_vec);
    void select_all(std::vector< int > &col_list);
};

#endif // TABLE_INCLUDED
