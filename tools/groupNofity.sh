#!/bin/bash

input=/home/marcin/toNotify.result
output=/home/marcin/myDig/tools/groupped

while read line
do
    person=`echo $line | cut -d ' ' -f 1`
    domain=`echo $line | cut -d ' ' -f 2`
    ip=`echo $line | cut -d ' ' -f 3`
    echo $domain $ip >> $output/$person
done < "$input"
