#!/bin/bash
processDir(){
    array=`find /media/marcin/home/marcin/output3/$1 -type f`
    sufix=`echo $1 | sed 's/test_//g'`
    for file in ${array[@]}; do
	fileLines=`cat $file | wc -l`
	if [ "$fileLines" -eq "1" ]
	then
	    dir=/media/marcin/results/oneLines
	    tld=/media/marcin/results/oneTlds_${sufix}.result
	    nsIp=/media/marcin/results/oneNsIp_${sufix}.result
	else
	    dir=/media/marcin/results/manyLines
	    tld=/media/marcin/results/manyTlds_${sufix}.result
	    nsIp=/media/marcin/results/manyNsIp_${sufix}.result
	fi
	filename=`basename $file`
	cp $file $dir/$filename
	echo $file | rev | cut -d '/' -f 1 | rev | cut -d _ -f 1 | rev | cut -d . -f 1 | rev >> $tld 
	echo $file | rev | cut -d'_' -f1 | rev | sed 's/.axfr//g' >> $nsIp
    done
}


#dirs=`ls /home/marcin/output3`
#mkdir /home/marcin/zone_size_result
#for dir in ${dirs[@]}; do
dirToAnalyze=$1

processDir ${dirToAnalyze}
