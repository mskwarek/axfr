#!/bin/bash

function query {
    echo "domain" $1 &> data/$1.log-dom
    dig +short @8.8.4.4 $1 &>> data/$1.log-dom &
    echo
    #PID=$!
    #sleep 10
    #if ps -p $PID > /dev/null
    #then
    #       kill  $PID
    #fi
    echo finishing $1
}

while read LINE
do
    query ${LINE} &
    sleep 0.2
done < $1

wait

for x in {{a..z},{0..9}} ; do cat data/$x*log-dom | awk '{if ($1=="domain") {dom=$2} else if ($1!="domain") {printf dom"|"$1"\n"} }' ; done > A_dig_8.8.4.4.dat
