<?php
$xml = simplexml_load_file("blooddonors.xml") or die("Error: cannot create object");
$name = $_POST["name"];
$region = $_POST["region"];
$phone_no = $_POST["phone_no"];
$blood_group = $_POST["blood_group"];
$last_child=$xml->xpath("(//blood_donors/donor)[last()]");
//print_r($last_child);
//echo count($xml->children());
if(count($xml->children()) == 0){
	$last_child_donor_id = ["100000"];
}
else{
$last_child_donor_id=$last_child[0]->donor_id;
}
print_r($last_child_donor_id);
$donor_id=(int)$last_child_donor_id[0]+1;

//print_r($donor_id);
$temp = $xml->addChild("donor","");
$temp->addChild("donor_id",$donor_id);
$temp->addChild("name",$name);
$temp->addChild("region",$region);
$temp->addChild("phone_no",$phone_no);
$temp->addChild("blood_group",$blood_group);

$xml->asXML("blooddonors.xml");
echo "Donor details added <br>";
    echo "<table style = \"width:100%\"><tr><th>Donor ID</th><th>Name</th><th>Region</th><th>Phone_no</th><th>Blood Group</th></tr>";
	echo "<tr><th>" . $donor_id . "</th><th>" . $name . "</th><th>" . $region . "</th><th>" . $phone_no . "</th><th>" . $blood_group . "</th></tr>";

echo "<a href=\"index.html\"><button>Home</button></a>";

?>
