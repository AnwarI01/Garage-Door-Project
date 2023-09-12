<?php
//Responible to connect our web page with a data base 

//To connect to database on web server
$sname = "localhost";//name for the server
$uname = "id20648182_root";//name full data basee
$password = "Ikl^roAnKR|f3ouV";
$db_name = "id20648182_garage_db";

//$db_name = "test_db";
//$db_name = "garage_db";

$conn = mysqli_connect($sname, $uname, $password,$db_name);

if(!$conn)
{
    echo "connection Failed";
}