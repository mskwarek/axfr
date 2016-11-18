#!/bin/bash 

function process_file {
    echo $1 $2


    python /home/mkoi/mgr/myDig/python_module/axfrWrapper.py --output_folder $2 --input_list $1

    cp -r $2 /home/mkoi/cp_res

    if [ $? -eq 0 ];
    then
        echo "OK"
	rm -rf $2
    else
        echo "NOK"
    fi
}


FILES=/home/mkoi/mgr/myDig/python_module/logs
s=($FILES/*.*)
numfiles=${#s[@]}

echo $numfiles

j=0

for i in $FILES/*.*
do
    process_file $i "~/results/iter_"$j
    j=$[$j+1]
done

