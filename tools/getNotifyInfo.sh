for file in /media/marcin/may/test_2017_04_24/iter_005*/*_*.axfr;do
    soa=`cat $file | grep -v 'pasuje do wzorca' | grep -P '\t6\t'`
    if [[ $soa = *[!\ ]* ]]; then
	person=`echo $soa | cut -d ' ' -f 5`
	fileName=`echo $file | rev | cut -d '/' -f 1 | rev | sed 's/.axfr//g'`
	domain=`echo $fileName | cut -d _ -f 1`
	nameserver=`echo $fileName | cut -d _ -f 2`
	echo $person $domain $nameserver
    fi

done
