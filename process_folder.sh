#!/bin/bash

j=0

for i in $1/*
do
	j=$[$j+1]
    nohup bash process_list.sh $i ans_test/ans_$j &
done
