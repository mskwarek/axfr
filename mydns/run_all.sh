#!/bin/bash

j=0
PROCMAX=$1 #100
OUTPUTDIR=$2 #/home/marcin/ClionProjects/myDig/mydns
INPUTDIR=$3 #/home/marcin/ClionProjects/myDig/splitted
BINARYDIR="$( cd "$(dirname "$0")" ; pwd -P )"

#mkdir /home/marcin/myDig/splitted
#split -l 100 /home/marcin/myDig/mydns/inputData2_ip /home/marcin/myDig/splitted/inp.

shopt -s nullglob
array=($INPUTDIR/*.*)
shopt -u nullglob # Turn off nullglob to make sure it doesn't interfere with anything later
#echo "${array[@]}"
tlen=${#array[@]}
x=`ps -aux | grep a.out | grep -v grep | wc -l`
i=0
STARTPROC=$[$PROCMAX-$x+$i]

for (( ; i<${tlen}; ));
do
    if [ "$x" -gt "$PROCMAX" ]; then
	echo "sleep $i"
	sleep 240;
	x=`ps -aux | grep a.out | grep -v grep | wc -l`
    fi
    
    if [ "$PROCMAX" -gt "$x" ]; then
        #echo "start couse x= $x"
        #j=$[$j+1]
        datetime=`date "+%Y_%m_%d"`
	iter_num=`echo ${array[$i]} | cut -d . -f 2`
	#echo "$iter_num"
        mkdir -p $OUTPUTDIR/test_$datetime/iter_$iter_num
        shuf ${array[$i]} -o ${array[$i]}
        nohup ${BINARYDIR}/a.out -l ${array[$i]} -t 3 -o $OUTPUTDIR/test_$datetime/iter_$iter_num >/dev/null 2>&1 &
        x=$[$x+1]
        i=$[$i+1]
    else
	echo "sleep else"
	sleep 240;
	x=`ps -aux | grep a.out | grep -v grep | wc -l`
    fi

done
