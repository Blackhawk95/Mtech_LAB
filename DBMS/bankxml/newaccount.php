<?php
//$acc_no = $_POST["acc_no"];
$xml = simplexml_load_file("customers.xml") or die("Error: cannot create object");
$name = $_POST["name"];
$branch_name = $_POST["branch_name"];
$balance = $_POST["balance"];
$last_child=$xml->xpath("(//customers/customer)[last()]");
if(count($xml->children()) == 0){
	$last_child_donor_id = ["100000"];
}
else
{
$last_child_acc_no=$last_child[0]->acc_no;
}
$acc_no=(int)$last_child_acc_no[0]+1;

print_r($acc_no);
$temp = $xml->addChild("customer","");
$temp->addChild("acc_no",$acc_no);
$temp->addChild("name",$name);
$temp->addChild("branch_name",$branch_name);
$temp->addChild("balance",$balance);

//$xml->customer->addChild("acc_no",$acc_no);
$xml->asXML("customers.xml");
echo "Account Created <br>";
    echo "<table style = \"width:100%\"><tr><th>Acc No</th><th>Name</th><th>Branch Name</th><th>Balance</th></tr>";
	echo "<tr><th>" . $acc_no . "</th><th>" . $name . "</th><th>" . $branch_name . "</th><th>" . $balance . "</th></tr>";

echo "<a href=\"index.html\"><button>Home</button></a>";

?>
