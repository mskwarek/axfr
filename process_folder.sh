#!/bin/bash

j=0

for i in $1/*
do
	j=$[$j+1]
	if [[ $j -gt 1000]]; then 
		break 
	fi
    nohup bash process_list.sh $i ans_test/ans_$j &
done
