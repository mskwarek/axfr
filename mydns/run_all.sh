for i in /home/marcin/myDig/mydns/splitted_data2/*.*
do
    nohup ./a.out $i 1 &
done
