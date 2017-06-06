#!/bin/bash
processDir(){
    array=`find /home/marcin/output3/$1 -type f`
    sufix=`echo $1 | sed 's/test_//g'`
    for file in ${array[@]}; do
	filename=`echo $file | rev | cut -d'/' -f1 | rev`
	ip=`echo $filename | cut -d'_' -f2 | sed 's/.axfr//g'` 
	domain=`echo $filename | cut -d'_' -f1`
	echo $domain $ip >> /home/marcin/responsedList/list_$sufix
    done
}


dirs=`ls /home/marcin/output3`
mkdir /home/marcin/responsedList/
for dir in ${dirs[@]}; do
    processDir ${dir} &
done
