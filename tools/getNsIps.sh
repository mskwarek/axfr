#!/bin/bash
processDir(){
    array=`find /home/marcin/output3/$1 -type f`
    sufix=`echo $1 | sed 's/test_//g'`
    for file in ${array[@]}; do
	echo $file | rev | cut -d'_' -f1 | rev | sed 's/.axfr//g' >> /home/marcin/ns_result/ip_ns_$sufix
    done
}


dirs=`ls /home/marcin/output3`
mkdir /home/marcin/ns_result
for dir in ${dirs[@]}; do
    processDir ${dir} &
done
