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
#python pro-tune-utility.py --folder learning
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('--folder', type=str, default ='learning')
args = parser.parse_args()
algo="learning"
data_folder_name=args.folder
program_name=data_folder_name
instance_table=[1,2,3,4,5,6]
tune_index_table=[0,1,2,3,4]
tune_value_table=["0.5","0.6","0.7","0.8","0.9"]
caps=[5000000,5000000,6000000,6000000,8000000,8000000]
duration=200;
flows=4
data_dir=data_folder_name+"_process"
out_path=data_dir+"/"
mkdir(out_path)
name=data_folder_name+"/"+program_name+"%s_%s_%s_%s_owd.txt"
for it in range(len(instance_table)):
    instance=instance_table[it]
    fileout=program_name+"_%s_%s_util.txt"%(algo,str(instance))
    fout=open(out_path+fileout,'w')
    for tune_index in range(len(tune_index_table)):
        bytes=0
        for i in range(flows):
            filename=name%(str(tune_index_table[tune_index]),str(instance),algo,str(i+1))
            bytes+=ReafByteInfo(filename)
        total=caps[it]*duration/8
        util=float(bytes)/float(total)
        fout.write(tune_value_table[tune_index]+"\t")
        fout.write(str(util)+"\n")
    fout.close()
