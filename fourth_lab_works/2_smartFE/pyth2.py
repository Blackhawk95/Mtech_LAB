import shutil
import glob
import os
import os.path
dir1="/rhome/btech/206117019/vibtxt"
dir2="/rhome/btech/206117019/vibpng"
dir3='/rhome/btech/206117019/vibpdf'
dir='/rhome/btech/206117019/vib'
dir4='/rhome/btech/206117019/vibcsv'
for root,dirs,files in os.walk("/rhome/btech/206117019/vib"):
    for file in files:
        if file.endswith('.txt'):
          shutil.copy2(file,dir1)

for jpgfile in glob.iglob(os.path.join(dir, "*.png")):
    shutil.copy(jpgfile, dir2)


for fname in os.listdir(dir):
    if fname.lower().endswith('.pdf'):
        shutil.move(os.path.join(dir, fname), dir3)


for root, dirs, files in os.walk(dir):
    for f in files:
        if f.endswith('.csv'):
            shutil.copy(os.path.join(root,f), dir4)
