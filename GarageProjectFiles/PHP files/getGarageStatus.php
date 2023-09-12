<?php
    $getGarageStatus = $_POST["getGarageStatus"];
    $Write = "<?php $" . "getGarageStatus='" . $getGarageStatus . "'; " . "echo $" . "getGarageStatus;" . " ?>";
    echo $getGarageStatus;
    file_put_contents('garageStatusContainer.php', $Write);
?>