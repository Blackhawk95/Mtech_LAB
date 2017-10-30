import random

def read():
	with open("myfile.txt") as f:
    		words = [word for line in f for word in line.split()]	
	print words
	freqTuple=[]
	for word in words:
		freqTuple.append((words.count(word),word))
	print freqTuple
	
	goodFreqTuple = sorted(set(freqTuple))
	print goodFreqTuple
	print '\n'
	print "UNCOMMON FEATURES\n"
	for ele in goodFreqTuple:
		if (ele[0] == 1):
			print ele[1]
		

read()
	

