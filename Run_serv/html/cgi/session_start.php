<?php
// Start the session
session_start();
?>
<!DOCTYPE html>
<html>
<body>
<h1><center><a href="/">Webserv Home</a> </center></h1>
<?php
$_SESSION["Session"] = "Webserv Session";
echo "Session variable is set.";
?>

</body>
</html>
