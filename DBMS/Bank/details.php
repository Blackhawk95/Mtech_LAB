<html>
<head>

</head>
<body>
	<h1>BANK</h1>
<h2>Account details</h1>
<?php
error_reporting(E_ALL & ~E_NOTICE);
$verbose = FALSE;
$conn=new mysqli("localhost","root","","ebanking");
if ($conn->connect_error){
	die("Connection Failed: " . $conn->connect_error );
}
if($verbose)
	echo "Connection Successful<br>";

if(!empty($_POST))
{	$acc_no=$_POST["acc_no"];
}
function getDetails($param){
global $conn;
$sql = "Select * from Customer where acc_no=$param";
$result = $conn->query($sql);

if($result->num_rows > 0){
	echo "<table class=\"details\" style=\"width:100%\"><tr><th>Acc_No</th><th>Name</th><th>Branch Name</th><th>Balance</th></tr>";
	while($row=$result->fetch_assoc()){
		$name=$row["name"];
		$branch_name=$row["branch_name"];
		$balance=$row["balance"];
		echo "<tr><th>$param</th><th>$name</th><th>$branch_name</th><th>$balance</th></tr>";
		}
	echo "</table>";
	}
	else
	{
		echo "ACC_NO not found $param <br>";
	}
}

if(!empty($_POST) and $acc_no)
{	getDetails($acc_no);
}
?>
<a href="index.html"><button type="button">Home</button></a>
</body>
</html>
