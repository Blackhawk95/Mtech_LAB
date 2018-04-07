<?php
$servername = "localhost";
$username = "root";
$password="";//null password
$dbname = "ebanking";
$verbose = FALSE;
// Create connection
$conn = new mysqli($servername, $username, $password,$dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
if($verbose){
echo "Connected successfully <br>";
}
//data from update.html
$acc_no=$_POST["acc_no"];
$amt=$_POST["amt"];
$transaction=$_POST["transaction"];

//depending on type of transaction
if($transaction=="withdraw")
{	$sql= "UPDATE Customer set balance = balance - $amt where acc_no=$acc_no";
}
else if($transaction=="deposit")
{	$sql= "UPDATE Customer set balance = balance + $amt where acc_no=$acc_no";
}

if($conn->query($sql)==TRUE){
	if($verbose){
	echo "Update successfully<br>";
	}
}
else{
	echo "Error: " . $sql . "<br>" . $conn->error;
	}

//calling the statement function
require_once 'details.php';

$conn->close();
?>
