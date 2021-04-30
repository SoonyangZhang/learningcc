import os
import argparse
def mkdir(path):
    folder = os.path.exists(path)
    if not folder:    
        os.makedirs(path)
def ReafByteInfo(fileName,left,right):
    bytes=0
    if os.path.exists(fileName):
        for index, line in enumerate(open(fileName,'r')):
            lineArr = line.strip().split()
            time=float(lineArr[0])
            if time>right:
                break
            if time>=left:
                bytes=bytes+int(lineArr[3])
    return bytes
#python pro-jain.py --cc learning --cc1 cubic
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('--cc', type=str, default ='learning')
parser.add_argument('--cc1', type=str, default ='cubic')
args = parser.parse_args()
cc=args.cc
cc1=args.cc1
data_in_dir=""
if cc==cc1:
    data_in_dir="pure"+cc
else:
    data_in_dir=cc+"_vs_"+cc1
data_out_dir=data_in_dir+"_process"
fileName1=data_in_dir+"/"+"%s_"+cc+"_1_owd.txt"
fileName2=data_in_dir+"/"+"%s_"+cc+"_2_owd.txt"
fileOutName=data_in_dir+"_fair"
left=0.0
right=200.0
instance=[1,2,3,4,5,6]
fileOut=fileOutName+".txt"
fout=open(fileOut,'w')
for case in range(len(instance)):
    f1=fileName1%(str(instance[case]))
    f2=fileName2%(str(instance[case]))
    bytes1=ReafByteInfo(f1,left,right)
    bytes2=ReafByteInfo(f2,left,right)
    J1=0.0
    R1=0.0
    if bytes1>0 and bytes2>0:
        J1=1.0*(bytes1+bytes2)*(bytes1+bytes2)/(2.0*(bytes1*bytes1+bytes2*bytes2))
        R1=1.0*bytes1/(bytes2) 
    fout.write(str(instance[case])+"\t"+str(J1)+"\t"+str(R1)+"\n")
fout.close()
