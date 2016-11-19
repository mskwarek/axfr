#!/bin/bash 

SCRIPT=`realpath -s $0`
SCRIPTPATH=`dirname $SCRIPT`

function process_file {
    echo $1 $2

    python $SCRIPTPATH/axfrWrapper.py --output_folder $2 --input_list $1 --root_project_dir $SCRIPTPATH/..

<<<<<<< HEAD
    if ( mountpoint -a $SCRIPTPATH/cp_res ); then	
	cp -r $2/../* $SCRIPTPATH/cp_res

	if [ $? -eq 0 ];
	then
            echo "OK"
	    rm -rf $2/../*
	else
            echo "NOK"
	fi
=======
    cp -r $2 /home/marcin/cp_res

    if [ $? -eq 0 ];
    then
        echo "OK"
	rm -rf $2
    else
        echo "NOK"
>>>>>>> parent of 8601b9a... added remote cp
    fi
}


<<<<<<< HEAD
FILES=$SCRIPTPATH/$1
=======
FILES=$SCRIPTPATH/logs
>>>>>>> parent of 8601b9a... added remote cp
s=($FILES/*.*)
numfiles=${#s[@]}

j=0

for i in $FILES/*.*
do
    process_file $i "~/results/iter_"$j
    j=$[$j+1]
done

