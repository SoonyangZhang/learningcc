import os
import argparse
def mkdir(path):
    folder = os.path.exists(path)
    if not folder:    
        os.makedirs(path)
def ReafByteInfo(fileName,left,right):
    bytes=0
    for index, line in enumerate(open(fileName,'r')):
        lineArr = line.strip().split()
        time=float(lineArr[0])
        if time>right:
            break
        if time>=left:
            bytes=bytes+int(lineArr[3])
    return bytes
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('--algo', type=str, default ='olia')
args = parser.parse_args()
algo=args.algo
data_dir="data_process"
fileName1="%s_"+algo+"_1_owd.txt"
fileName2="%s_"+algo+"_2_owd.txt"
fileOutName=algo+"_fair"
out_path=data_dir+"/"
left=0.0
right=300.0
mkdir(out_path)
instance=[1,2,3,4,5,6]
fileOut=fileOutName+".txt"
fout=open(out_path+fileOut,'w')
for case in range(len(instance)):
    f1=fileName1%(str(instance[case]))
    f2=fileName2%(str(instance[case]))
    bytes1=ReafByteInfo(f1,left,right)
    bytes2=ReafByteInfo(f2,left,right)
    J1=1.0*(bytes1+bytes2)*(bytes1+bytes2)/(2.0*(bytes1*bytes1+bytes2*bytes2))
    R1=1.0*bytes1/(bytes2) 
    fout.write(str(instance[case])+"\t"+str(J1)+"\t"+str(R1)+"\n")
fout.close()
