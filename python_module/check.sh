#!/bin/bash

function query {
    domain=$2
    server=$1
    OUTPUT=`dig @$server $domain +short`
    for i in $OUTPUT
    do
	echo "$domain|${server}|$i" >> logs/after_logs3.chk
    done
    if [ -z "$OUTPUT" ]
    then
	echo "$domain|${server}|" >> logs/after_logs3.chk
    fi
    #echo "$domain|${server}|$OUTPUT" >> .logs/logs.chk
    #echo "dig @$server ${domain} +short"

}

while read LINE
do
    domain=`echo $LINE | cut -d '|' -f 1`
    server=`echo $LINE | cut -d '|' -f 2`
    echo "$LINE"
    query $server $domain &
    #sleep 0.01
    sleep 0.05
done
wait
