input=$1
output=$2

arr=`find /home/marcin/output3 -type f -printf "%s %p\n" | sort -nr | cut -d\  -f2-`

for file in ${arr[@]}; do 
    soa=`cat $file | grep -v 'pasuje do wzorca' | grep -P '\t6\t'`
    if [[ $soa = *[!\ ]* ]]; then
	person=`echo $soa | cut -d ' ' -f 5`
	fileName=`echo $file | rev | cut -d '/' -f 1 | rev | sed 's/.axfr//g'`
	domain=`echo $fileName | cut -d _ -f 1`
	nameserver=`echo $fileName | cut -d _ -f 2`
	echo $person $domain $nameserver >> $output
    fi

done
