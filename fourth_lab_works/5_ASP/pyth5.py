 
def printthis(inp):
	print "a",inp+1,',',

def printMaxActivities(s , f ):
    n = len(f)
    print "The following activities are selected"
 
    # The first activity is always selected
    i = 0
    printthis(i)
 
    # Consider rest of the activities
    for j in range(0,n):
 
        # If this activity has start time greater than
        # or equal to the finish time of previously
        # selected activity, then select it
        if s[j] >= f[i]:
            printthis(j),
            i = j
	    j = 0
 
# Driver program to test above function
s = [1,3,0,5,3,5,6,8,8,2,12]
f = [4,5,6,7,8,9,10,11,12,13,14]

sk = raw_input("Enter elements followed by space for s")
sl = sk.strip().split(',')
fk = raw_input("Enter elements followed by space for f")
fl = fk.strip().split(',')
print sl
print fl

printMaxActivities(s , f)
