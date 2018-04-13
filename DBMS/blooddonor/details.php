<?php
$donor_id = $_POST["donor_id"];
$xml = simplexml_load_file("blooddonors.xml") or die("Error: cannot create object");
//print_r($xml);
echo "\n";
$flag = 0;
foreach ($xml->children() as $child)
 { //print_r($child);
  $donor = $child->donor_id[0];
  if($donor[0] == $donor_id)
	{//DONE TILL HERE
	echo "Donor details found <br>";
    echo "<table style = \"width:100%\"><tr><th>Donor ID</th><th>Name</th><th>Region</th><th>Phone_no</th><th>Blood Group</th></tr>";
	echo "<tr><th>" . $donor . "</th><th>" . $child->name[0] . "</th><th>" . $child->region[0] . "</th><th>" . $child->phone_no[0] . "</th><th>" . $child->blood_group[0] . "</th></tr>";

    echo"</table>";
    $flag = 1;
	}
	if($flag == 1){
		break;
	}
}
if($flag == 0){
	echo "DONOR NOT FOUND";
	}

	echo "<a href=\"index.html\"><button>Home</button></a>";

?>
