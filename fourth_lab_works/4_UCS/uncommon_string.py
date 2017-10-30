def unc(data1,data2):
	unc = []
	for word in data_1:
		unc.append(word)

	for word in data_2:
		if word not in unc:
			unc.append(word)
	return uncommon



file_name = "sample1.txt"
file_n = open(file_name, "r")
file_content = file_n.read()
data_1=[]
uncommon=[]

file_content=file_content.replace("."," ")
data_1=(file_content.strip().split(" "))

file_name = "sample2.txt"
file_n = open(file_name, "r")
file_content = file_n.readlines()


data_2=[]
for f in file_content:
	data_2=f.strip().split(" ")


for word in data_1:
	if word not in data_2:
		uncommon.append(word)

for word in data_2:
	if word not in data_1:
		uncommon.append(word)

for i in uncommon:
	print i

uniqWords=sorted(set(uncommon))
min_count=10000
for word in uniqWords:
	if (uncommon.count(word)<min_count):
		min_count = uncommon.count(word)

nooffeature=input('Enter number of features : ')

print "Least Uncommon Words\n-------------------------"

print '[',
for word in uniqWords:
	if min_count == uncommon.count(word):
		if nooffeature > 0:
			#print 	"Frequency->", uncommon.count(word) ,"Word->" ,word	
			print word,',',	
			nooffeature = nooffeature-1
print ']'

data_1_count = []
data_2_count = []

for word in (unc(data_1,data_2)):
	print word
	if word in data_2:
		data_1_count.append(unc(data_1,data_2).count(word))
	else:
		data_1_count.append(0)

for i in data_1_count:
	print i, 
		
