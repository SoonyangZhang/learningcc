#!/usr/bin/env python
import time
import os
import subprocess ,signal
import argparse
def tune_param(ns3_exe_path,cc,cc1,param_str,test_case,param_index_table):
    program_name=cc1
    exe_cmd=""
    if param_str=="beta":
        exe_cmd=ns3_exe_path+"learn-cc  --it=%s --cc=%s --bench=%s --name=%s  --trace=beta/%s --tune=beta --tpi=%s"
    elif param_str=="theta":
        exe_cmd=ns3_exe_path+"learn-cc  --it=%s --cc=%s --bench=%s --name=%s  --trace=theta/%s --tune=theta --tpi=%s"
    else:
        return 
    for case in range(len(test_case)):
        for param_index in range(len(param_index_table)):
            inst=str(test_case[case]);
            cmd=exe_cmd%(inst,cc,cc1,program_name,cc1,str(param_index_table[param_index]))
            process= subprocess.Popen(cmd,shell = True)
            while 1:
                time.sleep(1)
                ret=subprocess.Popen.poll(process)
                if ret is None:
                    continue
                else:
                    break
def tune_beta_param(ns3_exe_path):
    test_case=[1,2,3,4,5,6]
    beta_index_table=[0,1,2,3,4]
    param_str="beta"
    cc="learning"
    cc1="reno"
    tune_param(ns3_exe_path,cc,cc1,param_str,test_case,beta_index_table)
    cc1="cubic"
    tune_param(ns3_exe_path,cc,cc1,param_str,test_case,beta_index_table)
    cc1="learning"
    tune_param(ns3_exe_path,cc,cc1,param_str,test_case,beta_index_table)
def tune_theta_param(ns3_exe_path):
    test_case=[1,2,3,4,5,6]
    theta_index_table=[0,1,2,3,4]
    param_str="theta"
    cc="learning"
    cc1="reno"
    tune_param(ns3_exe_path,cc,cc1,param_str,test_case,theta_index_table)
    cc1="cubic"
    tune_param(ns3_exe_path,cc,cc1,param_str,test_case,theta_index_table)
    cc1="learning"
    tune_param(ns3_exe_path,cc,cc1,param_str,test_case,theta_index_table)

if __name__ == '__main__':
    ns3_root="/home/ipcom/zsy/ns-allinone-3.26/ns-3.26/"
    ns3_exe_path=ns3_root+"build/scratch/"
    ns3_lib_path=ns3_root+"build/"
    parser = argparse.ArgumentParser(description='manual to this script')
    parser.add_argument('--tune', type=str, default ='beta')
    args = parser.parse_args()
    tune_param_str=args.tune
    old = os.environ.get("LD_LIBRARY_PATH")
    if old:
        os.environ["LD_LIBRARY_PATH"] = old + ":" +ns3_lib_path
    else:
        os.environ["LD_LIBRARY_PATH"] = ns3_lib_path
    if tune_param_str=="beta":
        tune_beta_param(ns3_exe_path)
    elif tune_param_str=="theta":
        tune_theta_param(ns3_exe_path)
    print("done")
