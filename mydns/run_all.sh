j=0

for i in /home/marcin/myDig/mydns/splitted_data2/*.*
do
    j=$[$j+1]
    mkdir -p test/iter_$j
    nohup ./a.out $i 3 /home/marcin/myDig/mydns/test/iter_$j &
done
