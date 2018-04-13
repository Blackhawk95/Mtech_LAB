<?php
$acc_no = $_POST["acc_no"];
$xml = simplexml_load_file("customers.xml") or die("Error: cannot create object");
//print_r($xml);
echo "\n";
$flag = 0;
foreach ($xml->children() as $child)
 {
  //echo "Child node: " . $child . "<br>";
  $customer = $child->acc_no[0];
  //echo $customer;
  if($customer == $acc_no)
	{
	echo "Account Found <br>";
    echo "<table style = \"width:100%\"><tr><th>Acc No</th><th>Name</th><th>Branch Name</th><th>Balance</th></tr>";
	echo "<tr><th>" . $customer . "</th><th>" . $child->name[0] . "</th><th>" . $child->branch_name[0] . "</th><th>" . $child->balance[0] . "</th></tr>";


    echo"</table>";
    $flag = 1;
	}
	if($flag == 1){
		break;
	}
}
if($flag == 0){
	echo "ACC NOT FOUND";
	}

	echo "<a href=\"index.html\"><button>Home</button></a>";

?>
