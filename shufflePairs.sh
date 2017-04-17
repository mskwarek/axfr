#!/bin/bash

SRCFILE=pairs.dat
tmpPairs=pairsSplitProcessing.dat
outDir=shuffling
LINES=50000000

cp $SRCFILE $tmpPairs
for i in {4..10}
do
    i=$[11-$i]
    echo "Iteration split $i";
    linesToSplit=$[$i*$LINES]
    split $tmpPairs -l $linesToSplit -d -a 2 $outDir/in.
    rm -rf $tmpPairs
    touch $tmpPairs

    shopt -s nullglob
    array=($outDir/*.*)
    shopt -u nullglob # Turn off nullglob to make sure it doesn't interfere with anything later
    tlen=${#array[@]}
    echo "${array[@]}"
    for (( j=0; j<${tlen}; ));
    do
	currFile=${array[$j]}
	echo "Shuffling file $currFile"
	shuf $currFile -o $currFile
	cat $currFile >> $tmpPairs
	j=$[$j+1]
    done;
    rm -rf $outDir/*
done;


