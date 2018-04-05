<?php
$servername="127.0.0.1";
$username="root";
$password="!Mysql95";
$dbname="bank";
$conn=new mysqli($servername,$username,$password,$dbname);
if($conn->connect_error){
	die("Connection failed: ".$conn->connect_error);
}
$acc_no=$_POST["acc_no"];
$sql = "SELECT * FROM Customer WHERE acc_no=$acc_no";
$result = $conn->query($sql);
if($result->num_rows > 0){
	echo "Record Found ";

	while($row = $result->fetch_assoc()){
		$balance=$row['acc_no'] ;
		$name=$row['name'];
		$branchname=$row['branch_name']; 
	}
	echo "</table>";	
}
else{
	echo "Error:" .$sql. "<br>" .$conn->error;
}
$conn->close();

?>
<html>
<br> <table style="width:100%">
<tr>
    	<th>$acc_no?</th>
    	<th>$name</th>
    	<th>$balance</th>
	<th>$branch_name</th>
</tr>
<tr>
    	<th><?php echo $acc_no?></th>
    	<th><?php echo $name?></th>
    	<th><?php echo $balance?></th>
	<th><?php echo $branch_name?></th>
</tr>
</html>
