<?php
$servername="127.0.0.1";
$username="root";
$password="!Mysql95";
$dbname="bank";
$conn=new mysqli($servername,$username,$password,$dbname);
if($conn->connect_error){
	die("Connection failed: ".$conn->connect_error);
}
//$acc_no=$_POST["acc_no"];
$name=$_POST["name"];
$branchname=$_POST["branch_name"];
$balance=$_POST["balance"];
$sql = "INSERT INTO Customer (name,branch_name,balance) VALUES ('$name','$branchname',$balance)";
if($conn->query($sql)===TRUE){
	echo"created recorded";
}
else{
	echo "Error:" .$sql. "<br>" .$conn->error;
}
$conn->close();

?>
