#!/bin/bash
algos=(learning)
loss=()
#echo "algo：$1";
#python pro-loss.py --algo=$1
#python pro-owd.py --algo=$1
#python pro-jain.py --algo=$1
#python pro-utilit.py --algo=$1
for algo in ${algos[@]}
do
python pro-loss.py --algo=$algo
python pro-owd.py --algo=$algo
python pro-utilit.py --algo=$algo
python pro-jain.py --algo=$algo
done
for l in ${loss[@]}
do
    for element in ${algos[@]}
    do
    algo=$element$l
    python pro-loss.py --algo=$algo
    python pro-owd.py --algo=$algo
    python pro-utilit.py --algo=$algo
    python pro-jain.py --algo=$algo
    done
done


