'''
notes: count the latency of learning CC
the value of klatencyFactor impact on delay
'''
#change lineArr[1] the index accoring to the data trace format
import os
import argparse
def mkdir(path):
    folder = os.path.exists(path)
    if not folder:    
        os.makedirs(path)
def ReadDelayData(fileName):
    sum_owd=0.0;
    sum_lines=0;    
    with open(fileName) as txtData:
        for line in txtData.readlines():
            lineArr = line.strip().split()
            sum_owd+=float(lineArr[2])
            sum_lines+=1;
    return  sum_owd,sum_lines
#python pro-tune-owd.py --folder learning  
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('--folder', type=str, default ='learning')
args = parser.parse_args()
algo="learning"
data_folder_name=args.folder
program_name=data_folder_name
instance_table=[1,2,3,4,5,6]
tune_index_table=[0,1,2,3,4]
tune_value_table=["0.5","0.6","0.7","0.8","0.9"]
flows=2;
data_dir=data_folder_name+"_process"
out_path=data_dir+"/"
name=data_folder_name+"/"+program_name+"%s_%s_%s_%s_owd.txt"
mkdir(out_path)
for it in range(len(instance_table)):
    instance=instance_table[it]
    fileout=program_name+"_%s_%s_owd.txt"%(algo,str(instance))
    fout=open(out_path+fileout,'w')
    for tune_index in range(len(tune_index_table)):
        total_owd=0.0
        total_lines=0
        average_owd=0.0
        exist=False
        for i in range(flows):
            sum_delay=0.0
            sum_lines=0
            average_owd=0.0
            filename=name%(str(tune_index_table[tune_index]),str(instance),algo,str(i+1))
            if os.path.exists(filename):
                sum_delay,sum_lines=ReadDelayData(filename)
                total_owd+=sum_delay
                total_lines+=sum_lines
                exist=True
        if exist:
            average_owd=total_owd/total_lines
            fout.write(tune_value_table[tune_index]+"\t")
            fout.write(str(average_owd)+"\n")
        else:
            fout.write(str(instance[case])+"\n")
    fout.close()
