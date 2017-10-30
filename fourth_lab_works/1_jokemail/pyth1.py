import random
import smtplib
lines = open('a.txt').read().splitlines()
myline =random.choice(lines)
print(myline)



 
server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login("vibhadeshmukh09@gmail.com", "password")
 
msg = "YOUR MESSAGE!"
server.sendmail("vibhadeshmukh09@gmail.com", "vibhadeshmukh09a@gmail.com", myLine)
server.quit()
