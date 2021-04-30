'''
the value of klatencyFactor in learning impact on competitiveness
'''
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
#python pro-tune-jain.py --folder reno
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('--folder', type=str, default ='reno')
args = parser.parse_args()
algo="learning"
data_folder_name=args.folder
program_name=data_folder_name
instance_table=[1,2,3,4,5,6]
tune_index_table=[0,1,2,3,4]
tune_value_table=["0.5","0.6","0.7","0.8","0.9"]
fileName1=data_folder_name+"/"+program_name+"%s_%s_"+algo+"_1_owd.txt"
fileName2=data_folder_name+"/"+program_name+"%s_%s_"+algo+"_2_owd.txt"
data_dir=data_folder_name+"_process"
out_path=data_dir+"/"
left=0.0
right=200.0
mkdir(out_path)
for it in range(len(instance_table)):
    instance=instance_table[it]
    fileout=program_name+"_"+algo+"_"+str(instance)+"_fair.txt"
    fout=open(out_path+fileout,'w')
    for tune_index in range(len(tune_index_table)):
        f1=fileName1%(str(tune_index_table[tune_index]),str(instance))
        f2=fileName2%(str(tune_index_table[tune_index]),str(instance))
        bytes1=ReafByteInfo(f1,left,right)
        bytes2=ReafByteInfo(f2,left,right)
        J1=0.0
        R1=0.0
        if bytes1>0 and bytes2>0:
            J1=1.0*(bytes1+bytes2)*(bytes1+bytes2)/(2.0*(bytes1*bytes1+bytes2*bytes2))
            R1=1.0*bytes1/(bytes2) 
        fout.write(tune_value_table[tune_index]+"\t"+str(J1)+"\t"+str(R1)+"\n")
    fout.close()
