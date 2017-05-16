#!/bin/bash

days=`ls /home/marcin/output3`

for day in ${days[@]}; do
    iters=`ls -l /home/marcin/output3/$day | wc -l`
    echo $day $iters
done;
