<?php
  include 'db_conn.php';
  
  if (!empty($_POST)) {
    $id=$_POST["ID"];
    $sql = "SELECT * FROM garagestatus WHERE ID = ?";
    
    $q = $conn->prepare($sql);
    $q->bind_param("i", $id);
    $q->execute();
    $data = $q->get_result()->fetch_assoc();
    
    echo $data['Stat'];
  }
?>