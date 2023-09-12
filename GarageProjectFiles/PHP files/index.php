 <!-- Responsible for the login page where we enter username and password -->

 <!DOCTYPE html>
<html>
<head>
    <title> LOGIN </title>
    <link rel = "stylesheet" type= "text/css" href = "style.css">
</head>
<body>
    <div data-role="header">
        <h1>Smart Garage Opener </h1>
    </div>
    <form action = "login.php" method = "post">
        <h2> Login to Open Garage </h2>
        <?php if (isset($_GET['error']))
        {?><p class = "error" > <?php echo $_GET['error']; 
        ?> </p> <?php } ?>
        <label> User Name </label>
        <input type = "text" name = "uname" placeholder = "User Name"><br>

        <label> Password </label>
        <input type = "password" name = "password" placeholder = "Password"><br>

        <button type = "sumbit ">Login</button>


    </form>
</body>
</html>