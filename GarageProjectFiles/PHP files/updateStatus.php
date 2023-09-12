<?php
    require 'db_conn.php';

    if(!empty($_POST)){
        $Stat = $_POST['Stat'];

        $sql = "UPDATE garagestatus SET Stat = ? Where ID = 0";
        $q = $conn->prepare($sql);
        $q->bind_param("s", $Stat);
        $q->execute();
        header("Location: home.php");
        //insert data
    }
?>