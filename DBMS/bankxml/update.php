<?php
$acc_no=$_POST["acc_no"];
$amt=$_POST["amt"];
$transaction = $_POST["transaction"];

$xml = simplexml_load_file("customers.xml") or die("Error: cannot create object");

$flag = 0;

foreach($xml->children() as $child){
	if($child->acc_no[0] == $acc_no){
		if($transaction=="withdraw"){
			$child->balance[0]=(int)($child->balance[0]) - (int)$amt;
			}
		else if($transaction=="deposit"){
			$child->balance[0]=(int)($child->balance[0]) + (int)$amt;
			}

		$flag = 1;
	}

	if($flag == 1){
		break;
	}
}

if($flag == 0)
	{
		echo "acc not found";
	}


$xml->asXML("customers.xml");

$xml = simplexml_load_file("customers.xml") or die("Error: cannot create object");


foreach($xml->children() as $child){
	if($child->acc_no[0] == $acc_no){
		$name = $child->name[0];
		$branch_name = $child->branch_name[0];
		$balance = $child->balance[0];
		}
	}
echo "Account Found <br>";
echo "<table style = \"width:100%\"><tr><th>Acc No</th><th>Name</th><th>Branch Name</th><th>Balance</th></tr>";
echo "<tr><th>" . $acc_no . "</th><th>" . $name . "</th><th>" . $branch_name . "</th><th>" . $balance . "</th></tr>";


    echo"</table>";


echo "<a href=\"index.html\"><button>Home</button></a>";

?>
