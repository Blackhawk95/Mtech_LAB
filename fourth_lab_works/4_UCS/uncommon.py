file_name = "sample1.txt"
file_n = open(file_name, "r")
file_content = file_n.read()
data_1=[]
uncommon=[]
file_content=file_content.replace("."," ")
data_1=(file_content.strip().split(" "))
file_name = "sample2.txt"
file_n = open(file_name, "r")
file_content = file_n.read()
file_content=file_content.replace("."," ")
data_2=(file_content.strip().split(" "))


for i in uncommon:	
	print i
uniqWords=sorted(set(data_1+data_2))
Features_1=[]
print uniqWords
print "File 1 Data"
print data_1
print "\n File 2 Data" 
print data_2
print "\nFeatures :",uniqWords
for i in uniqWords:
	Features_1.append(data_1.count(i))
	
print "\nFile 1 Feature :" , Features_1
Features_2=[]
for i in uniqWords:
	Features_2.append(data_2.count(i))	
print "\nFile 2 Feature :",Features_2
