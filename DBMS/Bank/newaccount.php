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
if($verbose)
	echo "Connected successfully <br>";

//data from newaccount.html
$name=$_POST["name"];
$branch_name=$_POST["branch_name"];
$balance=$_POST["balance"];

$sql= "INSERT INTO Customer(name,branch_name,balance) values('$name','$branch_name',$balance)";

if($conn->query($sql)==TRUE){
	if($verbose)
		echo "New record created successfully <br>";
	$account_no = $conn->insert_id; //requires autoincrement
	$ac_no = ((int) $account_no);
	require_once 'details.php';
	getDetails($ac_no);
	}
else{
	echo "Error: " . $sql . "<br>" . $conn->error;
	}

$conn->close();

//echo "<a href=\"index.html\">Home</a>";
?>
