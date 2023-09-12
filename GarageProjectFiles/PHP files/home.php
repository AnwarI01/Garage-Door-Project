<?php
session_start();
$Write = "<?php $" . "getGarageStatus=''; " . "echo $" . "getGarageStatus;" . " ?>";
file_put_contents('garageStatusContainer.php', $Write);
//Home page when we login to our account
#include 'garageStatusContainer.php';

if (isset($_SESSION['ID'] ) && isset($_SESSION['user_name']))
{
    //$curGarageStatus = $_GET['getGarageStatus']
    ?>
    <!DOCTYPE html>
    <html>
    <head>
        <script src="jquery.min.js"></script>
        
        <script>
            $(document).ready(function(){
                $("#getGarageStats").load("garageStatusContainer.php");
                setInterval(function() {
                    $("#getGarageStats").load("garageStatusContainer.php");
                }, 500);
            });
        </script>
        <title> HOME </title>
        <link rel = "stylesheet" type = "text/css" href = "style.css">
    </head>
<body>
    <header>
    <h1> WELCOME, <?php echo $_SESSION['user_name']; ?></h1>
    <!-- $ is know as variable in php -->
    <form action="updateStatus.php" method ="post"> 
        <label> The Status of the Garage: </label>
        <h2 id = "statusOfGarage" style = "color:#FDFEFE;">The Garage is:</h2>
        <p id = "getGarageStats" hidden></p>

        <label> Garage Opener </label> 

        <!-- <button class = "button1" name = "Stat" type = "submit" value = "1"></button> -->
        
        <button class = "button3" name = "Stat" type = "submit" value = "1">Open or Close Garage</button>

        <!--<button class = "button2" name = "Stat" type = "submit" value = "0" ></button>   -->

	<a href = "logout.php"> LOGOUT</a>
   
    </form>


    
    <!-- Function in which status on website is being update based on data from nodemcu -->
    <!-- Will also update button value in database -->
    <script>
        var myVar = setInterval(myTimer, 500);
        function myTimer() {
            var getGarageStat = document.getElementById("getGarageStats").innerHTML;            

            if(getGarageStat == 1) {
                document.getElementById("statusOfGarage").innerHTML = "The Garage is: Open";
            }
            if(getGarageStat == 0) {
                document.getElementById("statusOfGarage").innerHTML = "The Garage is: Closed";
            }
            if(getGarageStat == "") {
                document.getElementById("statusOfGarage").innerHTML = "The Garage is: Waiting for status from nodeMCU";
            }
        }
    </script>
</body>
</html>
<?php
}
else
{
    header ("Location: index.php");
    exit();
}
?>
<!-- Complete code -->