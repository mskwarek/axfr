#!/bin/bash 

SCRIPT=`realpath -s $0`
SCRIPTPATH=`dirname $SCRIPT`

function process_file {
    echo $1 $2

    python $SCRIPTPATH/axfrWrapper.py --output_folder $2 --input_list $1 --root_project_dir $SCRIPTPATH/..

    #if ( mountpoint -a $SCRIPTPATH/cp_res ); then	
	cp -r $2/../* /home/maciej/DDNS/AXFR_output

	if [ $? -eq 0 ];
	then
            echo "OK"
	    rm -rf $2/../*
	else
            echo "NOK"
	fi
    #fi
}

FILES=$1
s=($FILES/*.*)
numfiles=${#s[@]}

PROCS=0

j=0

for i in $FILES/*.*
do
    process_file $i "/home/marcin/results/iter_"$j &
    while (( $PROCS > 1 ))
    do
	echo $PROCS
	sleep 30
	PROCS=(`jobs | wc -l`)
    done
    j=$[$j+1]
done

