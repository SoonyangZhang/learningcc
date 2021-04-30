#!/usr/bin/env python
import time
import os
import subprocess ,signal
import argparse
def run_algorithms(ns3_exe_path,trace_folder,cc,cc1,test_case):
    exe_cmd=ns3_exe_path+"learn-cc  --it=%s --cc=%s --bench=%s  --trace=%s"
    for case in range(len(test_case)):
        inst=str(test_case[case]);
        cmd=exe_cmd%(inst,cc,cc1,trace_folder)
        process= subprocess.Popen(cmd,shell = True)
        while 1:
            time.sleep(1)
            ret=subprocess.Popen.poll(process)
            if ret is None:
                continue
            else:
                break
def test_intra_fairness(ns3_exe_path):
    test_case=[1,2,3,4,5,6]
    algorithms=["learning","viva","cubic","reno"]
    for algo in range(len(algorithms)):
        cc=algorithms[algo]
        cc1=cc
        trace_folder="pure"+cc
        run_algorithms(ns3_exe_path,trace_folder,cc,cc1,test_case)
def test_inter_fairness(ns3_exe_path):
    test_case=[1,2,3,4,5,6]
    algorithms=["learning","viva"]
    bench_algorithms=["cubic","reno"]
    for algo in range(len(algorithms)):
        for bench in range(len(bench_algorithms)):
            cc=algorithms[algo]
            cc1=bench_algorithms[bench]
            trace_folder=cc+"_vs_"+cc1
            run_algorithms(ns3_exe_path,trace_folder,cc,cc1,test_case)
#python learn-cc-fair.py --fair inter
if __name__ == '__main__':
    ns3_root="/home/ipcom/zsy/ns-allinone-3.26/ns-3.26/"
    ns3_exe_path=ns3_root+"build/scratch/"
    ns3_lib_path=ns3_root+"build/"
    parser = argparse.ArgumentParser(description='manual to this script')
    parser.add_argument('--fair', type=str, default='intra')
    args = parser.parse_args()
    fair_str=args.fair
    old = os.environ.get("LD_LIBRARY_PATH")
    if old:
        os.environ["LD_LIBRARY_PATH"] = old + ":" +ns3_lib_path
    else:
        os.environ["LD_LIBRARY_PATH"] = ns3_lib_path
    if fair_str=="intra":
        test_intra_fairness(ns3_exe_path)
    elif fair_str=="inter":
        test_inter_fairness(ns3_exe_path)
    print("done")
