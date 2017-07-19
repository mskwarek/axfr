awk 'BEGIN {FS=OFS=" "} FNR==NR{a[$1]=$2;next} $2 in a{print $1, a[$2],$3}' isoMap.input ~/Documents/myDig/results/countriesCnt_header.result >> ~/Documents/myDig/results/countriesCnt_iso.result
