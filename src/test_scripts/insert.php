<?php
/* This file generates some random graph with repetitive data and
 * inserts it into RDBMS. It deletes any previous data in the database.
 * It also outputs all the data so that it can be piped into the new
 * Graph Like Database.
 * 
 * Objective : To compare storage requirement of new graph model with the
 * traditional RDBMS space requirements.
 */

$conn = mysql_connect("localhost:3308", "root", "");
if (!$conn)
{
    echo "Could Not Connect to Database";
    return;
}
$db_name = "graph";
mysql_select_db($db_name);

//Setting the table parameters
$table_name = "Employee";
$attributes = array('id', 'name', 'address', 'branch');
$attr_size = 80;
$max_records = 1000; //Minimum 10
$insert_graph = 1;
$insert_sql = 0;
$num_points = 10;
$graph_file = fopen($_SERVER['DOCUMENT_ROOT']."/graph_input.txt", "w") or die("Could Not create File");
$mysql_file = fopen($_SERVER['DOCUMENT_ROOT']."/mysql_size.txt", "w") or die("Could Not Create File");

//Creating Table in Graph Database
if($insert_graph)
{
    fwrite($graph_file, "test\n");
    fwrite($graph_file, "1 $table_name ".count($attributes));
    foreach ($attributes as $value)
    {
        fwrite($graph_file, " $value 2 $attr_size");
    }
    fwrite($graph_file, " 0\n"); //Primary key Span
}

//Creating Table in MySQL
if($insert_sql)
{
    $query = "CREATE TABLE $table_name( ";
    for($i = 0; $i < count($attributes) - 1; $i++)
    {
        $query .= "$attributes[$i] VARCHAR($attr_size) NOT NULL,";
    }
    $query .= $attributes[count($attributes) - 1]." VARCHAR($attr_size) NOT NULL)";
    mysql_query($query);
}

//Creating Random Data to be inserted in the database
$mydata = new SplFixedArray($max_records);
for($i = 0;$i < 10; $i++)
{
    $mydata[$i] = new SplFixedArray(count($attributes));
    for($j = 0 ; $j < count($attributes); $j++)
    {
        $mydata[$i][$j] = "";
        for($k = 0 ; $k < $attr_size; $k++)
        {
            $mydata[$i][$j].= chr( ord('a') + rand(0, 25));
        }
    }
}

//Inserting Data into Graph Database
if($insert_graph)
{
    for($i = 0; $i < $max_records; $i++)
    {
        if($i < 10)
        {
            $ind = $i;
        }
        else
        {
            $ind = rand(0, 9);
        }
        fwrite($graph_file, "2 1 ");
        for($j = 1; $j <= count($attributes); $j++)
        {
            fwrite($graph_file, "$j ".$mydata[$ind][$j - 1]." ");
        }
        fwrite($graph_file, " 0\n");
        
        if($i == $max_records - 1)
        {
            fwrite($graph_file, "0\n");
        }
    }
}

//Inserting Data into MySQL
if($insert_sql)
{
    for($j = 0; $j < $max_records; $j++)
    {
        if($i < 10)
        {
            $ind = $i;
        }
        else
        {
            $ind = rand(0, 9);
        }
        $query = "INSERT INTO $table_name (";
        for($i = 0; $i < count($attributes) - 1; $i++)
        {
            $query .= $attributes[$i].", ";
        }
        $query .= $attributes[count($attributes) - 1].") VALUES(";
        for($i = 0; $i < count($attributes) - 1; $i++)
        {
            $query .= "'".$mydata[$ind][$i]."', ";
        }
        $query .= "'".$mydata[$ind][count($attributes) - 1]."')";
        mysql_query($query);

        if ((($j+1) % ($max_records / $num_points) ) == 0)
        {
            $query = "SELECT SUM(data_length + index_length) FROM information_schema.TABLES WHERE table_schema = '$db_name'";
            $output = mysql_query($query);
            $res = mysql_fetch_array($output);
            fwrite($mysql_file, ($j + 1)."\t\t".$res[0]."\n");   
        }
    }
}

//Closing File handles
fclose($mysql_file);
fclose($graph_file);