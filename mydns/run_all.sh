#!/bin/bash

j=0
PROCMAX=100
OUTPUTDIR=/home/marcin/ClionProjects/myDig/mydns
INPUTDIR=/home/marcin/ClionProjects/myDig/splitted

#mkdir /home/marcin/myDig/splitted
#split -l 100 /home/marcin/myDig/mydns/inputData2_ip /home/marcin/myDig/splitted/inp.

shopt -s nullglob
array=($INPUTDIR/*.*)
shopt -u nullglob # Turn off nullglob to make sure it doesn't interfere with anything later
#echo "${array[@]}"
tlen=${#array[@]}

for (( i=0; i<${tlen}; ));
do
    x=`ps -aux | grep a.out | grep -v grep | wc -l`
    
    if [ "$PROCMAX" -gt "$x" ]; then
        echo "start couse x= $x"
        j=$[$j+1]
        datetime=`date "+%Y_%m_%d_%H_%M"`
        mkdir -p $OUTPUTDIR/test_$datetime/iter_$j
        nohup ./a.out ${array[$i]} 3 $OUTPUTDIR/test_$datetime/iter_$j &
        sleep 1
        i=$[$i+1]
    fi
done
