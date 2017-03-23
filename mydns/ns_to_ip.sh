#!/bin/bash
while IFS='' read -r line || [[ -n "$line" ]]; do
    DOMAIN=`echo $line | cut -d' ' -f 1`
    NS=`echo $line | cut -d' ' -f 2`
    IPS=`dig +short $DOMAIN`
    for x in $IPS; do echo $DOMAIN $x >> $2; done;
done < "$1"
