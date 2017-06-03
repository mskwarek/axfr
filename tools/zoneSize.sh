#!/bin/bash
processDir(){
    array=`find /home/marcin/output3/$1 -type f`
    sufix=`echo $1 | sed 's/test_//g'`
    for file in ${array[@]}; do
	cat $file | wc -l | grep -v "\<1\>" >> /home/marcin/zone_size_result/new_size_$sufix
	cat $file | wc -l | grep "\<1\>" >> /home/marcin/zone_size_result/new_ones_$sufix
    done
}


dirs=`ls /home/marcin/output3`
#mkdir /home/marcin/zone_size_result
for dir in ${dirs[@]}; do
    processDir ${dir} &
done
