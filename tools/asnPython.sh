#!/bin/bash
processDir(){
    day=`echo $1 | cut -d'_' -f2`
    month=`echo $1 | cut -d'_' -f3 | sed 's/.res//g'`
    echo $day $month
    python /home/marcin/myDig/tools/ip2asn.py --file_to_process /home/marcin/results/ipv4/ip_${day}_${month}.res --database /home/marcin/results/db_2017$month$day >> /home/marcin/results/ipv4/asnByDay/2017${month}${day}.result
}


files=`ls /home/marcin/results/ipv4/ip_*_*.res`
#mkdir /home/marcin/zone_size_result
for dir in ${files[@]}; do
    processDir $dir &
done
