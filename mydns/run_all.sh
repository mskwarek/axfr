#!/bin/bash

j=0

#mkdir /home/marcin/myDig/splitted
#split -l 100 /home/marcin/myDig/mydns/inputData2_ip /home/marcin/myDig/splitted/inp.
datetime=`date "+%Y_%m_%d_%H_%M"`


for i in {1..10}
do
    x=`ps -aux | grep a.out | grep -v grep | wc -l`
    
    if [ "1000" -gt "$x" ]; then
        echo $x
    
        j=$[$j+1]
        #mkdir -p /home/marcin/myDig/mydns/test_$datetime/iter_$j
        #nohup ./a.out $i 3 /home/marcin/myDig/mydns/test_$datetime/iter_$j &
        sleep 10
    fi
done
