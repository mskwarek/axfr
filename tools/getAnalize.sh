scans_dir=$1

scans=`ls $scans_dir`

for file in ${scans[@]}; do
    dirname=`echo $file | sed 's/test_//g'`
    mkdir /home/marcin/results/$dirname
    nohup bash /home/marcin/myDig/tools/analyze.sh $scans_dir/$file /home/marcin/results/$dirname &
done
