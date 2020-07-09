#!/usr/bin/env python
import time
import os
import subprocess ,signal
prefix_cmd="./waf --run 'scratch/learn-cc  --it=%s --cc=%s --lo=%s'"
test_case=[3,4,5,6]
loss_rate=[0]
cc_name="learning"
for case in range(len(test_case)):
    inst=str(test_case[case]);
    for i in range(len(loss_rate)):
        cmd=prefix_cmd%(inst,cc_name,str(loss_rate[i]))
        process= subprocess.Popen(cmd,shell = True)
        while 1:
            time.sleep(1)
            ret=subprocess.Popen.poll(process)
            if ret is None:
                continue
            else:
                break  


