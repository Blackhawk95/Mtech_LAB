#!/usr/bin/python3
# importing csv module
import csv
import sys
from sys import argv
import matplotlib.pyplot as plt
import numpy as np

 
# csv file name

def getfilename(argv):
	return argv[1]
 
filename = getfilename(argv) 
# initializing the titles and rows list
fields = []
rows = []
 
# reading csv file
with open(filename, 'r') as csvfile:
    # creating a csv reader object
    csvreader = csv.reader(csvfile)
     
    # extracting each data row one by one
    for row in csvreader:
        rows.append(row)
 
    # get total number of rows
    print("Total no. of rows: %d"%(csvreader.line_num))
 
 
nodes = []
pdr = []
plr = []
jitter = [] 
 
for row in rows:
    # parsing each column of a row
    #for col in row:
    nodes.append(int(row[0]))
    pdr.append(float(row[1]))
    plr.append(float(row[2]))
    jitter.append(float(row[3]))
    #print('%10s'%row),
    #print('\n')
    
#print(nodes)
#print(pdr)    
    
#t = np.arange(0,10).reshape(10,1)    

plt.figure(1)
plt.subplot(211)
plt.plot(nodes,pdr,'g',nodes,plr,'r')

plt.subplot(212)
plt.plot(nodes,jitter)
#plt.axis([0, 10, 0, 1])
plt.show()
