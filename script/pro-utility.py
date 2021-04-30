#!/usr/bin/env python
import os
import argparse
def mkdir(path):
    folder = os.path.exists(path)
    if not folder:    
        os.makedirs(path)
def ReafByteInfo(fileName):
    bytes=0
    if os.path.exists(fileName):
        for index, line in enumerate(open(fileName,'r')):
            lineArr = line.strip().split()
            bytes=bytes+int(lineArr[3])
    return bytes
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('--algo', type=str, default ='learning')
args = parser.parse_args()
algo=args.algo
name="%s_util.txt"
instance_table=[1,2,3,4,5,6]
flows=4
data_in_dir="pure"+algo
fileout=name%algo
caps=[5000000,5000000,6000000,6000000,8000000,8000000]
duration=200;
fout=open(fileout,'w')
filein_wild=data_in_dir+"/"+"%s_"+algo+"_%s_owd.txt"
for it in range(len(instance_table)):
    instance=instance_table[it]
    bytes=0
    total=caps[it]*duration/8
    for i in range(flows):
        name=filein_wild%(str(case),str(i+1))
        bytes=ReafByteInfo(name)
    util=float(bytes)/float(total)
    fout.write(str(instance)+"\t")
    fout.write(str(util)+"\n")
fout.close()
