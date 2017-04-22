#!/bin/bash

j=0
PROCMAX=$1 #100
OUTPUTDIR=$2 #/home/marcin/ClionProjects/myDig/mydns
INPUTDIR=$3 #/home/marcin/ClionProjects/myDig/splitted

#mkdir /home/marcin/myDig/splitted
#split -l 100 /home/marcin/myDig/mydns/inputData2_ip /home/marcin/myDig/splitted/inp.

shopt -s nullglob
array=($INPUTDIR/*.*)
shopt -u nullglob # Turn off nullglob to make sure it doesn't interfere with anything later
#echo "${array[@]}"
tlen=${#array[@]}
x=`ps -aux | grep a.out | grep -v grep | wc -l`
STARTPROC=$[$PROCMAX-$x]

for (( i=7500; i<${tlen}; ));
do
    if [ "$STARTPROC" -eq "$i" ]; then
	echo "sleep $i"
	sleep 60;
	x=`ps -aux | grep a.out | grep -v grep | wc -l`
	withoutasking=$[$PROCMAX-$x]
	STARTPROC=$[$STARTPROC+$withoutasking]
    fi
    
    if [ "$PROCMAX" -gt "$x" ]; then
        #echo "start couse x= $x"
        #j=$[$j+1]
        datetime=`date "+%Y_%m_%d"`
	iter_num=`echo ${array[$i]} | cut -d . -f 2`
	#echo "$iter_num"
        mkdir -p $OUTPUTDIR/test_$datetime/iter_$iter_num
        #shuf ${array[$i]} -o ${array[$i]}
        nohup ./a.out ${array[$i]} 3 $OUTPUTDIR/test_$datetime/iter_$iter_num >/dev/null 2>&1 &
	
        i=$[$i+1]
    else
	echo "sleep else"
	sleep 60;
	x=`ps -aux | grep a.out | grep -v grep | wc -l`
    fi

done
