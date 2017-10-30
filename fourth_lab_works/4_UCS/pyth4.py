
def intersection(a, b):
    return list(set(a) & set(b))
 
def main():

	wordstring1 = 'John is a programmer '
	wordstring2 = 'John likes coding He also likes python'

	#wordstring = 'John is a programmer '
	#wordstring += 'John likes coding He also likes python'
	
	wordstring = wordstring1 + wordstring2	

	wordlist = wordstring.split()

	wordfreq = []
	for w in wordlist:
		wordfreq.append(wordlist.count(w))

	print("String\n" + wordstring +"\n")
	print("List\n" + str(wordlist) + "\n")
	print("Frequencies\n" + str(wordfreq) + "\n")
	print("Pairs\n" + str(zip(wordlist, wordfreq)))

	wordlist = set(wordlist)

	seen = set()
	result = []
	for item in wordlist:
	    if item not in seen:
	        seen.add(item)
	        result.append(item)
	print("\n features:")
	print(result)


	wordlist1=wordstring1.split()
	wordlist2=wordstring2.split()

	print(intersection(wordlist,wordlist1))
	print(intersection(wordlist,wordlist2))

	new_list=[]
	for ele in wordlist:
		if ele in wordlist2:
			new_list.append(0)
		else:
			new_list.append(1)

	print(new_list)

	new_list1=[]
	for ele in wordlist:
		if ele in wordlist2:
			new_list1.append(1)
		else:
			new_list1.append(0)

	print(new_list1)




main()
