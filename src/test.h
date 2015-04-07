void create_dept(database *db)
{
    table *temp_table;

    temp_table = new table(std::string("DEPARTMENT"), 3);
    temp_table->add_attribute(1, "ID", 0);
    temp_table->add_attribute(2, "NAME", 1);
    temp_table->add_attribute(2, "LOCATION", 2);
    temp_table->add_primary_key(0);
    db->add_table(temp_table);
}

void create_emp(database *db)
{
    table *temp_table, *foreign_table;
    std::vector< int > fk_list;
    temp_table = new table(std::string("EMPLOYEE"), 6);
    temp_table->add_attribute(1, "ID", 0);
    temp_table->add_attribute(2, "NAME", 1);
    temp_table->add_attribute(2, "ADDRESS", 2);
    temp_table->add_attribute(3, "SALARY", 3);
    temp_table->add_attribute(2, "SEX", 4);
    temp_table->add_primary_key(0);

    foreign_table =  db->get_tables_index(0);
    temp_table->add_foreign_domain("DEPT_ID", foreign_table->get_domain(0), 5);
    fk_list.push_back(5);
    temp_table->add_foreign_key(foreign_table, fk_list);
    db->add_table(temp_table);
}

void insert_dept(database *db)
{
    srand(time(NULL));
    int i, j, success, num_records, unique_rec_count, attribute_len, failed = 0, start, stop;
    double result_time = 0;
    num_records = 100;
    unique_rec_count = 40;
    attribute_len = 30;

    table *temp_table;
    char *str = new char[num_records + 1];
    char *s = new char[attribute_len + 1];

    temp_table = db->get_tables_index(0);
    std::vector< std::string > values(temp_table->get_attribute_count());
    std::vector< std::string > unique_rec(unique_rec_count);

    for(i = 0; i < unique_rec_count; i++)
    {
        for(j = 0; j < attribute_len; j++)
        {
            s[j] = 'a' + (rand() % 26);
        }
        s[j] = '\0';

        unique_rec[i].assign(s);
    }

    for(i = 1; i <= num_records; i++)
    {
        sprintf(str, "%d", i);
        values[0].assign(str);
        values[1] = unique_rec[rand() % unique_rec_count];
        values[2] = unique_rec[rand() % unique_rec_count];

        start = clock();
            success = temp_table->add_new_record(values);
        stop = clock();
        result_time += stop - start;

        if(success != 1)
        {
            failed++;
        }
    }

    if(failed)
    {
        printf("Failed : %d\n", failed);
    }

    printf("Real Time for dept :%lf\n", result_time/double(CLOCKS_PER_SEC));
    delete[] str;
    delete[] s;
    values.clear();
    unique_rec.clear();
}

void insert_emp(database *db)
{
   // FILE *fp = fopen("Result_90000.txt", "w");
    srand(time(NULL));
    int i, j, success, num_records, unique_rec_count, attribute_len, max_attribute_len_name, max_attribute_len_address, failed = 0, start, stop, max_sal;
    long unsigned rel_size = 0;
    float sal;
    double result_time = 0;
    max_sal = 100000;
    num_records = 90000;
    unique_rec_count = 54000;
    max_attribute_len_name = 30;
    max_attribute_len_address = 50;

    table *temp_table;
    char *str = new char[num_records + 1];
    char s[100];

    temp_table = db->get_tables_index(1);
    std::vector< std::string > values(temp_table->get_attribute_count());
    std::vector< std::string > unique_rec_name(unique_rec_count);
    std::vector< std::string > unique_rec_address(unique_rec_count);

    for(i = 0; i < unique_rec_count; i++)
    {
        attribute_len = rand() % max_attribute_len_name;
        if(attribute_len < 8)
            attribute_len = 9;
        for(j = 0; j < attribute_len; j++)
        {
            s[j] = 'a' + (rand() % 26);
        }
        s[j] = '\0';

        unique_rec_name[i].assign(s);

        attribute_len = rand() % max_attribute_len_address;
        if(attribute_len < 8)
            attribute_len = 9;
        for(j = 0; j < attribute_len; j++)
        {
            s[j] = 'a' + (rand() % 26);
        }
        s[j] = '\0';

        unique_rec_address[i].assign(s);
    }

    for(i = 1; i <= num_records; i++)
    {
        sprintf(str, "%d", i);
        values[0].assign(str);
        values[1] = unique_rec_name[rand() % unique_rec_count];
        values[2] = unique_rec_address[rand() % unique_rec_count];
        sal = (rand()/(float)RAND_MAX) * max_sal;
        sprintf(s, "%f", sal);
        values[3].assign(s);
        if(rand() % 2)
        {
            values[4].assign("Male");
        }
        else
        {
            values[4].assign("Female");
        }
        j = (rand() % 100) + 1;
        sprintf(s, "%d", j);
        values[5].assign(s);

        rel_size += 10; //ID
        rel_size += 30; //NAME
        rel_size += 50; //ADDRESS
        rel_size += 16; //SALARY
        rel_size += 6;  //SEX
        rel_size += 10; //DEPT_ID
        start = clock();
            success = temp_table->add_new_record(values);
        stop = clock();
        result_time += stop - start;

        if(success != 1)
        {
            failed++;
        }
      //  if((i % 900) == 1)
        //    fprintf(fp, "%u %u\n", temp_table->get_size(), rel_size);
    }

    if(failed)
    {
        printf("Failed : %d\n", failed);
    }

    printf("Real Time for employee :%lf\n", result_time/double(CLOCKS_PER_SEC));
    delete[] str;
    values.clear();
    unique_rec_name.clear();
    unique_rec_address.clear();
    //fclose(fp);
}
