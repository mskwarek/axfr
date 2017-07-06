#!/bin/bash
processDir(){
    array=`find $3/$1 -type f`
    sufix=`echo $1 | sed 's/test_//g'`
    baseResultDir=$2
    mkdir ${baseResultDir}/oneLines/${sufix}
    mkdir ${baseResultDir}/manyLines/${sufix}
    for file in ${array[@]}; do
	fileLines=`cat $file | wc -l`
        baseResultDir=$2
	if [ "$fileLines" -eq "1" ]
	then
	    dir=${baseResultDir}/oneLines/${sufix}
	    tld=${baseResultDir}/oneTlds_${sufix}.result
	    nsIp=${baseResultDir}/oneNsIp_${sufix}.result
	else
	    dir=${baseResultDir}/manyLines/${sufix}
	    tld=${baseResultDir}/manyTlds_${sufix}.result
	    nsIp=${baseResultDir}/manyNsIp_${sufix}.result
	fi
	filename=`basename $file`
	mv $file $dir/$filename
	echo $file | rev | cut -d '/' -f 1 | rev | cut -d _ -f 1 | rev | cut -d . -f 1 | rev >> $tld 
	echo $file | rev | cut -d'_' -f1 | rev | sed 's/.axfr//g' >> $nsIp
    done
}


#dirs=`ls /home/marcin/output3`
#mkdir /home/marcin/zone_size_result
#for dir in ${dirs[@]}; do
dirToAnalyze=$1
outputDir=`readlink -f $2`
measuresDir=`readlink -f $3`

processDir ${dirToAnalyze} ${outputDir} ${measuresDir}
