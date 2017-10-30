#!/usr/bin/env python27
import random
import re #regex
import smtplib
import time
 
#server = smtplib.SMTP('smtp.gmail.com', 587)
#server.ehlo()
#server.starttls()
#server.login("8bitblackhawk@gmail.com","8 bit is all we need")

#line=random.choice(open('myfile.txt').readlines())
#line1=line
#server.sendmail("8bitblackhawk@gmail.com","206117024@nitt.edu",line)

def Mail():
	line=random.choice(open('myfile.txt').readlines())	
	friendmails=["206117024@nitt.edu","206117010@nitt.edu","206117011@nitt.edu"]
	for i in range (0,len(friendmails)):	
		server.sendmail("8bitblackhawk@gmail.com", friendmails[i], line)

try:	
	server = smtplib.SMTP('smtp.gmail.com', 587)
	server.ehlo()
	server.starttls()
	server.login("8bitblackhawk@gmail.com", "8 bit is all we need")
except smtplib.SMTPException,error:
	print str(error)

while(1):	
	Mail()
	time.sleep(30)

server.quit()


