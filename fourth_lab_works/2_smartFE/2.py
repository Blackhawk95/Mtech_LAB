import fnmatch
import shutil
import os

srcdir='./test'#shoudl have files
destdir='./target'#should exist

extType = ['pdf' , 'txt' , 'mp3' , 'jpeg' , 'csv' ]

def createDir():
    for i in extType:
        if not(os.path.exists(destdir+'/'+ i + '/')):
            os.mkdir(destdir+'/'+ i+'/')


def searchMove():
    #for file in os.listdir('.'):
    for root, dirs, files in os.walk(srcdir):
        for dir in dirs:
            for file in os.listdir(root+'/'+dir):#use files instead of os.listdir('')
                for i in extType:
                    if fnmatch.fnmatch(file,'*.'+ i):
                        if ((root+'/'+dir+'/') <> (destdir +'/'+ i +'/')): #to prevent self copying
                            print file
                            shutil.copy2(root+'/'+dir+'/'+file, destdir + '/'+i+'/') #remove the dot and add destination address
    
    for file in os.listdir(srcdir):    
        for i in extType:#for root directory thingi
            if fnmatch.fnmatch(file,'*.'+ i):
                if ((srcdir+'/') <> (destdir +'/'+ i +'/')): #to prevent self copying
                    print file
                    shutil.copy2(srcdir+'/'+file, destdir + '/'+i+'/') #remove the dot and add destination address       
                    #shutil.copy2(root+'/'+dir+'/'+file, destdir + '/csv/')# use os.path.join(a,b) instead of this

createDir()
searchMove()            
