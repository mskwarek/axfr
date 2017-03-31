#!/bin/bash

SCRIPT=`realpath -s $0`
SCRIPTPATH=`dirname $SCRIPT`

function perform_dig {
	$SCRIPTPATH/bind-9.11.0*/bin/dig/dig $1 @$2 axfr +short +tries=1 +timeout=3 > $3/$1\_$2\.resp
}

mkdir $2
while read domen ip
do
	perform_dig $domen $ip $2
done < $1
