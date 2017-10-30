#begining
BEGIN{
	FS=","
}
#loop part
{
	print $1 "," $4
	sum +=$4
	n++
}
#ending part
END{
	avg = sum / n;
	print "\nAverage: " avg
}
